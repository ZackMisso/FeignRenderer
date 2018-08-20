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

# Disable building extras we won't need (pure C++ project)
set(NANOGUI_BUILD_EXAMPLE OFF CACHE BOOL " " FORCE)
set(NANOGUI_BUILD_PYTHON  OFF CACHE BOOL " " FORCE)
set(NANOGUI_INSTALL       OFF CACHE BOOL " " FORCE)

add_subdirectory(ext/nanogui)

#set(ILMBASE_BUILD_SHARED_LIBS OFF CACHE BOOL " " FORCE)
#set(OPENEXR_BUILD_SHARED_LIBS OFF CACHE BOOL " " FORCE)
#set(ILMBASE_NAMESPACE_VERSIONING OFF CACHE BOOL " " FORCE)
#set(OPENEXR_NAMESPACE_VERSIONING OFF CACHE BOOL " " FORCE)

#add_subdirectory(ext/openexr)
#set_property(TARGET IexMath eLut toFloat b44ExpLogTable dwaLookups CopyIlmBaseLibs IlmThread Half Iex Imath IlmImf PROPERTY FOLDER "dependencies")

#set(OPENEXR_INCLUDE_DIRS
#${CMAKE_CURRENT_SOURCE_DIR}/ext/openexr/IlmBase/Imath
#${CMAKE_CURRENT_SOURCE_DIR}/ext/openexr/IlmBase/Iex
#${CMAKE_CURRENT_SOURCE_DIR}/ext/openexr/IlmBase/Half
#${CMAKE_CURRENT_SOURCE_DIR}/ext/openexr/OpenEXR/IlmImf
#${CMAKE_CURRENT_SOURCE_DIR}/ext/openexr/OpenEXR/config
#${CMAKE_CURRENT_SOURCE_DIR}/ext/openexr/IlmBase/config)

set(GLFW_INCLUDE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/ext/nanogui/ext/glfw/include)
set(GLEW_INCLUDE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/ext/nanogui/ext/glew/include)
set(NANOVG_INCLUDE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/ext/nanogui/ext/nanovg/src)
set(NANOGUI_INCLUDE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/ext/nanogui/include)
set(EIGEN_INCLUDE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/ext/nanogui/ext/eigen)

set(PCG32_INCLUDE_DIR
   ${CMAKE_CURRENT_SOURCE_DIR}/ext/pcg32)

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
# target_link_libraries(<your exe or lib> ${PYTHON_LIBRARIES})

# On top of adding the path to nanogui/include, you may need extras
include_directories(
    # Include directory
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    # Random number generator
    ${PCG32_INCLUDE_DIR}/
    # GLFW library for OpenGL context creation
    ${GLFW_INCLUDE_DIR}
    # GLEW library for accessing OpenGL functions
    ${GLEW_INCLUDE_DIR}
    # NanoVG drawing library
    ${NANOVG_INCLUDE_DIR}
    # NanoGUI user interface library
    ${NANOGUI_INCLUDE_DIR}
    ${NANOGUI_EXTRA_INCS}
    # OpenEXR high dynamic range bitmap library
    ${OPENEXR_INCLUDE_DIRS}
    ext
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

    # BSDF header files
    include/feign/bsdfs/bsdf.h
    include/feign/bsdfs/diffuse.h

    # Camera header files
    include/feign/cameras/camera.h
    include/feign/cameras/perspective.h

    # Emitter header files
    include/feign/emitters/emitter.h

    # Integrator header files
    include/feign/integrators/integrator.h
    include/feign/integrators/normal.h

    # Math header files
    include/feign/math/transform.h

    # Media header files
    include/feign/media/media.h

    # Misc header files
    include/feign/misc/prim.h
    include/feign/misc/primlist.h
    include/feign/misc/world.h

    # Parser header files
    include/feign/parser/parser.h
    include/feign/parser/token.h

    # Phase Function header files
    include/feign/phase/phase.h

    # Sampler header files
    include/feign/samplers/sampler.h
    include/feign/samplers/independent.h

    # Shapes header files
    include/feign/shapes/shape.h
    include/feign/shapes/objmesh.h

    # Texture header files
    include/feign/textures/texture.h

    # Tests header files
    include/tests/test.h
    include/tests/tester.h

    # Implementation files
    src/main.cpp
    src/scene.cpp
    src/node.cpp

    # BSDF implementation files
    src/bsdfs/bsdf.cpp
    src/bsdfs/diffuse.cpp

    # Camera implementation files
    src/cameras/camera.cpp
    src/cameras/perspective.cpp

    # Emitter implementation files
    src/emitters/emitter.cpp

    # Integrator implementation files
    src/integrators/integrator.cpp
    src/integrators/normal.cpp

    # Math implementation files
    src/math/transform.cpp

    # Media implementation files
    src/media/media.cpp

    # Misc implementation files
    src/misc/prim.cpp
    src/misc/primlist.cpp
    src/misc/world.cpp

    # Parser implementation files
    src/parser/parser.cpp
    src/parser/token.cpp

    # Phase Function implementation files
    src/phase/phase.cpp

    # Sampler implementation files
    src/samplers/sampler.cpp
    src/samplers/independent.cpp

    # Shapes implementation files
    src/shapes/shape.cpp
    src/shapes/objmesh.cpp

    # Texture implementation files
    src/textures/texture.cpp

    # Test implementation files
    src/tests/tester.cpp
)

#OPENEXR_INCLUDE_DIRS
set(CompilerFlags
      CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO CMAKE_C_FLAGS
      CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_MINSIZEREL
      CMAKE_C_FLAGS_RELWITHDEBINFO COMPILE_DEFINITIONS U_CMAKE_BUILD_TYPE
      CMAKE_MACOSX_RPATH
       PCG32_INCLUDE_DIR
      GLFW_INCLUDE_DIR GLEW_INCLUDE_DIR
      NANOVG_INCLUDE_DIR NANOGUI_INCLUDE_DIR EIGEN_INCLUDE_DIR
      NANOGUI_EXTRA_LIBS
)

# Lastly, additional libraries may have been built for you.  In addition to linking
# against NanoGUI, we need to link against those as well.
#target_link_libraries(feign IlmImf nanogui ${NANOGUI_EXTRA_LIBS})
target_link_libraries(feign nanogui ${NANOGUI_EXTRA_LIBS} ${PYTHON_LIBRARIES})
