#include "graham_scan.h"
#include <algorithm>
#include <cmath>

namespace filtros {

static double productoCruz(const cv::Point& O, const cv::Point& A, const cv::Point& B) {
    return (double)(A.x - O.x) * (B.y - O.y) - (double)(A.y - O.y) * (B.x - O.x);
}

static double dist2(const cv::Point& a, const cv::Point& b) {
    double dx = a.x - b.x, dy = a.y - b.y;
    return dx*dx + dy*dy;
}

Contorno grahamScan(const Contorno& puntos) {
    int n = puntos.size();
    if (n < 3) return puntos;

    // 1. Encontrar pivote: punto más bajo (mayor Y), si empata menor X
    int pivotIdx = 0;
    for (int i = 1; i < n; i++)
        if (puntos[i].y > puntos[pivotIdx].y ||
            (puntos[i].y == puntos[pivotIdx].y && puntos[i].x < puntos[pivotIdx].x))
            pivotIdx = i;

    Contorno pts = puntos;
    std::swap(pts[0], pts[pivotIdx]);
    cv::Point pivote = pts[0];

    // 2. Ordenar por ángulo polar respecto al pivote
    std::sort(pts.begin() + 1, pts.end(),
        [&pivote](const cv::Point& a, const cv::Point& b) {
            double angA = std::atan2(a.y - pivote.y, a.x - pivote.x);
            double angB = std::atan2(b.y - pivote.y, b.x - pivote.x);
            if (std::abs(angA - angB) > 1e-9) return angA < angB;
            return dist2(pivote, a) < dist2(pivote, b);
        });

    // 3. Recorrer manteniendo solo giros a la izquierda
    Contorno hull;
    hull.push_back(pts[0]);
    hull.push_back(pts[1]);

    for (int i = 2; i < n; i++) {
        while (hull.size() > 1) {
            cv::Point top = hull[hull.size()-1];
            cv::Point sec = hull[hull.size()-2];
            if (productoCruz(sec, top, pts[i]) <= 0)
                hull.pop_back();
            else break;
        }
        hull.push_back(pts[i]);
    }
    return hull;
}

} // namespace filtros
