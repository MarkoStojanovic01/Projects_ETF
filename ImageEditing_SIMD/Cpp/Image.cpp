
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "../Header/Image.h"
#include "../Header/stb_image.h"
#include "../Header/stb_image_write.h"
#include <filesystem>
#include <chrono>
#include "../Header/avx_mathfun.h"

#ifdef __SSE2__
#include <emmintrin.h> // SSE2 Intrinsics
#endif

#include <smmintrin.h>
#include <immintrin.h>
#include <avxintrin.h>



unsigned char *Image::getData() const {
    return data;
}

void Image::setData(unsigned char *data) {
    Image::data = data;
}

int Image::getWidth() const {
    return width;
}

void Image::setWidth(int width) {
    Image::width = width;
}

int Image::getHeight() const {
    return height;
}

void Image::setHeight(int height) {
    Image::height = height;
}

int Image::getChannels() const {
    return channels;
}

void Image::setChannels(int channels) {
    Image::channels = channels;
}

Image::Image(const std::string& filename) : data(nullptr), width(0), height(0), channels(0) {
    original = true;
    data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (data) {
        loadImage();
    } else {
        std::cerr << "Failed to load image: " << filename << std::endl;
    }

}

Image::Image(Image* copy) {
    if(copy->getData() != nullptr){
        original = false;
        width = copy->getWidth();
        height = copy->getHeight();
        channels = copy->getChannels();
        int dataSize = width * height * channels;
        data = nullptr;
        data = new uint8_t[dataSize];
        std::copy(copy->getData(), copy->getData() + dataSize, data);

        loadImage();

    }
    else{
        std::cerr << "Failed to copy image! " << std::endl;
    }


}

void Image::loadImage() {
    if(data == nullptr) {
        std::cout << "Error:Data is null\n";
        return;
    }

    redChannel = nullptr;
    greenChannel = nullptr;
    blueChannel = nullptr;
    alphaChannel = nullptr;

    // Allocate memory for channel arrays
    redChannel = new uint8_t[width * height];
    greenChannel = new uint8_t[width * height];
    blueChannel = new uint8_t[width * height];

    // Extract channel values from loaded data
    for (int i = 0; i < width * height; ++i) {
        redChannel[i] = data[i * channels];
        greenChannel[i] = data[i * channels + 1];
        blueChannel[i] = data[i * channels + 2];
    }

    if (channels==4){
        alphaChannel = new uint8_t[width * height];
        for (int i = 0; i < width * height; ++i) {
            alphaChannel[i] = data[i * channels + 3];
        }
    }
}

void Image::copyImages(Image *copy) {
    if(copy->getData() != nullptr){
        int dataSize = width * height * channels;
        std::copy(copy->getData(), copy->getData() + dataSize, data);
        // Extract channel values from loaded data
        for (int i = 0; i < width * height; ++i) {
            redChannel[i] = data[i * channels];
            greenChannel[i] = data[i * channels + 1];
            blueChannel[i] = data[i * channels + 2];
        }

        if (channels==4){
            alphaChannel = new uint8_t[width * height];
            for (int i = 0; i < width * height; ++i) {
                alphaChannel[i] = data[i * channels + 3];
            }
        }
    }
    else{
        std::cerr << "Failed to copy image! " << std::endl;
    }
}

Image::~Image() {
    if(redChannel != nullptr) delete[] redChannel;
    if(blueChannel != nullptr) delete[] blueChannel;
    if(greenChannel != nullptr) delete[] greenChannel;
    if(alphaChannel != nullptr) delete[] alphaChannel;
    if (original) {
        stbi_image_free(data);
    }
    else{
        delete[] data;
        data = nullptr;
    }

}

void Image::printInfo() const {
    std::cout << "Image Information:" << std::endl;
    std::cout << "Width: " << width << ", Height: " << height << ", Channels: " << channels << std::endl;
}

bool Image::isLoaded() const {
    return (data != nullptr);
}

void Image::save(const std::string& filename, const std::string& mark) const {
    if (data == nullptr) {
        std::cerr << "No image data to save." << std::endl;
        return;
    }

    // refreshing data
    for (int i=0; i<height*width; i++){
        data[i*channels] = redChannel[i];
        data[i*channels+1] = greenChannel[i];
        data[i*channels+2] = blueChannel[i];
    }


    std::filesystem::path outputPath(filename);
    std::filesystem::path outputDir = outputPath.parent_path();
    std::filesystem::path outputFilename = outputPath.filename();
    std::filesystem::path outputExtension = outputPath.extension();

    std::string outputName = outputFilename.stem().string();
    std::string outputExt = outputExtension.string();

    std::string newFilename = outputDir.string() + "/" + outputName + "_" + mark + outputExt;



    if (outputExt == ".jpg" || outputExt == ".jpeg") {
        int result = stbi_write_jpg(newFilename.c_str(), width, height, channels, data, 100);
        if (result == 0) {
            std::cerr << "Failed to save image: " << filename << std::endl;
        }
    } else if (outputExt == ".png") {
        int stride = width * channels;
        int result = stbi_write_png(newFilename.c_str(), width, height, channels, data, stride);
        if (result == 0) {
            std::cerr << "Failed to save image: " << filename << std::endl;
        }
    } else {
        std::cerr << "Unsupported image format: " << outputExt << std::endl;
    }
}

void Image::addConstant(uint8_t value) {

    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();

    int min = 0;
    int max = 255;

    for (size_t i = 0; i < width * height; ++i) {
        redChannel[i] = static_cast<uint8_t>(std::clamp(redChannel[i]+value, min, max));
        greenChannel[i] = static_cast<uint8_t>(std::clamp(greenChannel[i]+ value, min, max)) ;
        blueChannel[i] = static_cast<uint8_t>(std::clamp(blueChannel[i]+ value, min, max)) ;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "AddConstant execution Time (Non-optimized): " << duration << " microseconds" << std::endl;
}

void Image::addConstantMod(uint8_t value) {

    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();


    size_t imageSize = width * height;


    // SIMD vector size
    constexpr size_t simdSize = 32;
    size_t remainder = imageSize % simdSize;



    // Load the constant value into a SIMD register
    __m256i constant = _mm256_set1_epi8(value);


    // Process the image data in SIMD vectorized blocks
    size_t i = 0;
    for (; i < imageSize - remainder; i += simdSize) {

        // Load the pixel data from memory into a SIMD register
        __m256i red = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&redChannel[i]));
        __m256i green = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&greenChannel[i]));
        __m256i blue = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&blueChannel[i]));

        // Add the constant value to each channel of the pixel using SIMD addition
        __m256i resultR = _mm256_adds_epu8(red, constant);
        __m256i resultG = _mm256_adds_epu8(green, constant);
        __m256i resultB = _mm256_adds_epu8(blue, constant);

        // Store the result back to memory
        _mm256_store_si256(reinterpret_cast<__m256i*>(&redChannel[i]), resultR);
        _mm256_store_si256(reinterpret_cast<__m256i*>(&greenChannel[i]), resultG);
        _mm256_store_si256(reinterpret_cast<__m256i*>(&blueChannel[i]), resultB);
    }

    int min = 0;
    int max = 255;

    // Process the remaining elements (not a multiple of SIMD vector size) in a scalar manner
    for (; i < imageSize; ++i) {
        redChannel[i] = static_cast<uint8_t>(std::clamp(redChannel[i]+value, min, max));
        greenChannel[i] = static_cast<uint8_t>(std::clamp(greenChannel[i]+ value, min, max)) ;
        blueChannel[i] = static_cast<uint8_t>(std::clamp(blueChannel[i]+ value, min, max)) ;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "AddConstant execution Time (Optimized): " << duration << " microseconds" << std::endl;
}

