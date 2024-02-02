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

#include "Font.h"
#include "Point.h"
#include "Rect.h"
#include "Size.h"

#include <memory>
#include <string>

namespace U8W
{

class Display
{
public:
    explicit Display();
    ~Display();

    Size size() const;

    void clear();
    void clearBuffer();

    void update();

    void setContrast(uint8_t value);
    void setBacklightLevel(uint8_t value);
    void setDrawColor(Color color);
    void setFont(const Font& font);
    void setClipRect(const Rect& rect);
    void resetClipRect();

    [[nodiscard]] int calculateFontAscent() const;
    [[nodiscard]] int calculateFontDescent() const;
    [[nodiscard]] int calculateMaxCharHeight() const;
    [[nodiscard]] int calculateTextWidth(const std::string& text) const;

    void drawText(const Point& pos, const std::string& s);
    void drawBitmap(const Point& pos, int width, int height, const uint8_t* data);
    void drawRect(const Rect& rect);
    void drawLine(const Point& from, const Point& to);
    void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

    void fillRect(const Rect& rect);

private:
    struct Private;
    std::unique_ptr<Private> _p;

    void setup();
};

}
