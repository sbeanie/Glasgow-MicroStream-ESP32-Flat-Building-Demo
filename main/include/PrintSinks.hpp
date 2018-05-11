#ifndef PAPERDEMO_PRINTSINKS_HPP
#define PAPERDEMO_PRINTSINKS_HPP

#include <iostream>

void (*double_print_sink) (double) = [] (double val) {std::cout << "Double value: " << val << std::endl;};

void (*int_print_sink) (int) = [] (int val) {std::cout << "Int value: " << val << std::endl;};


#endif //PAPERDEMO_PRINTSINKS_HPP
