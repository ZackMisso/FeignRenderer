/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <feign/common.h>
#include <feign/core/sampler.h>
#include <feign/math/spectrum.h>

FEIGN_BEGIN()

// TODO: implement parameter setting to fit within the feign renderer's framework
class TransFunc
{
public:
    TransFunc() {}
    virtual ~TransFunc() {}

    Color3f eval(Color3f tau,
                 bool start_on_surface,
                 bool end_on_surface) const;

    Float sample(Sampler *sampler, bool start_on_surface) const;

    inline Color3f surface_probability(Color3f tau, bool start_on_surface) const;
    inline Color3f medium_pdf(Color3f tau, bool start_on_surface) const;

    virtual bool is_delta() const { return false; }

    virtual Float effective_sigma() const { return sigma_bar(); }

    virtual Color3f surface_surface(Color3f tau) const = 0;
    virtual Color3f surface_medium(Color3f tau) const = 0;
    virtual Color3f medium_surface(Color3f tau) const = 0;
    virtual Color3f medium_medium(Color3f tau) const = 0;

    virtual Float sigma_bar() const = 0;

    virtual Float sample_surface(Sampler *sampler) const = 0;
    virtual Float sample_medium(Sampler *sampler) const = 0;
};

/////////////////////////////////////////////////
// Linear Transmittance
/////////////////////////////////////////////////
class LinearTrans : public TransFunc
{
public:
    struct Params
    {
        Params() : max_t(1.f) {}
        Params(Float max_t) : max_t(max_t) {}

        Float max_t;
    };

    LinearTrans(Float maxT);

    virtual Color3f surface_surface(Color3f tau) const override final;
    virtual Color3f surface_medium(Color3f tau) const override final;
    virtual Color3f medium_surface(Color3f tau) const override final;
    virtual Color3f medium_medium(Color3f tau) const override final;

    virtual Float sigma_bar() const override final;

    virtual Float sample_surface(Sampler *sampler) const override final;
    virtual Float sample_medium(Sampler *sampler) const override final;

    Float max_t;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Exponential Transmittance
/////////////////////////////////////////////////
class ExpTrans : public TransFunc
{
public:
    virtual Color3f surface_surface(Color3f tau) const override final;
    virtual Color3f surface_medium(Color3f tau) const override final;
    virtual Color3f medium_surface(Color3f tau) const override final;
    virtual Color3f medium_medium(Color3f tau) const override final;

    virtual Float sigma_bar() const override final;

    virtual Float sample_surface(Sampler *sampler) const override final;
    virtual Float sample_medium(Sampler *sampler) const override final;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Davis Model Transmittance
/////////////////////////////////////////////////
class DavisTrans : public TransFunc
{
public:
    DavisTrans(Float h, Float c);

    virtual Color3f surface_surface(Color3f tau) const override final;
    virtual Color3f surface_medium(Color3f tau) const override final;
    virtual Color3f medium_surface(Color3f tau) const override final;
    virtual Color3f medium_medium(Color3f tau) const override final;

    virtual Float sigma_bar() const override final;

    virtual Float sample_surface(Sampler *sampler) const override final;
    virtual Float sample_medium(Sampler *sampler) const override final;

    Float compute_alpha(Float tau) const;

    Float beta;
    Float c;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Trans Func Node structure
/////////////////////////////////////////////////
struct TransFuncNode : public Node
{
public:
    TransFuncNode() : trans_func(nullptr) {}
    TransFuncNode(std::string name) : Node(name), trans_func(nullptr) {}
    TransFuncNode(TransFunc *trans_func) : trans_func(trans_func) {}

    TransFunc *operator()() { return trans_func; }

    ~TransFuncNode() { delete trans_func; }

    TransFunc *trans_func;
};
/////////////////////////////////////////////////

FEIGN_END()
