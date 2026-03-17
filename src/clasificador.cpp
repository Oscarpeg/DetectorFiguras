#include "clasificador.h"
#include "contornos.h"
#include "graham_scan.h"
#include "douglas_peucker.h"
#include <cmath>

namespace filtros {

FiguraDetectada clasificarFigura(const Contorno& contorno) {
    FiguraDetectada fig;
    fig.area = areaContorno(contorno);
    fig.centro = centroide(contorno);

    // 1. Graham Scan -> Convex Hull
    fig.hull = grahamScan(contorno);

    // 2. Douglas-Peucker sobre el hull
    //    epsilon = 4% del perímetro del hull (buen balance)
    double perimHull = perimetroContorno(fig.hull);
    double epsilon = 0.04 * perimHull;
    fig.aproximado = douglasPeucker(fig.hull, epsilon);
    fig.vertices = (int)fig.aproximado.size();

    // 3. Circularidad: 4π * area / perimetro²
    //    Circulo perfecto = 1.0, Cuadrado ≈ 0.78, Triángulo ≈ 0.60
    double perimContorno = perimetroContorno(contorno);
    fig.circularidad = (perimContorno > 0) ?
        (4.0 * CV_PI * fig.area) / (perimContorno * perimContorno) : 0;

    // 4. Clasificar combinando vértices y circularidad
    if (fig.circularidad > 0.75 && fig.vertices >= 6) {
        fig.nombre = "Circulo";
    } else if (fig.vertices == 3) {
        fig.nombre = "Triangulo";
    } else if (fig.vertices == 4) {
        // Distinguir cuadrado de rectángulo por aspect ratio
        cv::Rect bb = cv::boundingRect(fig.aproximado);
        double ratio = (double)bb.width / (double)bb.height;
        if (ratio > 0.8 && ratio < 1.2)
            fig.nombre = "Cuadrado";
        else
            fig.nombre = "Rectangulo";
    } else if (fig.vertices == 5) {
        fig.nombre = "Pentagono";
    } else if (fig.vertices == 6) {
        fig.nombre = "Hexagono";
    } else if (fig.vertices > 6) {
        fig.nombre = "Circulo";
    } else {
        fig.nombre = "Desconocido";
    }

    return fig;
}

} // namespace filtros