void Image::convertToGrayscale() {
    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();


    for (size_t i = 0; i < width*height; i++) {
        unsigned char grayValue = 0;

        // Calculate arithmetic mean of color channels
        float redV = redChannel[i] / 3.0;
        float greenV = greenChannel[i] / 3.0;
        float blueV = blueChannel[i] / 3.0;
        grayValue = redV+greenV+blueV;

        // Assign gray value to each channel of the pixel
        redChannel[i] = grayValue;
        greenChannel[i] = grayValue;
        blueChannel[i] = grayValue;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "ConvertToGrayscale execution Time (Non-optimized): " << duration << " microseconds" << std::endl;
}

void Image::convertToGrayscaleMod() {
    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();

    size_t imageSize = width * height;

    // SIMD vector size
    constexpr size_t simdSize = 32;
    size_t remainder = imageSize % simdSize;

    // Load the constant value into a SIMD register
    __m256 constant = _mm256_cvtepi32_ps(_mm256_set1_epi32(static_cast<char>(3)));

    // Process the image data in SIMD vectorized blocks
    size_t i = 0;
    for (; i < imageSize - remainder; i += simdSize) {
        // Load the pixel data from memory into a SIMD register
        __m256i red = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&redChannel[i]));
        __m256i green = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&greenChannel[i]));
        __m256i blue = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&blueChannel[i]));

        // extraxt channels on lower and higher 128-bits
        __m128i red_lower = _mm256_extracti128_si256(red, 0);
        __m128i red_higher = _mm256_extracti128_si256(red, 1);
        __m128i green_lower = _mm256_extracti128_si256(green, 0);
        __m128i green_higher = _mm256_extracti128_si256(green, 1);
        __m128i blue_lower = _mm256_extracti128_si256(blue, 0);
        __m128i blue_higher = _mm256_extracti128_si256(blue, 1);

        // convert 8-bits channels to 32-bits
        __m256 red32[4];
        red32[0] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(red_lower));
        red32[1] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(red_lower, red_lower)));
        red32[2] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(red_higher));
        red32[3] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(red_higher, red_higher)));

        __m256 green32[4];
        green32[0] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(green_lower));
        green32[1] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(green_lower, green_lower)));
        green32[2] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(green_higher));
        green32[3] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(green_higher, green_higher)));

        __m256 blue32[4];
        blue32[0] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(blue_lower));
        blue32[1] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(blue_lower, blue_lower)));
        blue32[2] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(blue_higher));
        blue32[3] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(blue_higher, blue_higher)));

        // divide with constant
        for(int j=0; j<4; j++){
            red32[j] = _mm256_div_ps(red32[j], constant);
            green32[j] = _mm256_div_ps(green32[j], constant);
            blue32[j] = _mm256_div_ps(blue32[j], constant);
        }

        // covert 32-bits back to 8-bits
        __m256i red16low = _mm256_packus_epi32(_mm256_cvtps_epi32(red32[0]), _mm256_cvtps_epi32(red32[1]));
        red16low  = _mm256_permute4x64_epi64(red16low, 0xd8);
        __m256i red16high = _mm256_packus_epi32(_mm256_cvtps_epi32(red32[2]), _mm256_cvtps_epi32(red32[3]));
        red16high  = _mm256_permute4x64_epi64(red16high, 0xd8);
        __m256i resultR = _mm256_packus_epi16(red16low, red16high);
        resultR  = _mm256_permute4x64_epi64(resultR, 0xd8);

        __m256i green16low = _mm256_packus_epi32(_mm256_cvtps_epi32(green32[0]), _mm256_cvtps_epi32(green32[1]));
        green16low  = _mm256_permute4x64_epi64(green16low, 0xd8);
        __m256i green16high = _mm256_packus_epi32(_mm256_cvtps_epi32(green32[2]), _mm256_cvtps_epi32(green32[3]));
        green16high  = _mm256_permute4x64_epi64(green16high, 0xd8);
        __m256i resultG = _mm256_packus_epi16(green16low, green16high);
        resultG  = _mm256_permute4x64_epi64(resultG, 0xd8);

        __m256i blue16low = _mm256_packus_epi32(_mm256_cvtps_epi32(blue32[0]), _mm256_cvtps_epi32(blue32[1]));
        blue16low  = _mm256_permute4x64_epi64(blue16low, 0xd8);
        __m256i blue16high = _mm256_packus_epi32(_mm256_cvtps_epi32(blue32[2]), _mm256_cvtps_epi32(blue32[3]));
        blue16high  = _mm256_permute4x64_epi64(blue16high, 0xd8);
        __m256i resultB = _mm256_packus_epi16(blue16low, blue16high);
        resultB  = _mm256_permute4x64_epi64(resultB, 0xd8);

        __m256i gray = _mm256_add_epi8(resultR, resultG);
        gray = _mm256_add_epi8(gray, resultB);

        // Store the result back to memory
        _mm256_store_si256(reinterpret_cast<__m256i*>(&redChannel[i]), gray);

        _mm256_store_si256(reinterpret_cast<__m256i*>(&greenChannel[i]), gray);
        _mm256_store_si256(reinterpret_cast<__m256i*>(&blueChannel[i]), gray);
    }


    // Perform remaining calculations for the remaining elements
    for (; i < width * height; i++) {
        unsigned char grayValue = 0;

        // Calculate arithmetic mean of color channels
        float redV = redChannel[i] / 3.0;
        float greenV = greenChannel[i] / 3.0;
        float blueV = blueChannel[i] / 3.0;
        grayValue = redV+greenV+blueV;

        // Assign gray value to each channel of the pixel
        redChannel[i] = grayValue;
        greenChannel[i] = grayValue;
        blueChannel[i] = grayValue;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "ConvertToGrayscale execution Time (Optimized): " << duration << " microseconds" << std::endl;
}

