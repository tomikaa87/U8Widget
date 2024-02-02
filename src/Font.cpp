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

#include "../Fonts.h"

#include <u8g2.h>

namespace U8W
{

Font::Font(
    Family family,
    Style style
)
    : _family{ family }
    , _style{ style }
{}

void Font::setFamily(const Family family)
{
    _family = family;
}

void Font::setStyle(const Style style)
{
    _style = style;
}

void Font::setBold(const bool bold)
{
    _bold = bold;
}

const uint8_t* Font::data() const
{
    switch (_family) {
        case Family::PfTempesta7: {
            switch (_style) {
                case Style::Regular:
                    return _bold
                        ? Fonts::PfTempesta7Bold
                        : Fonts::PfTempesta7;

                case Style::Compressed:
                    return _bold
                        ? Fonts::PfTempesta7CompressedBold
                        : Fonts::PfTempesta7CompressedBold;

                case Style::Condensed:
                default:
                    return _bold
                        ? Fonts::PfTempesta7CondensedBold
                        : Fonts::PfTempesta7Condensed;
            }
        }

        case Family::RpgSystem:
            return Fonts::RpgSystem;

        case Family::Pxl16x8:
            return Fonts::Pxl16x8;

        case Family::Pxl16x8_x2:
            return Fonts::Pxl16x8_x2;

        case Family::Pxl16x8_Mono:
            return Fonts::Pxl16x8_Mono;

        case Family::Pxl16x8_Mono_x2:
            return Fonts::Pxl16x8_Mono_x2;

        case Family::BitCell:
            return Fonts::BitCell;

        case Family::BitCellMonoNumbers:
            return Fonts::BitCellMonoNumbers;
    }

    return nullptr;
}

}
