#ifndef PAPERDEMO_FLATDETAILS_HPP
#define PAPERDEMO_FLATDETAILS_HPP

#include <sstream>

#define STRINGIZER(arg)     #arg
#define STR_VALUE(arg)      STRINGIZER(arg)

#ifndef FLAT
#error FLAT is not defined
#endif

#ifndef NUM_FLATS
#error NUM_FLATS is not defined
#endif

#ifndef ROOM
#error ROOM is not defined
#endif

#ifndef DEVICE_FLAT_AGGREGATOR_FOR
#error DEVICE_FLAT_AGGREGATOR_FOR is not set
#endif

#ifndef BUILDING_AGGREGATOR_FOR
#error BUILDING_AGGREGATOR_FOR is not set
#endif


std::string get_stream_id(int flat_num, const char* flat_room, const char* reading_type) {
    std::stringstream ss;
    ss << flat_num << "_" << flat_room << "_" << reading_type;
    std::string id = ss.str();
    return id;
}

std::string get_aggregation_stream_id(int flat_num, const char* reading_type) {
    std::stringstream ss;
    ss << flat_num << "_" << reading_type;
    std::string id = ss.str();
    return id;
}


#endif //PAPERDEMO_FLATDETAILS_HPP
