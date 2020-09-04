#include <feign/core/api.h>

FEIGN_BEGIN()

class VolExpShaderToy
{
protected:
    void initialize_lighting(int frame);
    void initialize_media(int frame);
    void initialize_camera(int frame);
    void initialize_base_structs(std::string test_name, int frame);

    void flush_render();
    void run();

public:
    VolExpShaderToy();
};

FEIGN_END()
