//
// Created by Benjamin DROUARD on 29/06/2018.
//

#include "Palette.h"
#include "PGSStructures.h"

bool pairCompare(const std::pair<unsigned int, int>& firstElem, const std::pair<unsigned int, int>& secondElem) {
    return firstElem.second > secondElem.second;

}

Palette::Palette(PNGImage &image) {
    std::map<unsigned int, int> color_counter;
    png_bytep *argb = image.getPixels();

    //Count each color
    for (int y = 0; y < image.getHeight(); y++) {
        png_bytep row = image.getPixels()[y];
        for (int x = 0; x < image.getWidth(); x++) {
            png_bytep px = &(row[x * 4]);

            color_counter[getRGBA(px[0], px[1], px[2], px[3])]++;
        }
    }

    if (color_counter.size() > 255) {
        std::cout << "There is more than 255 (" << color_counter.size() << ") colors in " << image.getPath() << std::endl;
        //abort();
    }

    //Sort it by the most used
    for (auto &c: color_counter)
        colorSorter.emplace_back(c);

    std::sort(colorSorter.begin(), colorSorter.end(), pairCompare);

    //Should remove some colors
    while (colorSorter.size() > 256) {
        //Get one of least used color
        std::pair<unsigned int, int> color = colorSorter.back();
        colorSorter.pop_back();

        //Find the nearest color
        int pos = -1;
        long distance = 0;
        for (int i = 0; i < colorSorter.size(); i++) {
            long newdist = RGBADistance(colorSorter[i].first, color.first);

            if (pos == -1 || newdist < distance) {
                pos = i;
                distance = newdist;
            }
        }

        //Merge them and sort again
        colorSorter[pos].second += color.second;
        mergeMap[color.first] = colorSorter[pos].first;
        std::sort(colorSorter.begin(), colorSorter.end(), pairCompare);
    }

    //Push in paletteMap
    for (int i = 0; i < colorSorter.size(); i++)
        paletteMap[colorSorter[i].first] = i;

    //Get a replacement color if we need 1 pixel of prefered color
    int pos = -1;
    long distance = 0;
    for (int i = 0; i < colorSorter.size(); i++) {
        long newdist = RGBADistance(colorSorter[i].first, colorSorter.front().first);

        if (pos == -1 || newdist < distance) {
            pos = i;
            distance = newdist;
        }
    }
    defaultColorReplacement = colorSorter[pos].first;
}

void Palette::generatePalette(int fd, unsigned int pts) {
    PGSHeader_t  header;
    PDSSegment_t pds;
    PDSPalette_t palette;

    header.pts = pts;
    header.segment_type = SEGMENT_TYPE_PDS;
    header.segment_size = sizeof(pds) + sizeof(palette) * colorSorter.size();

    pds.palette_id = 0;
    pds.palette_version = 0;

    write_struct(fd, header);
    write_struct(fd, pds);

    for (int i = 0; i < colorSorter.size(); i++) {
        palette.palette_entry_id = i;
        palette.y =  CRGB2Y(getR(colorSorter[i].first), getG(colorSorter[i].first), getB(colorSorter[i].first));
        palette.cb = CRGB2Cb(getR(colorSorter[i].first), getG(colorSorter[i].first), getB(colorSorter[i].first));
        palette.cr = CRGB2Cr(getR(colorSorter[i].first), getG(colorSorter[i].first), getB(colorSorter[i].first));
        palette.alpha = getA(colorSorter[i].first);

        write_struct(fd, palette);
    }
}

unsigned char Palette::getColor(unsigned int color) {
    if (paletteMap.find(color) == paletteMap.end()) {
        return paletteMap[mergeMap[color]];
    } else {
        return paletteMap[color];
    }
}

unsigned char Palette::getDefaultColorReplacement() {
    return paletteMap[defaultColorReplacement];
}
