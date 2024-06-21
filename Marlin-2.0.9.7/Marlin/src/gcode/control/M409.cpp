/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

// #include "../../inc/MarlinConfigPre.h"
// #include "../../inc/MarlinConfig.h"

#include "../gcode.h"
// #include "../../module/motion.h"
#include "../../core/serial.h"
#include "../../module/planner.h"
// #include "../../module/printcounter.h"
// #include "../../libs/duration_t.h"
// #include "../../lcd/marlinui.h"

/**
 * M409: print a msg
 */
void GcodeSuite::M409() {

  planner.synchronize();
  SERIAL_REP_MSG("PnP Done");
}
