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

#include "Label.h"

#include "Display.h"

namespace U8W
{

Label::Label(Widget* parent)
    : Widget{ parent }
{
    updateHeightByFont();
}

Label::Label(std::string text, Widget* parent)
    : Widget{ parent }
    , _text{ std::move(text) }
{
    updateHeightByFont();
}

void Label::setText(std::string text)
{
    _text = std::move(text);
    updateTextPosition();
}

void Label::setFont(const Font& font)
{
    _font = font;
    updateHeightByFont();
    updateTextPosition();
}

void Label::setAlignment(const Align alignment)
{
    _alignment = alignment;
    updateTextPosition();
}

void Label::setHeightCalculation(const HeightCalculation heightCalculation)
{
    _heightCalculation = heightCalculation;
    updateHeightByFont();
}

void Label::paint()
{
    _display->setDrawColor(Display::Color::Black);
    _display->setFont(_font);
    // _display->setClipRect(calculateClipRect());

    _display->drawText(_textPos, _text);

    // _display->resetClipRect();

    Widget::paint();
}

void Label::updateHeightByFont()
{
    _needsRepaint = true;
    _display->setFont(_font);

    switch (_heightCalculation) {
        case HeightCalculation::NoDescent:
            setHeight(_display->calculateFontAscent());
            break;

        case HeightCalculation::WithDescent:
            setHeight(_display->calculateMaxCharHeight() + 1);
            break;
    }
}

void Label::updateTextPosition()
{
    _needsRepaint = true;

    switch (_alignment) {
        case Align::Left:
            _display->setFont(_font);
            _textPos = 
                mapToGlobal(_rect.topLeft())
                + Point{
                    0,
                    _display->calculateFontAscent() + 1
                };
            break;

        case Align::Center: {
            _display->setFont(_font);
            const auto textWidth = _display->calculateTextWidth(_text);
            _textPos =
                mapToGlobal(_rect.topLeft())
                + Point{
                    _rect.width() / 2 - textWidth / 2,
                    _display->calculateFontAscent() + 1
                };
            break;
        }

        case Align::Right: {
            _display->setFont(_font);
            const auto textWidth = _display->calculateTextWidth(_text);
            _textPos =
                mapToGlobal(_rect.topLeft())
                + Point{
                    _rect.width() - textWidth,
                    _display->calculateFontAscent() + 1
                };
            break;
        }
    }
}

}
