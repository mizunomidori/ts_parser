#pragma once

#include <cinttypes>
#include <memory>
#include "TS_packet.h"

class TransportStream
{
public:
	TransportStream();
	~TransportStream() = default;

	template<typename String>
	auto open(const String file_path);

	int check_TS_unit_size();
	bool check_continuity();

	bool parse_payload(TSPacket &tsp);

	template <typename String>
	bool parse_stream(const String filepath);
	template <typename String>
	bool select_stream(const String filepath, const uint16_t PID);

private:
	std::ifstream input;

	int8_t   last_continuity_counter;
	uint16_t last_PID;
	bool     duplicate_flag;
	uint32_t drop_count;

	TSPHeader header;
	AdaptationField adapt;

	std::unique_ptr<uint8_t[]> buffer;
	uint8_t unit_size;
	uint8_t offset;

	// Table buffer
	std::vector<uint8_t> section_buffer;
	uint16_t entire_section_length;
	std::vector<std::vector<uint8_t>> section_list;

	uint16_t last_PSI_PID;
};
