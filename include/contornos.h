#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

namespace filtros {

using Contorno = std::vector<cv::Point>;

std::vector<Contorno> encontrarContornos(const cv::Mat& binaria);
double areaContorno(const Contorno& contorno);
double perimetroContorno(const Contorno& contorno);
cv::Point centroide(const Contorno& contorno);

} // namespace filtros
