#pragma once

#include <stdexcept>
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class UnrecognizedTokenException : public runtime_error {
public:
    UnrecognizedTokenException() : runtime_error("Exception: Unrecognized Token") { cout << "Unrecognized Token" << endl; }
    UnrecognizedTokenException(string str) : runtime_error("Exception: Unrecognized Token: " + str) { cout << "Unrecognized Token: " << str << endl; }
};

class MissingExpectedTokenException : public runtime_error {
public:
    MissingExpectedTokenException() : runtime_error("Exception: Expected Token") { cout << "Missing Expected Token" << endl; }
    MissingExpectedTokenException(string str) : runtime_error("Exception: Expected Token: " + str) { cout << "Missing Expected Token: " << str << endl; }
};

class UnsupportedTypeException : public runtime_error {
public:
    UnsupportedTypeException() : runtime_error("Exception: Unsupported Type") { cout << "Unsupported Type Token" << endl; }
    UnsupportedTypeException(string str) : runtime_error("Exception: Unsupported Type: " + str) { cout << "Unsupported Type Token: " << str << endl; }
};

class InvalidEndTokenException : public runtime_error {
public:
    InvalidEndTokenException() : runtime_error("Exception: Invalid End Token") { cout << "Invalid End Token" << endl; }
    InvalidEndTokenException(string str) : runtime_error("Exception: Invalid End Token: " + str) { cout << "Invalid End Token: " << str << endl; }
};

class TagMisMatch : public runtime_error {
public:
    TagMisMatch() : runtime_error("Exception: Tag Mismatch") { cout << "Tag MisMatch" << endl; }
    TagMisMatch(string str) : runtime_error("Exception: Tag Mismatch: " + str) { cout << "Tag MisMatch: " << str << endl; }
};

class GlobalTagMisMatch : public runtime_error {
public:
    GlobalTagMisMatch() : runtime_error("Exception: Global Tag Mismatch") { cout << "Global Tag MisMatch" << endl; }
    GlobalTagMisMatch(string str) : runtime_error("Exception: Global Tag Mismatch: " + str) { cout << "Global Tag MisMatch: " << str << endl; }
};

class SceneParseFailedException : public runtime_error {
public:
    SceneParseFailedException() : runtime_error("Exception: Scene Parse Failed") { cout << "Scene Parse Failed" << endl; }
    SceneParseFailedException(string str) : runtime_error("Exception: Scene Parse Failed: " + str) { cout << "Scene Parse Failed : " << str << endl; }
};

class NotImplementedException : public runtime_error {
public:
    NotImplementedException() : runtime_error("Exception: Method Not Implemented") {}
    NotImplementedException(string str) : runtime_error("Exception: Method Not Implemented: " + str) { cout << str << endl; }
};

class OBJUnopenableException : public runtime_error
{
public:
    OBJUnopenableException() : runtime_error("Exception: OBJ file could not be opened") { cout << "Unable to open OBJ file" << endl; }
    OBJUnopenableException(string str) : runtime_error("Exception: OBJ file could not be opened: " + str) { cout << "Unable to open OBJ file: " << str << endl; }
};

class MissingPrimitiveException : public runtime_error {
public:
    MissingPrimitiveException() : runtime_error("Missing Primitive Exception") { cout << "Missing Primitive" << endl; }
    MissingPrimitiveException(string str) : runtime_error("Missing Primitive Exception") { cout << "Missing Primitive: " << str << endl; }
};
