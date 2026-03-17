#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

namespace filtros {

using Contorno = std::vector<cv::Point>;

// Douglas-Peucker: simplifica un polígono conservando solo los vértices
// importantes (esquinas reales). epsilon controla la precisión:
//   epsilon bajo  = más vértices (más detalle)
//   epsilon alto  = menos vértices (más simplificado)
// Típico para imágenes 320x240: epsilon = 3% a 5% del perímetro
Contorno douglasPeucker(const Contorno& curva, double epsilon);

} // namespace filtros
