#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

namespace filtros {

using Contorno = std::vector<cv::Point>;

struct FiguraDetectada {
    std::string nombre;
    Contorno hull;              // Convex hull (Graham Scan)
    Contorno aproximado;        // Hull simplificado (Douglas-Peucker)
    int vertices;
    double area;
    double circularidad;
    cv::Point centro;
};

// Clasifica usando Graham Scan + Douglas-Peucker + circularidad
FiguraDetectada clasificarFigura(const Contorno& contorno);

} // namespace filtros
