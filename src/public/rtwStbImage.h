#ifndef RTW_STB_IMAGE_H
#define RTW_STB_IMAGE_H

#ifdef _MSC_VER
  #pragma warning (push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"

#include <cstdlib>
#include <iostream>

class RTWImage
{
private:
  const int m_bytesPerPixel {3};
  float* m_fdata {nullptr};
  unsigned char* m_bData {nullptr};
  int m_imageWidth {0};
  int m_imageHeight {0};
  int m_bytesPerScanline {0};

private:
  static int clamp(int x, int low, int high)
  {
    if (x < low) { return low; }
    if (x < high) { return x; }
    return (high - 1);
  }

  static unsigned char floatToByte(float value)
  {
    if (value <= 0.0) { return 0.0; }
    if (1.0 <= value) { return 255.0; }
    return static_cast<unsigned char>(256.0 * value);
  }

  void convertToBytes()
  {
    int totalByte {m_imageWidth * m_imageHeight * m_bytesPerPixel};
    m_bData = new unsigned char[totalByte];

    auto* bPtr {m_bData};
    auto* fPtr {m_fdata};
    for (auto i {0}; i < totalByte; ++i, ++fPtr, ++bPtr)
    { *bPtr = floatToByte(*fPtr); }
  }

public:
  RTWImage() {}

  RTWImage(const char* imageFilename)
  {
    auto filename {static_cast<std::string>(imageFilename)};
    auto imageDir {getenv("RTW_IMAGES")};

    if (imageDir && load(static_cast<std::string>(imageDir) + "/" + imageFilename)) return;
    if (load(filename)) return;
    if (load("../textures/" + filename)) return;

    std::cerr << "ERROR: Could not load image file " << imageFilename << ".\n";
  }

  ~RTWImage() 
  {
    delete[] m_bData;
    STBI_FREE(m_fdata);
  }

  bool load(const std::string& filename)
  {
    auto n {m_bytesPerPixel};
    m_fdata = stbi_loadf(filename.c_str(), &m_imageWidth, &m_imageHeight, &n, m_bytesPerPixel);
    if (m_fdata == nullptr) { return false; }

    m_bytesPerScanline = m_imageWidth * m_bytesPerPixel;
    convertToBytes();
    return true;
  }

  int width() const { return ((m_fdata == nullptr) ? 0 : m_imageWidth); }
  int height() const { return ((m_fdata == nullptr) ? 0 : m_imageHeight); }

  const unsigned char* pixelData(int x, int y) const
  {
    static unsigned char magenta[] {255, 0, 255};
    if (m_bData == nullptr) { return magenta; }

    x = clamp(x, 0, m_imageWidth);
    y = clamp(y, 0, m_imageHeight);

    return (m_bData + (y * m_bytesPerScanline) + (x * m_bytesPerPixel));
  }
};

#ifdef _MSC_VER
  #pragma warning (pop)
#endif

#endif