void Image::subConstant(uint8_t value) {
    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();


    int min = 0;
    int max = 255;

    for (size_t i = 0; i < width * height; ++i) {
        redChannel[i] = static_cast<uint8_t>(std::clamp(redChannel[i]-value, min, max));
        greenChannel[i] = static_cast<uint8_t>(std::clamp(greenChannel[i]- value, min, max)) ;
        blueChannel[i] = static_cast<uint8_t>(std::clamp(blueChannel[i]- value, min, max)) ;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "SubConstant execution Time (Non-optimized): " << duration << " microseconds" << std::endl;
}

void Image::subConstantMod(uint8_t value) {

    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();


    size_t imageSize = width * height;


    // SIMD vector size
    constexpr size_t simdSize = 32;
    size_t remainder = imageSize % simdSize;

    // Load the constant value into a SIMD register
    __m256i constant = _mm256_set1_epi8(value);

    // Process the image data in SIMD vectorized blocks
    size_t i = 0;
    for (; i < imageSize - remainder; i += simdSize) {
        // Load the pixel data from memory into a SIMD register
        __m256i red = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&redChannel[i]));
        __m256i green = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&greenChannel[i]));
        __m256i blue = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&blueChannel[i]));

        // Add the constant value to each channel of the pixel using SIMD addition
        __m256i resultR = _mm256_subs_epu8(red, constant);
        __m256i resultG = _mm256_subs_epu8(green, constant);
        __m256i resultB = _mm256_subs_epu8(blue, constant);

        // Store the result back to memory
        _mm256_store_si256(reinterpret_cast<__m256i*>(&redChannel[i]), resultR);
        _mm256_store_si256(reinterpret_cast<__m256i*>(&greenChannel[i]), resultG);
        _mm256_store_si256(reinterpret_cast<__m256i*>(&blueChannel[i]), resultB);
    }

    int min = 0;
    int max = 255;

    // Process the remaining elements (not a multiple of SIMD vector size) in a scalar manner
    for (; i < imageSize; ++i) {
        redChannel[i] = static_cast<uint8_t>(std::clamp(redChannel[i]-value, min, max));
        greenChannel[i] = static_cast<uint8_t>(std::clamp(greenChannel[i]- value, min, max)) ;
        blueChannel[i] = static_cast<uint8_t>(std::clamp(blueChannel[i]- value, min, max)) ;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "SubConstant execution Time (Optimized): " << duration << " microseconds" << std::endl;
}

void Image::InvSubConstant(uint8_t value) {
    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();

    int min = 0;
    int max = 255;

    for (size_t i = 0; i < width * height; ++i) {
        redChannel[i] = static_cast<uint8_t>(std::clamp(value-redChannel[i], min, max));
        greenChannel[i] = static_cast<uint8_t>(std::clamp(value-greenChannel[i], min, max)) ;
        blueChannel[i] = static_cast<uint8_t>(std::clamp(value-blueChannel[i], min, max)) ;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "InverseSubConstant execution Time (Non-optimized): " << duration << " microseconds" << std::endl;
}

void Image::InvSubConstantMod(uint8_t value) {
    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();


    size_t imageSize = width * height;


    // SIMD vector size
    constexpr size_t simdSize = 32;
    size_t remainder = imageSize % simdSize;

    // Load the constant value into a SIMD register
    __m256i constant = _mm256_set1_epi8(value);

    // Process the image data in SIMD vectorized blocks
    size_t i = 0;
    for (; i < imageSize - remainder; i += simdSize) {
        // Load the pixel data from memory into a SIMD register
        __m256i red = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&redChannel[i]));
        __m256i green = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&greenChannel[i]));
        __m256i blue = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&blueChannel[i]));

        // Add the constant value to each channel of the pixel using SIMD addition
        __m256i resultR = _mm256_subs_epu8(constant, red);
        __m256i resultG = _mm256_subs_epu8(constant, green);
        __m256i resultB = _mm256_subs_epu8(constant, blue);

        // Store the result back to memory
        _mm256_store_si256(reinterpret_cast<__m256i*>(&redChannel[i]), resultR);
        _mm256_store_si256(reinterpret_cast<__m256i*>(&greenChannel[i]), resultG);
        _mm256_store_si256(reinterpret_cast<__m256i*>(&blueChannel[i]), resultB);
    }

    int min = 0;
    int max = 0;

    // Process the remaining elements (not a multiple of SIMD vector size) in a scalar manner
    for (; i < imageSize; ++i) {
        redChannel[i] = static_cast<uint8_t>(std::clamp(value-redChannel[i], min, max));
        greenChannel[i] = static_cast<uint8_t>(std::clamp(value-greenChannel[i], min, max)) ;
        blueChannel[i] = static_cast<uint8_t>(std::clamp(value-blueChannel[i], min, max)) ;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "InverseSubConstant execution Time (Optimized): " << duration << " microseconds" << std::endl;
}

void Image::mulConstant(float value) {
    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();


    float min = 0;
    float max = 255;

    for (size_t i = 0; i < width * height; ++i) {
        redChannel[i] = static_cast<uint8_t>(std::clamp(redChannel[i]*value, min, max));
        greenChannel[i] = static_cast<uint8_t>(std::clamp(greenChannel[i]*value, min, max)) ;
        blueChannel[i] = static_cast<uint8_t>(std::clamp(blueChannel[i]*value, min, max)) ;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "MulConstant execution Time (Non-optimized): " << duration << " microseconds" << std::endl;
}

void Image::mulConstantMod(float value) {

    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();


    size_t imageSize = width * height;


    // SIMD vector size
    constexpr size_t simdSize = 32;
    size_t remainder = imageSize % simdSize;

    // Load the constant value into a SIMD register
    __m256 constant = _mm256_cvtepi32_ps(_mm256_set1_epi32(value));

    // Process the image data in SIMD vectorized blocks
    size_t i = 0;
    for (; i < imageSize - remainder; i += simdSize) {
        // Load the pixel data from memory into a SIMD register
        __m256i red = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&redChannel[i]));
        __m256i green = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&greenChannel[i]));
        __m256i blue = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&blueChannel[i]));

        // extraxt channels on lower and higher 128-bits
        __m128i red_lower = _mm256_extracti128_si256(red, 0);
        __m128i red_higher = _mm256_extracti128_si256(red, 1);
        __m128i green_lower = _mm256_extracti128_si256(green, 0);
        __m128i green_higher = _mm256_extracti128_si256(green, 1);
        __m128i blue_lower = _mm256_extracti128_si256(blue, 0);
        __m128i blue_higher = _mm256_extracti128_si256(blue, 1);

        // convert 8-bits channels to 32-bits
        __m256 red32[4];
        red32[0] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(red_lower));
        red32[1] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(red_lower, red_lower)));
        red32[2] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(red_higher));
        red32[3] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(red_higher, red_higher)));

        __m256 green32[4];
        green32[0] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(green_lower));
        green32[1] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(green_lower, green_lower)));
        green32[2] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(green_higher));
        green32[3] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(green_higher, green_higher)));

        __m256 blue32[4];
        blue32[0] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(blue_lower));
        blue32[1] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(blue_lower, blue_lower)));
        blue32[2] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(blue_higher));
        blue32[3] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(blue_higher, blue_higher)));

        // divide with constant
        for(int j=0; j<4; j++){
            red32[j] = _mm256_mul_ps(red32[j], constant);
            green32[j] = _mm256_mul_ps(green32[j], constant);
            blue32[j] = _mm256_mul_ps(blue32[j], constant);
        }

        // covert 32-bits back to 8-bits
        __m256i red16low = _mm256_packus_epi32(_mm256_cvtps_epi32(red32[0]), _mm256_cvtps_epi32(red32[1]));
        red16low  = _mm256_permute4x64_epi64(red16low, 0xd8);
        __m256i red16high = _mm256_packus_epi32(_mm256_cvtps_epi32(red32[2]), _mm256_cvtps_epi32(red32[3]));
        red16high  = _mm256_permute4x64_epi64(red16high, 0xd8);
        __m256i resultR = _mm256_packus_epi16(red16low, red16high);
        resultR  = _mm256_permute4x64_epi64(resultR, 0xd8);

        __m256i green16low = _mm256_packus_epi32(_mm256_cvtps_epi32(green32[0]), _mm256_cvtps_epi32(green32[1]));
        green16low  = _mm256_permute4x64_epi64(green16low, 0xd8);
        __m256i green16high = _mm256_packus_epi32(_mm256_cvtps_epi32(green32[2]), _mm256_cvtps_epi32(green32[3]));
        green16high  = _mm256_permute4x64_epi64(green16high, 0xd8);
        __m256i resultG = _mm256_packus_epi16(green16low, green16high);
        resultG  = _mm256_permute4x64_epi64(resultG, 0xd8);

        __m256i blue16low = _mm256_packus_epi32(_mm256_cvtps_epi32(blue32[0]), _mm256_cvtps_epi32(blue32[1]));
        blue16low  = _mm256_permute4x64_epi64(blue16low, 0xd8);
        __m256i blue16high = _mm256_packus_epi32(_mm256_cvtps_epi32(blue32[2]), _mm256_cvtps_epi32(blue32[3]));
        blue16high  = _mm256_permute4x64_epi64(blue16high, 0xd8);
        __m256i resultB = _mm256_packus_epi16(blue16low, blue16high);
        resultB  = _mm256_permute4x64_epi64(resultB, 0xd8);

        // Store the result back to memory
        _mm256_store_si256(reinterpret_cast<__m256i*>(&redChannel[i]), resultR);

        _mm256_store_si256(reinterpret_cast<__m256i*>(&greenChannel[i]), resultG);
        _mm256_store_si256(reinterpret_cast<__m256i*>(&blueChannel[i]), resultB);
    }


    float min = 0;
    float max = 255;

    // Process the remaining elements (not a multiple of SIMD vector size) in a scalar manner
    for (; i < imageSize; ++i) {
        redChannel[i] = static_cast<uint8_t>(std::clamp(redChannel[i]*value, min, max));
        greenChannel[i] = static_cast<uint8_t>(std::clamp(greenChannel[i]*value, min, max)) ;
        blueChannel[i] = static_cast<uint8_t>(std::clamp(blueChannel[i]*value, min, max)) ;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "MulConstant execution Time (Optimized): " << duration << " microseconds" << std::endl;
}

