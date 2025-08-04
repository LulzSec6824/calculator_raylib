#ifndef EMBEDDED_RESOURCES_H
#define EMBEDDED_RESOURCES_H

// This header file contains embedded resources for the calculator application
// Font and image data will be included from generated header files

#include "../raylib_v5/src/raylib.h"

// Include generated resource headers
#include "../includes/font_ubuntu.h"
#include "../includes/icon_calc.h"

// Function to load embedded font
Font LoadEmbeddedFont(void) {
    // Load font from memory
    Font font = {};

    // Create a new font from embedded data
    font.baseSize     = 64;  // From font_ubuntu.h
    font.glyphCount   = 95;  // From font_ubuntu.h
    font.glyphPadding = 4;   // From font_ubuntu.h

    // Load texture from memory using the decompressed data
    int fontDataSize = 0;
    unsigned char* data =
        DecompressData(fontData_FontUbuntu,
                       COMPRESSED_DATA_SIZE_FONT_FONTUBUNTU, &fontDataSize);
    Image imFont = {data, 1024, 512, 1, 2};

    font.texture = LoadTextureFromImage(imFont);
    UnloadImage(imFont);  // Uncompressed data can be unloaded from memory

    // Assign glyph recs and info data directly
    font.recs   = fontRecs_FontUbuntu;
    font.glyphs = fontGlyphs_FontUbuntu;

    return font;
}

// Function to load embedded icon
Image LoadEmbeddedIcon(void) {
    // Create image from embedded data
    Image icon   = {};
    icon.data    = ICON_CALC_DATA;
    icon.width   = ICON_CALC_WIDTH;
    icon.height  = ICON_CALC_HEIGHT;
    icon.format  = ICON_CALC_FORMAT;
    icon.mipmaps = 1;

    return icon;
}

#endif  // EMBEDDED_RESOURCES_H