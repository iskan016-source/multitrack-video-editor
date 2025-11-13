#ifndef CAPTION_H
#define CAPTION_H

#include "assets/IAsset.h"
#include "graphics/Text.h"
#include <string>

namespace csci3081 {

/**
 * @brief Asset that renders text as an image
 *
 * Caption is an adapter that converts text into an IAsset.
 * It uses the Text class (FreeType) to render text and provides
 * it as a static image asset.
 */
class Caption : public IAsset {
public:
    /**
     * @brief Construct a new Caption object
     *
     * @param text The text to render
     * @param fontPath Path to TrueType font file (defaults to GoogleSansCode-Regular.ttf)
     * @param fontSize Font size in pixels (default: 48)
     * @param textColor Color of the text (default: white)
     * @param duration How long to display the caption (default: 5.0 seconds)
     */
    Caption(const std::string& text,
            const std::string& fontPath = "assets/fonts/GoogleSansCode-Regular.ttf",
            unsigned int fontSize = 48,
            const Color& textColor = Color(255, 255, 255, 255),
            double duration = 5.0);

    /**
     * @brief Destroy the Caption object
     */
    virtual ~Caption();

    /**
     * @brief Get the duration of the caption
     *
     * @return double Duration in seconds
     */
    virtual double getDuration() const override { return captionDuration; }

    /**
     * @brief Get the rendered frame
     *
     * Caption ignores the time parameter since it's a static image
     *
     * @param time Time parameter (ignored)
     * @return const Image& Reference to the rendered text image
     */
    virtual const Image& getFrame(double time = 0.0) override;

    /**
     * @brief Get thumbnail image
     *
     * @return const Image& Reference to the rendered text image
     */
    virtual const Image& getThumbnail() override;

    /**
     * @brief Check if this is a video asset
     *
     * @return false Caption is a static asset like images
     */
    virtual bool isVideo() const override;

    /**
     * @brief Get the asset type
     *
     * @return AssetType::TEXT
     */
    virtual AssetType getAssetType() const override;

    /**
     * @brief Set the caption text
     *
     * @param text New text to display
     */
    void setText(const std::string& text);

    /**
     * @brief Get the current caption text
     *
     * @return std::string The current text
     */
    std::string getText() const { return captionText; }

    /**
     * @brief Set the text color
     *
     * @param color New text color
     */
    void setTextColor(const Color& color);

private:
    void regenerateImage();

    Text* textRenderer;
    Image* renderedImage;
    std::string captionText;
    Color textColor;
    double captionDuration;
};

}

#endif
