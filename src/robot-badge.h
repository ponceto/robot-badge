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
// RobotBadge
// ---------------------------------------------------------------------------

class RobotBadge final
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

    auto accelerate() -> void;

    auto decelerate() -> void;

private: // private data
    uint32_t _delay = 250;
    uint16_t _index = 0;
};

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------

#endif /* __ROBOT_BADGE_H__ */