void Image::divConstant(float value) {
    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();


    for (size_t i = 0; i < width * height; ++i) {
        redChannel[i] = (float)redChannel[i] / value ;
        greenChannel[i] = (float)greenChannel[i] / value ;
        blueChannel[i] = (float)blueChannel[i] / value ;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "DivConstant execution Time (Non-optimized): " << duration << " microseconds" << std::endl;
}

void Image::divConstantMod(float value) {

    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();

    size_t imageSize = width * height;


    // SIMD vector size
    constexpr size_t simdSize = 32;
    size_t remainder = imageSize % simdSize;

    // Load the constant value into a SIMD register
    __m256 constant = _mm256_cvtepi32_ps(_mm256_set1_epi32(value));


    // Process the image data in SIMD vectorized blocks
    size_t i = 0;
    for (; i < imageSize - remainder; i += simdSize) {
        // Load the pixel data from memory into a SIMD register
        __m256i red = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&redChannel[i]));
        __m256i green = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&greenChannel[i]));
        __m256i blue = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&blueChannel[i]));

        // extraxt channels on lower and higher 128-bits
        __m128i red_lower = _mm256_extracti128_si256(red, 0);
        __m128i red_higher = _mm256_extracti128_si256(red, 1);
        __m128i green_lower = _mm256_extracti128_si256(green, 0);
        __m128i green_higher = _mm256_extracti128_si256(green, 1);
        __m128i blue_lower = _mm256_extracti128_si256(blue, 0);
        __m128i blue_higher = _mm256_extracti128_si256(blue, 1);

        // convert 8-bits channels to 32-bits
        __m256 red32[4];
        red32[0] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(red_lower));
        red32[1] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(red_lower, red_lower)));
        red32[2] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(red_higher));
        red32[3] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(red_higher, red_higher)));

        __m256 green32[4];
        green32[0] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(green_lower));
        green32[1] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(green_lower, green_lower)));
        green32[2] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(green_higher));
        green32[3] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(green_higher, green_higher)));

        __m256 blue32[4];
        blue32[0] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(blue_lower));
        blue32[1] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(blue_lower, blue_lower)));
        blue32[2] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(blue_higher));
        blue32[3] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(blue_higher, blue_higher)));

        // divide with constant
        for(int j=0; j<4; j++){
            red32[j] = _mm256_div_ps(red32[j], constant);
            green32[j] = _mm256_div_ps(green32[j], constant);
            blue32[j] = _mm256_div_ps(blue32[j], constant);
        }

        // covert 32-bits back to 8-bits
        __m256i red16low = _mm256_packus_epi32(_mm256_cvtps_epi32(red32[0]), _mm256_cvtps_epi32(red32[1]));
        red16low  = _mm256_permute4x64_epi64(red16low, 0xd8);
        __m256i red16high = _mm256_packus_epi32(_mm256_cvtps_epi32(red32[2]), _mm256_cvtps_epi32(red32[3]));
        red16high  = _mm256_permute4x64_epi64(red16high, 0xd8);
        __m256i resultR = _mm256_packus_epi16(red16low, red16high);
        resultR  = _mm256_permute4x64_epi64(resultR, 0xd8);

        __m256i green16low = _mm256_packus_epi32(_mm256_cvtps_epi32(green32[0]), _mm256_cvtps_epi32(green32[1]));
        green16low  = _mm256_permute4x64_epi64(green16low, 0xd8);
        __m256i green16high = _mm256_packus_epi32(_mm256_cvtps_epi32(green32[2]), _mm256_cvtps_epi32(green32[3]));
        green16high  = _mm256_permute4x64_epi64(green16high, 0xd8);
        __m256i resultG = _mm256_packus_epi16(green16low, green16high);
        resultG  = _mm256_permute4x64_epi64(resultG, 0xd8);

        __m256i blue16low = _mm256_packus_epi32(_mm256_cvtps_epi32(blue32[0]), _mm256_cvtps_epi32(blue32[1]));
        blue16low  = _mm256_permute4x64_epi64(blue16low, 0xd8);
        __m256i blue16high = _mm256_packus_epi32(_mm256_cvtps_epi32(blue32[2]), _mm256_cvtps_epi32(blue32[3]));
        blue16high  = _mm256_permute4x64_epi64(blue16high, 0xd8);
        __m256i resultB = _mm256_packus_epi16(blue16low, blue16high);
        resultB  = _mm256_permute4x64_epi64(resultB, 0xd8);

        // Store the result back to memory
        _mm256_store_si256(reinterpret_cast<__m256i*>(&redChannel[i]), resultR);

        _mm256_store_si256(reinterpret_cast<__m256i*>(&greenChannel[i]), resultG);
        _mm256_store_si256(reinterpret_cast<__m256i*>(&blueChannel[i]), resultB);
    }


    // Process the remaining elements (not a multiple of SIMD vector size) in a scalar manner
    for (; i < imageSize; ++i) {
        redChannel[i] = redChannel[i] / value;
        greenChannel[i] = greenChannel[i] / value;
        blueChannel[i] = blueChannel[i] / value;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "DivConstant execution Time (Optimized): " << duration << " microseconds" << std::endl;
}

void Image::printChannels() {

    /*
    unsigned char red = redChannel[20];
    std::cout <<  static_cast<int>(red) << " ,";
    unsigned char green = greenChannel[20];
    std::cout <<  static_cast<int>(red) << " ,";
    unsigned char blue = blueChannel[20];
    std::cout <<  static_cast<int>(red) << " ,";
     */

    for(int i = width*height/4+(width*3/4); i < (width*height/4+(width*3/4))+100; i++){
        if(i%20==0) std::cout << "\n";
        unsigned char red = redChannel[i*this->channels];
        std::cout <<  static_cast<int>(red) << " ,";

    }

}

