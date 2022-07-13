#pragma once
#include <cstdint>
#include <vector>
class Color {
public:
    u_int8_t R = 0;
    u_int8_t G = 0;
    u_int8_t B = 0;
};

class Image {
public:
    Image(int w, int h);
    Color& GetPixel(int x, int y);
    Color GetPixel(int x, int y) const;
    size_t GetWidth() const;
    size_t GetHeight() const;
    void ResizeHeight(int new_h);
    void ResizeWidth(int new_w);

private:
    std::vector<std::vector<Color>> bitvector_;
};
