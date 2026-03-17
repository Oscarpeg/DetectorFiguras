# Detector de Figuras Geométricas

**Universidad Sergio Arboleda**  
**Materia:** Procesamiento Digital de Imágenes  
**Profesor:** Hernan Cruz Bueno  
**Estudiante:** Oscar Mauricio Peñuela  

---

## Descripción

Detección y clasificación de figuras geométricas en tiempo real.  
Todo implementado manualmente desde cero (sin funciones automáticas de OpenCV).

### Pipeline

```
Cámara → Grises → Canny Manual → Cierre Morfológico → Dilatación
    → Contornos (Moore Tracing) → Graham Scan → Douglas-Peucker → Clasificar
```

### Algoritmos implementados a mano

| Algoritmo | Descripción |
|---|---|
| Convolución 2D | 4 for anidados, zero-padding |
| Canny | 5 etapas: Gauss, Sobel, NMS, Double Threshold, Hysteresis |
| Dilatación / Erosión | Morfología manual pixel por pixel |
| Cierre Morfológico | Dilatación + Erosión (cierra huecos) |
| Moore Boundary Tracing | Seguimiento de borde con 8-vecinos |
| Graham Scan | Convex Hull: pivote, ángulo polar, producto cruz |
| Douglas-Peucker | Simplificación de polígonos (detecta esquinas reales) |

### Figuras detectadas

| Vértices | Figura | Color |
|---|---|---|
| 3 | Triángulo | Verde |
| 4 (ratio~1) | Cuadrado | Azul |
| 4 (ratio≠1) | Rectángulo | Azul claro |
| 5 | Pentágono | Amarillo |
| 6 | Hexágono | Magenta |
| 6+ y circularidad alta | Círculo | Naranja |

---

## Compilación

```bash
cd detector_figuras
mkdir build && cd build
cmake ..
make -j4
```

## Ejecución

```bash
./detector            # Con cámara
./detector video.mp4  # Con video
```

### Controles en tiempo real (Trackbars)

- **Canny Bajo**: umbral inferior de Canny
- **Canny Alto**: umbral superior de Canny
- **Area Min x100**: área mínima para considerar un contorno
- **Dilatar**: cantidad de veces que se dilata

**ESC** para salir.

---

## Estructura

```
detector_figuras/
├── CMakeLists.txt
├── README.md
├── include/
│   ├── convolucion.h
│   ├── canny_manual.h
│   ├── morfologia.h
│   ├── contornos.h
│   ├── graham_scan.h
│   ├── douglas_peucker.h
│   └── clasificador.h
└── src/
    ├── main.cpp
    ├── convolucion.cpp
    ├── canny_manual.cpp
    ├── morfologia.cpp
    ├── contornos.cpp
    ├── graham_scan.cpp
    ├── douglas_peucker.cpp
    └── clasificador.cpp
```
