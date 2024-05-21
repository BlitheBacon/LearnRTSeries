#ifndef RTW_STB_IMAGE_H
#define RTW_STB_IMAGE_H

// Diasble strict warnings for this header from the Microsoft Visual C++ compiler
#ifdef _MSC_VER
   #pragam warning (push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "../lib/stb/stb_image.h"

#include <cstdlib>
#include <iostream>

class rtw_image
{
public:
    rtw_image() = default;

    explicit rtw_image(const char *image_filename)
    {
        // Loads image data from the specified file. If the RTW_IMAGES environment variable is defined,
        // looks only in that directory for the image file. If the image was not found, searches for the
        // specified image file first from the current directory, then in the images/ subdirectory, then
        // the _parent's_ images/ subdirectory, and then _that_ parent, and so on, for six levels up. If
        // the image was not loaded successfully, width() and height() will return 0.

        const auto filename = std::string(image_filename);
        const auto imageDir = getenv("RTW_IMAGES");

        // Hunt for the image file in some likely locations.
        if (imageDir && load(std::string(imageDir) + "/" + image_filename)) return;
        if (load(filename)) return;
        if (load("images/" + filename)) return;
        if (load("../images/" + filename)) return;
        if (load("../../images/" + filename)) return;
        if (load("../../../images/" + filename)) return;
        if (load("../../../../images/" + filename)) return;
        if (load("../../../../../images/" + filename)) return;
        if (load("../../../../../../images/" + filename)) return;

        std::cerr << "ERROR: Could not load image file " << image_filename << ".\n";
    }

    ~rtw_image()
    {
        delete[] bData;
        STBI_FREE(fData);
    }

    bool load(const std::string &filename)
    {
        // Loads the linear (gamma  = 1) image data from the given file name. Returns true if the
        // load succeeded. The resulting data buffer contains the three [0.0, 1.0] floating-point
        // values for the first pixel (red, green, then blue). Pixels are contiguous, going left
        // to right for the width of the image, followed by the next row below, for the full height
        // of the image.

        auto n = bytes_per_pixel; // Dummy out parameter: original components per pixel
        fData  = stbi_loadf(filename.c_str(), &image_width, &image_height, &n, bytes_per_pixel);
        if (fData == nullptr) return false;

        bytes_per_scanline = image_width * bytes_per_pixel;
        convert_to_bytes();

        return true;
    }

    int width() const { return (fData == nullptr) ? 0 : image_width; }
    int height() const { return (fData == nullptr) ? 0 : image_height; }

    const unsigned char *pixel_data(int x, int y) const
    {
        // Return the address of the three RGB bytes of the pixel at x, y, z. If there is no image
        // data, returns magenta.

        static unsigned char magenta[] = {255, 0, 255};
        if (bData == nullptr) return magenta;

        x = clamp(x, 0, image_width);
        y = clamp(y, 0, image_height);

        return bData + (y * bytes_per_scanline) + (x * bytes_per_pixel);
    }

private:
    const int      bytes_per_pixel    = 3;
    float *        fData              = nullptr; // Linear floating point pixel data
    unsigned char *bData              = nullptr; // Linear 8-bit pixel data
    int            image_width        = 0;       // Loaded image width
    int            image_height       = 0;       // Loaded image height
    int            bytes_per_scanline = 0;

    static int clamp(const int x, const int low, const int high)
    {
        // Return the value clamped to the range [low, high).

        if (x < low) return low;
        if (x < high) return x;
        return high - 1;
    }

    static unsigned char float_to_byte(const float value)
    {
        if (value <= 0.0) return 0.0;
        if (value >= 1.0) return 255;

        return static_cast<unsigned char>(256.0 * value);
    }

    void convert_to_bytes()
    {
        // Convert the linear floating point pixel data to bytes, storing the resulting byte
        // data in the `bData` meber.

        const int total_bytes = image_width * image_height * bytes_per_pixel;
        bData                 = new unsigned char[total_bytes];

        // Iterate through all pixel components, converting from [0.0, 1.0] float values to
        // unsigned [0, 255] byte values.

        auto *bptr = bData;
        auto *fptr = fData;
        for (auto i = 0; i < total_bytes; i++, fptr++, bptr++) *bptr = float_to_byte(*fptr);
    }
};

// Restore MSVC compiler warnings
#ifdef _MSC_VER
    #pragma warning (pop)
#endif

#endif
