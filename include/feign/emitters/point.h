#pragma once

#include <feign/emitter.h>

class PointEmitter : public Emitter
{
public:
    PointEmitter();
    PointEmitter(Node* parent);

    virtual std::string getName() const;
};
