#ifndef PAPERDEMO_DOUBLEPOLLABLE_HPP
#define PAPERDEMO_DOUBLEPOLLABLE_HPP

#include "Stream.hpp"

using namespace glasgow_ustream;

// This class is used to poll the current sensor readings.
class DoubleValuePollable : public Pollable<double> {

    double *ptr_to_value;

public:

    explicit DoubleValuePollable(double *ptr) : ptr_to_value(ptr) {}

    double getData(PolledSource<double> *) override {
        return *ptr_to_value;
    }

    virtual ~DoubleValuePollable(){}
};

#endif //PAPERDEMO_DOUBLEPOLLABLE_HPP
