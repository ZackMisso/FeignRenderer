#include <feign/core/api.h>

class MediumTesting_Debug
{
protected:
    void initialize_materials(int frame);
    void initialize_hallway(int frame);
    void initialize_scene(int frame);
    void initialize_camera(int frame);
    void initialize_base_structs(std::string test_name, int frame);
    void initialize_lighting(int frame);
    void initialize_medium(int frame);
    void flush_render();
    void run();

public:
    MediumTesting_Debug();
};
