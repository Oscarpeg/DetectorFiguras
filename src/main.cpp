#include "convolucion.h"
#include "canny_manual.h"
#include "morfologia.h"
#include "contornos.h"
#include "graham_scan.h"
#include "douglas_peucker.h"
#include "clasificador.h"
#include <iostream>

cv::Scalar colorFigura(const std::string& nombre) {
    if (nombre == "Triangulo")   return cv::Scalar(0, 255, 0);     // Verde
    if (nombre == "Cuadrado")    return cv::Scalar(255, 0, 0);     // Azul
    if (nombre == "Rectangulo")  return cv::Scalar(255, 150, 0);   // Azul claro
    if (nombre == "Pentagono")   return cv::Scalar(0, 255, 255);   // Amarillo
    if (nombre == "Hexagono")    return cv::Scalar(255, 0, 255);   // Magenta
    if (nombre == "Circulo")     return cv::Scalar(0, 165, 255);   // Naranja
    return cv::Scalar(255, 255, 255);
}

// Variables globales para los trackbars
int tb_canny_bajo = 50;
int tb_canny_alto = 150;
int tb_area_min   = 10;    // x100 -> 1000
int tb_dilatar    = 2;

int main(int argc, char** argv) {
    cv::VideoCapture cap;
    if (argc > 1) {
        cap.open(argv[1]);
        std::cout << "Abriendo video: " << argv[1] << std::endl;
    } else {
        cap.open(0);
        std::cout << "Abriendo camara..." << std::endl;
    }
    if (!cap.isOpened()) {
        std::cerr << "Error: no se pudo abrir fuente de video" << std::endl;
        return -1;
    }

    std::cout << "=== Detector de Figuras Geometricas ===" << std::endl;
    std::cout << "Pipeline: Gris -> Canny -> Cierre -> Dilatacion -> Contornos -> GrahamScan -> Douglas-Peucker" << std::endl;
    std::cout << "Ajusta los trackbars para calibrar!" << std::endl;
    std::cout << "ESC para salir" << std::endl;

    // Crear ventana con trackbars
    cv::namedWindow("Controles", cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("Canny Bajo",  "Controles", &tb_canny_bajo, 200);
    cv::createTrackbar("Canny Alto",  "Controles", &tb_canny_alto, 300);
    cv::createTrackbar("Area Min x100", "Controles", &tb_area_min, 100);
    cv::createTrackbar("Dilatar",     "Controles", &tb_dilatar, 5);

    // Imagen negra para la ventana de controles
    cv::Mat control_img = cv::Mat::zeros(1, 400, CV_8UC3);
    cv::imshow("Controles", control_img);

    int frame_count = 0;
    cv::Mat frame, small, gris;

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        cv::resize(frame, small, cv::Size(320, 240));
        frame_count++;

        // Leer parámetros de los trackbars
        double canny_bajo = std::max(tb_canny_bajo, 1);
        double canny_alto = std::max(tb_canny_alto, (int)canny_bajo + 10);
        double area_minima = std::max(tb_area_min, 1) * 100.0;
        double area_maxima = 60000.0;
        int dilatar_veces = tb_dilatar;

        // ========================================
        // PRIMERA PARTE: Obtener contornos
        // ========================================

        // 1. Grises (manual)
        gris = utils::convertirGris(small);

        // 2. Canny manual (5 etapas)
        cv::Mat bordes = filtros::cannyManual(gris, canny_bajo, canny_alto, 1.4);

        // 3. Cierre morfológico (cierra huecos en los bordes)
        cv::Mat cerrada = filtros::cierreManual(bordes, 5);

        // 4. Dilatación (engrosa bordes para conectar)
        cv::Mat dilatada = cerrada;
        for (int d = 0; d < dilatar_veces; d++)
            dilatada = filtros::dilatarManual(dilatada, 3);

        // 5. Encontrar contornos (Moore boundary tracing)
        auto contornos = filtros::encontrarContornos(dilatada);

        // ========================================
        // SEGUNDA PARTE: Clasificar figuras
        // ========================================
        cv::Mat display = small.clone();
        int figuras_encontradas = 0;

        // Debug cada 30 frames
        bool debug = (frame_count % 30 == 1);
        if (debug) {
            std::cout << "\n--- Frame " << frame_count
                      << " | Canny [" << (int)canny_bajo << "," << (int)canny_alto
                      << "] | AreaMin=" << (int)area_minima
                      << " | Contornos: " << contornos.size() << " ---" << std::endl;
        }

        for (int idx = 0; idx < (int)contornos.size(); idx++) {
            auto& contorno = contornos[idx];
            double area = filtros::areaContorno(contorno);

            // Filtro por área
            if (area < area_minima || area > area_maxima) {
                if (debug && area > 200)
                    std::cout << "  [" << idx << "] DESCARTADO area=" << (int)area << std::endl;
                continue;
            }

            // Clasificar: Graham Scan + Douglas-Peucker + circularidad
            filtros::FiguraDetectada fig = filtros::clasificarFigura(contorno);

            if (debug) {
                std::cout << "  [" << idx << "] area=" << (int)area
                          << " circ=" << fig.circularidad
                          << " vert=" << fig.vertices
                          << " -> " << fig.nombre << std::endl;
            }

            // Dibujar
            cv::Scalar color = colorFigura(fig.nombre);

            // Dibujar polígono aproximado
            int nv = fig.aproximado.size();
            for (int i = 0; i < nv; i++) {
                int j = (i + 1) % nv;
                cv::line(display, fig.aproximado[i], fig.aproximado[j], color, 2);
            }

            // Dibujar vértices
            for (auto& pt : fig.aproximado)
                cv::circle(display, pt, 4, cv::Scalar(0, 0, 255), -1);

            // Texto
            std::string texto = fig.nombre + " (" + std::to_string(fig.vertices) + "v)";
            cv::putText(display, texto, cv::Point(fig.centro.x - 35, fig.centro.y - 10),
                        cv::FONT_HERSHEY_SIMPLEX, 0.45, color, 2);

            figuras_encontradas++;
        }

        // Info
        std::string info = "Figuras: " + std::to_string(figuras_encontradas);
        cv::putText(display, info, cv::Point(5, 15),
                    cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 255, 255), 1);

        cv::imshow("Original", small);
        cv::imshow("Canny", bordes);
        cv::imshow("Procesada", dilatada);
        cv::imshow("Figuras Detectadas", display);

        if (cv::waitKey(30) == 27) break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
