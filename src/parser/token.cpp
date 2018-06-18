#include <feign/parser/token.h>

Token::Token() {
    tokenName = "";
    tokenValue = "";

    parent = nullptr;
    children = vector<Token*>();

    closed = false;
}

Token::Token(Token* parent) : parent(parent) {
    tokenName = "";
    tokenValue = "";

    children = vector<Token*>();

    closed = false;
}

Token::Token(Token* parent, string name) : parent(parent), tokenName(name) {
    tokenValue = "";

    children = vector<Token*>();

    closed = false;
}

Token::Token(Token* parent, string name, string value) : parent(parent), tokenName(name), tokenValue(value) {
    children = vector<Token*>();

    closed = false;
}

Token::~Token() {
    for (int i = 0; i < children.size(); ++i) {
        delete children[i];
    }
}

void Token::print(string precursor) {
    // TODO
}
