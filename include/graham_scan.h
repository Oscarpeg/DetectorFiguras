#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

namespace filtros {

using Contorno = std::vector<cv::Point>;

// Graham Scan: pivote -> ángulo polar -> ordenar -> producto cruz
Contorno grahamScan(const Contorno& puntos);

} // namespace filtros
