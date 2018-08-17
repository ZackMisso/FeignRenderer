#include <feign/misc/primlist.h>

PrimitiveList::PrimitiveList()
{
    intPrimitives = vector<Primitive<int>*>();
    floatPrimitives = vector<Primitive<Float>*>();
    stringPrimitives = vector<Primitive<string>*>();
}

PrimitiveList::~PrimitiveList()
{
    clearPrimitiveList();
}

void PrimitiveList::addIntPrimitive(Primitive<int>* prim)
{
    intPrimitives.push_back(prim);
}

void PrimitiveList::addFloatPrimitive(Primitive<Float>* prim)
{
    floatPrimitives.push_back(prim);
}

void PrimitiveList::addStringPrimitive(Primitive<string>* prim)
{
    stringPrimitives.push_back(prim);
}

bool PrimitiveList::findInt(string name, int& val) const
{
    for (int i = 0; i < intPrimitives.size(); ++i)
    {
        if (intPrimitives[i]->getName() == name)
        {
            val = intPrimitives[i]->getValue();
            return true;
        }
    }

    return false;
}

bool PrimitiveList::findFloat(string name, Float& val) const
{
    for (int i = 0; i < floatPrimitives.size(); ++i)
    {
        if (floatPrimitives[i]->getName() == name)
        {
            val = floatPrimitives[i]->getValue();
            return true;
        }
    }

    return false;
}

bool PrimitiveList::findString(string name, string& val) const
{
    for (int i = 0; i < stringPrimitives.size(); ++i)
    {
        if (stringPrimitives[i]->getName() == name)
        {
            val = stringPrimitives[i]->getValue();
            return true;
        }
    }

    return false;
}

void PrimitiveList::clearPrimitiveList()
{
    for (int i = 0; i < intPrimitives.size(); ++i)
    {
        delete intPrimitives[i];
    }

    for (int i = 0; i < floatPrimitives.size(); ++i)
    {
        delete floatPrimitives[i];
    }

    for (int i = 0; i < stringPrimitives.size(); ++i)
    {
        delete stringPrimitives[i];
    }
}
