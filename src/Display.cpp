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

#include "Display.h"

#include "../Fonts.h"

#include <hardware/gpio.h>
#include <hardware/pwm.h>
#include <hardware/spi.h>

#include <pico/time.h>

#include <u8g2.h>
#include <u8x8.h>

#include <memory>

namespace U8W
{

namespace Pins
{
    constexpr auto CS = 5;
    constexpr auto DC = 8;
    constexpr auto RST = 9;

    constexpr auto BacklightPwm = 0;

    namespace SPI0
    {
        constexpr auto RX = 4;
        constexpr auto CS = 5;
        constexpr auto SCK = 6;
        constexpr auto TX = 7;
    }
}

extern "C" uint8_t u8x8_d_st7586s_erc240160_chunked(
    u8x8_t* const u8x8,
    const uint8_t msg,
    const uint8_t arg_int,
    void* const arg_ptr
);

extern "C" uint8_t u8x8_byte_hw_spi0_pico(
    u8x8_t* const u8x8,
    const uint8_t msg,
    const uint8_t arg_int,
    void* const arg_ptr
)
{
    switch (msg)
    {
        case U8X8_MSG_BYTE_SEND:
            spi_write_blocking(spi0, (uint8_t *)arg_ptr, arg_int);
            break;

        case U8X8_MSG_BYTE_INIT:
            if (u8x8->bus_clock == 0) /* issue 769 */
                u8x8->bus_clock = u8x8->display_info->sck_clock_hz;
            /* disable chipselect */
            u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);

            /* no wait required here */
            /* for SPI: setup correct level of the clock signal */
            spi_set_format(
                spi0,
                8,
                [&] {
                    switch (u8x8->display_info->spi_mode)
                    {
                        case 0:
                        case 1:
                        default:
                            return SPI_CPOL_0;
                        case 2:
                        case 3:
                            return SPI_CPOL_1;
                    }
                }(),
                [&] {
                    switch (u8x8->display_info->spi_mode)
                    {
                        case 0:
                        case 2:
                        default:
                            return SPI_CPHA_0;
                        case 1:
                        case 3:
                            return SPI_CPHA_1;
                    }
                }(),
                SPI_MSB_FIRST
            );

            gpio_set_function(Pins::SPI0::RX, GPIO_FUNC_SPI);
            gpio_set_function(Pins::SPI0::SCK, GPIO_FUNC_SPI);
            gpio_set_function(Pins::SPI0::TX, GPIO_FUNC_SPI);

            spi_init(spi0, 60'000'000);
            break;

        case U8X8_MSG_BYTE_SET_DC:
            u8x8_gpio_SetDC(u8x8, arg_int);
            break;

        case U8X8_MSG_BYTE_START_TRANSFER:
            u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_enable_level);
            u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);
            break;

        case U8X8_MSG_BYTE_END_TRANSFER:
            u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);
            u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
            break;

        default:
            return 0;
    }

    return 1;
}

extern "C" uint8_t u8x8_gpio_and_delay_pico(
    u8x8_t* const u8g2,
    const uint8_t msg,
    const uint8_t arg_int,
    void* const arg_ptr
)
{
    switch (msg)
    {
        case U8X8_MSG_GPIO_AND_DELAY_INIT:
            gpio_init(Pins::CS);
            gpio_init(Pins::DC);
            gpio_init(Pins::RST);
            gpio_set_dir(Pins::CS, true);
            gpio_set_dir(Pins::DC, true);
            gpio_set_dir(Pins::RST, true);
            break;

        case U8X8_MSG_DELAY_MILLI:
            sleep_ms(arg_int);
            break;

        case U8X8_MSG_GPIO_DC:
            gpio_put(Pins::DC, !!arg_int);
            break;

        case U8X8_MSG_GPIO_CS:
            gpio_put(Pins::CS, !!arg_int);
            break;

        case U8X8_MSG_GPIO_RESET:
            gpio_put(Pins::RST, !!arg_int);
            break;

        default:
            return 0;
    }

    return 1;
}

struct Display::Private
{
    u8g2_t u8g2;
};

Display::Display()
    : _p{ std::make_unique<Private>() }
{
    setup();
    setFont(Font{});
}

Display::~Display() = default;

Size Display::size() const
{
    return {
        u8g2_GetDisplayWidth(&_p->u8g2),
        u8g2_GetDisplayHeight(&_p->u8g2)
    };
}

