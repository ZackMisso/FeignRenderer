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

class InvalidEndTokenException : public runtime_error {
public:
    InvalidEndTokenException() : runtime_error("Exception: Invalid End Token") { }
    InvalidEndTokenException(string str) : runtime_error("Exception: Invalid End Token: " + str) { }
};

class TagMisMatch : public runtime_error {
public:
    TagMisMatch() : runtime_error("Exception: Tag Mismatch") { }
    TagMisMatch(string str) : runtime_error("Exception: Tag Mismatch: " + str) { }
};

class GlobalTagMisMatch : public runtime_error {
public:
    GlobalTagMisMatch() : runtime_error("Exception: Global Tag Mismatch") { }
    GlobalTagMisMatch(string str) : runtime_error("Exception: Global Tag Mismatch: " + str) { }
};

class SceneParseFailedException : public runtime_error {
public:
    SceneParseFailedException() : runtime_error("Exception: Scene Parse Failed") { }
    SceneParseFailedException(string str) : runtime_error("Exception: Scene Parse Failed: " + str) { }
};
