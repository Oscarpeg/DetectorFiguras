#pragma once
#include <opencv2/opencv.hpp>

namespace filtros {

cv::Mat dilatarManual(const cv::Mat& binaria, int ksize = 3);
cv::Mat erosionarManual(const cv::Mat& binaria, int ksize = 3);
// Cierre morfológico: dilatar + erosionar (cierra huecos)
cv::Mat cierreManual(const cv::Mat& binaria, int ksize = 3);

} // namespace filtros
