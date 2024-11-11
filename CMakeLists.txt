cmake_minimum_required(VERSION 3.11)
project(MyProject)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Optionally build shared or static libraries based on the BUILD_SHARED_LIBS flag
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

if (BUILD_SHARED_LIBS)
    set(LIBJPEGTURBO_STATIC_LIBRARY_FLAG OFF)
    set(LIBJPEGTURBO_SHARED_LIBRARY_FLAG ON)
else()
    set(LIBJPEGTURBO_STATIC_LIBRARY_FLAG ON)
    set(LIBJPEGTURBO_SHARED_LIBRARY_FLAG OFF)
endif()

# Include the ExternalProject module
include(ExternalProject)

# Define the libjpeg-turbo external project
ExternalProject_Add(JpegTurbo
    URL https://github.com/libjpeg-turbo/libjpeg-turbo/archive/refs/tags/3.0.4.zip
    PREFIX ${CMAKE_CURRENT_LIST_DIR}/turbojpeg
    CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_LIST_DIR}/turbojpeg
      -DCMAKE_BUILD_TYPE=Release
      -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
      -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
      -DENABLE_STATIC=${LIBJPEGTURBO_STATIC_LIBRARY_FLAG}
      -DENABLE_SHARED=${LIBJPEGTURBO_SHARED_LIBRARY_FLAG}
)

# Specify include and library paths after the build of JpegTurbo
set(LIBJPEG_TURBO_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/turbojpeg/include)
set(LIBJPEG_TURBO_LIB_DIR ${CMAKE_CURRENT_LIST_DIR}/turbojpeg/lib)

# Define the main project executable
add_executable(MyProject main.cpp)

# Make sure MyProject depends on JpegTurbo
add_dependencies(MyProject JpegTurbo)

# Delay finding the library until after JpegTurbo is built
ExternalProject_Get_Property(JpegTurbo install_dir)

if (BUILD_SHARED_LIBS)
    set(LIBJPEG_TURBO_LIB "${install_dir}/lib/libturbojpeg.so")   # Shared TurboJPEG library
else()
    set(LIBJPEG_TURBO_LIB "${install_dir}/lib/libturbojpeg.a")    # Static TurboJPEG library
endif()

# Include libjpeg-turbo headers and link to the built library
target_include_directories(MyProject PRIVATE ${LIBJPEG_TURBO_INCLUDE_DIR})
target_link_libraries(MyProject ${LIBJPEG_TURBO_LIB})
