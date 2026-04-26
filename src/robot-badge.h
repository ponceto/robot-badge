/*
 * robot-badge.h - Copyright (c) 2025-2026 - Olivier Poncet
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
#ifndef __ROBOT_BADGE_H__
#define __ROBOT_BADGE_H__

#include <bluefruit.h>

// ---------------------------------------------------------------------------
// RobotBadgeWiring
// ---------------------------------------------------------------------------

struct RobotBadgeWiring
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

// ---------------------------------------------------------------------------
// RobotBadge
// ---------------------------------------------------------------------------

class RobotBadge final
    : public RobotBadgeWiring
{
public: // public interface
    RobotBadge() = default;

    RobotBadge(RobotBadge&&) = delete;

    RobotBadge(const RobotBadge&) = delete;

    RobotBadge operator=(RobotBadge&&) = delete;

    RobotBadge operator=(const RobotBadge&) = delete;

   ~RobotBadge() = default;

    auto setup() -> void;

    auto loop() -> void;

    auto switch_mode() -> void;

    auto accelerate() -> void;

    auto decelerate() -> void;

private: // private data
    uint32_t _ptime = 0u;
    uint32_t _ctime = 0u;
    uint32_t _state = 0u;
    uint32_t _delay = 200u;
    uint16_t _index = 0u;
    uint8_t  _mode  = 0u;
};

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------

#endif /* __ROBOT_BADGE_H__ */
