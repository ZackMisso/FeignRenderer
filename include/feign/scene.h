#pragma once

#include <feign/common.h>
#include <feign/node.h>
#include <feign/accel/accel.h>
#include <feign/integrators/integrator.h>
#include <feign/emitters/emitter.h>
#include <feign/media/media.h>
#include <feign/samplers/sampler.h>
#include <feign/textures/texture.h>
#include <feign/cameras/camera.h>
#include <feign/misc/intersection.h>
#include <embree3/rtcore.h>
#include <embree3/rtcore_ray.h>
#include <embree3/rtcore_scene.h>

class Scene : public Node
{
public:
    Scene();
    Scene(Node* parent);
    ~Scene();

    virtual void preProcess();

    bool intersect(const Ray3f& ray, Intersection& its) const;

    // getter methods
    std::string getSceneName() const;
    Node* getRoot() const;
    std::vector<Node*> getSceneObjects() const;
    std::vector<Emitter*> getEmitters() const;
    Texture* getEnvMap() const;
    Integrator* getIntegrator() const;
    Media* getEnvMedium() const;
    Sampler* getSampler() const;
    Camera* getCamera() const;

    virtual NodeType getNodeType() const;
    virtual std::string getName() const;

    /* error reporting function */
    static void embree_error_handler(void* userPtr, const RTCError code, const char* str)
    {
        if (code == RTC_ERROR_NONE)
            return;

        printf("Embree: ");
        switch (code)
        {
            case RTC_ERROR_UNKNOWN          : printf("RTC_ERROR_UNKNOWN"); break;
            case RTC_ERROR_INVALID_ARGUMENT : printf("RTC_ERROR_INVALID_ARGUMENT"); break;
            case RTC_ERROR_INVALID_OPERATION: printf("RTC_ERROR_INVALID_OPERATION"); break;
            case RTC_ERROR_OUT_OF_MEMORY    : printf("RTC_ERROR_OUT_OF_MEMORY"); break;
            case RTC_ERROR_UNSUPPORTED_CPU  : printf("RTC_ERROR_UNSUPPORTED_CPU"); break;
            case RTC_ERROR_CANCELLED        : printf("RTC_ERROR_CANCELLED"); break;
            default                         : printf("invalid error code"); break;
        }
        if (str)
        {
            printf(" (");
            while (*str) putchar(*str++);
            printf(")\n");
        }
        exit(1);
    }

protected:
    // embree properties
    RTCScene scene = nullptr;
    RTCDevice device = nullptr;
    unsigned userGeomId;

    std::string sceneName;
    BBox3f sceneBounds;
    Node* root; // is this needed???
    Accel* acceleration;
    std::vector<Node*> sceneObjects; // keeping this for debugging purposes
    std::vector<Emitter*> emitters;
    Texture* envMap;
    Integrator* integrator;
    Media* envMedium;
    Sampler* sampler;
    Camera* camera;
};
