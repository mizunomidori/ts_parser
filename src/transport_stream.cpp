/*
 * Reference: ITU-T Rec. H.222.0 (05/2006)
 */

#include <algorithm>
#include <vector>
#include <fstream>
#include "transport_stream.h"
#include "ts_packet.h"
#include "ts_common_utils.h"
#include "ts_tables.h"

TransportStream::TransportStream() :
	last_continuity_counter(-1),
	last_PID(0x1FFF),
	last_PSI_PID(0x1FFF),
	drop_count(0)
{}

template<typename String>
auto TransportStream::open(const String file_path)
{
	input.open(file_path, std::ios::in | std::ios::binary);
	if (!input.is_open() || input.fail()) {
		throw std::runtime_error("file open failed.");
	}

	unit_size = check_TS_unit_size();
	if (!unit_size) {
		fprintf(stderr, "Unsupported file.\n");
		std::exit(EXIT_FAILURE);
	}

	offset = unit_size == TTS_PACKET_SIZE ? 4 : 0;

	//fprintf(stdout, "unit_size: %d", unit_size);
}

bool TransportStream::check_continuity()
{
	duplicate_flag = false;

	if (header.PID == 0x1FFF) {
		return true;
	}

	if (adapt.discontinuity_indicator == 1) {
		return false;
	}

	if (header.PID == last_PID) {
		if (header.adaptation_field_control == 0b00 ||
			header.adaptation_field_control == 0b10) {
			if (last_continuity_counter == header.continuity_counter) {
				return true;
			}
			else {
				return false;
			}
		}
		else if (header.adaptation_field_control == 0b01
			  || header.adaptation_field_control == 0b11) {
			// continuity_counter is a 4-bit field
			// and wraps around to 0 after its maximum value
			const int8_t lcc_plus_one = (last_continuity_counter + 1) & 0x0f;
			if (lcc_plus_one == header.continuity_counter) {
				return true;
			}
			else if (last_continuity_counter == header.continuity_counter) {
				// duplicate packets
				if (duplicate_flag) {
					duplicate_flag = false;
					return false;
				}
				else {
					duplicate_flag = true;
					return true;
				}
			}
			else {
				fprintf(stderr, "%d, %d\n", last_continuity_counter, header.continuity_counter);
				return false;
			}
		}
	}

	return true;
}

int TransportStream::check_TS_unit_size()
{
	input.seekg(std::ios::beg);

	constexpr auto buf_size = 1 << 12;
	std::unique_ptr<uint8_t[]> buffer(new uint8_t[buf_size]);
	if (!buffer) {
		std::runtime_error("malloc failed.");
	}

	input.read(reinterpret_cast<char *>(buffer.get()), buf_size);
	if (input.eof()) {
		fprintf(stderr, "End-of-file.\n");
		return 0;
	}

	input.seekg(std::ios::beg);

	auto sync_count = [buf_size, &buffer]
		(const size_t unit_size, const ptrdiff_t offset = 0) {
		auto cnt = 0;
		for (auto i = 0; i < buf_size; i += unit_size) {
			if (buffer[i + offset] == 0x47)
				++cnt;
		}
		return cnt;
	};

	const auto ts_cnt = sync_count(TS_PACKET_SIZE);
	const auto tts_cnt = sync_count(TTS_PACKET_SIZE, TTS_PACKET_SIZE - TS_PACKET_SIZE);
	const auto fects_cnt = sync_count(FEC_TS_PACKET_SIZE);

	return (ts_cnt > tts_cnt && ts_cnt > fects_cnt) ? TS_PACKET_SIZE :
		   (tts_cnt > fects_cnt) ? TTS_PACKET_SIZE : FEC_TS_PACKET_SIZE;
}

int resync(std::fstream &fs, uint8_t* buffer)
{
	return 0;
}

