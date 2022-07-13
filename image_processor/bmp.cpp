#include "bmp.h"

namespace BmpFile {
BMPHeader ReadBMPHeader(std::ifstream& in) {
    BMPHeader head;
    ReadNum(in, head.file_type);
    ReadNum(in, head.file_size);
    ReadNum(in, head.reserved1);
    ReadNum(in, head.reserved2);
    ReadNum(in, head.offset_data);
    return head;
}

DIBHeader ReadDIBHeader(std::ifstream& in) {
    DIBHeader dibhead;
    ReadNum(in, dibhead.header_size);
    ReadNum(in, dibhead.size.width);
    ReadNum(in, dibhead.size.height);
    ReadNum(in, dibhead.color_planes);
    ReadNum(in, dibhead.bits_per_pixel);
    ReadNum(in, dibhead.compression_method);
    ReadNum(in, dibhead.bitmap_data_size);
    ReadNum(in, dibhead.resolution.horizontal);
    ReadNum(in, dibhead.resolution.vertical);
    ReadNum(in, dibhead.colors.important);
    ReadNum(in, dibhead.colors.total);
    return dibhead;
}

void WriteBMPHeader(std::ofstream& out, const BMPHeader& header) {
    WriteNum(out, header.file_type);
    WriteNum(out, header.file_size);
    WriteNum(out, header.reserved1);
    WriteNum(out, header.reserved2);
    WriteNum(out, header.offset_data);
}

void WriteDIBHeader(std::ofstream& out, const DIBHeader& header) {
    WriteNum(out, header.header_size);
    WriteNum(out, header.size.width);
    WriteNum(out, header.size.height);
    WriteNum(out, header.color_planes);
    WriteNum(out, header.bits_per_pixel);
    WriteNum(out, header.compression_method);
    WriteNum(out, header.bitmap_data_size);
    WriteNum(out, header.resolution.horizontal);
    WriteNum(out, header.resolution.vertical);
    WriteNum(out, header.colors.important);
    WriteNum(out, header.colors.total);
}

Image LoadBMP(std::string_view path_in) {
    std::ifstream in{path_in, std::ios::binary};
    ReadBMPHeader(in);
    DIBHeader dib_header = ReadDIBHeader(in);
    Image image{dib_header.size.width, dib_header.size.height};
    for (auto i = 0; i < dib_header.size.height; ++i) {
        for (auto j = 0; j < dib_header.size.width; ++j) {
            Color c;
            ReadNum(in, c.R);
            ReadNum(in, c.G);
            ReadNum(in, c.B);
            image.GetPixel(dib_header.size.height - i - 1, j) = c;
        }
    }
    auto read_bytes = sizeof(Color) * dib_header.size.width;
    while (read_bytes % 4 != 0) {
        uint8_t padding;
        ReadNum(in, padding);
        ++read_bytes;
    }
    return image;
}

void SaveBMP(const Image& image, const std::string& path) {
    int fields=4;
    int per_pixel=3;
    BMPHeader bmphead;
    bmphead.file_size = sizeof(BMPHeader) + sizeof(DIBHeader) + image.GetHeight() * image.GetWidth() * 3;
    bmphead.offset_data = sizeof(BMPHeader) + sizeof(DIBHeader);
    DIBHeader dibhead;
    dibhead.size.width = image.GetWidth();
    dibhead.size.height = image.GetHeight();
    auto row_length = image.GetWidth() * per_pixel;
    auto padding = ((row_length + per_pixel) / fields) * fields - row_length;
    dibhead.bitmap_data_size = image.GetHeight() * (row_length + padding);

    std::ofstream out{path, std::ofstream::binary};
    WriteBMPHeader(out, bmphead);
    WriteDIBHeader(out, dibhead);
    for (int i = (image.GetHeight() - 1); i >= 0; --i) {
        for (size_t j = 0; j < image.GetWidth(); ++j) {
            WriteNum(out, image.GetPixel(i, j).R);
            WriteNum(out, image.GetPixel(i, j).G);
            WriteNum(out, image.GetPixel(i, j).B);
        }
        for (size_t j = 0; j < padding; ++j) {
            WriteNum<char>(out, 0);
        }
    }
    out.close();
}
}  // namespace BmpFile
