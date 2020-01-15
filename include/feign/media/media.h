/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>

class Media
{
public:
    Media();
    virtual ~Media();

    // TODO
};

/////////////////////////////////////////////////
// Material Node structure
/////////////////////////////////////////////////
struct MediaNode : public Node
{
public:
    MediaNode() : media(nullptr) { }
    MediaNode(std::string name) : Node(name), media(nullptr) { }
    MediaNode(Media* media) : media(media) { }

    ~MediaNode() { delete media; }

    Media* media;
};
/////////////////////////////////////////////////
