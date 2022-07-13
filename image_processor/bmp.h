#pragma once
#include <cstdint>
#include <fstream>
#include "image.h"

namespace BmpFile {
#pragma pack(push, 1)
struct BMPHeader {
    uint16_t file_type = 0x4d42;
    uint32_t file_size = 0;
    uint16_t reserved1 = 0;
    uint16_t reserved2 = 0;
    uint32_t offset_data = 0;
};

struct DIBHeader {
    uint32_t header_size = 40;
    struct Size {
        int32_t width = 0;
        int32_t height = 0;
    };
    Size size = {};
    uint16_t color_planes = 1;
    uint16_t bits_per_pixel = 24;
    uint32_t compression_method = 0;
    uint32_t bitmap_data_size = 0;
    struct Resolution {
        int32_t horizontal = 11811;
        int32_t vertical = 11811;
    };
    Resolution resolution = {};
    struct Colors {
        uint32_t total = 0;
        uint32_t important = 0;
    } colors = {};
};
#pragma pack(pop)

template <typename T>
void ReadNum(std::ifstream& in, T& val) {
    uint8_t buff[sizeof(val)];
    in.read(reinterpret_cast<char*>(buff), sizeof(val));
    for (int i = 0; i < sizeof(val); i++) {
        val |= buff[i] << (8 * i);
    }
}

template <typename T>
T ReadNum(std::ifstream& in) {
    T val;
    ReadNum(in, val);
    return val;
}

template <typename T>
void WriteNum(std::ofstream& out, const T& val) {
    for (int i = 0; i < sizeof(val); ++i) {
        out.put(val >> (8 * i) & 0xFF);
    }
}

Image LoadBMP(std::string_view path_in);
void SaveBMP(const Image& image, const std::string& path);
}  // namespace BmpFile
