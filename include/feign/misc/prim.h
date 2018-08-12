#pragma once

#include <feign/node.h>

template <class T>
class Primitive : public SceneNode
{
public:
    Primitive();
    Primitive(SceneNode* parent);
    virtual ~Primitive();

    // scene node methods
    virtual NodeType getNodeType() const;
    virtual bool isPrimitive() const;

    // getter methods
    T getValue() const;
    string getName() const;

    // setter methods
    void setValue(T param);
    void setName(string param);
protected:
    string name;
    T value;
};
