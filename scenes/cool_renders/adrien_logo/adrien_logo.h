#include <feign/core/api.h>

FEIGN_BEGIN()

class AdrienLogo
{
protected:
    void initialize_materials(int frame);
    void initialize_grid_scene_2(int frame);
    void initialize_camera_2(int frame);
    void initialize_base_structs(std::string test_name, int frame);
    void initialize_lighting(int frame);

    void initialize_initial_scene(int frame, bool verbose);

    void flush_render();
    void run();

public:
    AdrienLogo();
};

FEIGN_END()
