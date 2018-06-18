#pragma once

#include <feign/common.h>
#include <stdexcept>
#include <stdio.h>

class NotImplementedException : public std::runtime_error {
public:
    NotImplementedException() : std::runtime_error("Not Yet Implemented") { }
    NotImplementedException(string str) : std::runtime_error(str + " Not Yet Implemented") { }
};

class InvalidTokenException : public std::runtime_error {
public:
    InvalidTokenException() : std::runtime_error("Invalid Token") { }
    InvalidTokenException(string str) : std::runtime_error("Invalid Token: " + str) { }
    // maybe add line numbers inside exception handling
};

class MissingTokenEndException : public std::runtime_error {
public:
    MissingTokenEndException() : std::runtime_error("Token Missing Closure") { }
    MissingTokenEndException(string str) : std::runtime_error("Token Missing Closure : " + str) { }
    // maybe add line numbers inside exception handling
};

class GenericException : public std::runtime_error {
public:
    GenericException() : std::runtime_error("General Error") { }
    GenericException(string str) : std::runtime_error("General Error : " + str) { }
};
