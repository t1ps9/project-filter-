#include "filters.h"
#include <math.h>
#include "image.h"

void NegativeFilter::Apply(Image& image) {
    for (size_t i = 0; i < image.GetHeight(); ++i) {
        for (size_t j = 0; j < image.GetWidth(); ++j) {
            u_int8_t r = 255 - image.GetPixel(i, j).R;
            u_int8_t g = 255 - image.GetPixel(i, j).G;
            u_int8_t b = 255 - image.GetPixel(i, j).B;
            image.GetPixel(i, j).R = r;
            image.GetPixel(i, j).G = g;
            image.GetPixel(i, j).B = b;
        }
    }
}

void GrayScale::Apply(Image& image) {
    for (size_t i = 0; i < image.GetHeight(); ++i) {
        for (size_t j = 0; j < image.GetWidth(); ++j) {
            u_int8_t r =
                0.299 * image.GetPixel(i, j).R + 0.587 * image.GetPixel(i, j).G + 0.114 * image.GetPixel(i, j).B;
            image.GetPixel(i, j).R = r;
            image.GetPixel(i, j).G = r;
            image.GetPixel(i, j).B = r;
        }
    }
}

void Crop::Apply(Image& image) {
    if (new_h_ <= image.GetHeight()) {
        image.ResizeHeight(new_h_);
    }
    if (new_w_ < image.GetWidth()) {
        image.ResizeWidth(new_w_);
    }
}

void Sharp::Apply(Image& image) {
    Image new_image = image;
    for (size_t i = 0; i < new_image.GetHeight(); ++i) {
        for (size_t j = 0; j < new_image.GetWidth(); ++j) {
            using IJ = std::pair<int, int>;
            std::vector<IJ> coordinates = {{0, -1}, {-1, 0}, {0, 0}, {1, 0}, {0, 1}};
            std::vector<int> coeff = {-1, -1, 5, -1, -1};
            int r = 0;
            int g = 0;
            int b = 0;
            size_t index = 0;
            for (IJ& pair : coordinates) {
                size_t x = std::min(std::max(0, static_cast<int>(i) + pair.first),
                                    static_cast<int>(new_image.GetHeight()) - 1);
                size_t y = std::min(std::max(0, static_cast<int>(j) + pair.second),
                                    static_cast<int>(new_image.GetWidth()) - 1);
                r += coeff[index] * new_image.GetPixel(x, y).R;
                g += coeff[index] * new_image.GetPixel(x, y).G;
                b += coeff[index] * new_image.GetPixel(x, y).B;
                ++index;
            }
            image.GetPixel(i, j).R = static_cast<uint8_t>(std::min(255, std::max(0, r)));
            image.GetPixel(i, j).G = static_cast<uint8_t>(std::min(255, std::max(0, g)));
            image.GetPixel(i, j).B = static_cast<uint8_t>(std::min(255, std::max(0, b)));
        }
    }
}

void Edge::Apply(Image& image) {
    GrayScale::Apply(image);
    Image new_image = image;
    for (size_t i = 0; i < new_image.GetHeight(); ++i) {
        for (size_t j = 0; j < new_image.GetWidth(); ++j) {
            using IJ = std::pair<int, int>;
            std::vector<IJ> coordinates = {{0, -1}, {-1, 0}, {0, 0}, {1, 0}, {0, 1}};
            std::vector<int> coeff = {-1, -1, 4, -1, -1};
            int r = 0;
            int g = 0;
            int b = 0;
            size_t index = 0;
            for (IJ& pair : coordinates) {
                size_t x = std::min(std::max(0, static_cast<int>(i) + pair.first),
                                    static_cast<int>(new_image.GetHeight()) - 1);
                size_t y = std::min(std::max(0, static_cast<int>(j) + pair.second),
                                    static_cast<int>(new_image.GetWidth()) - 1);
                r += coeff[index] * new_image.GetPixel(x, y).R;
                g += coeff[index] * new_image.GetPixel(x, y).G;
                b += coeff[index] * new_image.GetPixel(x, y).B;
                ++index;
            }
            image.GetPixel(i, j).R = static_cast<uint8_t>(std::min(255, std::max(0, r)));
            image.GetPixel(i, j).G = static_cast<uint8_t>(std::min(255, std::max(0, g)));
            image.GetPixel(i, j).B = static_cast<uint8_t>(std::min(255, std::max(0, b)));
        }
    }
    for (size_t i = 0; i < image.GetHeight(); ++i) {
        for (size_t j = 0; j < image.GetWidth(); ++j) {
            if (image.GetPixel(i, j).R > treshold_) {
                image.GetPixel(i, j).R = 255;
                image.GetPixel(i, j).G = 255;
                image.GetPixel(i, j).B = 255;
            } else {
                image.GetPixel(i, j).R = 0;
                image.GetPixel(i, j).G = 0;
                image.GetPixel(i, j).B = 0;
            }
        }
    }
}

double Gauss::FuncGauss(int32_t x, int32_t y) {
    return pow(M_E, -(pow(x, 2) + pow(y, 2)) / (2 * pow(sigma_, 2))) / (2 * M_PI * pow(sigma_, 2));
}

void Gauss::Apply(Image& image) {
    int kernel_size = 3 * sigma_ + 1;
    std::vector<std::vector<double>> kernel;
    double all_sum = 0;
    for (int32_t x = -kernel_size; x <= kernel_size; ++x) {
        std::vector<double> tmp;
        for (int32_t y = -kernel_size; y <= kernel_size; ++y) {
            double func_gauss = FuncGauss(x, y);
            all_sum += func_gauss;
            tmp.push_back(func_gauss);
        }
        kernel.push_back(tmp);
    }
    for (int32_t i = -kernel_size; i < kernel_size; ++i) {
        for (int32_t j = kernel_size; j < kernel_size; ++j) {
            kernel[i + kernel_size][j + kernel_size] /= all_sum;
        }
    }
    MatrixApply(image, kernel);
}

void Twirl::Apply(Image& image) {
    Image new_image = image;
    for (int y = 0; y < new_image.GetHeight(); ++y) {
        for (int x = 0; x < new_image.GetWidth() - 3; ++x) {
            for (int q = 0; q < 3; ++q) {
                int y1 = y - new_image.GetHeight() / 2;
                int x1 = x - new_image.GetWidth() / 2;
                int rotation = 180 * (M_PI / 180);

                double radian = sqrt(pow(x1, 2) + pow(y1, 2));
                double cl =
                    atan2(y1, x1) + rotation * (((new_image.GetHeight() / 2) - radian) / (new_image.GetHeight() / 2));

                int rotation_y = (new_image.GetHeight() / 2) + radian * sin(cl);
                int rotation_x = (new_image.GetWidth() / 2) + radian * cos(cl);

                if (rotation_y > new_image.GetHeight() - 1) {
                    rotation_y = new_image.GetHeight() - 1;
                } else if (rotation_y < 0) {
                    rotation_y = 0;
                }
                if (rotation_x > new_image.GetWidth() - 1) {
                    rotation_x = new_image.GetWidth() - 1;
                } else if (rotation_x < 0) {
                    rotation_x = 0;
                }
                image.GetPixel(y, x + q) = new_image.GetPixel(rotation_y, rotation_x + q);
            }
        }
    }
}
