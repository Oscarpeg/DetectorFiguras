#include "morfologia.h"

namespace filtros {

cv::Mat dilatarManual(const cv::Mat& binaria, int ksize) {
    int rows = binaria.rows, cols = binaria.cols, pad = ksize / 2;
    cv::Mat res = cv::Mat::zeros(rows, cols, CV_8U);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
            bool encontro = false;
            for (int ki = -pad; ki <= pad && !encontro; ki++)
                for (int kj = -pad; kj <= pad && !encontro; kj++) {
                    int ni = i + ki, nj = j + kj;
                    if (ni >= 0 && ni < rows && nj >= 0 && nj < cols)
                        if (binaria.at<uchar>(ni, nj) == 255) encontro = true;
                }
            if (encontro) res.at<uchar>(i, j) = 255;
        }
    return res;
}

cv::Mat erosionarManual(const cv::Mat& binaria, int ksize) {
    int rows = binaria.rows, cols = binaria.cols, pad = ksize / 2;
    cv::Mat res = cv::Mat::zeros(rows, cols, CV_8U);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
            bool todos = true;
            for (int ki = -pad; ki <= pad && todos; ki++)
                for (int kj = -pad; kj <= pad && todos; kj++) {
                    int ni = i + ki, nj = j + kj;
                    if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
                        if (binaria.at<uchar>(ni, nj) != 255) todos = false;
                    } else todos = false;
                }
            if (todos) res.at<uchar>(i, j) = 255;
        }
    return res;
}

cv::Mat cierreManual(const cv::Mat& binaria, int ksize) {
    // Cierre = dilatar primero, luego erosionar
    // Cierra huecos pequeños en los contornos
    cv::Mat dilatada = dilatarManual(binaria, ksize);
    return erosionarManual(dilatada, ksize);
}

} // namespace filtros
