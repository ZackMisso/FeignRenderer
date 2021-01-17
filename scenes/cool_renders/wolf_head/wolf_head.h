#include <feign/core/api.h>

FEIGN_BEGIN()

class WolfHead
{
protected:
    void initialize_materials(int frame);
    void initialize_grid_scene_2(int frame);
    void initialize_camera_2(int frame);
    void initialize_base_structs(std::string test_name, int frame);
    void initialize_lighting(int frame);
    // void initialize_sphere_medium(int frame);

    void initialize_initial_scene(int frame, bool verbose);
    // void initialize_colored_scene(int frame, bool verbose);
    // void initialize_homo_sphere_medium_vary_scatter(int frame);
    // void initialize_homo_cornell_scene_vary_phase(int frame);
    // void initialize_hetero_smoke_vary_scatter(int frame);

    // void initialize_fully_absorbing_global_media(int frame);
    // void initialize_bounded_fully_absorbing_media(int frame);
    // void initialize_absorb_scat_global_media(int frame);
    // void initialize_absorb_scat_bounded_media(int frame);
    // void initialize_varying_absorb_scat_global_media(int frame);
    // void initialize_varying_absorb_scat_bounded_media(int frame);
    // void initialize_final_media(int frame);

    void flush_render();
    void run();

public:
    WolfHead();
};

FEIGN_END()
