#pragma once

#include <feign/common.h>
#include <stdexcept>
#include <stdio.h>

class UnrecognizedTokenException : public runtime_error {
public:
    UnrecognizedTokenException() : runtime_error("Exception: Unrecognized Token") { }
    UnrecognizedTokenException(string str) : runtime_error("Exception: Unrecognized Token: " + str) { }
};

class MissingExpectedTokenException : public runtime_error {
public:
    MissingExpectedTokenException() : runtime_error("Exception: Expected Token") { }
    MissingExpectedTokenException(string str) : runtime_error("Exception: Expected Token: " + str) { }
};

class UnsupportedTypeException : public runtime_error {
public:
    UnsupportedTypeException() : runtime_error("Exception: Unsupported Type") { }
    UnsupportedTypeException(string str) : runtime_error("Exception: Unsupported Type: " + str) { }
};
