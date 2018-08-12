#include <feign/misc/prim.h>

template <class T>
Primitive<T>::Primitive() : SceneNode()
{
    name = "";
}

template <class T>
Primitive<T>::Primitive(SceneNode* parent) : SceneNode(parent)
{
    name = "";
}

template <class T>
Primitive<T>::~Primitive()
{
    // does nothing
}

template <class T>
NodeType Primitive<T>::getNodeType() const
{
    return NT_Primitive;
}

template <class T>
bool Primitive<T>::isPrimitive() const
{
    return true;
}

template <class T>
T Primitive<T>::getValue() const
{
    return value;
}

template <class T>
string Primitive<T>::getName() const
{
    return name;
}

template <class T>
void Primitive<T>::setValue(T param)
{
    value = param;
}

template <class T>
void Primitive<T>::setName(string param)
{
    name = param;
}
