//  U8Widget - Simple widget library based on U8g2 by olikraus
//  Copyright (C) 2024  Tamas Karpati
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.#pragma once

#include "Image.h"

#include "Display.h"

namespace U8W
{

Image::Image(Widget* parent)
    : Widget{ parent }
{}

Image::Image(
    const unsigned char* imageData,
    const int width,
    const int height,
    Widget* parent
)
    : Widget{ parent }
{
    setImage(imageData, width, height);
}

void Image::setImage(
    const unsigned char* imageData,
    const int width,
    const int height
) {
    if (!imageData || width == 0 || height == 0) {
        return;
    }

    _imageData = imageData;
    _imageSize = Size{ width, height };

    setSize(_imageSize);
}

void Image::setInverted(const bool inverted)
{
    _inverted = inverted;
    _needsRepaint = true;
}

Size Image::imageSize() const
{
    return _imageSize;
}

bool Image::isNull() const
{
    return !_imageData || !_imageSize.isValid();
}

void Image::paint()
{
    if (!isNull()) {
        const auto globalRect = mapToGlobal(_rect);

        // _display->setClipRect(calculateClipRect());

        _display->setDrawColor(
            _inverted
                ? Display::Color::White
                : Display::Color::Black
        );

        _display->drawBitmap(
            globalRect.topLeft(),
            globalRect.width(),
            globalRect.height(),
            reinterpret_cast<const uint8_t*>(_imageData)
        );

        // _display->resetClipRect();
    }

    Widget::paint();
}

}
