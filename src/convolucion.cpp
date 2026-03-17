#include "convolucion.h"
#include <cmath>

namespace utils {

cv::Mat convertirGris(const cv::Mat& bgr) {
    int rows = bgr.rows, cols = bgr.cols;
    cv::Mat gris(rows, cols, CV_8U);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
            cv::Vec3b p = bgr.at<cv::Vec3b>(i, j);
            double val = 0.114 * p[0] + 0.587 * p[1] + 0.299 * p[2];
            gris.at<uchar>(i, j) = (uchar)std::min(std::max(val, 0.0), 255.0);
        }
    return gris;
}

cv::Mat convolucion(const cv::Mat& imagen, const cv::Mat& kernel) {
    int rows = imagen.rows, cols = imagen.cols;
    int kR = kernel.rows, kC = kernel.cols;
    int padY = kR / 2, padX = kC / 2;
    cv::Mat res(rows, cols, CV_64F, cv::Scalar(0.0));
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
            double sum = 0.0;
            for (int ki = 0; ki < kR; ki++)
                for (int kj = 0; kj < kC; kj++) {
                    int ni = i + ki - padY, nj = j + kj - padX;
                    if (ni >= 0 && ni < rows && nj >= 0 && nj < cols)
                        sum += (double)imagen.at<uchar>(ni, nj) * kernel.at<double>(ki, kj);
                }
            res.at<double>(i, j) = sum;
        }
    return res;
}

cv::Mat normalizar(const cv::Mat& src) {
    int rows = src.rows, cols = src.cols;
    double minV = src.at<double>(0, 0), maxV = minV;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
            double v = src.at<double>(i, j);
            if (v < minV) minV = v;
            if (v > maxV) maxV = v;
        }
    cv::Mat dst(rows, cols, CV_8U);
    double rango = maxV - minV;
    if (rango < 1e-10) rango = 1.0;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            dst.at<uchar>(i, j) = (uchar)(255.0 * (src.at<double>(i, j) - minV) / rango);
    return dst;
}

cv::Mat recortar(const cv::Mat& src) {
    int rows = src.rows, cols = src.cols;
    cv::Mat dst(rows, cols, CV_8U);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            dst.at<uchar>(i, j) = (uchar)std::min(std::max(src.at<double>(i, j), 0.0), 255.0);
    return dst;
}

} // namespace utils
