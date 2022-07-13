#pragma once
#include "image.h"
#include <unordered_map>

struct BaseFilter {
    virtual void Apply(Image& image) = 0;
    virtual ~BaseFilter() = default;
};

struct MatrixBaseFilter : BaseFilter {
    std::vector<std::vector<int>> base = {};
};

struct NegativeFilter : BaseFilter {
    void Apply(Image& image) override;
    ~NegativeFilter() = default;
};

struct GrayScale : BaseFilter {
    void Apply(Image& image) override;
    ~GrayScale() = default;
};

struct Crop : BaseFilter {
    Crop(int w, int h) : new_w_(w), new_h_(h){};
    void Apply(Image& image) override;
    ~Crop() = default;

protected:
    int new_w_;
    int new_h_;
};

struct Sharp : MatrixBaseFilter {
    void Apply(Image& image) override;
    ~Sharp() = default;
};

struct Edge : GrayScale {
    void Apply(Image& image) override;
    Edge(int treshold) : treshold_(treshold){};
    ~Edge() = default;

protected:
    int treshold_ = 0;
};

struct Gauss : MatrixBaseFilter {
    void Apply(Image& image) override;
    double FuncGauss(int32_t x, int32_t y);
    Gauss(double sigma) : sigma_(sigma){};
    ~Gauss() = default;

protected:
    double sigma_;
};

template <typename T>
void MatrixApply(Image& image, std::vector<std::vector<T>> coeff) {
    struct NewColor {
        T r, g, b;
    };
    std::vector<std::vector<NewColor>> new_bit;
    new_bit.resize(image.GetHeight(), std::vector<NewColor>(image.GetWidth()));
    int32_t edge = coeff.size() / 2;
    for (size_t i = 0; i < image.GetHeight(); ++i) {
        for (size_t j = 0; j < image.GetWidth(); ++j) {
            double n_r = 0;
            double n_g = 0;
            double n_b = 0;
            for (int32_t delta_x = -edge; delta_x <= edge; ++delta_x) {
                for (int32_t delta_y = -edge; delta_y <= edge; ++delta_y) {
                    int32_t x = i + delta_x;
                    int32_t y = j + delta_y;
                    x = std::min(std::max(x, 0), static_cast<int32_t>(image.GetHeight()) - 1);
                    y = std::min(std::max(y, 0), static_cast<int32_t>(image.GetWidth()) - 1);
                    n_r += image.GetPixel(x, y).R * coeff[delta_x + edge][delta_y + edge];
                    n_g += image.GetPixel(x, y).G * coeff[delta_x + edge][delta_y + edge];
                    n_b += image.GetPixel(x, y).B * coeff[delta_x + edge][delta_y + edge];
                }
            }
            NewColor cc;

            cc.r = std::min(255.0, std::max(0.0, n_r));
            cc.g = std::min(255.0, std::max(0.0, n_g));
            cc.b = std::min(255.0, std::max(0.0, n_b));
            new_bit[i][j] = cc;
        }
    }
    for (size_t i = 0; i < new_bit.size(); ++i) {
        for (size_t j = 0; j < new_bit.size(); ++j) {
            image.GetPixel(i, j).R = new_bit[i][j].r;
            image.GetPixel(i, j).G = new_bit[i][j].g;
            image.GetPixel(i, j).B = new_bit[i][j].b;
        }
    }
}

struct Twirl : BaseFilter {
    void Apply(Image& image) override;
    ~Twirl() = default;
};
namespace MakeAllFilters {
enum FilterTypes { Crop, GrayScale, NegativeFilter, Sharp, Edge, Gauss, Twirl };

class FilterDescriptor {
public:
    FilterTypes filter_;
    std::vector<std::string> filter_params;
};
}  // namespace MakeAllFilters
