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

#include "Font.h"
#include "Widget.h"

#include <string>

namespace U8W
{

class Label : public Widget
{
public:
    explicit Label(Widget* parent);
    Label(std::string text, Widget* parent);

    virtual void paint() override;

    void setText(std::string text);
    void setFont(const Font& font);

    void setAlignment(Align alignment);

    enum class HeightCalculation
    {
        WithDescent,
        NoDescent
    };

    void setHeightCalculation(HeightCalculation heightCalculation);

private:
    std::string _text;
    Font _font;
    Align _alignment = Align::Left;
    Point _textPos;
    HeightCalculation _heightCalculation = HeightCalculation::WithDescent;

    void updateHeightByFont();
    void updateTextPosition();
};

}