void Image::log() {
    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();

    double c  = 255.0 / std::log(255);


    for (size_t i = 0; i < width * height; ++i) {
        redChannel[i] = (unsigned char)(c * std::log((double)redChannel[i])) ;
        greenChannel[i] = (unsigned char)(c * std::log((double)greenChannel[i])) ;
        blueChannel[i] =(unsigned char)(c * std::log((double)blueChannel[i])) ;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Log execution Time (Non-optimized): " << duration << " microseconds" << std::endl;
}

void Image::logMod() {
    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();


    size_t imageSize = width * height;


    // SIMD vector size
    constexpr size_t simdSize = 32;
    size_t remainder = imageSize % simdSize;

    unsigned char c  = 255 / std::log(255);
    __m256 v_c = _mm256_set1_ps(c);

    // Process the image data in SIMD vectorized blocks
    size_t i = 0;
    for (; i < imageSize - remainder; i += simdSize) {

        // Load the pixel data from memory into a SIMD register
        __m256i red = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&redChannel[i]));
        __m256i green = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&greenChannel[i]));
        __m256i blue = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&blueChannel[i]));

        // extraxt channels on lower and higher 128-bits
        __m128i red_lower = _mm256_extracti128_si256(red, 0);
        __m128i red_higher = _mm256_extracti128_si256(red, 1);
        __m128i green_lower = _mm256_extracti128_si256(green, 0);
        __m128i green_higher = _mm256_extracti128_si256(green, 1);
        __m128i blue_lower = _mm256_extracti128_si256(blue, 0);
        __m128i blue_higher = _mm256_extracti128_si256(blue, 1);

        // convert 8-bits channels to 32-bits
        __m256 red32[4];
        red32[0] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(red_lower));
        red32[1] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(red_lower, red_lower)));
        red32[2] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(red_higher));
        red32[3] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(red_higher, red_higher)));

        __m256 green32[4];
        green32[0] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(green_lower));
        green32[1] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(green_lower, green_lower)));
        green32[2] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(green_higher));
        green32[3] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(green_higher, green_higher)));

        __m256 blue32[4];
        blue32[0] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(blue_lower));
        blue32[1] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(blue_lower, blue_lower)));
        blue32[2] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(blue_higher));
        blue32[3] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(blue_higher, blue_higher)));

        __m256i redLog[4];
        __m256i greenLog[4];
        __m256i blueLog[4];

        // log
        for(int j=0; j<4; j++){
            __m256 val = red32[j];
            val = log256_ps(val);
            redLog[j] = _mm256_cvtps_epi32(_mm256_mul_ps(val, v_c));

            val = green32[j];
            val = log256_ps(val);
            greenLog[j] = _mm256_cvtps_epi32(_mm256_mul_ps(val, v_c));

            val = blue32[j];
            val = log256_ps(val);
            blueLog[j] = _mm256_cvtps_epi32(_mm256_mul_ps(val, v_c));
        }

        // covert 32-bits back to 8-bits
        __m256i red16low = _mm256_packus_epi32(redLog[0], redLog[1]);
        red16low  = _mm256_permute4x64_epi64(red16low, 0xd8);
        __m256i red16high = _mm256_packus_epi32(redLog[2], redLog[3]);
        red16high  = _mm256_permute4x64_epi64(red16high, 0xd8);
        __m256i resultR = _mm256_packus_epi16(red16low, red16high);
        resultR  = _mm256_permute4x64_epi64(resultR, 0xd8);

        __m256i green16low = _mm256_packus_epi32(greenLog[0], greenLog[1]);
        green16low  = _mm256_permute4x64_epi64(green16low, 0xd8);
        __m256i green16high = _mm256_packus_epi32(greenLog[2], greenLog[3]);
        green16high  = _mm256_permute4x64_epi64(green16high, 0xd8);
        __m256i resultG = _mm256_packus_epi16(green16low, green16high);
        resultG  = _mm256_permute4x64_epi64(resultG, 0xd8);

        __m256i blue16low = _mm256_packus_epi32(blueLog[0], blueLog[1]);
        blue16low  = _mm256_permute4x64_epi64(blue16low, 0xd8);
        __m256i blue16high = _mm256_packus_epi32(blueLog[2], blueLog[3]);
        blue16high  = _mm256_permute4x64_epi64(blue16high, 0xd8);
        __m256i resultB = _mm256_packus_epi16(blue16low, blue16high);
        resultB  = _mm256_permute4x64_epi64(resultB, 0xd8);

        // Store the result back to memory
        _mm256_store_si256(reinterpret_cast<__m256i*>(&redChannel[i]), resultR);

        _mm256_store_si256(reinterpret_cast<__m256i*>(&greenChannel[i]), resultG);
        _mm256_store_si256(reinterpret_cast<__m256i*>(&blueChannel[i]), resultB);
    }

    // Process the remaining elements (not a multiple of SIMD vector size) in a scalar manner
    for (; i < imageSize; ++i) {
        redChannel[i] = c * std::log(redChannel[i] ) ;
        greenChannel[i] = c * std::log(greenChannel[i]) ;
        blueChannel[i] = c * std::log(blueChannel[i] ) ;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "AddConstant execution Time (Optimized): " << duration << " microseconds" << std::endl;
}

void Image::minI(uint8_t value) {
    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();


    for (size_t i = 0; i < width * height; ++i) {
        redChannel[i] = std::min(redChannel[i],value) ;
        greenChannel[i] = std::min(greenChannel[i],value) ;
        blueChannel[i] = std::min(blueChannel[i],value) ;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Min execution Time (Non-optimized): " << duration << " microseconds" << std::endl;
}

void Image::minIMod(uint8_t value) {
    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();


    size_t imageSize = width * height;


    // SIMD vector size
    constexpr size_t simdSize = 32;
    size_t remainder = imageSize % simdSize;



    // Load the constant value into a SIMD register
    __m256i constant = _mm256_set1_epi8(value);


    // Process the image data in SIMD vectorized blocks
    size_t i = 0;
    for (; i < imageSize - remainder; i += simdSize) {

        // Load the pixel data from memory into a SIMD register
        __m256i red = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&redChannel[i]));
        __m256i green = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&greenChannel[i]));
        __m256i blue = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&blueChannel[i]));

        // Add the constant value to each channel of the pixel using SIMD addition
        __m256i resultR = _mm256_min_epu8(red, constant);
        __m256i resultG = _mm256_min_epu8(green, constant);
        __m256i resultB = _mm256_min_epu8(blue, constant);

        // Store the result back to memory
        _mm256_store_si256(reinterpret_cast<__m256i*>(&redChannel[i]), resultR);
        _mm256_store_si256(reinterpret_cast<__m256i*>(&greenChannel[i]), resultG);
        _mm256_store_si256(reinterpret_cast<__m256i*>(&blueChannel[i]), resultB);
    }


    // Process the remaining elements (not a multiple of SIMD vector size) in a scalar manner
    for (; i < imageSize; ++i) {
        redChannel[i] = std::min(redChannel[i],value) ;
        greenChannel[i] = std::min(greenChannel[i],value) ;
        blueChannel[i] = std::min(blueChannel[i],value) ;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Min execution Time (Optimized): " << duration << " microseconds" << std::endl;
}

