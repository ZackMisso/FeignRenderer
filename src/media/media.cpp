#include <feign/media/media.h>

Media::Media() : Node()
{
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
