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

#include "intel-hex.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

IntelHex::IntelHex()
: record_bytes(16)
, byte_count(0)
, address(0)
, record_type(RECORD_TYPE_DATA)
, check_sum(0)
{
}

IntelHex::IntelHex(uint16_t addr)
: record_bytes(16)
, byte_count(0)
, address(addr)
, record_type(RECORD_TYPE_DATA)
, check_sum(0)
{
}

IntelHex::IntelHex(string path)
: record_bytes(16)
, byte_count(0)
, address(0)
, record_type(RECORD_TYPE_DATA)
, check_sum(0)
{
	open(path);
}

bool IntelHex::open(std::string path)
{
	if (out_file.is_open())
		return false;		// It is already opend.

	out_file.open(path);
	return out_file.is_open();
}

bool IntelHex::close()
{
	if (!out_file.is_open())
		return false;		// It is already closed.

	out_file.close();
	return !out_file.is_open();
}

size_t IntelHex::write(const uint8_t* data, size_t length)
{
	unsigned idx = 0;		// index of data buffer

	while (idx != length)
	{
		// Calculate number of bytes.
		byte_count = (idx + record_bytes < length) ? record_bytes : (length - idx);
		write_record_header();

		check_sum = 0;

		// Write data to file.
		for (int i = 0; i < byte_count; ++i, ++idx)
		{
			out_file << hex << uppercase << setw(2) << setfill('0') << (int)data[idx];
			check_sum += data[idx];
		}

		// Calculate and write check sum to file.
		write_check_sum();
		address += byte_count;
	}

	write_end_of_file();

	return out_file.tellp();
}

size_t IntelHex::write_record(const uint8_t* data)
{
	size_t curr_pos = out_file.tellp();
	check_sum = 0;
	byte_count = record_bytes;
	write_record_header();
	for (unsigned i = 0; i < record_bytes; ++i)
	{
		out_file << hex << uppercase << setw(2) << setfill('0') << (int)data[i];
		check_sum += data[i];
	}
	write_check_sum();
	address += record_bytes;
	size_t end_pos = out_file.tellp();
	return end_pos - curr_pos;
}

size_t IntelHex::write_record(const uint8_t* data, uint8_t length)
{
	set_record_bytes(length);
	return write_record(data);
}

void IntelHex::write_end_of_file()
{
	out_file << ":00000001FF" << endl;
}

bool IntelHex::check_integrity(string path)
{
	ifstream input_file(path);

	if (!input_file.is_open())
		return false;

	int line_number = 0;
	while (!input_file.eof())
	{
		uint8_t check_sum = 0;
		string line;
		getline(input_file, line);
		++line_number;

		for (unsigned i = 1; i < line.length(); i += 2)
		{
			int num;
			stringstream ss;
			string str_num = line.substr(i, 2);

			ss << hex << str_num;
			ss >> num;
			//cout << ss.str() << endl;		// Debug only!
			check_sum += num;
		}

		//cout << "CHECK_SUM:" << hex << (int)check_sum << endl;		// Debug only!
		if (check_sum != 0)
		{
			cout << "File '" << path << "' integrity failed at line : " << line_number << endl;
			return false;
		}

		//cout << line << endl;		// Debug only!
	}

	return true;
}

void IntelHex::write_record_header()
{
	if (byte_count == 0)
		return;

	out_file << START_CODE;
	out_file << hex << uppercase << setw(2) << setfill('0') << (int)byte_count;
	out_file << hex << uppercase << setw(4) << setfill('0') << (int)address;
	out_file << hex << uppercase << setw(2) << setfill('0') << (int)record_type;
}

void IntelHex::write_check_sum()
{
	check_sum += byte_count;
	check_sum += address & 0xFF;
	check_sum += (address >> 8) & 0xFF;
	check_sum += record_type;
	out_file << hex << uppercase << setw(2) << setfill('0') << (int)((~check_sum + 1) & 0xFF) << endl;
}

