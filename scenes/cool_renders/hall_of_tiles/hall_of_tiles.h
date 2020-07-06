#include <feign/core/api.h>

FEIGN_BEGIN()

class HallOfTiles
{
protected:
    void initialize_materials(int frame);
    void initialize_hallway(int frame);
    void initialize_camera(int frame);
    void initialize_base_structs(std::string test_name, int frame);
    void initialize_lighting(int frame);
    void flush_render();
    void run();

public:
    HallOfTiles();
};

FEIGN_END()
