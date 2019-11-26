/**
 * Author:    Zackary Misso
 * Last Edited:   10.12.2019
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/media/media.h>

Media::Media() : Node()
{
    throw new NotImplementedException("media constructore");
    // TODO
}

Media::~Media()
{
    std::cout << "deleting media" << std::endl;
    // TODO
}

std::string Media::getName() const
{
    return "media: ";
}
