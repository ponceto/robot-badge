/*
 * robot-badge.cpp - Copyright (c) 2025-2026 - Olivier Poncet
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <Arduino.h>
#include "robot-badge.h"

#ifndef countof
#define countof(array) (sizeof(array) / sizeof(array[0]))
#endif

// ---------------------------------------------------------------------------
// globals
// ---------------------------------------------------------------------------

namespace {

constexpr auto make_pattern(const uint16_t rgb, const uint16_t leds) -> uint16_t
{
    return (((rgb & 0b111) << 6) | ((leds & 0b111111) << 0));
}

const uint16_t g_patterns[] = {
    /* from left to right */
    make_pattern(0b000, 0b000000),
    make_pattern(0b000, 0b100000),
    make_pattern(0b000, 0b010000),
    make_pattern(0b000, 0b001000),
    make_pattern(0b000, 0b000100),
    make_pattern(0b000, 0b000010),
    make_pattern(0b000, 0b000001),
    make_pattern(0b000, 0b000000),
    /* from right to left */
    make_pattern(0b000, 0b000000),
    make_pattern(0b000, 0b000001),
    make_pattern(0b000, 0b000010),
    make_pattern(0b000, 0b000100),
    make_pattern(0b000, 0b001000),
    make_pattern(0b000, 0b010000),
    make_pattern(0b000, 0b100000),
    make_pattern(0b000, 0b000000),
    /* left eyebrow */
    make_pattern(0b000, 0b000000),
    make_pattern(0b000, 0b100000),
    make_pattern(0b000, 0b110000),
    make_pattern(0b000, 0b111000),
    make_pattern(0b000, 0b110000),
    make_pattern(0b000, 0b100000),
    make_pattern(0b000, 0b000000),
    /* right eyebrow */
    make_pattern(0b000, 0b000000),
    make_pattern(0b000, 0b000001),
    make_pattern(0b000, 0b000011),
    make_pattern(0b000, 0b000111),
    make_pattern(0b000, 0b000011),
    make_pattern(0b000, 0b000001),
    make_pattern(0b000, 0b000000),
    /* animate eyebrows */
    make_pattern(0b000, 0b000000),
    make_pattern(0b000, 0b100001),
    make_pattern(0b000, 0b110011),
    make_pattern(0b000, 0b111111),
    make_pattern(0b000, 0b011110),
    make_pattern(0b000, 0b001100),
    make_pattern(0b000, 0b000000),
    /* alternate eyebrows */
    make_pattern(0b000, 0b000000),
    make_pattern(0b000, 0b111000),
    make_pattern(0b000, 0b000111),
    make_pattern(0b000, 0b111000),
    make_pattern(0b000, 0b000111),
    make_pattern(0b000, 0b111000),
    make_pattern(0b000, 0b000111),
    make_pattern(0b000, 0b000000),
};

}

// ---------------------------------------------------------------------------
// RobotBadge
// ---------------------------------------------------------------------------

auto RobotBadge::setup() -> void
{
    _ctime = _ptime = ::millis();
    ::pinMode(LED0, OUTPUT);
    ::pinMode(LED1, OUTPUT);
    ::pinMode(LED2, OUTPUT);
    ::pinMode(LED3, OUTPUT);
    ::pinMode(LED4, OUTPUT);
    ::pinMode(LED5, OUTPUT);
    ::pinMode(LED6, OUTPUT);
    ::pinMode(LED7, OUTPUT);
    ::pinMode(LED8, OUTPUT);
}

auto RobotBadge::loop() -> void
{
    constexpr bool     debug    = true;
    constexpr uint32_t duration = 30000u;

    auto update_mode0 = [&]() -> void
    {
        const     auto index = _index++;
        constexpr auto count = countof(g_patterns);

        if(index < count) {
            _state = g_patterns[index];
        }
        else {
            _index = 0;
            _state = 0;
        }
    };

    auto update_mode1 = [&]() -> void
    {
        const uint32_t bit0 = (_state << 16);
        const uint32_t bit3 = (_state << 13);
        const uint32_t msw  = (~(bit0 ^ bit3) & 0x10000);
        const uint32_t lsw  = ((_state >> 1) & 0x0ffff);

        _state = (msw | lsw);
    };

    auto update_mode2 = [&]() -> void
    {
        const uint32_t bit0 = (_state >> 0);
        const uint32_t bit3 = (_state >> 3);
        const uint32_t msw  = (~(bit0 ^ bit3) & 0x00001);
        const uint32_t lsw  = ((_state << 1) & 0xffffe);

        _state = (msw | lsw);
    };

    auto update = [&]() -> void
    {
        _ctime = ::millis();
        if(((_ctime - _ptime) >= duration)) {
            switch_mode();
        }
        switch(_mode) {
            case 0: // patterns
                update_mode0();
                break;
            case 1: // LFSR to the right
                update_mode1();
                break;
            case 2: // LFSR to the left
                update_mode2();
                break;
            default:
                break;
        }
    };

    auto output = [&]() -> void
    {
        const uint32_t state = _state;

        if(debug != false) {
            const char buffer[] = {
                ((state & 0x0100) != 0 ? 'O' : '-'),
                ((state & 0x0080) != 0 ? 'O' : '-'),
                ((state & 0x0040) != 0 ? 'O' : '-'),
                ((state & 0x0020) != 0 ? 'O' : '-'),
                ((state & 0x0010) != 0 ? 'O' : '-'),
                ((state & 0x0008) != 0 ? 'O' : '-'),
                ((state & 0x0004) != 0 ? 'O' : '-'),
                ((state & 0x0002) != 0 ? 'O' : '-'),
                ((state & 0x0001) != 0 ? 'O' : '-'),
                '\0'
            };
            Serial.println(buffer);
        }
        ::digitalWrite(LED0, ((state & 0x0001) != 0 ? HIGH : LOW));
        ::digitalWrite(LED1, ((state & 0x0002) != 0 ? HIGH : LOW));
        ::digitalWrite(LED2, ((state & 0x0004) != 0 ? HIGH : LOW));
        ::digitalWrite(LED3, ((state & 0x0008) != 0 ? HIGH : LOW));
        ::digitalWrite(LED4, ((state & 0x0010) != 0 ? HIGH : LOW));
        ::digitalWrite(LED5, ((state & 0x0020) != 0 ? HIGH : LOW));
        ::digitalWrite(LED6, ((state & 0x0040) != 0 ? LOW : HIGH));
        ::digitalWrite(LED7, ((state & 0x0080) != 0 ? LOW : HIGH));
        ::digitalWrite(LED8, ((state & 0x0100) != 0 ? LOW : HIGH));
        ::delay(_delay);
    };

    update();
    output();
}

auto RobotBadge::switch_mode() -> void
{
    _mode = ((_mode + 1) % 3);
    _ptime = _ctime;
}

auto RobotBadge::accelerate() -> void
{
    if(_delay > 10) {
        _delay -= 10;
    }
}

auto RobotBadge::decelerate() -> void
{
    if(_delay < 1000) {
        _delay += 10;
    }
}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
