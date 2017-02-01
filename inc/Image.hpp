#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <regex>


struct RGBA {
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
};

class Image {
public:

  inline Image() {}

  inline Image(unsigned int width, unsigned int height) :
    _width(width),
    _height(height),
    _data(width * height) { }

  enum Error { NONE=0, NOFILE, FILETYPE };

  // TODO: allow images to read and write raw data
  virtual Image::Error write(std::string fname) const = 0;
  virtual Image::Error read(std::string fname) = 0;

  inline unsigned int width() const { return _width; }
  inline unsigned int height() const { return _height; }

  // Number of pixels
  inline unsigned long size() const { return _data.size(); }
  // Number of bytes in our representation
  inline unsigned long bytes() const { return _data.size() * sizeof(RGBA); }

  inline void set(unsigned long index, RGBA color) { _data[index] = color; }
  inline void set(unsigned int x, unsigned int y, RGBA color)
  { _data[x + y * _width] = color; }

  inline RGBA get(unsigned long index) const { return _data[index]; }
  inline RGBA get(unsigned int x, unsigned int y) const
  { return  _data[x + y * _width];}

  inline RGBA operator[](unsigned long index) const { return _data[index]; }
  inline RGBA operator()(unsigned long index) const { return _data[index]; }
  inline RGBA operator()(unsigned int x, unsigned int y) { return _data[x + y * _width]; }

protected:

  unsigned int _width;
  unsigned int _height;

  std::vector<RGBA> _data;

private:
};

class PNGImage : public Image {
public:

  // Don't do this if you are going to read from file
  inline PNGImage(unsigned int width, unsigned int height): Image(width, height) {}
  inline PNGImage() : Image() {}

  virtual Image::Error write(const std::string fname) const;
  virtual Image::Error read(const std::string fname);
};

class PPMImage : public Image {
public:
  // Don't do this if you are going to read from file
  inline PPMImage(unsigned int width, unsigned int height): Image(width, height) {}
  inline PPMImage() : Image() {}


  virtual Image::Error write(const std::string fname) const;
  virtual Image::Error read(const std::string fname) ;

protected:
  bool _binary; // p6 or p3 header
};

#endif//IMAGE_HPP
