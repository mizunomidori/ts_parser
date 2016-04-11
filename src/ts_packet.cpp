/*
 * Reference: ITU-T Rec. H.222.0 (05/2006)
 *            ARIB STD-B10 v5.7
 */

#include <chrono>
#include "ts_packet.h"

template<typename T>
constexpr T twos_complement(T value)
{
	typedef std::make_unsigned<T>::type U;
	return T(-uintmax_t(U(value)));
}

TSPacket::TSPacket(uint8_t* _packet)
	: packet(_packet), data_byte(nullptr)
{}

auto TSPacket::get_current_byte() const -> uint8_t
{
	return packet[bit_index];
}

auto TSPacket::get_current_byte_address() const ->uint8_t*
{
	return &packet[bit_index];
}

auto TSPacket::get_bit_index() const -> uint8_t
{
	return bit_index;
}

auto TSPacket::set_bit_index(const uint8_t _idx)
{
	bit_index = _idx;
}

auto TSPacket::skip_bit_index(const uint8_t _idx)
{
	bit_index += _idx;
}

// The pointer_field points to the start of the first section
// in the Transport Stream packet
auto TSPacket::skip_pointer_field() -> uint8_t
{
	auto pointer_field = packet[bit_index];
	bit_index += 1 + pointer_field;

	return bit_index;
}

/* ITU-T Rec. H.222.0 */
bool TSPacket::parse_TS_packet(TSPHeader* header, AdaptationField* adapt = nullptr)
{
	if ((header->synchronization_byte = packet[0]) != TS_SYNC_BYTE) {
		fprintf(stderr, "sync_byte not found. [%x]\n", header->synchronization_byte);
		return false;
	}
	header->transport_error_indicator    =  (packet[1] & 0x80) >> 7;
	header->payload_unit_start_indicator =  (packet[1] & 0x40) >> 6;
	header->transport_priority           =  (packet[1] & 0x20) >> 5;
	header->PID                          = ((packet[1] & 0x1f) << 8) | packet[2];
	header->transport_scrambling_control =  (packet[3] & 0x60) >> 6;
	header->adaptation_field_control     =  (packet[3] & 0x30) >> 4;
	header->continuity_counter           =  (packet[3] & 0x0f);

	bit_index = 4;

	if (header->transport_scrambling_control != 0) {
		// scrambled
		return false;
	}

	//fprintf(stdout, "PID: %x\n", h->PID);
	if (header->adaptation_field_control == 0b10 || header->adaptation_field_control == 0b11) {
		if (!parse_adaptation_field(adapt)) {
			fprintf(stderr, "Adaptation field parse-error.\n");
		}
	}
	if (header->adaptation_field_control == 0b01 || header->adaptation_field_control == 0b11) {
		/* payload */
		data_byte = &packet[bit_index];
		data_byte_length = TS_PACKET_SIZE - bit_index;
	}

	return true;
}

