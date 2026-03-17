cmake_minimum_required(VERSION 3.15)
project(DetectorFiguras CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} include)

add_executable(detector
    src/main.cpp
    src/convolucion.cpp
    src/canny_manual.cpp
    src/morfologia.cpp
    src/contornos.cpp
    src/graham_scan.cpp
    src/douglas_peucker.cpp
    src/clasificador.cpp
)
target_link_libraries(detector ${OpenCV_LIBS})
