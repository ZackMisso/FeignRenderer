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
                 bool startOnSurface,
                 bool endOnSurface) const;

    Float sample(Sampler *sampler, bool startOnSurface) const;

    inline Color3f surfaceProbability(Color3f tau, bool startOnSurface) const;
    inline Color3f mediumPdf(Color3f tau, bool startOnSurface) const;

    virtual bool isDirac() const { return false; }

    virtual Float effectiveSigma() const { return sigmaBar(); }

    virtual Color3f surfaceSurface(Color3f tau) const = 0;
    virtual Color3f surfaceMedium(Color3f tau) const = 0;
    virtual Color3f mediumSurface(Color3f tau) const = 0;
    virtual Color3f mediumMedium(Color3f tau) const = 0;

    virtual Float sigmaBar() const = 0;

    virtual Float sampleSurface(Sampler *sampler) const = 0;
    virtual Float sampleMedium(Sampler *sampler) const = 0;
};

/////////////////////////////////////////////////
// Linear Transmittance
/////////////////////////////////////////////////
class LinearTrans : public TransFunc
{
public:
    struct Params
    {
        Params() : maxT(1.f) {}
        Params(Float maxT) : maxT(maxT) {}

        Float maxT;
    };

    LinearTrans(Float maxT);

    virtual Color3f surfaceSurface(Color3f tau) const override final;
    virtual Color3f surfaceMedium(Color3f tau) const override final;
    virtual Color3f mediumSurface(Color3f tau) const override final;
    virtual Color3f mediumMedium(Color3f tau) const override final;

    virtual Float sigmaBar() const override final;

    virtual Float sampleSurface(Sampler *sampler) const override final;
    virtual Float sampleMedium(Sampler *sampler) const override final;

    Float maxT;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Exponential Transmittance
/////////////////////////////////////////////////
class ExpTrans : public TransFunc
{
public:
    virtual Color3f surfaceSurface(Color3f tau) const override final;
    virtual Color3f surfaceMedium(Color3f tau) const override final;
    virtual Color3f mediumSurface(Color3f tau) const override final;
    virtual Color3f mediumMedium(Color3f tau) const override final;

    virtual Float sigmaBar() const override final;

    virtual Float sampleSurface(Sampler *sampler) const override final;
    virtual Float sampleMedium(Sampler *sampler) const override final;
};
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Davis Model Transmittance
/////////////////////////////////////////////////
class DavisTrans : public TransFunc
{
public:
    DavisTrans(Float h, Float c);

    virtual Color3f surfaceSurface(Color3f tau) const override final;
    virtual Color3f surfaceMedium(Color3f tau) const override final;
    virtual Color3f mediumSurface(Color3f tau) const override final;
    virtual Color3f mediumMedium(Color3f tau) const override final;

    virtual Float sigmaBar() const override final;

    virtual Float sampleSurface(Sampler *sampler) const override final;
    virtual Float sampleMedium(Sampler *sampler) const override final;

    Float computeAlpha(Float tau) const;

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
