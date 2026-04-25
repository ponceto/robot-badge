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
// Wiring
// ---------------------------------------------------------------------------

namespace {

struct Wiring
{
    static constexpr uint8_t LED0 = D3;
    static constexpr uint8_t LED1 = D4;
    static constexpr uint8_t LED2 = D5;
    static constexpr uint8_t LED3 = D6;
    static constexpr uint8_t LED4 = D7;
    static constexpr uint8_t LED5 = D8;
    static constexpr uint8_t LED6 = LED_BLUE;
    static constexpr uint8_t LED7 = LED_GREEN;
    static constexpr uint8_t LED8 = LED_RED;
};

}

// ---------------------------------------------------------------------------
// RobotBadge
// ---------------------------------------------------------------------------

auto RobotBadge::setup() -> void
{
    ::pinMode(Wiring::LED0, OUTPUT);
    ::pinMode(Wiring::LED1, OUTPUT);
    ::pinMode(Wiring::LED2, OUTPUT);
    ::pinMode(Wiring::LED3, OUTPUT);
    ::pinMode(Wiring::LED4, OUTPUT);
    ::pinMode(Wiring::LED5, OUTPUT);
    ::pinMode(Wiring::LED6, OUTPUT);
    ::pinMode(Wiring::LED7, OUTPUT);
    ::pinMode(Wiring::LED8, OUTPUT);
}

auto RobotBadge::loop() -> void
{
    const     auto index = _index++;
    constexpr auto count = countof(g_patterns);
    constexpr auto debug = true;

    if(index < count) {
        const uint16_t pattern = g_patterns[index];
        const uint8_t leds[9] = {
            static_cast<uint8_t>((pattern >> 0) & 1),
            static_cast<uint8_t>((pattern >> 1) & 1),
            static_cast<uint8_t>((pattern >> 2) & 1),
            static_cast<uint8_t>((pattern >> 3) & 1),
            static_cast<uint8_t>((pattern >> 4) & 1),
            static_cast<uint8_t>((pattern >> 5) & 1),
            static_cast<uint8_t>((pattern >> 6) & 1),
            static_cast<uint8_t>((pattern >> 7) & 1),
            static_cast<uint8_t>((pattern >> 8) & 1),
        };
        if(debug != false) {
            const char buffer[] = {
                (leds[8] ? 'X' : '-'),
                (leds[7] ? 'X' : '-'),
                (leds[6] ? 'X' : '-'),
                (leds[5] ? 'X' : '-'),
                (leds[4] ? 'X' : '-'),
                (leds[3] ? 'X' : '-'),
                (leds[2] ? 'X' : '-'),
                (leds[1] ? 'X' : '-'),
                (leds[0] ? 'X' : '-'),
                '\0'
            };
            Serial.println(buffer);
        }
        ::digitalWrite(Wiring::LED0, (leds[0] != 0 ? HIGH : LOW));
        ::digitalWrite(Wiring::LED1, (leds[1] != 0 ? HIGH : LOW));
        ::digitalWrite(Wiring::LED2, (leds[2] != 0 ? HIGH : LOW));
        ::digitalWrite(Wiring::LED3, (leds[3] != 0 ? HIGH : LOW));
        ::digitalWrite(Wiring::LED4, (leds[4] != 0 ? HIGH : LOW));
        ::digitalWrite(Wiring::LED5, (leds[5] != 0 ? HIGH : LOW));
        ::digitalWrite(Wiring::LED6, (leds[6] != 0 ? LOW : HIGH));
        ::digitalWrite(Wiring::LED7, (leds[7] != 0 ? LOW : HIGH));
        ::digitalWrite(Wiring::LED8, (leds[8] != 0 ? LOW : HIGH));
        ::delay(_delay);
    }
    else {
        _index = 0;
    }
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
