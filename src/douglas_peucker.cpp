#include "douglas_peucker.h"
#include <cmath>

namespace filtros {

// Distancia perpendicular de un punto a la recta definida por A-B
static double distanciaPuntoRecta(const cv::Point& P, const cv::Point& A, const cv::Point& B) {
    double dx = B.x - A.x;
    double dy = B.y - A.y;
    double longitud2 = dx * dx + dy * dy;

    if (longitud2 < 1e-10) {
        // A y B son el mismo punto
        double px = P.x - A.x, py = P.y - A.y;
        return std::sqrt(px * px + py * py);
    }

    // Distancia perpendicular = |cross product| / |AB|
    double cross = std::abs(dy * (P.x - A.x) - dx * (P.y - A.y));
    return cross / std::sqrt(longitud2);
}

// Versión recursiva interna
static void dpRecursivo(const Contorno& puntos, int inicio, int fin,
                        double epsilon, std::vector<bool>& mantener) {
    if (fin - inicio < 2) return;

    // Encontrar el punto más lejano de la recta inicio-fin
    double maxDist = 0;
    int maxIdx = inicio;

    for (int i = inicio + 1; i < fin; i++) {
        double d = distanciaPuntoRecta(puntos[i], puntos[inicio], puntos[fin]);
        if (d > maxDist) {
            maxDist = d;
            maxIdx = i;
        }
    }

    // Si la distancia máxima > epsilon, ese punto es importante (esquina)
    if (maxDist > epsilon) {
        mantener[maxIdx] = true;
        // Recursión en las dos mitades
        dpRecursivo(puntos, inicio, maxIdx, epsilon, mantener);
        dpRecursivo(puntos, maxIdx, fin, epsilon, mantener);
    }
    // Si no, todos los puntos intermedios se descartan (son casi colineales)
}

Contorno douglasPeucker(const Contorno& curva, double epsilon) {
    int n = curva.size();
    if (n < 3) return curva;

    // Para polígono cerrado: encontrar los dos puntos más distantes
    // Estos serán los puntos de partición
    int idx1 = 0, idx2 = 0;
    double maxDist = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double dx = curva[i].x - curva[j].x;
            double dy = curva[i].y - curva[j].y;
            double d = dx * dx + dy * dy;
            if (d > maxDist) {
                maxDist = d;
                idx1 = i;
                idx2 = j;
            }
        }
    }

    // Reordenar para que idx1 < idx2
    if (idx1 > idx2) std::swap(idx1, idx2);

    // Marcar qué puntos mantener
    std::vector<bool> mantener(n, false);
    mantener[idx1] = true;
    mantener[idx2] = true;

    // Aplicar DP a las dos mitades del polígono cerrado
    // Primera mitad: idx1 -> idx2
    dpRecursivo(curva, idx1, idx2, epsilon, mantener);

    // Segunda mitad: idx2 -> idx1 (pasando por el final y el inicio)
    // Crear secuencia lineal para esta mitad
    Contorno segunda;
    for (int i = idx2; i < n; i++) segunda.push_back(curva[i]);
    for (int i = 0; i <= idx1; i++) segunda.push_back(curva[i]);

    // Mapeo de índices de 'segunda' a 'curva'
    std::vector<int> mapeo;
    for (int i = idx2; i < n; i++) mapeo.push_back(i);
    for (int i = 0; i <= idx1; i++) mapeo.push_back(i);

    std::vector<bool> mantener2(segunda.size(), false);
    mantener2[0] = true;
    mantener2[segunda.size() - 1] = true;
    dpRecursivo(segunda, 0, (int)segunda.size() - 1, epsilon, mantener2);

    // Transferir resultados
    for (int i = 0; i < (int)segunda.size(); i++)
        if (mantener2[i]) mantener[mapeo[i]] = true;

    // Construir resultado
    Contorno resultado;
    for (int i = 0; i < n; i++)
        if (mantener[i]) resultado.push_back(curva[i]);

    return resultado;
}

} // namespace filtros
