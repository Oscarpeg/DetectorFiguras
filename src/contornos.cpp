#include "contornos.h"
#include <cmath>

namespace filtros {

static const int dx[8] = {-1,-1, 0, 1,1, 1, 0,-1};
static const int dy[8] = { 0, 1, 1, 1,0,-1,-1,-1};

static Contorno trazarContorno(const cv::Mat& binaria, int si, int sj, cv::Mat& vis) {
    Contorno c;
    int rows = binaria.rows, cols = binaria.cols;
    int ci = si, cj = sj, dir = 6;
    int maxPasos = rows * cols;
    bool primero = true;

    for (int paso = 0; paso <= maxPasos; paso++) {
        c.push_back(cv::Point(cj, ci));
        vis.at<uchar>(ci, cj) = 255;

        int startDir = (dir + 5) % 8;
        bool found = false;
        for (int k = 0; k < 8; k++) {
            int d = (startDir + k) % 8;
            int ni = ci + dx[d], nj = cj + dy[d];
            if (ni >= 0 && ni < rows && nj >= 0 && nj < cols &&
                binaria.at<uchar>(ni, nj) == 255) {
                ci = ni; cj = nj; dir = d;
                found = true;
                break;
            }
        }
        if (!found) break;
        if (!primero && ci == si && cj == sj) break;
        primero = false;
    }
    return c;
}

std::vector<Contorno> encontrarContornos(const cv::Mat& binaria) {
    int rows = binaria.rows, cols = binaria.cols;
    std::vector<Contorno> contornos;
    cv::Mat vis = cv::Mat::zeros(rows, cols, CV_8U);

    for (int i = 1; i < rows - 1; i++)
        for (int j = 1; j < cols - 1; j++)
            if (binaria.at<uchar>(i, j) == 255 && vis.at<uchar>(i, j) == 0) {
                bool esBorde = false;
                for (int k = 0; k < 8; k++) {
                    int ni = i + dx[k], nj = j + dy[k];
                    if (ni >= 0 && ni < rows && nj >= 0 && nj < cols)
                        if (binaria.at<uchar>(ni, nj) == 0) { esBorde = true; break; }
                }
                if (esBorde) {
                    Contorno c = trazarContorno(binaria, i, j, vis);
                    if (c.size() >= 15) contornos.push_back(c);
                }
            }
    return contornos;
}

double areaContorno(const Contorno& c) {
    double area = 0.0;
    int n = c.size();
    if (n < 3) return 0;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        area += c[i].x * c[j].y - c[j].x * c[i].y;
    }
    return std::abs(area) / 2.0;
}

double perimetroContorno(const Contorno& c) {
    double p = 0;
    int n = c.size();
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        double ddx = c[j].x - c[i].x, ddy = c[j].y - c[i].y;
        p += std::sqrt(ddx*ddx + ddy*ddy);
    }
    return p;
}

cv::Point centroide(const Contorno& c) {
    double cx = 0, cy = 0;
    for (auto& p : c) { cx += p.x; cy += p.y; }
    int n = c.size();
    return cv::Point((int)(cx/n), (int)(cy/n));
}

} // namespace filtros
