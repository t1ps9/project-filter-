#include "image.h"
Image::Image(int w, int h) {
    bitvector_.resize(h, std::vector<Color>(w, {.R = 0, .G = 0, .B = 0}));
}

Color& Image::GetPixel(int x, int y) {
    return bitvector_[x][y];
}

Color Image::GetPixel(int x, int y) const {
    return bitvector_[x][y];
}

size_t Image::GetWidth() const {
    return bitvector_[0].size();
}

size_t Image::GetHeight() const {
    return bitvector_.size();
}

void Image::ResizeHeight(int new_h) {
    if (new_h <= static_cast<int>(bitvector_.size())) {
        bitvector_.resize(new_h);
    }
}

void Image::ResizeWidth(int new_w) {
    if (new_w <= static_cast<int>(bitvector_[0].size())) {
        for (size_t i = 0; i < bitvector_.size(); ++i) {
            bitvector_[i].resize(new_w);
        }
    }
}
