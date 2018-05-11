# Glasgow-MicroStream-ESP32-Flat-Building-Demo

## Setup
Change the wifi credentials in 'main/include/WifiCredentials.h'.

## Builing
```
git clone git@github.com:sbeanie/Glasgow-MicroStream-ESP32-Flat-Building-Demo.git
cd Glasgow-MicroStream-ESP32-Flat-Building-Demo
git submodule init
git submodule update --recursive
```
Run each command in build-commands.txt for each device.  Make sure to run 'make clean' before each run.

### Eclipse
Follow the instructions here https://esp-idf.readthedocs.io/en/v2.0/eclipse-setup.html.
However, replace the full command from the "CDT GCC Build Output Parser" step with "xtensa-esp32-elf-((g?cc)|([gc]++)|(clang))".
Then clean the project and build.  All headers should resolve.
