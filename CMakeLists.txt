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

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

include_directories(
    # Include directory
    ${CMAKE_CURRENT_SOURCE_DIR}/include/

    # Random number generator
    ${PCG32_INCLUDE_DIR}/

    # Programatical image editing library
    ${IMGLIB_INCLUDE_DIR}/

    ${IMGLIB_NEEDED_DIR}/

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
    include/feign/scene.h
    include/feign/node.h
    include/feign/exceptions.h

    # Accel header files
    include/feign/accel/accel.h
    include/feign/accel/bbh.h
    include/feign/accel/naive.h

    # BSDF header files
    include/feign/bsdfs/bsdf.h
    include/feign/bsdfs/diffuse.h

    # Camera header files
    include/feign/cameras/camera.h
    include/feign/cameras/perspective.h
    include/feign/cameras/ortho.h

    # Emitter header files
    include/feign/emitters/emitter.h

    # Filter header files
    include/feign/filters/recon_filter.h
    include/feign/filters/box_filter.h
    include/feign/filters/gauss_filter.h

    # Integrator header files
    include/feign/integrators/integrator.h
    include/feign/integrators/normal.h

    # Logger header files
    include/feign/log/logger.h

    # Math header files
    include/feign/math/transform.h
    include/feign/math/ray.h
    include/feign/math/vector.h
    include/feign/math/bbox.h
    include/feign/math/warp.h
    include/feign/math/coord_frame.h

    # Media header files
    include/feign/media/media.h

    # Misc header files
    include/feign/misc/prim.h
    include/feign/misc/primlist.h
    include/feign/misc/world.h
    include/feign/misc/transformnode.h
    include/feign/misc/intersection.h

    # Parser header files
    include/feign/parser/parser.h

    # Phase Function header files
    include/feign/phase/phase.h

    # Sampler header files
    include/feign/samplers/sampler.h
    include/feign/samplers/independent.h

    # Shapes header files
    include/feign/shapes/shape.h
    include/feign/shapes/objmesh.h
    include/feign/shapes/triangle.h

    # Texture header files
    include/feign/textures/texture.h

    # Tests header files
    include/tests/test.h
    include/tests/tester.h
    include/tests/vector_test.h
    include/tests/transform_test.h
    include/tests/matrix_test.h
    include/tests/ray_intersect_test.h

    # Implementation files
    src/main.cpp
    src/scene.cpp
    src/node.cpp

    # Accel implementation files
    src/accel/accel.cpp
    src/accel/bbh.cpp
    src/accel/naive.cpp

    # BSDF implementation files
    src/bsdfs/bsdf.cpp
    src/bsdfs/diffuse.cpp

    # Camera implementation files
    src/cameras/camera.cpp
    src/cameras/perspective.cpp
    src/cameras/ortho.cpp

    # Emitter implementation files
    src/emitters/emitter.cpp

    # Filter implementation files
    src/filters/recon_filter.cpp
    src/filters/box_filter.cpp
    src/filters/gauss_filter.cpp

    # Integrator implementation files
    src/integrators/integrator.cpp
    src/integrators/normal.cpp

    # Logger implementation files
    src/log/logger.cpp

    # Math implementation files
    src/math/transform.cpp
    src/math/ray.cpp
    src/math/warp.cpp
    src/math/coord_frame.cpp

    # Media implementation files
    src/media/media.cpp

    # Misc implementation files
    src/misc/primlist.cpp
    src/misc/world.cpp
    src/misc/transformnode.cpp
    src/misc/intersection.cpp

    # Parser implementation files
    src/parser/parser.cpp

    # Phase Function implementation files
    src/phase/phase.cpp

    # Sampler implementation files
    src/samplers/sampler.cpp
    src/samplers/independent.cpp

    # Shapes implementation files
    src/shapes/shape.cpp
    src/shapes/objmesh.cpp
    src/shapes/triangle.cpp

    # Texture implementation files
    src/textures/texture.cpp

    # Test implementation files
    src/tests/tester.cpp
    src/tests/vector_test.cpp
    src/tests/transform_test.cpp
    src/tests/matrix_test.cpp
    src/tests/ray_intersect_test.cpp
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
)

# Lastly, additional libraries may have been built for you.  In addition to linking
# against NanoGUI, we need to link against those as well.
#target_link_libraries(feign)
target_link_libraries(feign ${PYTHON_LIBRARIES})
