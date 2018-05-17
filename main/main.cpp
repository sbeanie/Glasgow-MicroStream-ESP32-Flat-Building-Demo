#include <driver/gpio.h>
#include <driver/i2c.h>
#include <freertos/task.h>

#include <esp_err.h>
#include <esp_log.h>
#include "sdkconfig.h" // generated by "make menuconfig"

#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "include/WifiCredentials.h"

#include "FlatDetails.hpp"
#include "Stream.hpp"
#include "DoublePollable.hpp"

#include "PrintSinks.hpp"
#include "Serialization.hpp"

#include "include/Fetch_BME280.hpp"

#ifdef __cplusplus
extern "C" {
#endif

using namespace glasgow_ustream;

void setup_sensor_readings_publication(Topology*);
void publish_flat_aggregated_sensor_readings(Topology*);
void setup_flat_aggregation(Topology*);
void connect_wifi();

void app_main(void) {
	connect_wifi();
	i2c_master_init();
	// Use a priority of 10 to prevent preemption. (default Glasgow MicroStream thread has priority 5)
	xTaskCreate(&task_bme280_normal_mode, "bme280_normal_mode",  2048, NULL, 10, NULL);

    Topology topology = Topology(std::chrono::seconds(5));

    setup_sensor_readings_publication(&topology);
    publish_flat_aggregated_sensor_readings(&topology);
    setup_flat_aggregation(&topology);
    topology.run();
    return;
}


void setup_sensor_readings_publication(Topology* topology) {
    std::string HUM_STREAM_ID = get_stream_id(FLAT, STR_VALUE(ROOM), "humidity");
    std::string PRESSURE_STREAM_ID = get_stream_id(FLAT, STR_VALUE(ROOM), "pressure");
    std::string TEMPERATURE_STREAM_ID = get_stream_id(FLAT, STR_VALUE(ROOM), "temperature");

    DoubleValuePollable* hPollable = new DoubleValuePollable(&compensated_humidity_double);
    DoubleValuePollable* pPollable = new DoubleValuePollable(&compensated_pressure_double);
    DoubleValuePollable* tPollable = new DoubleValuePollable(&compensated_temperature_double);

    PolledSource<double>* humidityStream = topology->addPolledSource(std::chrono::seconds(1), hPollable);
    PolledSource<double>* pressureStream = topology->addPolledSource(std::chrono::seconds(1), pPollable);
    PolledSource<double>* temperatureStream = topology->addPolledSource(std::chrono::seconds(1), tPollable);

    humidityStream->networkSink(topology, HUM_STREAM_ID, double_to_bytes);
    pressureStream->networkSink(topology, PRESSURE_STREAM_ID, double_to_bytes);
    temperatureStream->networkSink(topology, TEMPERATURE_STREAM_ID, double_to_bytes);
}

void publish_flat_aggregated_sensor_readings(Topology* topology) {
    const char *reading_type = STR_VALUE(DEVICE_FLAT_AGGREGATOR_FOR); // humidity/pressure/temperature
    if (strcmp(reading_type, "") == 0) return;

    std::cout << "Aggregating " << reading_type << " readings across rooms in flat " << FLAT << std::endl;

    optional<NetworkSource<double>*> bathroom_source_opt =
            topology->addNetworkSource(get_stream_id(FLAT, "bathroom", reading_type), byte_array_to_double);

    optional<NetworkSource<double>*> kitchen_source_opt =
            topology->addNetworkSource(get_stream_id(FLAT, "kitchen", reading_type), byte_array_to_double);

    optional<NetworkSource<double>*> bedroom_source_opt =
            topology->addNetworkSource(get_stream_id(FLAT, "bedroom", reading_type), byte_array_to_double);

    if ( ! bathroom_source_opt.is_initialized()) exit (1);
    if ( ! kitchen_source_opt.is_initialized()) exit (1);
    if ( ! bedroom_source_opt.is_initialized()) exit (1);

    NetworkSource<double>* bathroom_source = bathroom_source_opt.value(),
            *bedroom_source = bedroom_source_opt.value(),
            *kitchen_source = kitchen_source_opt.value();

    Stream<double>* union_stream = bathroom_source->union_streams(std::list<Subscribeable<double>*>{bedroom_source, kitchen_source});
    union_stream->networkSink(topology, get_aggregation_stream_id(FLAT, reading_type), double_to_bytes);
}

void setup_flat_aggregation(Topology* topology) {
    const char *reading_type = STR_VALUE(BUILDING_AGGREGATOR_FOR); // humidity/pressure/temperature
    if (strcmp(reading_type, "") == 0) return;

    std::list<Subscribeable<double>*> streams;
    for (int i = 1; i <= NUM_FLATS; i++) {
        optional<NetworkSource<double>*> opt = topology->addNetworkSource(get_aggregation_stream_id(i, reading_type), byte_array_to_double);
        if ( ! opt.is_initialized()) exit(1);
        streams.push_back(opt.value());
    }
    Stream<double>* unioned_stream = topology->union_streams(streams);
    unioned_stream->networkSink(topology, STR_VALUE(BUILDING_AGGREGATOR_FOR), double_to_bytes);
}

esp_err_t event_handler(void *ctx, system_event_t *event) {
	std::cout << "ERROR CAPTURED" << std::endl;
    return ESP_OK;
}

void connect_wifi(void) {
    nvs_flash_init();
	tcpip_adapter_init();
	ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
	ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
	ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
	wifi_config_t sta_config;
	unsigned char ssid[32] = {WIFI_SSID};
	unsigned char password[128] = {WIFI_PASSWORD};
	memcpy(sta_config.sta.ssid, ssid, 32);
	memcpy(sta_config.sta.password, password, 32);
	sta_config.sta.bssid_set = false;

	ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &sta_config) );
	ESP_ERROR_CHECK( esp_wifi_start() );
	ESP_ERROR_CHECK( esp_wifi_connect() );
}

#ifdef __cplusplus
}
#endif
