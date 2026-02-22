# VideoEditor - Multi-Track Video Editing Application
Group Members: Shane McCue, Idiris Muumin, Manvel Iskandaryan, Noah Turnquist

A C++ video editing application with timeline-based composition, multiple tracks, filters, captions, and export capabilities built using modern design patterns.

## Table of Contents
- [Architecture Overview](#architecture-overview)
- [Assets System](#assets-system)
- [Timeline & Tracks](#timeline--tracks)
- [Graphics & Rendering](#graphics--rendering)
- [UI Components](#ui-components)
- [Export System](#export-system)
- [Filters](#filters)
- [Build Instructions](#build-instructions)

---

## Architecture Overview

```mermaid
graph TB
    App[Application] --> Timeline
    App --> AssetSys[Asset System]
    App --> UI[UI Layer]
    App --> Export[Export System]

    Timeline --> Tracks
    Tracks --> Entries[Timeline Entries]
    Entries --> AssetSys

    UI --> ExportMenu[Export Menu]
    UI --> TrackViz[Track Visualization]
    UI --> Panels[Button Panels]

    Export --> Facade[Export Facade]
    Facade --> FFmpeg[FFmpeg Video Writer]
    Facade --> STB[STB Image Writer]
```

---

## Assets System

The asset system provides a flexible framework for loading and adapting different media types into a common interface using the **Adapter** and **Abstract Factory** patterns.

```mermaid
classDiagram
    class IAsset {
        <<interface>>
        +getDuration() double
        +getFrame(time) Image
        +isVideo() bool
    }

    class IAssetFactory {
        <<interface>>
        +create(filename) IAsset*
    }

    class Image {
        +getWidth() int
        +getHeight() int
        +getData() uint8_t*
        +setPixel(x, y, color)
        +getPixel(x, y) Color
    }

    class Video {
        +getWidth() int
        +getHeight() int
        +getFrame(time) Image
        +getDuration() double
    }

    class ImageAsset {
        -Image image
    }

    class VideoAsset {
        -Video video
    }

    class TextAsset {
        -Text text
        -TextOverlay overlay
    }

    class Caption {
        -string text
        -double startTime
        -double endTime
    }

    class CompositeAssetFactory {
        -vector~IAssetFactory*~ factories
        +add(factory)
    }

    IAsset <|.. ImageAsset : implements
    IAsset <|.. VideoAsset : implements
    IAsset <|.. TextAsset : implements
    IAsset <|.. Caption : implements

    ImageAsset o-- Image : adapts
    VideoAsset o-- Video : adapts

    IAssetFactory <|.. ImageAssetFactory : implements
    IAssetFactory <|.. VideoAssetFactory : implements
    IAssetFactory <|.. TextAssetFactory : implements
    IAssetFactory <|.. CaptionAssetFactory : implements
    IAssetFactory <|.. CompositeAssetFactory : implements
    IAssetFactory <|.. DefaultAssetFactory : implements

    CompositeAssetFactory o-- IAssetFactory : contains
```

### Core Classes

#### `IAsset` (Interface)
**Pattern**: Target interface for Adapter pattern
**Functionality**: Defines common interface for all media types with `getDuration()` and `getFrame(time)` methods. Enables polymorphic handling of images, videos, text, and captions.

#### `Image` & `Video`
**Pattern**: Adaptee classes
**Functionality**: Core media classes wrapping STB image library and FFmpeg video decoder respectively. Provide raw pixel data access and manipulation.

#### `ImageAsset` & `VideoAsset`
**Pattern**: Adapter pattern
**Functionality**: Adapt `Image` and `Video` to `IAsset` interface. `ImageAsset` returns same frame for any time, `VideoAsset` seeks to requested timestamp.

#### `TextAsset` & `Caption`
**Pattern**: Adapter pattern
**Functionality**: Render text overlays and time-based captions. `TextAsset` generates dynamic text graphics, `Caption` displays text within specified time ranges.

#### `IAssetFactory` (Interface)
**Pattern**: Abstract Factory pattern
**Functionality**: Factory interface with `create(filename)` method. Returns `IAsset*` or `nullptr` if format unsupported. Enables extensible asset loading.

#### `CompositeAssetFactory`
**Pattern**: Composite pattern + Chain of Responsibility
**Functionality**: Contains multiple factories and tries each sequentially until asset is created. Enables adding new asset types without modifying existing code.

#### `DefaultAssetFactory`
**Pattern**: Factory pattern + Null Object pattern
**Functionality**: Fallback factory that always succeeds, creating colored rectangles for unrecognized files. Prevents null pointer exceptions.

---

## Timeline & Tracks

The timeline system enables multi-track video composition with frame-accurate control.

```mermaid
classDiagram
    class Timeline {
        -vector~Track*~ tracks
        -double currentTime
        +addTrack(name) size_t
        +getTrack(index) Track*
        +addEntryToTrack(index, entry) bool
        +renderFrameAt(time, w, h) Image*
        +getTotalDuration() double
        -compositeImages(bottom, top)
    }

    class Track {
        -string name
        -Color color
        -bool visible
        -vector~TimelineEntry~ entries
        +addEntry(entry) bool
        +removeEntry(index) bool
        +getEntryAt(time) TimelineEntry*
        +getTotalDuration() double
        -wouldOverlap(entry) bool
    }

    class TimelineEntry {
        -IAsset* asset
        -double startTime
        -double duration
        +getFrameAt(globalTime) Image
        +isActiveAt(time) bool
        +overlapsWith(other) bool
        +getEndTime() double
    }

    Timeline "1" *-- "many" Track
    Track "1" *-- "many" TimelineEntry
    TimelineEntry "many" o-- "1" IAsset : references
```

### Core Classes

#### `Timeline`
**Pattern**: Composite pattern + Strategy pattern
**Functionality**: Manages multiple tracks and composites them into final output. Renders frames by iterating tracks bottom-to-top with alpha blending.

#### `Track`
**Pattern**: Collection with validation
**Functionality**: Represents single layer containing non-overlapping timeline entries. Enforces temporal constraints preventing entry overlap on same track.

#### `TimelineEntry`
**Pattern**: Proxy pattern
**Functionality**: Binds asset to track with timing metadata (start, duration). Converts global timeline time to asset-local time for frame retrieval.

---

## Graphics & Rendering

OpenGL-based rendering system with shader support.

```mermaid
classDiagram
    class Window {
        -GLFWwindow* window
        -vector~IWindowObserver*~ observers
        +swapBuffers()
        +shouldClose() bool
        +addObserver(observer)
        -onMouseMove(x, y)
        -onMouseClick(btn, action)
    }

    class IWindowObserver {
        <<interface>>
        +onMouseMove(x, y)
        +onMouseClick(btn, action, mods)
    }

    class Glyph {
        -Texture texture
        -Quad quad
        +update(image)
        +draw()
    }

    class Texture {
        -GLuint textureID
        +loadFromImage(image)
        +bind()
    }

    class Text {
        -string content
        -Color color
        -int fontSize
        +renderToImage() Image*
    }

    class ShaderProgram {
        -GLuint programID
        +use()
        +setUniform(name, value)
    }

    Window "1" o-- "many" IWindowObserver : notifies
    Application ..|> IWindowObserver : implements
    Glyph *-- Texture
    Glyph *-- Quad
    Glyph -- ShaderProgram : uses
```

### Core Classes

#### `Window`
**Pattern**: Observer pattern (Subject)
**Functionality**: GLFW window wrapper that notifies observers of mouse events. Manages OpenGL context and event polling.

#### `Glyph`
**Pattern**: Facade pattern
**Functionality**: Simplifies rendering by combining texture and geometry. Provides high-level `update(image)` and `draw()` interface abstracting OpenGL calls.

#### `Texture`
**Pattern**: Wrapper pattern
**Functionality**: Manages OpenGL texture lifecycle. Uploads image data to GPU and provides binding interface for rendering.

#### `Text`
**Pattern**: Builder pattern
**Functionality**: Renders TrueType fonts to images using FreeType library. Supports color, size, and anti-aliasing customization.

#### `ShaderProgram`
**Pattern**: Wrapper pattern
**Functionality**: Compiles and links GLSL vertex/fragment shaders. Provides type-safe uniform variable setting for shader parameters.

---

## UI Components

Model-View-Controller architecture for user interface.

```mermaid
classDiagram
    class Button {
        <<abstract>>
        -float x, y, w, h
        -bool highlighted
        -bool clicked
        -bool selected
        -function~void~ onClick
        +contains(x, y) bool
        +setHighlighted(bool)
        +draw()*
    }

    class TextButton {
        -string label
        -Color color
        +draw()
    }

    class IconButton {
        -Texture iconTexture
        +draw()
    }

    class ButtonPanel {
        -Container background
        -vector~Button*~ buttons
        +addTextButton(label, callback)
        +addIconButton(icon, callback)
        +draw()
    }

    class ExportMenuModel {
        -IAsset* asset
        -Timeline* timeline
        -ExportSettings settings
        -string filename
        +setAsset(asset)
        +setTimeline(timeline)
        +canExport() bool
    }

    class ExportMenuView {
        -ExportMenuModel* model
        -ExportMenuController* controller
        -vector~Button*~ buttons
        +draw()
        +update()
        -createUI()
    }

    class ExportMenuController {
        -ExportMenuModel* model
        -ExportMenuView* view
        -ExportFacade* facade
        +onFormatSelected(format)
        +onExportClicked()
        +onExportVideoClicked()
    }

    class TrackVisualization {
        -Timeline* timeline
        -int selectedTrack
        -int selectedEntry
        +draw()
        +handleMouse(x, y)
        -renderTrackLanes()
        -renderEntries()
    }

    Button <|-- TextButton
    Button <|-- IconButton
    ButtonPanel o-- Button

    ExportMenuView -- ExportMenuModel : reads
    ExportMenuView -- ExportMenuController : delegates to
    ExportMenuController -- ExportMenuModel : updates
    ExportMenuController -- ExportFacade : uses

    TrackVisualization -- Timeline : visualizes
```

### Core Classes

#### `Button` (Abstract)
**Pattern**: Template Method pattern + Command pattern
**Functionality**: Base class defining button interface with state management (highlighted, clicked, selected). Stores callback function executed on click.

#### `ButtonPanel`
**Pattern**: Composite pattern
**Functionality**: Container managing multiple buttons with automatic layout. Provides factory methods for creating text and icon buttons.

#### Export Menu (MVC)
**Pattern**: Model-View-Controller
**Functionality**: Separates export UI into model (settings/state), view (rendering), and controller (event handling). Enables testing and modification without coupling.

#### `TrackVisualization`
**Pattern**: Observer pattern + Visualization
**Functionality**: Renders timeline tracks as interactive lanes. Supports drag-and-drop for reordering, resizing entries, and visual feedback for selections.

---

## Export System

Facade pattern for multi-format export with FFmpeg integration.

```mermaid
classDiagram
    class ExportFacade {
        -string lastError
        +exportImage(image, filename, settings) bool
        +exportAsset(asset, filename, settings) bool
        +exportVideo(frames, filename, settings) bool
        +exportTimeline(timeline, filename, settings, w, h) bool
        +getLastError() string
        -writeImageFile(image, filename, format)
    }

    class ExportSettings {
        +ExportFormat format
        +int quality
        +int width
        +int height
        +double frameRate
    }

    class ExportAssetCommand {
        -ExportFacade* facade
        -IAsset* asset
        -string filename
        -ExportSettings settings
        +execute()
        +canExecute() bool
    }

    class ICommand {
        <<interface>>
        +execute()
        +canExecute() bool
    }

    class VideoWriterState {
        +int width
        +int height
        +int fps
        -AVFormatContext* formatCtx
        -AVCodecContext* codecCtx
        -SwsContext* scalerCtx
    }

    ExportFacade -- ExportSettings : uses
    ExportFacade -- VideoWriterState : creates
    ICommand <|.. ExportAssetCommand : implements
    ExportAssetCommand -- ExportFacade : invokes
```

### Core Classes

#### `ExportFacade`
**Pattern**: Facade pattern
**Functionality**: Simplifies complex export operations behind unified interface. Supports PNG, JPEG, BMP (via STB) and MP4 (via FFmpeg) formats.

#### `ExportAssetCommand`
**Pattern**: Command pattern
**Functionality**: Encapsulates export operation as object. Enables undo/redo capability, queuing, and validation before execution.

#### `VideoWriterState`
**Pattern**: Wrapper pattern + State pattern
**Functionality**: Wraps FFmpeg H.264 encoder with simple C++ interface. Handles RGBA→YUV420P conversion, frame encoding, and MP4 container writing.

---

## Filters

(Not yet implemented - reserved for future enhancements like color correction, blur, transforms)

---

## Build Instructions

### Prerequisites
- CMake 3.3+
- C++11 compiler (g++, clang++)
- OpenGL 4.3+
- GLFW3
- FreeType2
- FFmpeg (libavcodec, libavformat, libswscale)

### Build Steps

#### 1. Configure with CMake
```bash
mkdir build
cmake -S . -B build
```

#### 2. Compile
```bash
cmake --build build
```
Or:
```bash
cd build
make -j4
```

#### 3. Run
```bash
./build/VideoEditor assets/images/img_small.jpeg assets/videos/test.mp4
```

### Testing
```bash
cd build
./VideoEditorTests
```

---

## Project Structure

```
├── include/           # Header files
│   ├── assets/       # Asset system interfaces and adapters
│   ├── export/       # Export facade and commands
│   ├── filters/      # Filter system (future)
│   ├── graphics/     # OpenGL rendering primitives
│   ├── timeline/     # Timeline, Track, TimelineEntry
│   ├── ui/           # UI components and MVC
│   └── commands/     # Command pattern implementations
├── src/              # Implementation files (mirrors include/)
├── lib/              # Third-party libraries
│   ├── ffmpeg/       # FFmpeg CMake configuration
│   ├── video_reader  # FFmpeg video decoder
│   └── video_writer  # FFmpeg video encoder
├── tests/            # GTest unit tests
└── assets/           # Sample media files
    ├── images/
    ├── videos/
    └── icons/
```

---

## Design Patterns Summary

| Pattern | Usage | Location |
|---------|-------|----------|
| **Adapter** | Image/Video → IAsset | `assets/ImageAsset`, `assets/VideoAsset` |
| **Abstract Factory** | Asset creation from files | `assets/*Factory` |
| **Composite** | Multiple factories | `assets/CompositeAssetFactory` |
| **Facade** | Export simplification | `export/ExportFacade` |
| **Command** | Export operations | `commands/ExportAssetCommand` |
| **MVC** | Export UI separation | `ui/export/ExportMenu*` |
| **Observer** | Window event notifications | `graphics/Window`, `Application` |
| **Template Method** | Button base class | `ui/Button` |
| **Strategy** | Timeline compositing | `timeline/Timeline` |

---

# team-41-hw3
# ISKAN016
