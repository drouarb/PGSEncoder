//
// Created by Benjamin DROUARD on 29/06/2018.
//

#ifndef PGSENCODER_PALETTE_H
#define PGSENCODER_PALETTE_H

#include <map>
#include <iostream>
#include <vector>
#include "PNGImage.h"
#include "ColorTools.h"

class Palette {
public:
    Palette(PNGImage &image);
    void generatePalette(int fd, unsigned int pts);
    unsigned char getColor(unsigned int color);

    unsigned char getDefaultColorReplacement();

private:
    unsigned int defaultColorReplacement;
    std::map<unsigned int, unsigned int> mergeMap;
    std::map<unsigned int, unsigned char> paletteMap;
    std::vector<std::pair<unsigned int, int>> colorSorter;
};


#endif //PGSENCODER_PALETTE_H
