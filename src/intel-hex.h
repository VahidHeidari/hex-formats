#ifndef INTEL_HEX_H_
#define INTEL_HEX_H_

#include <cstdint>
#include <fstream>
#include <string>

class IntelHex
{
public:
	static constexpr char START_CODE = ':';

	enum RecordType : uint8_t
	{
		RECORD_TYPE_DATA						= 0x00,
		RECORD_TYPE_END_OF_FILE					= 0x01,
		RECORD_TYPE_EXTENDED_SEGMENT_ADDRESS	= 0x02,
		RECORD_TYPE_START_SEGMENT_ADDRESS		= 0x03,
		RECORD_TYPE_EXTENDED_LINEAR_ADDRESS		= 0x04,
		RECORD_TYPE_START_LINEAR_ADDRESS		= 0x05
	};

	enum Format
	{
		FORMAT_I8HEX,			// Files use only record types 00 and 01 (16 bit address)
		FORMAT_I16HEX,			// Files use only record types 00 through 03 (20 bit address)
		FORMAT_I32HEX			// Files use only record types 00, 01, 04, and 05 (32 bit addresses)
	};

	IntelHex();
	IntelHex(uint16_t addr);
	IntelHex(std::string path);

	bool open(std::string path);
	bool close();
	size_t write(const uint8_t* data, size_t length);
	size_t write_record(const uint8_t* data);
	size_t write_record(const uint8_t* data, uint8_t length);
	void write_end_of_file();
	bool check_integrity(std::string path);

	void set_record_bytes(uint8_t bytes) { record_bytes = bytes; }
	uint8_t get_record_bytes() const { return record_bytes; }
	void set_address(uint16_t addr) { address = addr; }
	uint16_t get_address() const { return address; }
	void set_record_type(RecordType type) { record_type = type; }
	RecordType get_record_type() const { return record_type; }

private:
	void write_record_header();
	void write_check_sum();

	uint8_t record_bytes;
	uint8_t byte_count;
	uint16_t address;
	RecordType record_type;
	uint8_t check_sum;
	std::ofstream out_file;
};

#endif

