/*
 * main.cpp - Copyright (c) 2025-2026 - Olivier Poncet
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

// ---------------------------------------------------------------------------
// Badge
// ---------------------------------------------------------------------------

namespace {

RobotBadge Badge;

}

// ---------------------------------------------------------------------------
// Application
// ---------------------------------------------------------------------------

void setup()
{
    Serial.begin(115200);
    if(!Serial) {
        const auto prev = ::millis();
        do {
            const auto curr = ::millis();
            if(((curr - prev) < 2000)) {
                ::delay(10);
            }
            else {
                break;
            }
        } while(!Serial);
    }
    return Badge.setup();
}

void loop()
{
    while(Serial && Serial.available()) { 
        switch(Serial.read()) {
            case '!':
                enterSerialDfu();
                break;
            case 'm':
                Badge.switch_mode();
                break;
            case '+':
                Badge.accelerate();
                break;
            case '-':
                Badge.decelerate();
                break;
            default:
                break;
        }
    }
    return Badge.loop();
}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
