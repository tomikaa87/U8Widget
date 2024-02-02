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

#include "ProgressBar.h"

#include <algorithm>
#include <string>

namespace U8W
{

ProgressBar::ProgressBar(u8g2_t& display)
    : _display{ display }
{}

void ProgressBar::setRect(const int x, const int y, const int w, const int h)
{
    const Rect r{ x, y, w, h };
    _rect = r;
}

void ProgressBar::setPosition(int pos)
{
    pos = std::max(0, std::min(100, pos));
    _position = pos;
}

void ProgressBar::setText(std::string text)
{
    _text = std::move(text);
    _useDefaultText = _text.empty();
}

void ProgressBar::update()
{
    // Background
    u8g2_SetDrawColor(&_display, 0);
    u8g2_DrawRBox(&_display, _rect.x(), _rect.y(), _rect.width(), _rect.height(), 1);

    // Frame
    u8g2_SetDrawColor(&_display, 1);
    u8g2_DrawRFrame(&_display, _rect.x(), _rect.y(), _rect.width(), _rect.height(), 1);

    // Indicator
    const int wMax = _rect.width() - 4;
    const int wActual = wMax * _position / 100;
    if (wActual > 0) {
        u8g2_DrawBox(&_display, _rect.x() + 2, _rect.y() + 2, wActual, _rect.height() - 4);
    }

    // Text
    const auto& text = _useDefaultText ? std::to_string(_position) + '%' : _text;
    if (!text.empty()) {
        u8g2_SetDrawColor(&_display, 2);
        u8g2_SetFont(&_display, u8g2_font_p01type_tr);
        u8g2_SetFontMode(&_display, 1);
        const auto strW = u8g2_GetStrWidth(&_display, text.c_str());
        const auto strH = u8g2_GetMaxCharHeight(&_display);
        const auto textX = _rect.width() / 2 - strW / 2;
        const auto textY = _rect.height() / 2 - strH / 2 + u8g2_GetAscent(&_display);
        u8g2_DrawStr(&_display, textX, textY, text.c_str());
    }
}

}