void Image::maxI(uint8_t value) {
    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();


    for (size_t i = 0; i < width * height; ++i) {
        redChannel[i] = std::max(redChannel[i],value) ;
        greenChannel[i] = std::max(greenChannel[i],value) ;
        blueChannel[i] = std::max(blueChannel[i],value) ;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Max execution Time (Non-optimized): " << duration << " microseconds" << std::endl;
}

void Image::maxIMod(uint8_t value) {
    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();


    size_t imageSize = width * height;


    // SIMD vector size
    constexpr size_t simdSize = 32;
    size_t remainder = imageSize % simdSize;



    // Load the constant value into a SIMD register
    __m256i constant = _mm256_set1_epi8(value);


    // Process the image data in SIMD vectorized blocks
    size_t i = 0;
    for (; i < imageSize - remainder; i += simdSize) {

        // Load the pixel data from memory into a SIMD register
        __m256i red = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&redChannel[i]));
        __m256i green = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&greenChannel[i]));
        __m256i blue = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&blueChannel[i]));

        // Add the constant value to each channel of the pixel using SIMD addition
        __m256i resultR = _mm256_max_epu8(red, constant);
        __m256i resultG = _mm256_max_epu8(green, constant);
        __m256i resultB = _mm256_max_epu8(blue, constant);

        // Store the result back to memory
        _mm256_store_si256(reinterpret_cast<__m256i*>(&redChannel[i]), resultR);
        _mm256_store_si256(reinterpret_cast<__m256i*>(&greenChannel[i]), resultG);
        _mm256_store_si256(reinterpret_cast<__m256i*>(&blueChannel[i]), resultB);
    }


    // Process the remaining elements (not a multiple of SIMD vector size) in a scalar manner
    for (; i < imageSize; ++i) {
        redChannel[i] = std::max(redChannel[i],value) ;
        greenChannel[i] = std::max(greenChannel[i],value) ;
        blueChannel[i] = std::max(blueChannel[i],value) ;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Min execution Time (Optimized): " << duration << " microseconds" << std::endl;
}

void Image::inverse() {

    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();

    uint8_t value = 255;

    int min = 0;
    int max = 255;

    for (size_t i = 0; i < width * height; ++i) {
        redChannel[i] = static_cast<uint8_t>(std::clamp(value-redChannel[i], min, max));
        greenChannel[i] = static_cast<uint8_t>(std::clamp(value-greenChannel[i], min, max)) ;
        blueChannel[i] = static_cast<uint8_t>(std::clamp(value-blueChannel[i], min, max)) ;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Inverse execution Time (Non-optimized): " << duration << " microseconds" << std::endl;

}

void Image::inverseMod() {
    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();

    uint8_t value = 255;

    size_t imageSize = width * height;


    // SIMD vector size
    constexpr size_t simdSize = 32;
    size_t remainder = imageSize % simdSize;

    // Load the constant value into a SIMD register
    __m256i constant = _mm256_set1_epi8(value);

    // Process the image data in SIMD vectorized blocks
    size_t i = 0;
    for (; i < imageSize - remainder; i += simdSize) {
        // Load the pixel data from memory into a SIMD register
        __m256i red = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&redChannel[i]));
        __m256i green = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&greenChannel[i]));
        __m256i blue = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&blueChannel[i]));

        // Add the constant value to each channel of the pixel using SIMD addition
        __m256i resultR = _mm256_subs_epu8(constant, red);
        __m256i resultG = _mm256_subs_epu8(constant, green);
        __m256i resultB = _mm256_subs_epu8(constant, blue);

        // Store the result back to memory
        _mm256_store_si256(reinterpret_cast<__m256i*>(&redChannel[i]), resultR);
        _mm256_store_si256(reinterpret_cast<__m256i*>(&greenChannel[i]), resultG);
        _mm256_store_si256(reinterpret_cast<__m256i*>(&blueChannel[i]), resultB);
    }

    int min = 0;
    int max = 255;

    // Process the remaining elements (not a multiple of SIMD vector size) in a scalar manner
    for (; i < imageSize; ++i) {
        redChannel[i] = static_cast<uint8_t>(std::clamp(redChannel[i]-value, min, max));
        greenChannel[i] = static_cast<uint8_t>(std::clamp(greenChannel[i]- value, min, max)) ;
        blueChannel[i] = static_cast<uint8_t>(std::clamp(blueChannel[i]- value, min, max)) ;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Inverse execution Time (Optimized): " << duration << " microseconds" << std::endl;
}

int get_cache_line_size ( int id ) {
    // Variables to hold the contents of the 4 i386 legacy registers
    uint32_t eax = 4; // get cache info
    uint32_t ebx;
    uint32_t ecx = id; // cache id
    uint32_t edx;

    // generates output in 4 registers eax, ebx, ecx and edx
    __asm__ (
            "cpuid" // call i386 cpuid instruction
            :   "+a" (eax), // contains the cpuid command code, 4 for cache query
    "=b" (ebx),
    "+c" (ecx), // contains the cache id
    "=d" (edx)
            );

    // See the page 3-191 of the manual.
    int cache_type = eax & 0x1F;

    // end of valid cache identifiers
    if ( cache_type == 0 ) {
        return -1;
    }

    return (ebx & 0xFFF) + 1;
}

