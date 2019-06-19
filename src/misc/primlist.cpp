#include <feign/misc/primlist.h>

// TODO: replace all of these with macros

PrimitiveList::PrimitiveList()
{
    int_primitives = std::vector<Primitive<int>*>();
    float_primitives = std::vector<Primitive<Float>*>();
    vec3_primitives = std::vector<Primitive<Vec3f>*>();
    string_primitives = std::vector<Primitive<std::string>*>();
    transform_primitives = std::vector<Primitive<Transform>*>();
    color_primitives = std::vector<Primitive<Color3f>*>();
}

PrimitiveList::~PrimitiveList()
{
    clearPrimitiveList();
}

void PrimitiveList::addIntPrimitive(Primitive<int>* prim)
{
    int_primitives.push_back(prim);
}

void PrimitiveList::addFloatPrimitive(Primitive<Float>* prim)
{
    float_primitives.push_back(prim);
}

void PrimitiveList::addVec3Primitive(Primitive<Vec3f>* prim)
{
    vec3_primitives.push_back(prim);
}

void PrimitiveList::addStringPrimitive(Primitive<std::string>* prim)
{
    string_primitives.push_back(prim);
}

void PrimitiveList::addTransformPrimitive(Primitive<Transform>* prim)
{
    transform_primitives.push_back(prim);
}

void PrimitiveList::addColorPrimitive(Primitive<Color3f>* prim)
{
    color_primitives.push_back(prim);
}

bool PrimitiveList::findInt(std::string name, int& val) const
{
    for (int i = 0; i < int_primitives.size(); ++i)
    {
        if (int_primitives[i]->getName() == name)
        {
            val = int_primitives[i]->getValue();
            return true;
        }
    }

    return false;
}

bool PrimitiveList::findFloat(std::string name, Float& val) const
{
    for (int i = 0; i < float_primitives.size(); ++i)
    {
        if (float_primitives[i]->getName() == name)
        {
            val = float_primitives[i]->getValue();
            return true;
        }
    }

    return false;
}

bool PrimitiveList::findVec3(std::string name, Vec3f& val) const
{
    for (int i = 0; i < vec3_primitives.size(); ++i)
    {
        if (vec3_primitives[i]->getName() == name)
        {
            val = vec3_primitives[i]->getValue();
            return true;
        }
    }

    return false;
}

bool PrimitiveList::findString(std::string name, std::string& val) const
{
    for (int i = 0; i < string_primitives.size(); ++i)
    {
        if (string_primitives[i]->getName() == name)
        {
            val = string_primitives[i]->getValue();
            return true;
        }
    }

    return false;
}

bool PrimitiveList::findTransform(std::string name, Transform& val) const
{
    for (int i = 0; i < transform_primitives.size(); ++i)
    {
        if (transform_primitives[i]->getName() == name)
        {
            val = transform_primitives[i]->getValue();
            return true;
        }
    }

    return false;
}

bool PrimitiveList::findColor(std::string name, Color3f& val) const
{
    for (int i = 0; i < color_primitives.size(); ++i)
    {
        if (color_primitives[i]->getName() == name)
        {
            val = color_primitives[i]->getValue();
            return true;
        }
    }

    return false;
}

void PrimitiveList::findInt(std::string name, int& val, int base) const
{
    bool found = findInt(name, val);
    if (!found)
    {
        val = base;
    }
}

void PrimitiveList::findFloat(std::string name, Float& val, Float base) const
{
    bool found = findFloat(name, val);
    if (!found)
    {
        val = base;
    }
}

void PrimitiveList::findString(std::string name, std::string& val, std::string base) const
{
    bool found = findString(name, val);
    if (!found)
    {
        val = base;
    }
}

void PrimitiveList::findTransform(std::string name, Transform& val, Transform base) const
{
    bool found = findTransform(name, val);
    if (!found)
    {
        val = base;
    }
}

void PrimitiveList::findColor(std::string name, Color3f& val, Color3f base) const
{
    bool found = findColor(name, val);
    if (!found)
    {
        val = base;
    }
}

void PrimitiveList::clearPrimitiveList()
{
    for (int i = 0; i < int_primitives.size(); ++i)
    {
        delete int_primitives[i];
    }

    for (int i = 0; i < float_primitives.size(); ++i)
    {
        delete float_primitives[i];
    }

    for (int i = 0; i < vec3_primitives.size(); ++i)
    {
        delete vec3_primitives[i];
    }

    for (int i = 0; i < string_primitives.size(); ++i)
    {
        delete string_primitives[i];
    }

    for (int i = 0; i < transform_primitives.size(); ++i)
    {
        delete transform_primitives[i];
    }

    for (int i = 0; i < color_primitives.size(); ++i)
    {
        delete color_primitives[i];
    }

    int_primitives.clear();
    float_primitives.clear();
    vec3_primitives.clear();
    string_primitives.clear();
    transform_primitives.clear();
    color_primitives.clear();
}

int PrimitiveList::size() const
{
    return int_primitives.size() +
           float_primitives.size() +
           vec3_primitives.size() +
           string_primitives.size() +
           transform_primitives.size() +
           color_primitives.size();
}
