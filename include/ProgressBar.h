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

#pragma once

#include "Rect.h"

#include <u8g2.h>
#include <u8x8.h>

#include <string>

namespace U8W
{

class ProgressBar
{
public:
    explicit ProgressBar(u8g2_t& display);

    void setRect(int x, int y, int w, int h);
    void setPosition(int pos);
    void setText(std::string text);

    void update();

private:
    u8g2_t& _display;
    Rect _rect;
    int _position = 0;
    std::string _text;
    bool _useDefaultText = true;
};

}
