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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
  elseif (HAS_CPP11_FLAG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
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

    #ext/
)

if(APPLE)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -flto -Wno-unused-result -fno-strict-aliasing")
else()
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -flto -Wno-unused-result -fno-strict-aliasing -Wlto-type-mismatch")
endif()


add_executable(feign
    # Header files
    include/feign/common.h
    include/feign/exceptions.h

    # Core header files
    include/feign/core/scene.h
    include/feign/core/node.h
    include/feign/core/accel.h
    include/feign/core/api.h
    include/feign/core/bsdf.h
    include/feign/core/camera.h
    include/feign/core/emitter.h
    include/feign/core/recon_filter.h
    include/feign/core/integrator.h
    include/feign/core/sampler.h

    # Logger header files
    include/feign/log/logger.h

    # Material header files
    include/feign/materials/material.h

    # Math header files
    include/feign/math/transform.h
    include/feign/math/ray.h
    include/feign/math/vector.h
    include/feign/math/bbox.h
    include/feign/math/warp.h
    include/feign/math/coord_frame.h

    # Media header files
    include/feign/media/media.h

    # Media Density header files
    include/feign/media/density/density_func.h

    # Media Transmittance Estimator header files
    include/feign/media/trans_ests/trans_est.h

    # Misc header files
    include/feign/misc/intersection.h
    include/feign/misc/embree_util.h
    include/feign/misc/scene_generator.h

    # Parser header files
    include/feign/parser/json_parser.h

    # Phase Function header files
    include/feign/media/phase/phase.h

    # Shapes header files
    include/feign/shapes/shape.h
    include/feign/shapes/objmesh.h
    include/feign/shapes/triangle.h

    # Stats header files
    include/feign/stats/clocker.h

    # Texture header files
    include/feign/textures/texture.h

    # Tests header files
    include/tests/test.h
    include/tests/tester.h
    include/tests/vector_test.h
    include/tests/transform_test.h
    include/tests/matrix_test.h
    include/tests/ray_intersect_test.h
    include/tests/json_test.h

    # Implementation files
    src/main.cpp
    src/scene.cpp
    src/node.cpp

    # Accel implementation files
    src/accel/accel.cpp
    src/accel/naive.cpp
    src/accel/embree.cpp

    # Api implementation files
    src/api/api.cpp

    # BSDF implementation files
    src/bsdfs/bsdf.cpp
    src/bsdfs/diffuse.cpp

    # Camera implementation files
    src/cameras/camera.cpp
    src/cameras/perspective.cpp
    src/cameras/ortho.cpp

    # Emitter implementation files
    src/emitters/emitter.cpp
    src/emitters/point.cpp
    src/emitters/distant.cpp
    src/emitters/directional.cpp
    src/emitters/environment.cpp

    # Filter implementation files
    src/filters/recon_filter.cpp
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

    # Logger implementation files
    src/log/logger.cpp

    # Material implementation files
    src/materials/material.cpp

    # Math implementation files
    src/math/transform.cpp
    src/math/ray.cpp
    src/math/warp.cpp
    src/math/coord_frame.cpp

    # Media implementation files
    src/media/media.cpp

    # Media Density implementation files

    # Media Transmittance Estimator implementation files

    # Phase Function implementation files
    src/media/phase/phase.cpp

    # Misc implementation files
    src/misc/intersection.cpp

    # Parser implementation files
    src/parser/json_parser.cpp

    # Sampler implementation files
    src/samplers/sampler.cpp
    src/samplers/independent.cpp
    src/samplers/latin.cpp
    src/samplers/halton.cpp

    # Shapes implementation files
    src/shapes/shape.cpp
    src/shapes/objmesh.cpp
    src/shapes/triangle.cpp

    # Stats implementation files
    src/stats/clocker.cpp

    # Texture implementation files
    src/textures/texture.cpp

    # Test implementation files
    src/tests/tester.cpp
    src/tests/vector_test.cpp
    src/tests/transform_test.cpp
    src/tests/matrix_test.cpp
    src/tests/ray_intersect_test.cpp
    src/tests/json_test.cpp

    # imedit implementation files
    ext/ImageEditLib/src/image.cpp
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
#target_link_libraries(feign tbb openvdb embree3 ${PYTHON_LIBRARIES})
target_link_libraries(feign tbb embree3 ${PYTHON_LIBRARIES})
