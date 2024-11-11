# libturbo-cmake

## Summary Table of Compatible Combinations

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