bool TransportStream::parse_payload(TSPacket &tsp)
{
	const auto is_pes = tsp.is_PES();
	bool exist_sections = false;

	//printf("PID: %x", header.PID);
	if (header.payload_unit_start_indicator == 1) {
		if (is_pes) {
			// the first byte of a PES packet
			PesPacket pes;
			if (!tsp.parse_PES_packet(&pes)) {
				return false;
			}
		}
		else {
			// the first byte of a PSI section
			//     -> the first byte carries the pointer_field
			// a value of 0x00 in the pointer_field indicates that
			// the section starts immediately after the pointer_field
			const auto pointer_field = tsp.data_byte[0];
			if (pointer_field != 0 && !section_buffer.empty()) {
				// Multiple sections
				if (entire_section_length - section_buffer.size() == pointer_field) {
					section_buffer.insert(
						section_buffer.end(),
						&tsp.data_byte[1],
						&tsp.data_byte[1 + pointer_field]
						);
					section_list.push_back(section_buffer);
					section_buffer.clear();
					exist_sections = true;
				}
				else {
					// invalid
				}
			}

			// section_start_idx indicates the start of a section
			auto section_start_idx = 1 + pointer_field;

			do {
				const auto table_id = tsp.data_byte[section_start_idx];
				if (table_id == 0xFF) // Stuffing
					break;

				entire_section_length = 3 + read_bits<uint16_t>(&tsp.data_byte[section_start_idx + 1], 4, 12);

				section_buffer.assign(&tsp.data_byte[section_start_idx],
					&tsp.data_byte[
						std::min(
							section_start_idx + entire_section_length,
							static_cast<int>(tsp.data_byte_length))
					]);

				if (entire_section_length == section_buffer.size()) {
					section_list.push_back(section_buffer);
					section_buffer.clear();
					exist_sections = true;
				}

				section_start_idx += entire_section_length;

			} while (section_start_idx < tsp.data_byte_length);

			last_PSI_PID = header.PID;
		}
	}
	else {
		// no PES packet
		if (is_pes) {
		}
		else {
			// not the first byte of a PSI section
			//     -> no pointer_field in the payload
			if (!section_buffer.empty() && header.PID == last_PSI_PID) {
				section_buffer.insert(section_buffer.end(), tsp.data_byte,
					&tsp.data_byte[
						std::min(
							entire_section_length - section_buffer.size(),
							static_cast<size_t>(tsp.data_byte_length))
					]);

				if (entire_section_length == section_buffer.size()) {
					section_list.push_back(section_buffer);
					section_buffer.clear();
					exist_sections = true;
				}
			}
		}
	}

	return exist_sections;
}

template <typename String>
bool TransportStream::parse_stream(const String filepath)
{
	open(filepath);

	buffer = std::make_unique<uint8_t[]>(unit_size);
	if (!buffer) return false;

	do {
		input.read(reinterpret_cast<char *>(buffer.get()), unit_size);
		if (input.eof())
			break;

		TSPacket tsp(&buffer[offset]);
		if (!tsp.parse_TS_packet(&header, &adapt)) {
			// TODO: need resync?
			continue;
		}

		if (!check_continuity()) {
			++drop_count;
			fprintf(stderr, "DROP\n");
			continue;
		}

		if (header.PID != 0x1FFF && tsp.data_byte) {
			const auto exist_tables = parse_payload(tsp);
			if (exist_tables) {
				PsiTable table;
				for (auto& section : section_list) {
					table.decode(section.data(), static_cast<uint16_t>(section.size()));
				}
				section_list.clear();
			}
		}

		last_continuity_counter = header.continuity_counter;
		last_PID = header.PID;

	} while (buffer[0] == TS_SYNC_BYTE);

	return true;
}

template <typename String>
bool TransportStream::select_stream(const String filepath, const uint16_t PID)
{
	return true;
}

int main(int argc, char* argv[])
{
	TransportStream ts;
	ts.parse_stream("src/test.ts");
}
