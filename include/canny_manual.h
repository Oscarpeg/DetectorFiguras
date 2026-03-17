#pragma once
#include <opencv2/opencv.hpp>

namespace filtros {

cv::Mat cannyManual(const cv::Mat& gris, double umbral_bajo, double umbral_alto,
                    double sigma = 1.4);

} // namespace filtros
