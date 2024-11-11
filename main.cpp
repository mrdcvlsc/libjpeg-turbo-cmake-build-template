#include <iostream>
#include <fstream>
#include <vector>

/** libjpeg-turbo - namespace that contains C++ wrapper functions for basic jpeg read and write opreations */
namespace tj {

extern "C"
{
#include <turbojpeg.h>
}

/// pixel format conversion can only happen here not in the write function.
bool readJPEG(const std::string &filename, std::vector<unsigned char> &img_pixels, int &width, int &height, int &sub_sampling, const int pixel_format) {
    
    tjhandle jpeg_decompressor = tjInitDecompress();

    if (!jpeg_decompressor) {
        std::cerr << "Failed to initialize JPEG decompressor." << '\n';
        return false;
    }

    // Read the JPEG file into a buffer
    std::ifstream jpeg_file(filename, std::ios::binary);
    
    if (!jpeg_file) {
        std::cerr << "Could not open file: " << filename << '\n';
        tjDestroy(jpeg_decompressor);
        return false;
    }

    jpeg_file.seekg(0, std::ios::end);
    long jpeg_byte_size = jpeg_file.tellg();
    jpeg_file.seekg(0, std::ios::beg);

    std::vector<unsigned char> jpeg_read_buffer(jpeg_byte_size);
    jpeg_file.read(reinterpret_cast<char*>(jpeg_read_buffer.data()), jpeg_byte_size);
    jpeg_file.close();

    // Get image dimensions and subsampling type
    bool decompress_header_err = tjDecompressHeader2(
        jpeg_decompressor,
        jpeg_read_buffer.data(), jpeg_read_buffer.size(),
        &width, &height, &sub_sampling
    );
    
    if (decompress_header_err) {
        std::cerr << "Error decompressing JPEG header: " << tjGetErrorStr() << '\n';
        tjDestroy(jpeg_decompressor);
        return false;
    }

    // Allocate buffer for decompressed image
    img_pixels.resize(width * height * tjPixelSize[pixel_format]);

    // is pixel format an output variable or we just need to pass its addresss for some fancy reasons?

    bool decompress_img_err = tjDecompress2(
        jpeg_decompressor,
        jpeg_read_buffer.data(), jpeg_read_buffer.size(),
        img_pixels.data(), width, /* pitch = */ 0, height, pixel_format,
        TJFLAG_FASTDCT
    );

    if (decompress_img_err) {
        std::cerr << "Error decompressing JPEG image: " << tjGetErrorStr() << '\n';
        tjDestroy(jpeg_decompressor);
        return false;
    }

    tjDestroy(jpeg_decompressor);
    return true;
}

bool writeJPEG(const std::string &filename, const std::vector<unsigned char> &img_pixels, const int width, const int height, const int sub_sampling, const int pixel_format, const int quality) {
    
    tjhandle jpeg_compressor = tjInitCompress();

    if (!jpeg_compressor) {
        std::cerr << "Failed to initialize JPEG compressor." << '\n';
        return false;
    }

    unsigned char *jpeg_compress_output_data = nullptr;
    unsigned long jpeg_compress_output_byte_size = 0;

    // Compress the image
    bool compression_err = tjCompress2(
        jpeg_compressor, img_pixels.data(),
        width, /* pitch = */ 0, height, pixel_format,
        &jpeg_compress_output_data, &jpeg_compress_output_byte_size,
        sub_sampling, quality, TJFLAG_FASTDCT
    );

    if (compression_err) {
        std::cerr << "Error compressing JPEG image: " << tjGetErrorStr() << '\n';
        tjDestroy(jpeg_compressor);
        return false;
    }

    // Write to file
    std::ofstream jpeg_file(filename, std::ios::binary);
    if (!jpeg_file) {
        std::cerr << "Could not open file for writing: " << filename << '\n';
        tjFree(jpeg_compress_output_data);
        tjDestroy(jpeg_compressor);
        return false;
    }

    jpeg_file.write(reinterpret_cast<const char*>(jpeg_compress_output_data), jpeg_compress_output_byte_size);
    jpeg_file.close();

    tjFree(jpeg_compress_output_data);
    tjDestroy(jpeg_compressor);

    return true;
}

} // namespace tj

int main() {

    std::vector<unsigned char> img_data;
    
    // output variables
    int width, height, sub_sampling;

    // constant variables
    const int channels = tj::tjPixelSize[tj::TJPF_RGB];

    // Read the image
    if (tj::readJPEG("test.jpg", img_data, width, height, sub_sampling, tj::TJPF_RGB)) {
        std::cout << "Image read successfully: " << width << "x" << height  << "x" << channels << " = " << width * height * channels << '\n';
    } else {
        std::cout << "Image read failed: " << width << "x" << height << "x" << channels << " = " << width * height * channels << '\n';
        return 1;
    }

    std::cout << "Image Read Total Pixel Data = " << img_data.size() << '\n';
    
    // quality range: 1 - 100
    if (tj::writeJPEG("output.jpg", img_data, width, height, sub_sampling, tj::TJPF_RGB, 80)) {
        std::cout << "Image written successfully to output.jpg" << '\n';
    } else {
        return 2;
    }

    return 0;
}
