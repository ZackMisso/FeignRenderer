/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 *
 * This class is loosley based off of an implementation by Wojciech Jarosz
 * from the Dartmouth Academic Ray Tracing Skeleton (DARTS)
 * https://github.com/cs87-dartmouth/darts-2022
 *
 **/

#pragma once

#include <feign/common.h>
#include <feign/math/vector.h>

FEIGN_BEGIN()

template <typename Object, typename... Args>
class Factory
{
public:
    using Constructor = std::function<Object(Args &&...args)>;

    static Object createInstance(const std::string &name, Args &&...args)
    {
        if (getRegistry().find(name) == getRegistry().end())
        {
            throw FeignRendererException("Cannot find a constructor for " + name);
        }
        return getRegistry().at(name)(std::forward<Args>(args)...);
    }

    static void registerType(const std::string &name, const Constructor constructor)
    {
        if (getRegistry().find(name) != getRegistry().end())
        {
            throw FeignRendererException("A constructor for " + name + " has already been registered");
        }
        getRegistry().insert(std::make_pair(name, constructor));
    }

    static std::set<std::string> registeredTypes()
    {
        const std::map<std::string, Constructor> reg = getRegistry();
        std::set<std::string> keys;
        for (auto pair : reg)
        {
            keys.insert(pair.first);
        }
        return keys;
    }

protected:
    static std::map<std::string, Constructor> &getRegistry()
    {
        static std::map<std::string, Constructor> registry;
        return registry;
    }
};

// TODO: create a specific factory for the current json file format
// TODO: create a specific factory for a USD file format

// TODO: get this working
// #define DARTS_REGISTER_CLASS_IN_FACTORY(T, cls, name)                                                          \
//     static struct cls##_FACTORY_HELPER                                                                         \
//     {                                                                                                          \
//         cls##_FACTORY_HELPER()                                                                                 \
//         {                                                                                                      \
//             Factory<T>::registerType(name, [](const nlohmann::json &j) { return make_shared<cls>(j); }); \
//         }                                                                                                      \
//     } cls##__FACTORY_HELPER_INSTANCE;

FEIGN_END()