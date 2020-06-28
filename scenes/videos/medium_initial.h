#include <feign/core/api.h>

// plan for volumetric media video:
//
// 10 seconds of fully absorbing global media
// 10 seconds of bounded fully absorbing media
// 10 seconds of absorbing + scattering global media
// 10 seconds of absorbing + scattering bounded media
// 20 seconds of varying absorbing to scattering for a global medium
// 20 seconds of varying absorbing to scattering for a bounded medium
// 30 seconds final medium show off
//

class MediumTesting_Debug
{
protected:
    void initialize_materials(int frame);
    void initialize_hallway(int frame);
    void initialize_scene(int frame);
    void initialize_grid_scene(int frame);
    void initialize_camera(int frame);
    void initialize_base_structs(std::string test_name, int frame);
    void initialize_lighting(int frame);
    void initialize_box_medium(int frame);
    void initialize_sphere_medium(int frame);

    void initialize_initial_scene(int frame);
    void initialize_homo_sphere_medium_vary_scatter(int frame);
    void initialize_homo_cornell_scene_vary_phase(int frame);
    void initialize_hetero_smoke_vary_scatter(int frame);

    void initialize_fully_absorbing_global_media(int frame);
    void initialize_bounded_fully_absorbing_media(int frame);
    void initialize_absorb_scat_global_media(int frame);
    void initialize_absorb_scat_bounded_media(int frame);
    void initialize_varying_absorb_scat_global_media(int frame);
    void initialize_varying_absorb_scat_bounded_media(int frame);
    void initialize_final_media(int frame);

    void flush_render();
    void run();

public:
    MediumTesting_Debug();
};
