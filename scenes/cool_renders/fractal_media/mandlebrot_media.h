#include <feign/core/api.h>

FEIGN_BEGIN()

class MandlebrotMediaTests
{
protected:
    void initialize_materials(int frame);
    // void initialize_hallway(int frame);
    // void initialize_scene(int frame);
    void initialize_grid_scene(int frame);
    void initialize_camera(int frame);
    void initialize_base_structs(std::string test_name, int frame);
    void initialize_lighting(int frame);
    // void initialize_box_medium(int frame);
    // void initialize_sphere_medium(int frame);

    // void initialize_initial_scene(int frame, bool verbose);
    // void initialize_colored_scene(int frame, bool verbose);
    void initialize_homo_sphere_medium_vary_scatter(int frame);

    void flush_render();
    void run();

public:
    MandlebrotMediaTests();
};

FEIGN_END()
