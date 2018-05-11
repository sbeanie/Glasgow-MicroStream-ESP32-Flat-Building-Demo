#ifndef PAPERDEMO_SERIALIZERS_HPP
#define PAPERDEMO_SERIALIZERS_HPP

#include "Stream.hpp"

using namespace glasgow_ustream;

//########### SERIALIZATION ############
auto double_to_bytes = [] (double val) {
    void *data = malloc(sizeof(double));
    *((double *) data) = val;
    return std::pair<uint32_t, void *>(sizeof(double), data);
};

optional<double> (*byte_array_to_double) (std::pair<uint32_t, void*>) = [] (std::pair<uint32_t, void*> data) {
    return optional<double>(*((double*) data.second));
};

#endif //PAPERDEMO_SERIALIZERS_HPP
