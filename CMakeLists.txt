cmake_minimum_required(VERSION 3.11)
project(LibJpegTurboCmakeTemplate)

##############################################################################
                    # DOWNLOAD AND BUILD LIBJPEG-TURBO
##############################################################################

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

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
ExternalProject_Add(LibJpegTurbo
    URL https://github.com/libjpeg-turbo/libjpeg-turbo/archive/refs/tags/3.0.4.zip
    PREFIX ${CMAKE_CURRENT_LIST_DIR}/turbojpeg
    CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_LIST_DIR}/turbojpeg
      -DCMAKE_BUILD_TYPE=Release
      -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
      -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
      -DENABLE_STATIC=${LIBJPEGTURBO_STATIC_LIBRARY_FLAG}
      -DENABLE_SHARED=${LIBJPEGTURBO_SHARED_LIBRARY_FLAG}
      -DENABLE_TESTING=OFF
)

# Delay finding the library until after LibJpegTurbo is built
ExternalProject_Get_Property(LibJpegTurbo install_dir)

# Specify include and library paths after the build of LibJpegTurbo
set(LIBJPEG_TURBO_INCLUDE_DIR ${install_dir}/include)
set(LIBJPEG_TURBO_LIB_DIR ${install_dir}/lib)

if (MSVC)
    file(GLOB LIBJPEG_TURBO_DLL "${install_dir}/bin/*.dll")
endif ()

if (BUILD_SHARED_LIBS)
    set(LIBJPEG_TURBO_LIB "${install_dir}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}turbojpeg${CMAKE_SHARED_LIBRARY_SUFFIX}")
else()
    set(LIBJPEG_TURBO_LIB "${install_dir}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}turbojpeg${CMAKE_STATIC_LIBRARY_SUFFIX}")
endif()

##############################################################################
                        # LINK WITH LIBJPEG-TURBO
##############################################################################

# Define the main project executable
add_executable(main main.cpp)

# Make sure main depends on LibJpegTurbo
add_dependencies(main LibJpegTurbo)

# Include libjpeg-turbo headers and link to the built library
target_include_directories(main PRIVATE ${LIBJPEG_TURBO_INCLUDE_DIR})
target_link_libraries(main ${LIBJPEG_TURBO_LIB})

# Copy DLLs to the output directory on windows if there are
if (MSVC)
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${LIBJPEG_TURBO_DLL}
        $<TARGET_FILE_DIR:main>)
endif ()