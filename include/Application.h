#ifndef APPLICATION_H
#define APPLICATION_H

#include "assets/IAssetFactory.h"
#include "export/ExportFacade.h"
#include "graphics/Window.h"
#include "timeline/Timeline.h"
#include "ui/Button.h"
#include "ui/ButtonPanel.h"
#include "ui/TrackVisualization.h"
#include "ui/export/ExportMenuController.h"
#include "ui/export/ExportMenuModel.h"
#include "ui/export/ExportMenuView.h"
#include "video_reader.hpp"
#include <chrono>
#include <vector>

namespace csci3081 {

class Application : public IWindowObserver {
public:
  Application();
  ~Application();

  int run(int argc, char *argv[]);
  void onResize(float width, float height) {}
  void onMouseMove(float x, float y);
  void onMouseClick(int button, int action, int mods);
  void onKeyPress(int key);

  void addFilters();

private:
  Window *window;
  std::vector<Button *> buttons;
  std::vector<Glyph *> labels;
  IAssetFactory *assetFactory;
  std::vector<Texture *> trackTextures;
  std::vector<std::string> trackFilters;
  Glyph *video;
  Image blank;
  TrackShader *trackShader;

  // Assets loaded from command line
  std::vector<IAsset *> assets;

  // Selection state
  int assetSelected = -1; // Index of selected asset (-1 = none)
  int trackSelected = 0;  // Index of selected track
  int entrySelected = -1; // Index of selected entry in track (-1 = none)

  // Drag state for moving timeline entries
  bool isDraggingEntry = false;
  double dragStartTime = 0.0; // Original start time of entry being dragged
  double dragStartX = 0.0;    // Mouse X position when drag started

  // Resize state for adjusting entry duration
  bool isResizingEntry = false;
  double resizeStartDuration = 0.0; // Original duration of entry being resized

  // Timeline system
  Timeline *timeline;

  // New UI components
  ButtonPanel *assetActionsPanel;
  ButtonPanel *trackActionsPanel;
  ButtonPanel *filterPanel;
  TrackVisualization *trackVisualization;

  // Export menu (MVC pattern)
  ExportFacade *exportFacade;
  ExportMenuModel *exportMenuModel;
  ExportMenuView *exportMenuView;
  ExportMenuController *exportMenuController;

  // Video playback members
  VideoReaderState video_state;
  Image *video_frame;
  bool video_loaded = false;
  bool is_playing = false;
  std::chrono::steady_clock::time_point startPlayTime;
  int64_t current_pts = 0;
  double frame_rate = 30.0; // Default frame rate
};

} // namespace csci3081

#endif