#
# Main component makefile.
#
# This Makefile can be left empty. By default, it will take the sources in the 
# src/ directory, compile them and link them into lib(subdirectory_name).a 
# in the build directory. This behaviour is entirely configurable,
# please read the ESP-IDF documents if you need to do this.
#
CPPFLAGS += -DDEVICE_FLAT_NUM=$(DEVICE_FLAT_NUM) -DNUM_FLATS=$(NUM_FLATS) -DDEVICE_FLAT_ROOM=$(DEVICE_FLAT_ROOM) -DDEVICE_FLAT_AGGREGATOR_FOR=$(DEVICE_FLAT_AGGREGATOR_FOR) -DBUILDING_AGGREGATOR_FOR=$(BUILDING_AGGREGATOR_FOR)
