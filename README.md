

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
