#pragma once

#include <feign/common.h>

enum TokenType {
    TT_NONE,
    TT_INT,
    TT_FLOAT,
    TT_DOUBLE,
    TT_LONG
};

// This will need some fleshing out

struct Token {
    Token();
    Token(Token* parent);
    Token(Token* parent, string name);
    Token(Token* parent, string name, string value);
    ~Token();

    void print(string precursor);

    string tokenObject;
    string tokenType;
    string tokenName;
    string tokenValue;

    Token* parent;
    vector<Token*> children;

    bool closed;
};

struct ObjectToken : public Token {
    // TODO
};

struct TypeToken : public Token {
    // TODO
};

struct NameToken : public Token {
    // TODO
};

struct ValueToken : public Token {
    // TODO
};

struct TargetToken : public Token {
    // TODO
};

struct OriginToken : public Token {
    // TODO
};

struct UpToken : public Token {
    // TODO
};
