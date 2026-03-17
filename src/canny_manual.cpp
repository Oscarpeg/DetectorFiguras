#include "canny_manual.h"
#include "convolucion.h"
#include <cmath>

namespace filtros {

static cv::Mat suavizar(const cv::Mat& gris, double sigma) {
    int ksize = 5;
    cv::Mat kernel(ksize, ksize, CV_64F);
    int c = ksize / 2;
    double suma = 0.0;
    for (int i = 0; i < ksize; i++)
        for (int j = 0; j < ksize; j++) {
            double x = i - c, y = j - c;
            double val = exp(-(x*x + y*y) / (2.0 * sigma * sigma));
            kernel.at<double>(i, j) = val;
            suma += val;
        }
    for (int i = 0; i < ksize; i++)
        for (int j = 0; j < ksize; j++)
            kernel.at<double>(i, j) /= suma;
    return utils::recortar(utils::convolucion(gris, kernel));
}

static void gradientes(const cv::Mat& suav, cv::Mat& mag, cv::Mat& dir) {
    cv::Mat kx = (cv::Mat_<double>(3,3) << -1,0,1, -2,0,2, -1,0,1);
    cv::Mat ky = (cv::Mat_<double>(3,3) << -1,-2,-1, 0,0,0, 1,2,1);
    cv::Mat gx = utils::convolucion(suav, kx);
    cv::Mat gy = utils::convolucion(suav, ky);
    int rows = suav.rows, cols = suav.cols;
    mag = cv::Mat(rows, cols, CV_64F);
    dir = cv::Mat(rows, cols, CV_64F);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
            double vx = gx.at<double>(i, j), vy = gy.at<double>(i, j);
            mag.at<double>(i, j) = std::sqrt(vx*vx + vy*vy);
            dir.at<double>(i, j) = std::atan2(vy, vx);
        }
}

static cv::Mat nms(const cv::Mat& mag, const cv::Mat& dir) {
    int rows = mag.rows, cols = mag.cols;
    cv::Mat out = cv::Mat::zeros(rows, cols, CV_64F);
    for (int i = 1; i < rows - 1; i++)
        for (int j = 1; j < cols - 1; j++) {
            double m = mag.at<double>(i, j);
            double ang = dir.at<double>(i, j) * 180.0 / CV_PI;
            if (ang < 0) ang += 180.0;
            double v1 = 0, v2 = 0;
            if ((ang < 22.5) || (ang >= 157.5))
                { v1 = mag.at<double>(i, j-1); v2 = mag.at<double>(i, j+1); }
            else if (ang < 67.5)
                { v1 = mag.at<double>(i-1, j+1); v2 = mag.at<double>(i+1, j-1); }
            else if (ang < 112.5)
                { v1 = mag.at<double>(i-1, j); v2 = mag.at<double>(i+1, j); }
            else
                { v1 = mag.at<double>(i-1, j-1); v2 = mag.at<double>(i+1, j+1); }
            if (m >= v1 && m >= v2) out.at<double>(i, j) = m;
        }
    return out;
}

static cv::Mat dobleUmbral(const cv::Mat& nmsImg, double bajo, double alto) {
    int rows = nmsImg.rows, cols = nmsImg.cols;
    cv::Mat out = cv::Mat::zeros(rows, cols, CV_8U);
    double maxV = 0;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            if (nmsImg.at<double>(i, j) > maxV) maxV = nmsImg.at<double>(i, j);
    if (maxV < 1e-10) return out;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
            double val = (nmsImg.at<double>(i, j) / maxV) * 255.0;
            if (val >= alto) out.at<uchar>(i, j) = 255;
            else if (val >= bajo) out.at<uchar>(i, j) = 128;
        }
    return out;
}

static cv::Mat hysteresis(const cv::Mat& dt) {
    int rows = dt.rows, cols = dt.cols;
    cv::Mat res = dt.clone();
    int dx[8] = {-1,-1,-1,0,0,1,1,1};
    int dy[8] = {-1,0,1,-1,1,-1,0,1};
    bool cambio = true;
    while (cambio) {
        cambio = false;
        for (int i = 1; i < rows - 1; i++)
            for (int j = 1; j < cols - 1; j++)
                if (res.at<uchar>(i, j) == 128)
                    for (int k = 0; k < 8; k++)
                        if (res.at<uchar>(i+dx[k], j+dy[k]) == 255) {
                            res.at<uchar>(i, j) = 255;
                            cambio = true;
                            break;
                        }
    }
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            if (res.at<uchar>(i, j) == 128) res.at<uchar>(i, j) = 0;
    return res;
}

cv::Mat cannyManual(const cv::Mat& gris, double umbral_bajo, double umbral_alto, double sigma) {
    cv::Mat suav = suavizar(gris, sigma);
    cv::Mat mag, dir;
    gradientes(suav, mag, dir);
    cv::Mat nmsImg = nms(mag, dir);
    cv::Mat dt = dobleUmbral(nmsImg, umbral_bajo, umbral_alto);
    return hysteresis(dt);
}

} // namespace filtros
