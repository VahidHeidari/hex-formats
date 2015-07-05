/**
 * Hex format reader and writer.
 *
 * Copyright (C) 2015  Vahid Heidari (DeltaCode)
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MOTOROLA_HEX_H_
#define MOTOROLA_HEX_H_

#include <cstdint>

class MotorolaHex
{
public:
	constexpr uint8_t START_CODE = 'S';

	enum RecordType : uint8_t
	{
		S0 = '0',		// Header			16 bit address
		S1 = '1',		// Data				16 bit address
		S2 = '2',		// Data				24 bit address
		S3 = '3',		// Data				32 bit address

		S4 = '4',		// Reserved

		S5 = '5',		// Count			16 bit address
		S6 = '6',		// Count			24 bit address

		S7 = '7',		// Start Address	32 bit address
		S8 = '8',		// Start Address	24 bit address
		S9 = '9'		// Start Address	16 bit address
	};

private:
	uint8_t type;
	uint16_t byte_count;
	uint32_t address;
	uint8_t check_sum;
};

#endif

