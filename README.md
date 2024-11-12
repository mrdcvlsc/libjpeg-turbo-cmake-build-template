# libjpeg-turbo-cmake

## Requirements/Recommendations

- It is recommended to install nasm or yasm first to your system (and add the bin paths to env path variables) before cmake configuration for better performance.

- When building with windows GCC, I would recommend to use the Unix Makefile Generator:

    ```cmake
    -G "Unix Makefiles"
    ```

- When building with windows Clang, I would recommend to use the **MSVC-like command-line not the GNU** one:

    ```cmake
    -G "Visual Studio 17 2022" -A x64 -T ClangCL
    ```

- When building with Mac I would recommend to add the installation path `<clone-or-download-folder>/libjpeg-turbo-cmake-template/turbojpeg/lib` to the `DYLD_LIBRARY_PATH` env variable.

## Table of Compatible Pixel Format & Subsampling Combinations

| Pixel Format      | Description                | Compatible Subsampling     |
|-------------------|----------------------------|----------------------------|
| `TJPF_GRAY`       | Grayscale                  | `TJSAMP_GRAY`              |
| `TJPF_RGB`        | RGB                        | `TJSAMP_444`, `TJSAMP_422`, `TJSAMP_420`, `TJSAMP_440`, `TJSAMP_411` |
| `TJPF_BGR`        | BGR                        | `TJSAMP_444`, `TJSAMP_422`, `TJSAMP_420`, `TJSAMP_440`, `TJSAMP_411` |
| `TJPF_RGBX`       | RGB with unused alpha byte | `TJSAMP_444`, `TJSAMP_422`, `TJSAMP_420`, `TJSAMP_440`, `TJSAMP_411` |
| `TJPF_BGRX`       | BGR with unused alpha byte | `TJSAMP_444`, `TJSAMP_422`, `TJSAMP_420`, `TJSAMP_440`, `TJSAMP_411` |
| `TJPF_XRGB`       | RGB with unused alpha byte | `TJSAMP_444`, `TJSAMP_422`, `TJSAMP_420`, `TJSAMP_440`, `TJSAMP_411` |
| `TJPF_XBGR`       | BGR with unused alpha byte | `TJSAMP_444`, `TJSAMP_422`, `TJSAMP_420`, `TJSAMP_440`, `TJSAMP_411` |
| `TJPF_CMYK`       | CMYK                       | `TJSAMP_444`               |
