#pragma once

#include <stdexcept>
#include <stdio.h>
#include <string>
#include <iostream>

class UnrecognizedTokenException : public std::runtime_error {
public:
    UnrecognizedTokenException() : std::runtime_error("Exception: Unrecognized Token")
    {
        std::cout << "Unrecognized Token" << std::endl;
    }
    UnrecognizedTokenException(std::string str) : std::runtime_error("Exception: Unrecognized Token: " + str)
    {
        std::cout << "Unrecognized Token: " << str << std::endl;
    }
};

class MissingExpectedTokenException : public std::runtime_error {
public:
    MissingExpectedTokenException() : std::runtime_error("Exception: Expected Token")
    {
        std::cout << "Missing Expected Token" << std::endl;
    }
    MissingExpectedTokenException(std::string str) : std::runtime_error("Exception: Expected Token: " + str)
    {
        std::cout << "Missing Expected Token: " << str << std::endl;
    }
};

class UnsupportedTypeException : public std::runtime_error {
public:
    UnsupportedTypeException() : std::runtime_error("Exception: Unsupported Type")
    {
        std::cout << "Unsupported Type Token" << std::endl;
    }
    UnsupportedTypeException(std::string str) : std::runtime_error("Exception: Unsupported Type: " + str)
    {
        std::cout << "Unsupported Type Token: " << str << std::endl;
    }
};

class InvalidEndTokenException : public std::runtime_error {
public:
    InvalidEndTokenException() : std::runtime_error("Exception: Invalid End Token")
    {
        std::cout << "Invalid End Token" << std::endl;
    }
    InvalidEndTokenException(std::string str) : std::runtime_error("Exception: Invalid End Token: " + str)
    {
        std::cout << "Invalid End Token: " << str << std::endl;
    }
};

class TagMisMatch : public std::runtime_error {
public:
    TagMisMatch() : std::runtime_error("Exception: Tag Mismatch")
    {
        std::cout << "Exception: Tag Mismatch" << std::endl;
    }
    TagMisMatch(std::string str) : std::runtime_error("Exception: Tag Mismatch: " + str)
    {
        std::cout << "Exception: Tag Mismatch: " << str << std::endl;
    }
};

class GlobalTagMisMatch : public std::runtime_error {
public:
    GlobalTagMisMatch() : std::runtime_error("Exception: Global Tag Mismatch")
    {
        std::cout << "Global Tag MisMatch" << std::endl;
    }
    GlobalTagMisMatch(std::string str) : std::runtime_error("Exception: Global Tag Mismatch: " + str)
    {
        std::cout << "Global Tag MisMatch: " << str << std::endl;
    }
};

class SceneParseFailedException : public std::runtime_error {
public:
    SceneParseFailedException() : std::runtime_error("Exception: Scene Parse Failed")
    {
        std::cout << "Scene Parse Failed" << std::endl;
    }
    SceneParseFailedException(std::string str) : std::runtime_error("Exception: Scene Parse Failed: " + str)
    {
        std::cout << "Scene Parse Failed : " << str << std::endl;
    }
};

class NotImplementedException : public std::runtime_error {
public:
    NotImplementedException() : std::runtime_error("Exception: Method Not Implemented")
    {
    }
    NotImplementedException(std::string str) : std::runtime_error("Exception: Method Not Implemented: " + str)
    {
        std::cout << str << std::endl;
    }
};

class OBJUnopenableException : public std::runtime_error
{
public:
    OBJUnopenableException() : std::runtime_error("Exception: OBJ file could not be opened")
    {
        std::cout << "Unable to open OBJ file" << std::endl;
    }
    OBJUnopenableException(std::string str) : std::runtime_error("Exception: OBJ file could not be opened: " + str)
    {
        std::cout << "Unable to open OBJ file: " << str << std::endl;
    }
};

class ParseException : public std::runtime_error
{
public:
    ParseException() : std::runtime_error("Parse Exception")
    {
        std::cout << "Parse Exception" << std::endl;
    }
    ParseException(std::string str) : std::runtime_error("Parse Exception: " + str)
    {
        std::cout << "Parse Exception: " << str << std::endl;
    }
};

class MissingPrimitiveException : public std::runtime_error {
public:
    MissingPrimitiveException() : std::runtime_error("Missing Primitive Exception")
    {
        std::cout << "Missing Primitive" << std::endl;
    }
    MissingPrimitiveException(std::string str) : std::runtime_error("Missing Primitive Exception")
    {
        std::cout << "Missing Primitive: " << str << std::endl;
    }
};

class ClockerException : public std::runtime_error
{
public:
    ClockerException() : std::runtime_error("Clocker Exception")
    {
        std::cout << "Clocker Exception" << std::endl;
    }
    ClockerException(std::string str) : std::runtime_error("Clocker Exception: " + str)
    {
        std::cout << "Clocker Exception: " << str << std::endl;
    }
}
