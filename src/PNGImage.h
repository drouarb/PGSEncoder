//
// Created by Benjamin DROUARD on 29/06/2018.
//

#ifndef PGSENCODER_PNGIMAGE_H
#define PGSENCODER_PNGIMAGE_H


#include <string>
#include <png.h>

class PNGImage {
public:
    PNGImage(std::string path);
    ~PNGImage();

    int getWidth() const;
    int getHeight() const;

    const std::string &getPath() const;

    png_bytep *getPixels() const;

private:
    int width;
    int height;
    int color_type;
    int bit_depth;
    std::string path;
    png_bytep *row_pointers;
};


#endif //PGSENCODER_PNGIMAGE_H
