cmake_minimum_required (VERSION 2.8.3)
project(feign)

if(APPLE)
    set(CMAKE_MACOSX_RPATH ON)
endif()

include(CheckCXXCompilerFlag)
include(CheckCXXSourceRuns)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # Quench annoying deprecation warnings when compiling GLFW on OSX
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-declarations")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|Clang|Intel)$")
  CHECK_CXX_COMPILER_FLAG("-std=c++14" HAS_CPP14_FLAG)
  CHECK_CXX_COMPILER_FLAG("-std=c++11" HAS_CPP11_FLAG)

  if (HAS_CPP14_FLAG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread -O3")
  elseif (HAS_CPP11_FLAG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")
  else()
    message(FATAL_ERROR "Unsupported compiler -- pybind11 requires C++11 support!")
  endif()
endif()

IF(APPLE)
    # Fix linking on 10.14+. See https://stackoverflow.com/questions/54068035
    LINK_DIRECTORIES(/usr/local/lib)
ENDIF()

if (APPLE)
  find_library(cocoa_library Cocoa)
  find_library(opengl_library OpenGL)
  find_library(corevideo_library CoreVideo)
  find_library(iokit_library IOKit)
  list(APPEND NANOGUI_EXTRA_LIBS ${cocoa_library} ${opengl_library} ${corevideo_library} ${iokit_library})
  list(APPEND LIBNANOGUI_EXTRA_SOURCE src/darwin.mm)
 endif()

#add_subdirectory(ext/ImageEditLib)

set(PCG32_INCLUDE_DIR
   ${CMAKE_CURRENT_SOURCE_DIR}/ext/pcg32)

set(IMGLIB_INCLUDE_DIR
   ${CMAKE_CURRENT_SOURCE_DIR}/ext/ImageEditLib/include)

set(IMGLIB_NEEDED_DIR
   ${CMAKE_CURRENT_SOURCE_DIR}/ext/ImageEditLib/ext)

set(JSON_INCLUDE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/ext/rapidjson/include)

set(TINYEXR_INCLUDE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/ext/ImageEditLib/ext/tinyexr)

set(OPENVDB_INCLUDE_DIR
   /usr/local/Cellar/openvdb/7.0.0_2/include/)

set(OPENEXR_INCLUDE_DIR
   /usr/local/include/)

find_package(PythonLibs REQUIRED)
# find_package(openvdb REQUIRED)
find_package(embree 3.0 REQUIRED)

include_directories(${PYTHON_INCLUDE_DIRS})

include_directories(
    # Include directory
    ${CMAKE_CURRENT_SOURCE_DIR}/include/

    # Random number generator
    ${PCG32_INCLUDE_DIR}/

    # Programatical image editing library
    ${IMGLIB_INCLUDE_DIR}/

    ${IMGLIB_NEEDED_DIR}/

    ${JSON_INCLUDE_DIR}

    ${TINYEXR_INCLUDE_DIR}

    ${OPENVDB_INCLUDE_DIR}

    ${OPENEXR_INCLUDE_DIR}

    #ext/
)

if(APPLE)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -flto -Wno-unused-result -fno-strict-aliasing")
else()
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -flto -Wno-unused-result -fno-strict-aliasing -Wlto-type-mismatch")
endif()

# TODO: it would probably just be nicer to clean this up a bit instead of listing
#       all of the files out. Sure it makes compiling faster instead of using .*,
#       but it just looks gross.
# all of these files are listed out instead of using * notation because
# it avoids having to rebuild the entire project when you add a new file
add_executable(feign
    # Header files
    include/feign/common.h

    # Core header files
    include/feign/core/scene.h
    include/feign/core/node.h
    include/feign/core/accel_ray.h
    include/feign/core/accel_light.h
    include/feign/core/accel_photons.h
    include/feign/core/api.h
    include/feign/core/bsdf.h
    include/feign/core/camera.h
    include/feign/core/closure.h
    include/feign/core/emitter.h
    include/feign/core/recon_filter.h
    include/feign/core/integrator.h
    include/feign/core/sampler.h
    include/feign/core/material.h
    include/feign/core/shape.h
    include/feign/core/sdf.h
    include/feign/core/shader.h
    include/feign/core/texture.h

    # Math header files
    include/feign/math/transform.h
    include/feign/math/ray.h
    include/feign/math/vector.h
    include/feign/math/bbox.h
    include/feign/math/warp.h
    include/feign/math/coord_frame.h
    include/feign/math/functor.h
    include/feign/math/discrete_pdf.h

    # Media header files
    include/feign/media/media.h
    include/feign/media/density_func.h
    include/feign/media/phase.h
    include/feign/media/sampling.h
    include/feign/media/trans_est.h

    # Misc header files
    include/feign/misc/intersection.h
    include/feign/misc/embree_util.h
    include/feign/misc/render_pool.h

    # Parser header files
    include/feign/parser/json_parser.h

    # Shapes header files
    include/feign/shapes/objmesh.h
    include/feign/shapes/triangle.h
    include/feign/shapes/grid_obj.h

    # Stats header files
    include/feign/stats/clocker.h

    # Tests header files
    include/feign/test/tester.h

    # Implementation files
    src/main.cpp
    src/scene.cpp

    # LightAccel implementation files
    src/accel/light_naive.cpp
    src/accel/light_spatial.cpp

    # PhotonAccel implementation files
    src/accel/photon_array.cpp
    src/accel/photon_bvh.cpp

    # Api implementation files
    src/api/api.cpp

    # BSDF implementation files
    src/bsdfs/diffuse.cpp
    src/bsdfs/mirror.cpp
    src/bsdfs/null.cpp
    src/bsdfs/dielectric.cpp
    src/bsdfs/phong.cpp
    src/bsdfs/blinn.cpp

    # RayAccel implementation files
    src/accel/ray_embree.cpp
    src/accel/ray_sdf.cpp

    # Camera implementation files
    src/cameras/perspective.cpp
    src/cameras/ortho.cpp

    # Closure implementation files
    src/closure/material.cpp
    src/closure/photon.cpp

    # Emitter implementation files
    src/emitters/point.cpp
    src/emitters/distant.cpp
    src/emitters/directional.cpp
    src/emitters/spot.cpp
    src/emitters/environment.cpp
    src/emitters/mesh.cpp
    src/emitters/interp_env.cpp

    # Filter implementation files
    src/filters/box_filter.cpp
    src/filters/gauss_filter.cpp

    # Integrator implementation files
    src/integrators/amb_occ.cpp
    src/integrators/integrator.cpp
    src/integrators/normal.cpp
    src/integrators/light_unidir.cpp
    src/integrators/path_bidir.cpp
    src/integrators/path_unidir.cpp
    src/integrators/whitted.cpp
    src/integrators/cosine_term.cpp
    src/integrators/barycentric.cpp
    src/integrators/nice_normal.cpp
    src/integrators/volpath.cpp
    src/integrators/volpath_trans.cpp
    src/integrators/sandbox.cpp
    src/integrators/photon.cpp

    # Material implementation files
    src/materials/simple.cpp

    # Math implementation files
    src/math/transform.cpp
    src/math/ray.cpp
    src/math/warp.cpp
    src/math/coord_frame.cpp
    src/math/discrete_pdf.cpp

    # Media implementation files
    src/media/media.cpp
    src/media/homo_abs.cpp
    src/media/phase.cpp
    src/media/standard.cpp
    src/media/trans_ests/delta.cpp
    src/media/trans_ests/pseries_cmf.cpp
    src/media/trans_ests/ratio.cpp
    src/media/density_funcs/point_average.cpp
    src/media/density_funcs/vdb_density.cpp

    # Misc implementation files
    src/misc/intersection.cpp
    src/misc/render_pool.cpp

    # Parser implementation files
    src/parser/json_parser.cpp

    # Sampler implementation files
    src/samplers/sampler.cpp
    src/samplers/independent.cpp
    src/samplers/latin.cpp
    src/samplers/halton.cpp

    # Shader implementation files
    src/shaders/interp_verts_to_sphere.cpp
    src/shaders/simple_material.cpp
    src/shaders/radar_material.cpp
    src/shaders/wireframe_material.cpp

    # Shapes implementation files
    src/shapes/shape.cpp
    src/shapes/objmesh.cpp
    src/shapes/triangle.cpp
    src/shapes/grid_obj.cpp
    src/shapes/sdf_sphere.cpp
    src/shapes/sdf_box.cpp
    src/shapes/sdf_plane.cpp
    src/shapes/sdf_cylinder.cpp
    src/shapes/sdf_cone.cpp

    # Stats implementation files
    src/stats/clocker.cpp

    # Texture implementation files
    src/textures/image_texture.cpp
    src/textures/noise_texture.cpp
    src/textures/texture.cpp
    src/textures/sin_texture.cpp

    # Test implementation files
    src/test/test.cpp
    src/test/tester.cpp

    # imedit implementation files
    ext/ImageEditLib/src/image.cpp

    # scene implementation files
    scenes/cool_renders/hall_of_tiles/hall_of_tiles.cpp
    scenes/cool_renders/fractal_media/mandlebrot_media.cpp

    # debug scene implementation files
    scenes/videos/medium_initial.cpp

    scenes/shadertoy_recreations/vol_exp/vol_exp.cpp
)

#OPENEXR_INCLUDE_DIRS
set(CompilerFlags
      CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO CMAKE_C_FLAGS
      CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_MINSIZEREL
      CMAKE_C_FLAGS_RELWITHDEBINFO COMPILE_DEFINITIONS U_CMAKE_BUILD_TYPE
      CMAKE_MACOSX_RPATH
      PCG32_INCLUDE_DIR
      IMGLIB_INCLUDE_DIR
      TINYEXR_INCLUDE_DIR
)

# Lastly, additional libraries may have been built for you.  In addition to linking
# against NanoGUI, we need to link against those as well.
#target_link_libraries(feign)
# TODO: get openvdb support running
target_link_libraries(feign tbb openvdb embree3 half ${PYTHON_LIBRARIES})
#target_link_libraries(feign tbb embree3 ${PYTHON_LIBRARIES})