void Image::filter(float *mat, int size, Image* img) {

    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }
    if(size%2==0){
        std::cerr << "Size of matrix must be odd number" << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();

    int rows = this->getHeight();
    int columns = this->getWidth();
    int rad = (size-1)/2;


    for (int row = rad; row < ( rows - rad ); ++row ) {
        for (int column = rad; column < (columns - rad); ++column ) {

            float valueR = 0;
            float valueG = 0;
            float valueB = 0;

            for (int i = 0; i < size; ++i ) {
                for (int j = 0; j < size; ++j ) {
                    int image_row = row + i - rad;
                    int image_column = column + j - rad;
                    int index = image_row * columns + image_column;

                    valueR += mat[i*size+j] * redChannel[index] ;
                    valueG += mat[i*size+j] * greenChannel[index] ;
                    valueB += mat[i*size+j] * blueChannel[index];

                }
            }
            int ic = row*columns+column;

            float min = 0;
            float max = 255;

            img->redChannel[ic] = static_cast<uint8_t>(std::clamp(valueR, min, max));
            img->greenChannel[ic] = static_cast<uint8_t>(std::clamp(valueG, min, max));
            img->blueChannel[ic] = static_cast<uint8_t>(std::clamp(valueB, min, max));
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Filter execution Time (Non-optimized): " << duration << " microseconds" << std::endl;
}

void Image::filterMod(float *mat, int size,Image* img) {
    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }
    if(size%2==0){
        std::cerr << "Size of matrix must be odd number" << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();

    int rows = this->getHeight();
    int columns = this->getWidth();
    int rad = (size-1)/2;

    const int number_of_words  = get_cache_line_size ( 0 );


    for(int rowB = rad; rowB<(rows-rad); rowB+=number_of_words) {
        for(int columnB = rad; columnB<(columns-rad); columnB+=number_of_words) {
            for (int row = rowB; row < (rows - rad) && row < (rowB + number_of_words); ++row) {
                for (int column = columnB; column < (columns - rad) && column < (columnB + number_of_words); ++column) {

                    float valueR = 0;
                    float valueG = 0;
                    float valueB = 0;

                    for (int i = 0; i < size; ++i) {
                        for (int j = 0; j < size; ++j) {
                            int image_row = row + i - rad;
                            int image_column = column + j - rad;
                            int index = image_row * columns + image_column;

                            valueR += mat[i * size + j] * redChannel[index];
                            valueG += mat[i * size + j] * greenChannel[index];
                            valueB += mat[i * size + j] * blueChannel[index];

                        }
                    }
                    int ic = row * columns + column;

                    float min = 0;
                    float max = 255;

                    img->redChannel[ic] = static_cast<uint8_t>(std::clamp(valueR, min, max));
                    img->greenChannel[ic] = static_cast<uint8_t>(std::clamp(valueG, min, max));
                    img->blueChannel[ic] = static_cast<uint8_t>(std::clamp(valueB, min, max));
                }
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Filter execution Time (Optimized): " << duration << " microseconds" << std::endl;
}

void Image::pow(double value) {
    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();

    double min = 0;
    double max = 255;

    for (size_t i = 0; i < width * height; ++i) {
        redChannel[i] = static_cast<uint8_t>(std::clamp(std::pow(redChannel[i], value), min, max));
        greenChannel[i] = static_cast<uint8_t>(std::clamp(std::pow(greenChannel[i], value), min, max)) ;
        blueChannel[i] = static_cast<uint8_t>(std::clamp(std::pow(blueChannel[i], value), min, max)) ;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Pow execution Time (Non-optimized): " << duration << " microseconds" << std::endl;
}

void Image::powMod(double value) {
    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();


    size_t imageSize = width * height;


    // SIMD vector size
    constexpr size_t simdSize = 32;
    size_t remainder = imageSize % simdSize;


    // Load the constant value into a SIMD register
    __m256 constant = _mm256_set1_ps(value);

    // Process the image data in SIMD vectorized blocks
    size_t i = 0;
    for (; i < imageSize - remainder; i += simdSize) {

        // Load the pixel data from memory into a SIMD register
        __m256i red = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&redChannel[i]));
        __m256i green = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&greenChannel[i]));
        __m256i blue = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&blueChannel[i]));

        // extraxt channels on lower and higher 128-bits
        __m128i red_lower = _mm256_extracti128_si256(red, 0);
        __m128i red_higher = _mm256_extracti128_si256(red, 1);
        __m128i green_lower = _mm256_extracti128_si256(green, 0);
        __m128i green_higher = _mm256_extracti128_si256(green, 1);
        __m128i blue_lower = _mm256_extracti128_si256(blue, 0);
        __m128i blue_higher = _mm256_extracti128_si256(blue, 1);

        // convert 8-bits channels to 32-bits
        __m256 red32[4];
        red32[0] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(red_lower));
        red32[1] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(red_lower, red_lower)));
        red32[2] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(red_higher));
        red32[3] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(red_higher, red_higher)));

        __m256 green32[4];
        green32[0] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(green_lower));
        green32[1] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(green_lower, green_lower)));
        green32[2] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(green_higher));
        green32[3] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(green_higher, green_higher)));

        __m256 blue32[4];
        blue32[0] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(blue_lower));
        blue32[1] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(blue_lower, blue_lower)));
        blue32[2] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(blue_higher));
        blue32[3] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(blue_higher, blue_higher)));

        __m256i redLog[4];
        __m256i greenLog[4];
        __m256i blueLog[4];

        // exp
        for(int j=0; j<4; j++){
            __m256 val = red32[j];
            val = log256_ps(val);
            val = exp256_ps(_mm256_mul_ps(val, constant));
            redLog[j] = _mm256_cvtps_epi32(val);

            val = green32[j];
            val = log256_ps(val);
            val = exp256_ps(_mm256_mul_ps(val, constant));
            greenLog[j] = _mm256_cvtps_epi32(val);

            val = blue32[j];
            val = log256_ps(val);
            val = exp256_ps(_mm256_mul_ps(val, constant));
            blueLog[j] = _mm256_cvtps_epi32(val);
        }

        // covert 32-bits back to 8-bits
        __m256i red16low = _mm256_packus_epi32(redLog[0], redLog[1]);
        red16low  = _mm256_permute4x64_epi64(red16low, 0xd8);
        __m256i red16high = _mm256_packus_epi32(redLog[2], redLog[3]);
        red16high  = _mm256_permute4x64_epi64(red16high, 0xd8);
        __m256i resultR = _mm256_packus_epi16(red16low, red16high);
        resultR  = _mm256_permute4x64_epi64(resultR, 0xd8);

        __m256i green16low = _mm256_packus_epi32(greenLog[0], greenLog[1]);
        green16low  = _mm256_permute4x64_epi64(green16low, 0xd8);
        __m256i green16high = _mm256_packus_epi32(greenLog[2], greenLog[3]);
        green16high  = _mm256_permute4x64_epi64(green16high, 0xd8);
        __m256i resultG = _mm256_packus_epi16(green16low, green16high);
        resultG  = _mm256_permute4x64_epi64(resultG, 0xd8);

        __m256i blue16low = _mm256_packus_epi32(blueLog[0], blueLog[1]);
        blue16low  = _mm256_permute4x64_epi64(blue16low, 0xd8);
        __m256i blue16high = _mm256_packus_epi32(blueLog[2], blueLog[3]);
        blue16high  = _mm256_permute4x64_epi64(blue16high, 0xd8);
        __m256i resultB = _mm256_packus_epi16(blue16low, blue16high);
        resultB  = _mm256_permute4x64_epi64(resultB, 0xd8);

        // Store the result back to memory
        _mm256_store_si256(reinterpret_cast<__m256i*>(&redChannel[i]), resultR);

        _mm256_store_si256(reinterpret_cast<__m256i*>(&greenChannel[i]), resultG);
        _mm256_store_si256(reinterpret_cast<__m256i*>(&blueChannel[i]), resultB);
    }

    double min = 0;
    double max = 255;

    // Process the remaining elements (not a multiple of SIMD vector size) in a scalar manner
    for (; i < imageSize; ++i) {
        redChannel[i] = static_cast<uint8_t>(std::clamp(std::pow(redChannel[i], value), min, max));
        greenChannel[i] = static_cast<uint8_t>(std::clamp(std::pow(greenChannel[i], value), min, max)) ;
        blueChannel[i] = static_cast<uint8_t>(std::clamp(std::pow(blueChannel[i], value), min, max)) ;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Pow execution Time (Optimized): " << duration << " microseconds" << std::endl;
}

