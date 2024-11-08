cmake_minimum_required(VERSION 3.18)
project(libjpeg_cmake_template LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(ExternalProject)

# Define the installation prefix for libjpeg-turbo
set(LIBJPEG_TURBO_INSTALL_DIR "${CMAKE_BINARY_DIR}/libjpegturbo_install_dir")

# Download and build libjpeg-turbo with both shared and static enabled
ExternalProject_Add(JpegTurbo
    URL https://github.com/libjpeg-turbo/libjpeg-turbo/archive/refs/tags/3.0.4.zip
    # PREFIX ${CMAKE_CURRENT_BINARY_DIR}/libjpegturbo
    CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX=${LIBJPEG_TURBO_INSTALL_DIR}
      -DENABLE_STATIC=ON
      -DENABLE_SHARED=ON
)

# Set the include directory for libjpeg-turbo
include_directories(${LIBJPEG_TURBO_INSTALL_DIR}/include)

# Conditionally find and link shared or static libraries
file(GLOB SHARED_LIBRARIES "${LIBJPEG_TURBO_INSTALL_DIR}/lib/lib*.so" "${LIBJPEG_TURBO_INSTALL_DIR}/lib/lib*.dylib")
file(GLOB STATIC_LIBRARIES "${LIBJPEG_TURBO_INSTALL_DIR}/lib/lib*.a")

message("#########################################")
if (SHARED_LIBRARIES)
    set(LIBJPEG_TURBO_LIBRARIES ${SHARED_LIBRARIES})
    message("LINKING SHARED LIBRARIES")
else ()
    set(LIBJPEG_TURBO_LIBRARIES ${STATIC_LIBRARIES})
    message("LINKING STATIC LIBRARIES")
endif ()
message("#########################################")

# Function to create a binary with libjpeg-turbo as a dependency
function(compile_binary OUT_EXE SRC_LIST)
  add_executable(${OUT_EXE} ${SRC_LIST})

  # Add dependency on JpegTurbo for correct build order
  add_dependencies(${OUT_EXE} JpegTurbo)
  
  # Link the executable with the selected jpeg-turbo library type
  target_link_libraries(${OUT_EXE} ${LIBJPEG_TURBO_LIBRARIES})
  
  # Set C++ standard
  set_property(TARGET ${OUT_EXE} PROPERTY CXX_STANDARD 17)
endfunction()

# Compile the main executable
compile_binary(main "main.cpp")
