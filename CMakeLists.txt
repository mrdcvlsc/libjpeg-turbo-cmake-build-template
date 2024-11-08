cmake_minimum_required(VERSION 3.18)
project(libjpeg_cmake_template LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Define the installation prefix for libjpeg-turbo
set(LIBJPEG_TURBO_INSTALL_DIR "${CMAKE_BINARY_DIR}/libjpegturbo_install_dir")

include(CMakePrintHelpers)
message("{{{{{{{{{{{{{{{{{{{{{{[}}}}}}}}}}}}}}}}}}}}}}")
cmake_print_variables(LINK_DIRECTORIES)
cmake_print_variables(BINARY_DIR)
cmake_print_variables(SOURCE_DIR)
cmake_print_variables(PROJECT_BINARY_DIR)
cmake_print_variables(INCLUDE_DIRECTORIES)
cmake_print_variables(WORKING_DIRECTORY)
cmake_print_variables(LIBRARY_OUTPUT_DIRECTORY)
cmake_print_variables(CMAKE_BINARY_DIR)
cmake_print_variables(CMAKE_SOURCE_DIR)
message("{{{{{{{{{{{{{{{{{{{{{{[}}}}}}}}}}}}}}}}}}}}}}")

include(ExternalProject)

# Download and build libjpeg-turbo with both shared and static enabled
ExternalProject_Add(JpegTurbo
    URL https://github.com/libjpeg-turbo/libjpeg-turbo/archive/refs/tags/3.0.4.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/libjpegturbo
    CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX=${LIBJPEG_TURBO_INSTALL_DIR}
      -DENABLE_STATIC=ON
      -DENABLE_SHARED=ON
)

# Set include directory for libjpeg-turbo
include_directories(${LIBJPEG_TURBO_INSTALL_DIR}/include)

message("-----------------------------------------------------")
# Conditionally find libraries based on the platform
if (MSVC)
    message("USING MSVC SHARED LIBRARIES")

    # On Windows, look for import libraries (.lib) and runtime DLLs
    file(GLOB IMPORT_LIBRARIES "${LIBJPEG_TURBO_INSTALL_DIR}/lib/*.lib")
    file(GLOB DLL_FILES "${LIBJPEG_TURBO_INSTALL_DIR}/bin/*.dll")

    # Set the libraries to link with
    set(LIBJPEG_TURBO_LIBRARIES ${IMPORT_LIBRARIES})
else ()
    # On Linux/macOS, prefer shared libraries, fallback to static if needed
    file(GLOB SHARED_LIBRARIES "${LIBJPEG_TURBO_INSTALL_DIR}/lib/lib*.so" "${LIBJPEG_TURBO_INSTALL_DIR}/lib/lib*.dylib")
    file(GLOB STATIC_LIBRARIES "${LIBJPEG_TURBO_INSTALL_DIR}/lib/lib*.a")

    if (SHARED_LIBRARIES)
        message("USING SHARED LIBRARIES")
        set(LIBJPEG_TURBO_LIBRARIES ${SHARED_LIBRARIES})
    else ()
        message("USING STATIC LIBRARIES")
        set(LIBJPEG_TURBO_LIBRARIES ${STATIC_LIBRARIES})
    endif ()
endif ()
message("-----------------------------------------------------")

# Function to create a binary with libjpeg-turbo as a dependency
function(compile_binary OUT_EXE SRC_LIST)
  add_executable(${OUT_EXE} ${SRC_LIST})

  # Add dependency on JpegTurbo for correct build order
  add_dependencies(${OUT_EXE} JpegTurbo)
  
  # Link the executable with the appropriate jpeg-turbo libraries
  target_link_libraries(${OUT_EXE} ${LIBJPEG_TURBO_LIBRARIES})
  
  # Set C++ standard
  set_property(TARGET ${OUT_EXE} PROPERTY CXX_STANDARD 17)

  # Copy DLLs to the output directory on Windows
  if (MSVC)
    add_custom_command(TARGET ${OUT_EXE} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${DLL_FILES}
      $<TARGET_FILE_DIR:${OUT_EXE}>)
  endif ()
endfunction()

# Compile the main executable
compile_binary(main "main.cpp")
