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
    file(GLOB LIBJPEG_TURBO_DLL "${install_dir}/bin/*.dll" "${install_dir}/lib/*.dll")
endif ()

if (BUILD_SHARED_LIBS)
    if(WIN32 AND NOT MSVC)
        set(LIBJPEG_TURBO_LIB "${LIBJPEG_TURBO_LIB_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}turbojpeg${CMAKE_SHARED_LIBRARY_SUFFIX}.a")
    elseif(MSVC)
        set(LIBJPEG_TURBO_LIB "${LIBJPEG_TURBO_LIB_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}turbojpeg.lib")
    else()
        set(LIBJPEG_TURBO_LIB "${LIBJPEG_TURBO_LIB_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}turbojpeg${CMAKE_SHARED_LIBRARY_SUFFIX}")
    endif()
else()
    if (MSVC)
        set(LIBJPEG_TURBO_LIB "${LIBJPEG_TURBO_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}turbojpeg-static${CMAKE_STATIC_LIBRARY_SUFFIX}")
    else()
        set(LIBJPEG_TURBO_LIB "${LIBJPEG_TURBO_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}turbojpeg${CMAKE_STATIC_LIBRARY_SUFFIX}")
    endif()
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
if (WIN32)
    add_custom_command(TARGET main POST_BUILD
        COMMENT "Copying DLL files to main bin directory"

        # copy needed dll file to main binary path
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${install_dir}/bin $<TARGET_FILE_DIR:main>

        # remove not needed binaries
        COMMAND ${CMAKE_COMMAND} -E rm $<TARGET_FILE_DIR:main>/cjpeg${CMAKE_EXECUTABLE_SUFFIX}
        COMMAND ${CMAKE_COMMAND} -E rm $<TARGET_FILE_DIR:main>/djpeg${CMAKE_EXECUTABLE_SUFFIX}
        COMMAND ${CMAKE_COMMAND} -E rm $<TARGET_FILE_DIR:main>/jpegtran${CMAKE_EXECUTABLE_SUFFIX}
        COMMAND ${CMAKE_COMMAND} -E rm $<TARGET_FILE_DIR:main>/tjbench${CMAKE_EXECUTABLE_SUFFIX}
        COMMAND ${CMAKE_COMMAND} -E rm $<TARGET_FILE_DIR:main>/rdjpgcom${CMAKE_EXECUTABLE_SUFFIX}
        COMMAND ${CMAKE_COMMAND} -E rm $<TARGET_FILE_DIR:main>/wrjpgcom${CMAKE_EXECUTABLE_SUFFIX}
    )
endif ()

include(CMakePrintHelpers)
message("============================================================")
message("                      VARIABLES")
message("============================================================")
cmake_print_variables(LIBJPEG_TURBO_LIB)
cmake_print_variables(LIBJPEG_TURBO_DLL)
cmake_print_variables(CMAKE_SHARED_LINKER_FLAGS)
cmake_print_variables(CMAKE_SHARED_LIBRARY_PREFIX)
cmake_print_variables(CMAKE_SHARED_LIBRARY_SUFFIX)
cmake_print_variables(CMAKE_SHARED_MODULE_PREFIX)
cmake_print_variables(CMAKE_SHARED_MODULE_SUFFIX)
cmake_print_variables(CMAKE_EXTRA_SHARED_LIBRARY_SUFFIXES)
cmake_print_variables(CMAKE_EXECUTABLE_SUFFIX)
message("============================================================")

add_custom_command(TARGET main POST_BUILD
    COMMENT "TESTING COMMENT"
    COMMAND ${CMAKE_COMMAND} -E echo "MAIN TARGET POST BUILD"
    COMMAND ${CMAKE_COMMAND} -E echo "LIBJPEG_TURBO_DLL = ${LIBJPEG_TURBO_DLL}"
)