#include "Image.hpp"

#include "lodepng.h"


/* PNG images */

Image::Error PNGImage::write(const std::string fname) const {
  std::vector<unsigned char> img;
  for (auto x: _data) {
    img.push_back(x.r);
    img.push_back(x.g);
    img.push_back(x.b);
    img.push_back(x.a);
  }
  unsigned error = lodepng::encode(fname, img, _width, _height);
  if (error)  {
    std::cerr << "Encode Error: " << error << ": " << lodepng_error_text(error) << '\n';
    return Image::Error::FILETYPE;
  }
  return Image::Error::NONE;
}

Image::Error PNGImage::read(const std::string fname) {
  std::vector<unsigned char> png;
  std::vector<unsigned char> image;
  unsigned error = lodepng::load_file(png, fname);
  if (error) return Image::Error::NOFILE;
  error = lodepng::decode(image, _width, _height, png);
  if (error) return Image::Error::FILETYPE;

  for (auto itr = image.begin(); itr != image.end(); itr += 4)
    _data.push_back({*itr, *(itr+1), *(itr+2), *(itr+3)});

  std::cout << "Image Loaded: " << _width << "×" << _height << '\n';
  std::cout << this->size() << ", " << this->bytes() << " bytes\n";
  return Image::Error::NONE;
}


/* PPM images */

Image::Error PPMImage::write(const std::string fname) const {
  return Image::Error::FILETYPE; // Not implemented yet
  return Image::Error::NONE;
}

Image::Error PPMImage::read(const std::string fname) {
  return Image::Error::FILETYPE; // Not implemented yet
}
