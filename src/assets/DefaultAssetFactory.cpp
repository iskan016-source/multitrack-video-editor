#include "assets/DefaultAssetFactory.h"
#include "graphics/ColorRect.h"
#include <iostream>
#include <random>

namespace csci3081 {

class DefaultAsset : public IAsset {
public:
  DefaultAsset() {
    std::random_device seed;
    std::mt19937 gen{seed()};
    std::uniform_int_distribution<> dist{0, 255};
    int r = dist(gen);
    int g = dist(gen);
    int b = dist(gen);

    // Use ColorRect to create a solid color image
    ColorRect colorRect(10, 10, Color(r, g, b, 255));
    image = colorRect.getImage();
  }

  double getDuration() const { return 3.0; }
  const Image &getFrame(double time) {
    for (int x = 0; x < image.getWidth(); x++) {
      for (int y = 0; y < image.getWidth(); y++) {
        Color c = image.getPixel(x, y);
        int q = time / 3;
        time = time - (3.0 * q);
        c.setRed(255.0 * time / 3.0);
        image.setPixel(x, y, c);
      }
    }
    return image;
  }

  const Image &getThumbnail() {
    return image;
  }

  bool isVideo() const { return false; }

  AssetType getAssetType() const { return AssetType::DEFAULT; }

private:
  Image image;
};

IAsset *DefaultAssetFactory::create(const std::string &value) const {
  std::cout << value << ": No suitable factory found (creating default asset)."
            << std::endl;
  return new DefaultAsset();
}

} // namespace csci3081