/**
 * Author:    Zackary Misso
 * Version:   0.2.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

// TODO: what is this used for again????

#pragma once

#include <feign/common.h>

FEIGN_BEGIN()

struct Functor
{
    virtual ~Functor(){};

    virtual Float evaluate(Point3f p, Float time) const = 0;
};

struct SquareWave : public Functor
{
    SquareWave(Point3f start_pos,
               Point3f freq,
               Float amp,
               Float wavelength,
               Float start_time,
               Float end_time)
        : start_pos(start_pos),
          freq(freq),
          amp(amp),
          wavelength(wavelength),
          start_time(start_time),
          end_time(end_time) {}

    virtual Float evaluate(Point3f p, Float time) const
    {
        Float cur_dist = (start_pos + freq * time).norm();
        Float dist = (p - start_pos).norm();

        if (std::abs(dist - cur_dist) < wavelength)
            return amp;
        return 0.0;
    }

    Point3f start_pos;
    Point3f freq;
    Float amp;
    Float wavelength;
    Float start_time;
    Float end_time;
};

struct TriangleWave : public Functor
{
    TriangleWave(Point3f start_pos,
                 Point3f freq,
                 Float amp,
                 Float wavelength,
                 Float start_time,
                 Float end_time)
        : start_pos(start_pos),
          freq(freq),
          amp(amp),
          wavelength(wavelength),
          start_time(start_time),
          end_time(end_time) {}

    virtual Float evaluate(Point3f p, Float time) const
    {
        Float cur_dist = (start_pos + freq * time).norm();
        Float dist = (p - start_pos).norm();

        Float proxy = (std::abs(dist - cur_dist));
        Float val = 1.0 - proxy / wavelength;
        return std::min(val * amp, 0.f);
    }

    Point3f start_pos;
    Point3f freq;
    Float amp;
    Float wavelength;
    Float start_time;
    Float end_time;
};

struct SinWave
{
};

FEIGN_END()
