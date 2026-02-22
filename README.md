# C++ Multi-Track Video Editor

A modular, timeline-based video editing engine built in modern C++ with OpenGL rendering and FFmpeg export.

This project demonstrates systems design, real-time rendering, media processing, and clean object-oriented architecture using industry-relevant design patterns.

---

## 🚀 Features

- Multi-track timeline composition  
- Frame-accurate video and image rendering  
- Text overlays and timed captions  
- Alpha compositing across layered tracks  
- Real-time OpenGL rendering  
- MP4 export (H.264 via FFmpeg)  
- Image export (PNG / JPEG via STB)  
- Unit testing with GoogleTest  

---

## 🧠 Architecture Overview

The system is divided into clear layers to maintain separation of concerns:

| Layer | Responsibility |
|-------|---------------|
| Asset System | Media abstraction & loading |
| Timeline Engine | Multi-track composition logic |
| Rendering Layer | GPU rendering via OpenGL |
| UI Layer | MVC-based interaction |
| Export System | Media encoding & file output |

### Key Design Decisions

- **Unified Media Interface (`IAsset`)**  
  Standardizes frame access across images, videos, text, and captions.

- **Non-overlapping Track Model**  
  Each track enforces time constraints to prevent overlapping entries.

- **Bottom-to-Top Alpha Compositing**  
  Tracks are rendered in order and blended per frame.

- **Export Facade**  
  Simplifies complex FFmpeg encoding behind a clean interface.

---

## 🧩 Design Patterns Used

- Adapter  
- Abstract Factory  
- Composite  
- Facade  
- Command  
- MVC  
- Observer  
- Template Method  

Patterns were used intentionally to improve modularity, extensibility, and maintainability.

---

## 🛠 Tech Stack

- C++11  
- OpenGL 4.3+  
- GLFW  
- FreeType  
- FFmpeg (libavcodec, libavformat, libswscale)  
- STB Image Library  
- CMake  
- GoogleTest  

---

## 📂 Project Structure

```
include/
    assets/
    timeline/
    graphics/
    ui/
    export/
src/
lib/
tests/
assets/
```

---

## 🏗 Build Instructions

### Requirements

- CMake 3.3+  
- C++11 compatible compiler  
- OpenGL 4.3+  
- GLFW3  
- FreeType2  
- FFmpeg  

### Build

```bash
mkdir build
cmake -S . -B build
cmake --build build
```

### Run

```bash
./build/VideoEditor
```

### Run Tests

```bash
cd build
./VideoEditorTests
```

---

## 🔬 Engineering Focus

This project demonstrates:

- Systems-level C++ programming  
- Memory management across C and C++ boundaries (FFmpeg integration)  
- Real-time rendering pipelines  
- Media encoding and decoding workflows  
- Clean architectural separation  
- Scalable, extensible software design  

---

## 📈 Future Improvements

- Audio track support  
- GPU shader-based video effects  
- Undo / Redo system  
- Plugin-based filter architecture  
- Performance optimization for large timelines  