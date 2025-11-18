#include "Application.h"
#include "Image.h"
#include "Video.h"
#include "assets/CompositeAssetFactory.h"
#include "assets/DefaultAssetFactory.h"
#include "assets/ImageAssetFactory.h"
#include "assets/TextAssetFactory.h"
#include "assets/VideoAssetFactory.h"
#include "graphics/Glyph.h"
#include "graphics/ShaderProgram.h"
#include "graphics/Text.h"
#include "ui/Button.h"
#include "ui/IconButton.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

// Include glad and glfw for graphics and windowing
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace csci3081 {

Application::Application() : blank(10, 10) {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  window = new Window();
  window->addObserver(*this);

  // Initialize new components
  timeline = nullptr;
  assetActionsPanel = nullptr;
  trackActionsPanel = nullptr;
  filterPanel = nullptr;
  trackVisualization = nullptr;
  exportFacade = nullptr;
  exportMenuModel = nullptr;
  exportMenuView = nullptr;
  exportMenuController = nullptr;

  for (int y = 0; y < blank.getHeight(); y++) {
    for (int x = 0; x < blank.getWidth(); x++) {
      // blank.setPixel(x, y, Color(32, 32, 32, 255));
      blank.setPixel(x, y, Color(255, 255, 255, 0));
    }
  }
}

Application::~Application() {
  // Delete assets
  for (IAsset *asset : assets) {
    delete asset;
  }

  delete timeline;
  delete assetFactory;
  delete exportMenuController;
  delete exportMenuView;
  delete exportMenuModel;
  delete exportFacade;
  delete trackVisualization;
  delete assetActionsPanel;
  delete trackActionsPanel;
  delete window;
  glfwTerminate();
}

void Application::addFilters() {

  // The following filters are run on the GPU to make them lightning fast.
  // Remember that on the GPU calculations are done on a per pixel basis.
  // Here are some variables that are available for you to use:
  //
  // * time - a floating point value between 0 and 1 that represents the current
  // time on a track. (See gradient filter)
  // * pos - the x,y position on the screen (See gradient filter)
  // * aggregateColor - the color (RGBA) that has been aggregated from all
  // previous tracks (See Bright filter)
  // * trackColor -  the color (RGBA) of the current track texture (See Low
  // threshold filter)

  filterPanel->addTextButton("None", [this]() {
    // Reset the filter
    std::string code = "";
    this->trackFilters[this->trackSelected] = code;
    this->trackShader->update(this->trackFilters);
  });

  filterPanel->addTextButton("Bright", [this]() {
    // Calculate brighten the entire image
    std::string code = "aggregateColor *= vec4(2.0, 2.0, 2.0, 1);\n"
                       "trackColor *= vec4(2.0, 2.0, 2.0, 1);\n";
    this->trackFilters[this->trackSelected] = code;
    this->trackShader->update(this->trackFilters);
  });

  filterPanel->addTextButton("Gradient", [this]() {
    // Draw a color gradient
    std::string code = "trackColor *= vec4(pos.x , pos.y, time, 1.0);\n";
    this->trackFilters[this->trackSelected] = code;
    this->trackShader->update(this->trackFilters);
  });

  filterPanel->addTextButton("Low", [this]() {
    // Calculate a low threshold
    std::string code = "if (trackColor.r < 0.25) {\n"
                       "   trackColor = vec4(0, 0, 0, 0);\n"
                       "}\n";
    this->trackFilters[this->trackSelected] = code;
    this->trackShader->update(this->trackFilters);
  });

  filterPanel->addTextButton("High", [this]() {
    // Calculate a high threshold
    std::string code = "if (trackColor.r < 0.75) {\n"
                       "   trackColor = vec4(0, 0, 0, 0);\n"
                       "}\n";
    this->trackFilters[this->trackSelected] = code;
    this->trackShader->update(this->trackFilters);
  });

  filterPanel->addTextButton("Greyscale", [this]() {
    // Greyscale the image
    std::string code =
        "float L = (trackColor.r + trackColor.g + trackColor.b) / 3.0;\n"
        "trackColor = vec4(L, L, L, trackColor.a);\n";
    this->trackFilters[this->trackSelected] = code;
    this->trackShader->update(this->trackFilters);
  });

  filterPanel->addTextButton("Red", [this]() {
    // Only show the red channel
    std::string code = "trackColor = vec4(trackColor.r, 0.0, 0.0, 1.0);\n";
    this->trackFilters[this->trackSelected] = code;
    this->trackShader->update(this->trackFilters);
  });

  filterPanel->addTextButton("Chroma", [this]() {
    // Green screen effect
    std::string code =
        "float supressionCoefficient = 30.0;"
        "float greenStrength = trackColor.g - (trackColor.r + trackColor.b) / "
        "2;"
        "if (greenStrength > 0) {"
        "  trackColor.r -= greenStrength * supressionCoefficient;"
        "  trackColor.g -= greenStrength * supressionCoefficient;"
        "  trackColor.b -= greenStrength * supressionCoefficient;"
        "  trackColor.a -= greenStrength * supressionCoefficient;"
        "}";
    this->trackFilters[this->trackSelected] = code;
    this->trackShader->update(this->trackFilters);
  });

  filterPanel->addTextButton("Circle", [this]() {
    // Set the transparency the the trackColor to 0 if it is outside of the
    // circle
  });

  filterPanel->addTextButton("Disolve", [this]() {
    // Set the transparency of the trackColor to disolve over time
  });

  filterPanel->addTextButton("Special", [this]() {
    // Come up with your own filter.
  });
}

int Application::run(int argc, char *argv[]) {

  // --------------------------------------------------------------------
  // Setup composite factory with all asset types
  // --------------------------------------------------------------------

  // Create composite factory and add all factories
  CompositeAssetFactory *compositeFactory = new CompositeAssetFactory();
  compositeFactory->add(new TextAssetFactory());
  compositeFactory->add(new ImageAssetFactory());
  compositeFactory->add(new VideoAssetFactory());
  compositeFactory->add(new DefaultAssetFactory());
  assetFactory = compositeFactory;

  // Loops through command line arguments and create assets using factory
  for (int i = 1; i < argc; i++) {
    assets.push_back(assetFactory->create(argv[i]));
  }

  if (assets.size() == 0) {
    assets.push_back(assetFactory->create("default"));
  }

  // --------------------------------------------------------------------
  // Setup Timeline with initial track
  // --------------------------------------------------------------------
  timeline = new Timeline();
  trackSelected = timeline->addTrack("Video Layer 1");
  this->trackTextures.push_back(new Texture(blank));
  this->trackFilters.push_back("");

  std::cout << "Timeline created with " << timeline->getTrackCount()
            << " track(s)" << std::endl;

  // --------------------------------------------------------------------
  // Initialize selection state
  // --------------------------------------------------------------------
  assetSelected = (assets.size() > 0) ? 0 : -1;
  entrySelected = -1;

  // -------------------------------------
  // Create User Interface - NEW LAYOUT
  // -------------------------------------

  // Layout constants for new design
  const float TITLE_HEIGHT = 0.035f;         // Title space at top
  const float ASSET_AREA_WIDTH = 0.12f;      // Left side for assets (35%)
  const float ASSET_THUMBNAIL_HEIGHT = 0.5f; // Height for thumbnails
  const float ASSET_ACTIONS_HEIGHT = 0.06f;  // Height for asset action buttons
  const float VIEWPORT_X = 0.35f;            // Viewport starts at 35%
  const float VIEWPORT_WIDTH = 0.57;         // Viewport width (50%)
  const float VIEWPORT_HEIGHT = 0.57;        // Viewport height (50%)
  const float EXPORT_MENU_WIDTH = 0.08f;     // Right side export menu (15%)
  const float EXPORT_MENU_X = 0.92f;         // Export menu X position
  const float TRACK_ACTIONS_WIDTH = 0.08f;   // Left fuschia section (10%)
  const float TRACKS_Y = 0.62f;              // Tracks start at 62%
  const float TRACKS_HEIGHT = 0.38f;         // Tracks section height

  // Create UI labels at top of each section
  Text assetsLabel("Assets", Color(255, 255, 255, 255), 40);
  Image *assetsImage = assetsLabel.renderToImage();
  labels.push_back(new Glyph(0.02, 0.0, 0.09, TITLE_HEIGHT, *assetsImage));
  delete assetsImage;

  Text viewportLabel("Viewport", Color(255, 255, 255, 255), 40);
  Image *viewportImage = viewportLabel.renderToImage();
  labels.push_back(
      new Glyph(VIEWPORT_X + 0.02, 0.0, 0.1, TITLE_HEIGHT, *viewportImage));
  delete viewportImage;

  Text exportLabel("Export", Color(255, 255, 255, 255), 40);
  Image *exportImage = exportLabel.renderToImage();
  labels.push_back(
      new Glyph(EXPORT_MENU_X - 0.01, 0.0, 0.09, TITLE_HEIGHT, *exportImage));
  delete exportImage;

  Text tracksLabel("Tracks", Color(255, 255, 255, 255), 40);
  Image *tracksImage = tracksLabel.renderToImage();
  labels.push_back(
      new Glyph(TRACK_ACTIONS_WIDTH + 0.02, TRACKS_Y, 0.1, 0.05, *tracksImage));
  delete tracksImage;

  // Create thumbnail buttons in ASSET AREA (blue section)
  for (int i = 0; i < assets.size(); i++) {
    float buttonX = 0.02 + 0.11 * (i % 3);                // 3 columns
    float buttonY = TITLE_HEIGHT + 0.01 + 0.11 * (i / 3); // Multiple rows

    // Determine badge icon based on asset type
    std::string badgeIconPath;
    switch (assets[i]->getAssetType()) {
    case AssetType::VIDEO:
      badgeIconPath = "assets/icons/movie.png";
      break;
    case AssetType::IMAGE:
      badgeIconPath = "assets/icons/image.png";
      break;
    case AssetType::TEXT:
      badgeIconPath = "assets/icons/text_fields_alt.png";
      break;
    default:
      badgeIconPath = ""; // No badge for default assets
      break;
    }

    // Create IconButton with badge
    IconButton *button = new IconButton(
        buttonX, buttonY, 0.1, 0.1, assets[i]->getThumbnail(),
        [this, i]() {
          this->assetSelected = i;
          this->is_playing = false;
          std::cout << "Selected asset " << i << std::endl;
        },
        badgeIconPath, 0.025f);

    buttons.push_back(button);
  }

  // Create ASSET ACTIONS panel (yellow section) - now HORIZONTAL for
  // side-by-side buttons
  assetActionsPanel = new ButtonPanel(
      0.0f, TITLE_HEIGHT + ASSET_THUMBNAIL_HEIGHT, ASSET_AREA_WIDTH,
      ASSET_ACTIONS_HEIGHT, Color(80, 80, 80, 255), // Dark gray background
      ButtonPanel::LayoutDirection::HORIZONTAL);

  // Calculate explicit button width for 2 buttons side-by-side
  // Width = (total_width - 2*padding - spacing) / 2
  float padding = 0.01f;
  float spacing = 0.005f;
  float buttonWidth = (ASSET_AREA_WIDTH - 2 * padding - spacing) / 2.0f;

  assetActionsPanel->setButtonSize(buttonWidth, ASSET_ACTIONS_HEIGHT * 0.7);
  assetActionsPanel->setButtonSpacing(spacing);

  // Add action buttons for assets - with color-coded text
  assetActionsPanel->addTextButton("+ Add", [this]() {
    std::cout << "Add to Track clicked" << std::endl;

    if (assetSelected < 0 || assetSelected >= assets.size()) {
      std::cerr << "No asset selected" << std::endl;
      return;
    }

    if (trackSelected < 0 || trackSelected >= timeline->getTrackCount()) {
      std::cerr << "No valid track selected" << std::endl;
      return;
    }

    // Get selected asset
    IAsset *asset = assets[assetSelected];

    // Get the selected track to calculate start time
    Track *track = timeline->getTrack(trackSelected);
    double startTime = track->getTotalDuration(); // Add at end of this track

    // Get duration from asset
    double duration = asset->getDuration();

    // Create timeline entry
    TimelineEntry entry(asset, startTime, duration);

    // Add to selected track
    bool success = timeline->addEntryToTrack(trackSelected, entry);

    if (success) {
      std::cout << "Added asset " << assetSelected << " to track "
                << trackSelected << " at time " << startTime
                << "s (duration: " << duration << "s)" << std::endl;
    } else {
      std::cerr << "Failed to add asset to track (may overlap)" << std::endl;
    }
  });

  assetActionsPanel->addTextButton("- Remove", [this]() {
    std::cout << "Remove from Track clicked" << std::endl;

    if (entrySelected < 0) {
      std::cerr << "No timeline entry selected" << std::endl;
      return;
    }

    if (trackSelected < 0 || trackSelected >= timeline->getTrackCount()) {
      std::cerr << "No valid track selected" << std::endl;
      return;
    }

    Track *track = timeline->getTrack(trackSelected);
    if (entrySelected >= track->getEntryCount()) {
      std::cerr << "Invalid entry index" << std::endl;
      return;
    }

    bool success = track->removeEntry(entrySelected);
    if (success) {
      std::cout << "Removed entry " << entrySelected << " from track "
                << trackSelected << std::endl;
      entrySelected = -1; // Clear selection
    } else {
      std::cerr << "Failed to remove entry" << std::endl;
    }
  });

  filterPanel = new ButtonPanel(VIEWPORT_X, TITLE_HEIGHT + VIEWPORT_HEIGHT,
                                VIEWPORT_WIDTH, ASSET_ACTIONS_HEIGHT,
                                Color(80, 80, 80, 255), // Dark gray background
                                ButtonPanel::LayoutDirection::HORIZONTAL);

  filterPanel->setButtonSize(buttonWidth / 1.2, ASSET_ACTIONS_HEIGHT * 0.7);
  filterPanel->setButtonSpacing(spacing);

  addFilters();

  // Create play button (centered below viewport)
  Button *play =
      new Button(VIEWPORT_X + VIEWPORT_WIDTH / 2 - 0.025f, // Centered
                 TITLE_HEIGHT + VIEWPORT_HEIGHT - 0.12f,   // Bottom of viewport
                 0.05, 0.1, Image("assets/images/play.png"), [this]() {
                   this->is_playing = !this->is_playing;
                   if (this->is_playing) {
                     startPlayTime = std::chrono::steady_clock::now();
                   }
                 });
  play->setBorder(false);

  buttons.push_back(play);

  // Create TRACK ACTIONS panel (fuschia section)
  trackActionsPanel = new ButtonPanel(
      0.0f,
      TRACKS_Y + 0.05f, // Leave room for Tracks label
      TRACK_ACTIONS_WIDTH, TRACKS_HEIGHT - 0.05f, Color(80, 80, 80, 255),
      // Color(100, 0, 100, 255),  // Darker magenta background
      ButtonPanel::LayoutDirection::VERTICAL);

  // Make buttons have spacing and reasonable size
  trackActionsPanel->setButtonSpacing(0.01f);
  trackActionsPanel->setButtonSize(-1, 0.04f);

  // Add action buttons for tracks with symbols
  trackActionsPanel->addTextButton("+ New Track", [this]() {
    std::cout << "New Track clicked" << std::endl;

    // Create a new track
    size_t trackIndex = timeline->addTrack();

    // Switch to the new track
    trackSelected = trackIndex;
    entrySelected = -1; // Clear entry selection

    std::cout << "Created track " << trackIndex << ", now using track "
              << trackSelected << std::endl;

    this->trackTextures.push_back(new Texture(blank));
    this->trackFilters.push_back("");
    this->video->setTextures(trackTextures);
    this->trackShader->update(this->trackFilters);
  });

  trackActionsPanel->addTextButton("X Clear All", [this]() {
    std::cout << "Clear Tracks clicked" << std::endl;

    // Clear all tracks and create a new one
    timeline->clearTracks();
    trackSelected = timeline->addTrack("Video Layer 1");
    entrySelected = -1; // Clear entry selection

    std::cout << "Cleared all tracks, created new track 0" << std::endl;

    this->trackTextures.clear();
    this->trackTextures.push_back(new Texture(blank));
    this->trackFilters.clear();
    this->trackFilters.push_back("");
    this->video->setTextures(trackTextures);
    this->trackShader->update(this->trackFilters);
  });

  trackActionsPanel->addTextButton("< Prev Track", [this]() {
    std::cout << "Previous Track clicked" << std::endl;

    if (timeline->getTrackCount() == 0) {
      std::cerr << "No tracks available" << std::endl;
      return;
    }

    // Move to previous track (with wrapping)
    if (trackSelected > 0) {
      trackSelected--;
    } else {
      trackSelected = timeline->getTrackCount() - 1; // Wrap to last track
    }

    entrySelected = -1; // Clear entry selection when switching tracks
    std::cout << "Selected track " << trackSelected << std::endl;
  });

  trackActionsPanel->addTextButton("> Next Track", [this]() {
    std::cout << "Next Track clicked" << std::endl;

    if (timeline->getTrackCount() == 0) {
      std::cerr << "No tracks available" << std::endl;
      return;
    }

    // Move to next track (with wrapping)
    if (trackSelected < timeline->getTrackCount() - 1) {
      trackSelected++;
    } else {
      trackSelected = 0; // Wrap to first track
    }

    entrySelected = -1; // Clear entry selection when switching tracks
    std::cout << "Selected track " << trackSelected << std::endl;
  });

  // Create EXPORT MENU (MVC pattern on right side)
  exportFacade = new ExportFacade();
  exportMenuModel = new ExportMenuModel();
  exportMenuView =
      new ExportMenuView(EXPORT_MENU_X, TITLE_HEIGHT, EXPORT_MENU_WIDTH,
                         VIEWPORT_HEIGHT, exportMenuModel);
  exportMenuController =
      new ExportMenuController(exportMenuModel, exportMenuView, exportFacade);
  exportMenuView->setController(exportMenuController);

  // Set initial asset for export
  if (assets.size() > 0) {
    exportMenuModel->setAsset(assets[0]);
    exportMenuModel->setFilename("export.png");
  }

  // Set timeline for video export
  exportMenuModel->setTimeline(timeline);

  // Update view to create UI buttons now that controller is set
  exportMenuView->update();

  // Create video frame (viewport - red section)
  const Image &image = assets[0]->getFrame();
  // Texture videoTexture(image);
  trackShader = new TrackShader();
  trackShader->update(trackFilters);
  video = new Glyph(VIEWPORT_X, TITLE_HEIGHT, VIEWPORT_WIDTH, VIEWPORT_HEIGHT,
                    trackShader);
  video->setTextures(trackTextures);
  // video.addTexture(videoTexture);

  // video.addTexture(imageTexture);

  // Create the track panel background (dark gray, no asset overlay)
  /*ColorRect trackPanelBg(10, 10, Color(30, 30, 30, 255));
  Glyph trackPanel(TRACK_ACTIONS_WIDTH, TRACKS_Y + 0.05f,
                   1.0f - TRACK_ACTIONS_WIDTH, TRACKS_HEIGHT - 0.05f,
                   trackPanelBg.getImage());*/

  // Create track visualization
  trackVisualization = new TrackVisualization(
      TRACK_ACTIONS_WIDTH, TRACKS_Y + 0.05f, 1.0f - TRACK_ACTIONS_WIDTH,
      TRACKS_HEIGHT - 0.05f, timeline);

  // Set the window drawing area
  glViewport(0, 0, image.getWidth(), image.getHeight());

  // -------------------------------------
  // Window operations
  // -------------------------------------
  while (!glfwWindowShouldClose(window->getWindow())) {
    if (glfwGetKey(window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window->getWindow(), true);
    }

    double timeSinceStart = 0.0;
    if (is_playing) {
      std::chrono::steady_clock::time_point currentTime =
          std::chrono::steady_clock::now();
      timeSinceStart =
          std::chrono::duration<double>(currentTime - startPlayTime).count();

      // Update timeline time
      timeline->setCurrentTime(timeSinceStart);

      // Loop playback if we reach the end
      double totalDuration = timeline->getTotalDuration();
      if (totalDuration > 0 && timeSinceStart >= totalDuration) {
        startPlayTime = std::chrono::steady_clock::now();
        timeSinceStart = 0.0;
        timeline->setCurrentTime(0.0);
      }
    }

    // Render the timeline composite at current time
    // If timeline is empty, show the currently selected asset
    const Image *current_image = nullptr;
    Image *timeline_image = nullptr;

    trackShader->use();
    trackShader->setFloat("duration", timeline->getTotalDuration());
    trackShader->setFloat("timeSinceStart", timeSinceStart);

    if (timeline->getTrackCount() > 0 && timeline->getTotalDuration() > 0) {
      // Render timeline composite
      // timeline_image = timeline->renderFrameAt(timeSinceStart,
      // image.getWidth(),
      // image.getHeight());
      // current_image = timeline_image;
      const std::vector<Track *> &tracks = timeline->getTracks();

      for (int i = 0; i < tracks.size(); i++) {
        const Track *track = tracks[i];

        if (!track->isVisible()) {
          trackTextures[i]->copyToGPU(blank);
          continue;
        }

        // Get the active entry at this time
        const TimelineEntry *entry = track->getEntryAt(timeSinceStart);
        if (!entry) {
          // Uncomment for verbose debugging:
          // std::cout << "Track " << i << " (" << track->getName() << ") has no
          // entry at time " << time << "s" << std::endl;
          trackTextures[i]->copyToGPU(blank);
          continue; // No entry active on this track at this time
        }

        // Uncomment for debugging:
        // std::cout << "Rendering track " << i << " (" << track->getName() <<
        // ") at time " << time << "s" << std::endl;

        // Get the frame from the entry
        const Image &layerImage = entry->getFrameAt(timeSinceStart);

        //(*result) = layerImage;
        // return result;

        // Composite this layer onto the result
        trackTextures[i]->copyToGPU(layerImage);
      }

    } else {
      // No timeline content, show selected asset
      if (assetSelected >= 0 && assetSelected < assets.size()) {
        current_image = &(assets[assetSelected]->getFrame(timeSinceStart));
      } else {
        // Fallback to first asset if selection invalid
        current_image = &(assets[0]->getFrame(timeSinceStart));
      }
    }

    // videoTexture.copyToGPU(*current_image);
    // trackPanel.update(*current_image);

    // Clean up timeline rendered image
    if (timeline_image) {
      delete timeline_image;
    }

    // -------------------------------------
    // Render Graphics - NEW LAYOUT
    // -------------------------------------
    {
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      // Draw viewport (red section)
      video->draw();

      // Draw asset thumbnail buttons (first N buttons are asset thumbnails)
      for (int i = 0; i < buttons.size(); i++) {
        // Set selected state for asset thumbnail buttons
        if (i < assets.size()) {
          buttons[i]->setSelected(i == assetSelected);
        }
        buttons[i]->draw();
      }

      // Draw section labels
      for (int i = 0; i < labels.size(); i++) {
        labels[i]->draw();
      }

      // Draw track panel background (green section)
      // trackPanel.draw();

      // Draw track visualization on top
      if (trackVisualization) {
        trackVisualization->draw(trackSelected, entrySelected,
                                 timeline->getCurrentTime());
      }

      // Draw asset actions panel (yellow section)
      if (assetActionsPanel) {
        assetActionsPanel->draw();
      }

      // Draw asset actions panel (yellow section)
      if (filterPanel) {
        filterPanel->draw();
      }

      // Draw track actions panel (fuschia section)
      if (trackActionsPanel) {
        trackActionsPanel->draw();
      }

      // Draw export menu (right side)
      if (exportMenuView) {
        exportMenuView->draw();
      }
    }

    window->swapBuffers();
    glfwPollEvents();
  }

  // Clean up labels (assets cleaned up in destructor)
  for (int i = 0; i < labels.size(); i++) {
    delete labels[i];
  }

  return 0;
}

void Application::onMouseMove(float x, float y) {
  // Handle entry resizing
  if (isResizingEntry && trackVisualization) {
    Track *track = timeline->getTrack(trackSelected);
    if (track && entrySelected >= 0 && entrySelected < track->getEntryCount()) {
      const TimelineEntry &entry = track->getEntries()[entrySelected];

      // Calculate new duration based on mouse position
      double clickTime = trackVisualization->getTimeAtPosition(x);
      double newDuration = clickTime - entry.getStartTime();

      // Update entry duration
      bool success = track->updateEntryDuration(entrySelected, newDuration);
      if (success) {
        resizeStartDuration = newDuration;
      }
    }
  }

  // Handle entry dragging
  if (isDraggingEntry && trackVisualization) {
    // Calculate time offset based on mouse movement
    double deltaX = x - dragStartX;
    double timeOffset =
        trackVisualization->getTimeAtPosition(x + 0.001f) -
        trackVisualization->getTimeAtPosition(dragStartX + 0.001f);

    double newStartTime = dragStartTime + timeOffset;

    // Update entry position
    Track *track = timeline->getTrack(trackSelected);
    if (track && entrySelected >= 0 && entrySelected < track->getEntryCount()) {
      bool success = track->updateEntryStartTime(entrySelected, newStartTime);
      if (success) {
        // Update drag start for next frame
        dragStartX = x;
        dragStartTime = newStartTime;
      }
      // If failed (would overlap), just don't update
    }
  }

  // Handle asset thumbnail buttons
  for (int i = 0; i < buttons.size(); i++) {
    Button *button = buttons[i];
    if (button->contains(x, y)) {
      button->setHighlighted(true);
    } else {
      button->setHighlighted(false);
    }
  }

  // Handle asset actions panel buttons
  if (assetActionsPanel) {
    for (Button *btn : assetActionsPanel->getButtons()) {
      if (btn->contains(x, y)) {
        btn->setHighlighted(true);
      } else {
        btn->setHighlighted(false);
      }
    }
  }

  // Handle asset actions panel buttons
  if (filterPanel) {
    for (Button *btn : filterPanel->getButtons()) {
      if (btn->contains(x, y)) {
        btn->setHighlighted(true);
      } else {
        btn->setHighlighted(false);
      }
    }
  }

  // Handle track actions panel buttons
  if (trackActionsPanel) {
    for (Button *btn : trackActionsPanel->getButtons()) {
      if (btn->contains(x, y)) {
        btn->setHighlighted(true);
      } else {
        btn->setHighlighted(false);
      }
    }
  }

  // Handle export menu mouse move
  if (exportMenuController) {
    exportMenuController->onMouseMove(x, y);
  }
}

void Application::onMouseClick(int button, int action, int mods) {
  if (button != GLFW_MOUSE_BUTTON_LEFT) {
    return;
  }

  // Get mouse position from window
  double xpos, ypos;
  glfwGetCursorPos(window->getWindow(), &xpos, &ypos);

  int width, height;
  glfwGetWindowSize(window->getWindow(), &width, &height);

  float x = xpos / width;
  float y = ypos / height;

  // Handle mouse release - stop dragging/resizing
  if (action == GLFW_RELEASE) {
    if (isDraggingEntry) {
      isDraggingEntry = false;
      std::cout << "Finished dragging entry" << std::endl;
    }
    if (isResizingEntry) {
      isResizingEntry = false;
      std::cout << "Finished resizing entry" << std::endl;
    }
  }

  // Handle track visualization clicks for track/entry selection, dragging, and
  // resizing
  if (action == GLFW_PRESS && trackVisualization) {
    int clickedTrack, clickedEntry;

    // Check if clicking near right edge of entry (resize handle)
    if (trackVisualization->isNearEntryRightEdge(x, y, clickedTrack,
                                                 clickedEntry)) {
      trackSelected = clickedTrack;
      entrySelected = clickedEntry;

      // Start resizing this entry
      Track *track = timeline->getTrack(trackSelected);
      if (track && entrySelected >= 0 &&
          entrySelected < track->getEntryCount()) {
        const TimelineEntry &entry = track->getEntries()[entrySelected];
        resizeStartDuration = entry.getDuration();
        isResizingEntry = true;
        std::cout << "Started resizing entry " << clickedEntry << " on track "
                  << clickedTrack << std::endl;
      }
      return; // Don't process other clicks
    }

    // Otherwise check if clicking on entry body (drag handle)
    if (trackVisualization->getEntryAtPosition(x, y, clickedTrack,
                                               clickedEntry)) {
      // Clicked on an entry
      trackSelected = clickedTrack;
      entrySelected = clickedEntry;

      // Start dragging this entry
      Track *track = timeline->getTrack(trackSelected);
      if (track && entrySelected >= 0 &&
          entrySelected < track->getEntryCount()) {
        const TimelineEntry &entry = track->getEntries()[entrySelected];
        dragStartTime = entry.getStartTime();
        dragStartX = x;
        isDraggingEntry = true;
        std::cout << "Started dragging entry " << clickedEntry << " on track "
                  << clickedTrack << std::endl;
      }
      return; // Don't process other clicks
    }

    // Check if clicked on a track (but not on an entry)
    int clickedTrack2 = trackVisualization->getTrackAtPosition(x, y);
    if (clickedTrack2 >= 0) {
      trackSelected = clickedTrack2;
      entrySelected = -1;      // Deselect entry
      isDraggingEntry = false; // Cancel any drag
      isResizingEntry = false; // Cancel any resize
      std::cout << "Selected track " << clickedTrack2 << std::endl;
      return; // Don't process other clicks
    }
  }

  // Handle asset thumbnail buttons
  for (int i = 0; i < buttons.size(); i++) {
    Button *b = buttons[i];
    if (action == GLFW_PRESS) {
      if (b->isHighlighted()) {
        b->setClicked(true);
        b->onClick();
      }
    }
    if (action == GLFW_RELEASE) {
      b->setClicked(false);
    }
  }

  // Handle asset actions panel buttons
  if (assetActionsPanel) {
    for (Button *btn : assetActionsPanel->getButtons()) {
      if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (btn->isHighlighted()) {
          btn->setClicked(true);
          btn->onClick();
        }
      }
      if (action == GLFW_RELEASE) {
        btn->setClicked(false);
      }
    }
  }

  // Handle asset actions panel buttons
  if (filterPanel) {
    for (Button *btn : filterPanel->getButtons()) {
      if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (btn->isHighlighted()) {
          btn->setClicked(true);
          btn->onClick();
        }
      }
      if (action == GLFW_RELEASE) {
        btn->setClicked(false);
      }
    }
  }

  // Handle track actions panel buttons
  if (trackActionsPanel) {
    for (Button *btn : trackActionsPanel->getButtons()) {
      if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (btn->isHighlighted()) {
          btn->setClicked(true);
          btn->onClick();
        }
      }
      if (action == GLFW_RELEASE) {
        btn->setClicked(false);
      }
    }
  }

  // Handle export menu mouse click
  if (exportMenuController) {
    exportMenuController->onMouseClick(button, action, mods);
  }
}

} // namespace csci3081