inline void print_PCR(const uint64_t PCR_base, const uint16_t PCR_ext)
{
	// PCR_base: 90kHz, PCR_ext: 27kHz
	const auto PCR_sec{ PCR_base / 90'000. };
	const auto h{ static_cast<uint32_t>(PCR_sec / 3600) };
	const auto m{ static_cast<uint32_t>(PCR_sec / 60) % 60 };
	const auto s{ static_cast<uint32_t>(PCR_sec) % 60 };
	const auto ms{ static_cast<uint32_t>((PCR_sec - (uint32_t)(PCR_sec)) * 1000) };

	fprintf(stdout, "PCR: %u:%u:%u.%03u ", h, m, s, ms);
	fprintf(stdout, "%llu+%u\n", PCR_base, PCR_ext);
}

/* ITU-T Rec. H.222.0 */
bool TSPacket::parse_adaptation_field(AdaptationField* adapt)
{
	//uint8_t buf[24] = {
	//	       183, 0b11111111, 0b10101010, 0b10101010, 0b10101010, 0b10101010,
	//	0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b10101010,
	//	0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b10101010,
	//	0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b10101010 };
	//auto p = &buf[0];

	auto p = &packet[bit_index];

	if ((adapt->adaptation_field_length = *(p++)) <= 0) {
		return false;
	}
	//fprintf(stdout, "Adaptation field length: %d\n", adapt->adaptation_field_length);
	const auto tail = p + adapt->adaptation_field_length;
	if (p + 1 > tail) {
		return false;
	}

	adapt->discontinuity_indicator              = (*p & 0x80) >> 7;
	adapt->random_access_indicator              = (*p & 0x40) >> 6;
	adapt->elementary_stream_priority_indicator = (*p & 0x20) >> 5;
	adapt->PCR_flag                             = (*p & 0x10) >> 4;
	adapt->OPCR_flag                            = (*p & 0x08) >> 3;
	adapt->splicing_point_flag                  = (*p & 0x04) >> 2;
	adapt->transport_private_data_flag          = (*p & 0x02) >> 1;
	adapt->adaptation_field_extension_flag      = (*p & 0x01);

	p += 1;

	if (adapt->PCR_flag == 1) {
		if (p + 6 > tail) {
			return false;
		}
		// PCR is a 33 + 9 = 42-bit field
		adapt->program_clock_reference_base = (uint64_t)p[0] << 25
			                                | (uint64_t)p[1] << 17
			                                | (uint64_t)p[2] << 9
			                                | (uint64_t)p[3] << 1
			                                | (uint64_t)(p[4] & 0x80) >> 7;
		//adapt->program_clock_reference_base <<= 9;
		adapt->program_clock_reference_extension = ((p[4] & 0x01) << 8 | p[5]);

		//print_PCR(adapt->program_clock_reference_base, adapt->program_clock_reference_extension);

		p += 6;
	}
	if (adapt->OPCR_flag == 1) {
		if (p + 6 > tail) {
			return false;
		}
		adapt->original_program_clock_reference_base = (uint64_t)p[0] << 25
			                                         | (uint64_t)p[1] << 17
			                                         | (uint64_t)p[2] << 9
			                                         | (uint64_t)p[3] << 1
			                                         | (uint64_t)(p[4] & 0x80) >> 7;
		//adapt->original_program_clock_reference_base <<= 9;
		adapt->original_program_clock_reference_extension = ((p[4] & 0x01) << 8 | p[5]);

		auto OPCR = adapt->original_program_clock_reference_base * 300
			      + adapt->original_program_clock_reference_extension;
		//fprintf(stdout, "OPCR: %lld\n", OPCR);

		p += 6;
	}
	if (adapt->splicing_point_flag == 1) {
		if (p + 1 > tail) {
			return false;
		}
		adapt->splice_countdown = *p;
		p += 1;
	}
	if (adapt->transport_private_data_flag == 1) {
		if (p + 1 > tail) {
			return false;
		}
		adapt->transport_private_data_length = *p;
		p += adapt->transport_private_data_length;
		if (p > tail) {
			return false;
		}
	}
	if (adapt->adaptation_field_extension_flag == 1) {
		if (p + 2 > tail) {
			return false;
		}
		adapt->adaptation_field_extension_length = *(p++);
		adapt->ltw_flag                          = (*p & 0x80) >> 7;
		adapt->piecewise_flag                    = (*p & 0x40) >> 6;
		adapt->seamless_splice_flag              = (*p & 0x20) >> 5;
		p += 1;

		if (adapt->ltw_flag == 1) {
			if (adapt->adaptation_field_extension_length < 2) {
				return false;
			}
			adapt->ltw_valid_flag = (p[0] & 0x80) >> 7;
			adapt->ltw_offset     = (p[0] & 0x3f) << 8 | p[1];
			p += 2;
		}
		if (adapt->piecewise_flag == 1) {
			if (adapt->adaptation_field_extension_length < 3) {
				return false;
			}
			adapt->piecewise_rate = (p[0] & 0x7f) << 16 | (p[1]) << 8 | p[2];
			p += 3;
		}
		if (adapt->seamless_splice_flag == 1) {
			if (adapt->adaptation_field_extension_length < 5) {
				return false;
			}
			adapt->splice_type = (p[0] & 0xf0) >> 4;
			adapt->DTS_next_AU = (p[0] & 0x0e) << 14 | p[1] << 7 | (p[2] & 0xfe) >> 1;
			adapt->DTS_next_AU <<= 15;
			adapt->DTS_next_AU |= (p[3] << 7 | (p[4] & 0xfe) >> 1);
			p += 5;

			//fprintf(stdout, "DTS next AU: %llx\n", adapt->DTS_next_AU);
		}

	}

	bit_index += 1 + adapt->adaptation_field_length;

	return true;
}

auto TSPacket::is_PES() const ->bool
{
	auto p = &packet[bit_index];
	const uint32_t packet_start_code_prefix = p[0] << 16 | p[1] << 8 | p[2];
	if (packet_start_code_prefix == 0x000'001) {
		return true; // PES packet
	}
	else {
		return false; // PSI packet
	}
}

/* ITU-T Rec. H.222.0 */
bool TSPacket::parse_PES_packet(PesPacket* pes)
{
	auto p = &packet[bit_index];

	auto getTimeStamp = [](uint8_t* p) -> uint64_t {
		if ((p[0] & 0x01) != 0x01 ||
			(p[2] & 0x01) != 0x01 ||
			(p[4] & 0x01) != 0x01) {
			fprintf(stderr, "marker_bit must have the value '1'\n");
			exit(EXIT_FAILURE);
		}
		return
			(uint64_t)(p[0] & 0x0e) << 29 |
			(uint64_t) p[1] << 22         |
			(uint64_t)(p[2] & 0xfe) << 14 |
			(uint64_t) p[3] << 7          |
			(uint64_t)(p[4] & 0xfe) >> 1;
	};

	p += 3; // Skip packet_start_code_prefix

	pes->stream_id = static_cast<PesPacket::StreamIdType>(*p);
	p += 1;

	pes->PES_packet_length = p[0] << 8 | p[1];
	// if 0, payload consists of bytes from a video elementary stream
	p += 2;

	if (pes->stream_id != PesPacket::StreamIdType::program_stream_map       &&
		pes->stream_id != PesPacket::StreamIdType::padding_stream           &&
		pes->stream_id != PesPacket::StreamIdType::private_stream_2         &&
		pes->stream_id != PesPacket::StreamIdType::ecm_stream               &&
		pes->stream_id != PesPacket::StreamIdType::emm_stream               &&
		pes->stream_id != PesPacket::StreamIdType::program_stream_directory &&
		pes->stream_id != PesPacket::StreamIdType::dsmcc_stream             &&
		pes->stream_id != PesPacket::StreamIdType::h222_type_e)
	{
		pes->PES_scrambling_control   = (*p & 0x30) >> 4;
		pes->PES_priority             = (*p & 0x08) >> 3;
		pes->data_alignment_indicator = (*p & 0x04) >> 2;
		pes->copyright                = (*p & 0x02) >> 1;
		pes->original_or_copy         = (*p & 0x01);
		p += 1;

		pes->PTS_DTS_flags             = (*p & 0xc0) >> 6;
		pes->ESCR_flag                 = (*p & 0x20) >> 5;
		pes->es_rate_flag              = (*p & 0x10) >> 4;
		pes->DSM_trick_mode_flag       = (*p & 0x08) >> 3;
		pes->additional_copy_info_flag = (*p & 0x04) >> 2;
		pes->PES_CRC_flag              = (*p & 0x02) >> 1;
		pes->PES_extension_flag        = (*p & 0x01);
		p += 1;

		pes->PES_header_data_length = *p;
		p += 1;

		if (pes->PTS_DTS_flags == 0b10) {
			if ((*p & 0xe0) != 0b0010) {
				return false;
			}
			pes->PTS = getTimeStamp(p);
			p += 5;
		}
		if (pes->PTS_DTS_flags == 0b11) {
			if ((*p & 0xe0) != 0b0011) {
				return false;
			}
			pes->PTS = getTimeStamp(p);
			p += 5;
			if ((*p & 0xe0) != 0b0001) {
				return false;
			}
			pes->DTS = getTimeStamp(p);
			p += 5;
		}
		if (pes->ESCR_flag == 1) {
			pes->ESCR = (uint64_t)(p[0] & 0x38) << 27
				      | (uint64_t)(p[0] & 0x03) << 28
				      | (uint64_t) p[1] << 20
				      | (uint64_t)(p[2] & 0xf8) << 12
				      | (uint64_t)(p[2] & 0x03) << 13
				      | (uint64_t) p[3] << 5
				      | (uint64_t)(p[4] & 0xf8) >> 3;
			pes->ESCR <<= 9;
			// extension
			pes->ESCR |= (p[5] & 0x03) << 7 | (p[5] & 0xfe) >> 1;
			p += 6;
		}
		if (pes->es_rate_flag == 1) {
			pes->es_rate = (p[0] & 0xef) << 14 | p[1] << 7 | (p[2] & 0xfe) >> 1;
			p += 3;
		}
		if (pes->DSM_trick_mode_flag == 1) {
			pes->trick_mode_control =
				static_cast<PesPacket::TMCType>((*p & 0xe0) >> 5);
			if (pes->trick_mode_control == PesPacket::TMCType::fast_forward ||
				pes->trick_mode_control == PesPacket::TMCType::fast_reverse) {
				pes->field_id             = (*p & 0x18) >> 3;
				pes->intra_slice_refresh  = (*p & 0x04) >> 2;
				pes->frequency_truncation =  *p & 0x03;
			}
			else if (pes->trick_mode_control == PesPacket::TMCType::slow_motion
				  || pes->trick_mode_control == PesPacket::TMCType::slow_reverse) {
				pes->rep_cntrl = *p & 0x1f;
			}
			else if (pes->trick_mode_control == PesPacket::TMCType::freeze_frame) {
				pes->field_id = (*p & 0x18) >> 3;
			}
			p += 1;
		}
		if (pes->additional_copy_info_flag == 1) {
			pes->additional_copy_info = *p & 0x7f;
			p += 1;
		}
		if (pes->PES_CRC_flag == 1) {
			pes->previous_PES_packet_CRC = (p[0] << 8) | p[1];
			p += 2;
		}
		if (pes->PES_extension_flag == 1) {
		}
	}
	else if (pes->stream_id == PesPacket::StreamIdType::program_stream_map
		  || pes->stream_id == PesPacket::StreamIdType::private_stream_2
		  || pes->stream_id == PesPacket::StreamIdType::ecm_stream
		  || pes->stream_id == PesPacket::StreamIdType::emm_stream
		  || pes->stream_id == PesPacket::StreamIdType::program_stream_directory
		  || pes->stream_id == PesPacket::StreamIdType::dsmcc_stream
		  || pes->stream_id == PesPacket::StreamIdType::h222_type_e)
	{
		pes->PES_private_data_flag                = (*p & 0x80) >> 7;
		pes->pack_header_field_flag               = (*p & 0x40) >> 6;
		pes->program_packet_sequence_counter_flag = (*p & 0x20) >> 5;
		pes->P_STD_buffer_flag                    = (*p & 0x10) >> 4;
		pes->PES_extension_flag_2                 = (*p & 0x01);
		if (pes->PES_private_data_flag == 1) {
			p += 16;
		}
		if (pes->pack_header_field_flag == 1) {
			pes->pack_field_length = *p;
			p += 1;

			p += pes->pack_field_length;
		}
		if (pes->program_packet_sequence_counter_flag == 1) {
			p += 2;
		}
		if (pes->P_STD_buffer_flag == 0x01) {
			p += 2;
		}
		if (pes->PES_extension_flag_2 == 1) {
			pes->PES_extension_field_length = *p & 0x7f;
			pes->stream_id_extension_flag = (*p & 0x80) >> 7;

			p += pes->PES_extension_field_length;
		}
	}

	bit_index += pes->PES_packet_length + 6;

	return true;
}

