/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/core/node.h>

class Media
{
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
    MediaNode(std::string name) : Node(name) { }
    MediaNode(Media* media) : media(media) { }

    Media* media;
};
/////////////////////////////////////////////////