void Image::sobel(Image* edges) {
    int GX[] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
    int GY[] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };

    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();

    int size = 3;
    int rows = this->getHeight();
    int columns = this->getWidth();
    int rad = (size-1)/2;


    for (int row = rad; row < ( rows - rad ); ++row ) {
        for (int column = rad; column < (columns - rad); ++column ) {

            double gx = 0;
            double gy = 0;

            for (int i = 0; i < size; ++i ) {
                for (int j = 0; j < size; ++j ) {
                    int image_row = row + i - rad;
                    int image_column = column + j - rad;
                    int index = image_row * columns + image_column;

                    double image_value = (double)(redChannel[index] + greenChannel[index] + blueChannel[index])/3;

                    int kernel_index = i * size + j;

                    gx += image_value * GX[kernel_index];
                    gy += image_value * GY[kernel_index];


                }
            }
            int index = row * width + column;
            edges->redChannel[index] = edges->greenChannel[index] = edges->blueChannel[index] = (sqrt ( gx * gx + gy * gy ));
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Sobel execution Time (Non-optimized): " << duration << " microseconds" << std::endl;
}

void Image::sobelMod(Image* edges){
    int GX[] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
    int GY[] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };

    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();

    int size = 3;
    int rows = this->getHeight();
    int columns = this->getWidth();
    int rad = (size-1)/2;

    const int number_of_words  = get_cache_line_size ( 0 );


    for(int rowB = rad; rowB<(rows-rad); rowB+=number_of_words) {
        for (int columnB = rad; columnB < (columns - rad); columnB += number_of_words) {
            for (int row = rad; row < (rows - rad); ++row) {
                for (int column = rad; column < (columns - rad); ++column) {

                    double gx = 0;
                    double gy = 0;

                    for (int i = 0; i < size; ++i) {
                        for (int j = 0; j < size; ++j) {
                            int image_row = row + i - rad;
                            int image_column = column + j - rad;
                            int index = image_row * columns + image_column;

                            double image_value =
                                    (double) (redChannel[index] + greenChannel[index] + blueChannel[index]) / 3;

                            int kernel_index = i * size + j;

                            gx += image_value * GX[kernel_index];
                            gy += image_value * GY[kernel_index];


                        }
                    }
                    int index = row * width + column;
                    edges->redChannel[index] = edges->greenChannel[index] = edges->blueChannel[index] = (sqrt(
                            gx * gx + gy * gy));
                }
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Sobel execution Time (Optimized): " << duration << " microseconds" << std::endl;


}

void Image::InvDivConstant(float value) {
    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();


    for (size_t i = 0; i < width * height; ++i) {
        redChannel[i] = ((float)redChannel[i]!=0) ? (value / (float)redChannel[i]):redChannel[i];
        greenChannel[i] = ((float)greenChannel[i]!=0) ? (value / (float)greenChannel[i]):greenChannel[i];
        blueChannel[i] =   ((float)blueChannel[i]!=0) ? (value / (float)blueChannel[i]):blueChannel[i];
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "InvDivConstant execution Time (Non-optimized): " << duration << " microseconds" << std::endl;
}

void Image::InvDivConstantMod(float value) {
    if (data == nullptr) {
        std::cerr << "No image data loaded." << std::endl;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();

    size_t imageSize = width * height;


    // SIMD vector size
    constexpr size_t simdSize = 32;
    size_t remainder = imageSize % simdSize;

    // Load the constant value into a SIMD register
    __m256 constant = _mm256_cvtepi32_ps(_mm256_set1_epi32(value));


    // Process the image data in SIMD vectorized blocks
    size_t i = 0;
    for (; i < imageSize - remainder; i += simdSize) {
        // Load the pixel data from memory into a SIMD register
        __m256i red = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&redChannel[i]));
        __m256i green = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&greenChannel[i]));
        __m256i blue = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&blueChannel[i]));

        // extraxt channels on lower and higher 128-bits
        __m128i red_lower = _mm256_extracti128_si256(red, 0);
        __m128i red_higher = _mm256_extracti128_si256(red, 1);
        __m128i green_lower = _mm256_extracti128_si256(green, 0);
        __m128i green_higher = _mm256_extracti128_si256(green, 1);
        __m128i blue_lower = _mm256_extracti128_si256(blue, 0);
        __m128i blue_higher = _mm256_extracti128_si256(blue, 1);

        // convert 8-bits channels to 32-bits
        __m256 red32[4];
        red32[0] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(red_lower));
        red32[1] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(red_lower, red_lower)));
        red32[2] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(red_higher));
        red32[3] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(red_higher, red_higher)));

        __m256 green32[4];
        green32[0] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(green_lower));
        green32[1] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(green_lower, green_lower)));
        green32[2] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(green_higher));
        green32[3] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(green_higher, green_higher)));

        __m256 blue32[4];
        blue32[0] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(blue_lower));
        blue32[1] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(blue_lower, blue_lower)));
        blue32[2] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(blue_higher));
        blue32[3] = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(blue_higher, blue_higher)));

        // divide with constant
        for(int j=0; j<4; j++){
            red32[j] = _mm256_div_ps(constant,red32[j]);
            green32[j] = _mm256_div_ps(constant,green32[j]);
            blue32[j] = _mm256_div_ps(constant,blue32[j]);
        }

        // covert 32-bits back to 8-bits
        __m256i red16low = _mm256_packus_epi32(_mm256_cvtps_epi32(red32[0]), _mm256_cvtps_epi32(red32[1]));
        red16low  = _mm256_permute4x64_epi64(red16low, 0xd8);
        __m256i red16high = _mm256_packus_epi32(_mm256_cvtps_epi32(red32[2]), _mm256_cvtps_epi32(red32[3]));
        red16high  = _mm256_permute4x64_epi64(red16high, 0xd8);
        __m256i resultR = _mm256_packus_epi16(red16low, red16high);
        resultR  = _mm256_permute4x64_epi64(resultR, 0xd8);

        __m256i green16low = _mm256_packus_epi32(_mm256_cvtps_epi32(green32[0]), _mm256_cvtps_epi32(green32[1]));
        green16low  = _mm256_permute4x64_epi64(green16low, 0xd8);
        __m256i green16high = _mm256_packus_epi32(_mm256_cvtps_epi32(green32[2]), _mm256_cvtps_epi32(green32[3]));
        green16high  = _mm256_permute4x64_epi64(green16high, 0xd8);
        __m256i resultG = _mm256_packus_epi16(green16low, green16high);
        resultG  = _mm256_permute4x64_epi64(resultG, 0xd8);

        __m256i blue16low = _mm256_packus_epi32(_mm256_cvtps_epi32(blue32[0]), _mm256_cvtps_epi32(blue32[1]));
        blue16low  = _mm256_permute4x64_epi64(blue16low, 0xd8);
        __m256i blue16high = _mm256_packus_epi32(_mm256_cvtps_epi32(blue32[2]), _mm256_cvtps_epi32(blue32[3]));
        blue16high  = _mm256_permute4x64_epi64(blue16high, 0xd8);
        __m256i resultB = _mm256_packus_epi16(blue16low, blue16high);
        resultB  = _mm256_permute4x64_epi64(resultB, 0xd8);

        // Store the result back to memory
        _mm256_store_si256(reinterpret_cast<__m256i*>(&redChannel[i]), resultR);

        _mm256_store_si256(reinterpret_cast<__m256i*>(&greenChannel[i]), resultG);
        _mm256_store_si256(reinterpret_cast<__m256i*>(&blueChannel[i]), resultB);
    }


    // Process the remaining elements (not a multiple of SIMD vector size) in a scalar manner
    for (; i < imageSize; ++i) {
        redChannel[i] = ((float)redChannel[i]!=0) ? (value / (float)redChannel[i]):redChannel[i];
        greenChannel[i] = ((float)greenChannel[i]!=0) ? (value / (float)greenChannel[i]):greenChannel[i];
        blueChannel[i] =   ((float)blueChannel[i]!=0) ? (value / (float)blueChannel[i]):blueChannel[i];
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "InvDivConstant execution Time (Optimized): " << duration << " microseconds" << std::endl;
}