void Display::clear()
{
    u8g2_ClearDisplay(&_p->u8g2);
}

void Display::clearBuffer()
{
    u8g2_ClearBuffer(&_p->u8g2);
}

void Display::update()
{
    u8g2_SendBuffer(&_p->u8g2);
}

void Display::setContrast(const uint8_t value)
{
    u8g2_SetContrast(&_p->u8g2, value);
}

void Display::setBacklightLevel(const uint8_t value)
{
    pwm_set_gpio_level(Pins::BacklightPwm, value * value);
}

void Display::setDrawColor(const Color color)
{
    u8g2_SetDrawColor(&_p->u8g2, static_cast<int>(color));
}

void Display::setFont(const Font& font)
{
    const auto *const data = font.data();
    if (data != nullptr)
    {
        u8g2_SetFont(&_p->u8g2, data);
    }
}

void Display::setClipRect(const Rect& rect)
{
    u8g2_SetClipWindow(
        &_p->u8g2,
        rect.x(),
        rect.y(),
        rect.x() + rect.width(),
        rect.y() + rect.height()
    );
}

void Display::resetClipRect()
{
    u8g2_SetMaxClipWindow(&_p->u8g2);
}

int Display::calculateFontAscent() const
{
    return u8g2_GetAscent(&_p->u8g2);
}

int Display::calculateFontDescent() const
{
    return u8g2_GetDescent(&_p->u8g2);
}

int Display::calculateMaxCharHeight() const
{
    return u8g2_GetMaxCharHeight(&_p->u8g2);
}

int Display::calculateTextWidth(const std::string& text) const
{
    return u8g2_GetStrWidth(&_p->u8g2, text.c_str());
}

void Display::drawText(const Point &pos, const std::string& s)
{
    u8g2_DrawStr(&_p->u8g2, pos.x(), pos.y(), s.c_str());
}

void Display::drawBitmap(
    const Point& pos,
    const int width,
    const int height,
    const uint8_t* const data
)
{
    u8g2_DrawXBM(&_p->u8g2, pos.x(), pos.y(), width, height, data);
}

void Display::drawRect(const Rect& rect)
{
    u8g2_DrawFrame(&_p->u8g2, rect.x(), rect.y(), rect.width(), rect.height());
}

void Display::drawLine(const Point& from, const Point& to)
{
    u8g2_DrawLine(&_p->u8g2, from.x(), from.y(), to.x(), to.y());
}

void Display::drawLine(
    const uint8_t x1,
    const uint8_t y1,
    const uint8_t x2,
    const uint8_t y2
)
{
    u8g2_DrawLine(&_p->u8g2, x1, y1, x2, y2);
}

void Display::fillRect(const Rect& rect)
{
    u8g2_DrawBox(&_p->u8g2, rect.x(), rect.y(), rect.width(), rect.height());
}

void Display::setup()
{
    printf("%s\r\n", __FUNCTION__);

    uint8_t tileBufHeight;
    uint8_t* buf = nullptr;

    u8g2_SetupDisplay(
        &_p->u8g2,
        u8x8_d_st7586s_erc240160_chunked,
        u8x8_cad_011,
        u8x8_byte_hw_spi0_pico,
        u8x8_gpio_and_delay_pico
    );

    printf("u8g2_SetupDisplay OK\r\n");

    buf = u8g2_m_30_20_f(&tileBufHeight);

    u8g2_SetupBuffer(
        &_p->u8g2,
        buf,
        tileBufHeight,
        u8g2_ll_hvline_horizontal_right_lsb,
        U8G2_R0
    );

    printf("u8g2_SetupBuffer OK\r\n");

    // Setup the backlight control PWM pin
    gpio_set_function(Pins::BacklightPwm, GPIO_FUNC_PWM);
    const auto blPwmPinSlice = pwm_gpio_to_slice_num(Pins::BacklightPwm);
    auto blPwmConfig = pwm_get_default_config();
    pwm_config_set_clkdiv(&blPwmConfig, 4.f);
    pwm_config_set_wrap(&blPwmConfig, 255 * 255);
    pwm_init(blPwmPinSlice, &blPwmConfig, true);
    // setBacklightLevel(60);

    u8g2_InitDisplay(&_p->u8g2);
    u8g2_SetPowerSave(&_p->u8g2, 0);
    u8g2_SetContrast(&_p->u8g2, 60);

    printf("%s OK\r\n", __FUNCTION__);
}

}
