#pragma once
#include <opencv2/opencv.hpp>

namespace utils {

cv::Mat convertirGris(const cv::Mat& bgr);
cv::Mat convolucion(const cv::Mat& imagen, const cv::Mat& kernel);
cv::Mat normalizar(const cv::Mat& src);
cv::Mat recortar(const cv::Mat& src);

} // namespace utils
