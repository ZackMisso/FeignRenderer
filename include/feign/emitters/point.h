#pragma once

#include <feign/emitters/emitter.h>

class PointEmitter : public Emitter
{
public:
    PointEmitter();
    PointEmitter(Node* parent);

    virtual std::string getName() const;
};
