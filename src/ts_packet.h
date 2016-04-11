#pragma once

#include <cinttypes>

constexpr uint16_t TS_PACKET_SIZE     = 188;
constexpr uint16_t TTS_PACKET_SIZE    = 192; // Timestamped TS
constexpr uint16_t FEC_TS_PACKET_SIZE = 204; // Forward Error Correction
constexpr uint16_t TS_SYNC_BYTE       = 0x47;
constexpr uint16_t TS_PID_MAX         = 8192; // 0x2000

struct TSPHeader
{
	int8_t   synchronization_byte;
	int8_t   transport_error_indicator;
	int8_t   payload_unit_start_indicator;
	int8_t   transport_priority;
	uint16_t PID;
	int8_t   transport_scrambling_control;
	int8_t   adaptation_field_control;
	uint8_t  continuity_counter;
};

struct AdaptationField
{
	uint8_t  adaptation_field_length;
	int8_t   discontinuity_indicator;
	int8_t   random_access_indicator;
	int8_t   elementary_stream_priority_indicator;

	int8_t   PCR_flag;
	int8_t   OPCR_flag;
	int8_t   splicing_point_flag;
	int8_t   transport_private_data_flag;
	int8_t   adaptation_field_extension_flag;

	uint64_t program_clock_reference_base;
	uint16_t program_clock_reference_extension;
	uint64_t original_program_clock_reference_base;
	uint16_t original_program_clock_reference_extension;

	uint8_t  splice_countdown;
	uint8_t  transport_private_data_length;
	int8_t   private_data_byte;
	uint8_t  adaptation_field_extension_length;
	int8_t   ltw_flag;
	int8_t   piecewise_flag;
	int8_t   seamless_splice_flag;
	int8_t   ltw_valid_flag;
	uint16_t ltw_offset;
	uint32_t piecewise_rate;
	int8_t   splice_type;
	int64_t  DTS_next_AU;

	int8_t   stuffing_byte;
};

struct PesPacket
{
	enum class StreamIdType : uint8_t {
		program_stream_map       = 0xbc,
		private_stream_1         = 0xbd,
		padding_stream           = 0xbe,
		private_stream_2         = 0xbf,
		audio_13818_3            = 0xc0,
		video_13818_2            = 0xe0,
		ecm_stream               = 0xf0,
		emm_stream               = 0xf1,
		dsmcc_stream             = 0xf2,
		iec_13522_stream         = 0xf3,
		h222_type_a              = 0xf4,
		h222_type_b              = 0xf5,
		h222_type_c              = 0xf6,
		h222_type_d              = 0xf7,
		h222_type_e              = 0xf8,
		ancillary_stream         = 0xf9,
		extended_stream_id       = 0xfd,
		program_stream_directory = 0xff
	};
	StreamIdType stream_id;

	uint16_t PES_packet_length;

	uint8_t PES_scrambling_control;
	uint8_t PES_priority;
	uint8_t data_alignment_indicator;
	uint8_t copyright;
	uint8_t original_or_copy;
	uint8_t PTS_DTS_flags;
	uint8_t ESCR_flag;
	uint8_t es_rate_flag;
	uint8_t DSM_trick_mode_flag;
	uint8_t additional_copy_info_flag;
	uint8_t PES_CRC_flag;
	uint8_t PES_extension_flag;
	uint8_t PES_header_data_length;

	uint64_t PTS;
	uint64_t DTS;
	uint64_t ESCR;
	uint32_t es_rate;

	enum class TMCType : uint8_t {
		fast_forward = 0b000,
		slow_motion = 0b001,
		freeze_frame = 0b010,
		fast_reverse = 0b011,
		slow_reverse = 0b100,
	};
	TMCType trick_mode_control;

	uint8_t field_id;
	uint8_t intra_slice_refresh;
	uint8_t frequency_truncation;
	uint8_t rep_cntrl;

	uint8_t additional_copy_info;

	uint16_t previous_PES_packet_CRC;

	uint8_t  PES_private_data_flag;
	uint8_t  pack_header_field_flag;
	uint8_t  program_packet_sequence_counter_flag;
	uint8_t  P_STD_buffer_flag;
	uint8_t  PES_extension_flag_2;
	uint64_t PES_private_data[2];
	uint8_t  pack_field_length;
	uint8_t  program_packet_sequence_counter;
	uint8_t  mpeg1_mpeg2_identifier;
	uint8_t  original_stuff_length;
	uint8_t  P_STD_buffer_scale;
	uint16_t P_STD_buffer_size;
	uint8_t  PES_extension_field_length;
	uint8_t  stream_id_extension_flag;
	uint8_t  stream_id_extension;
};

class TSPacket
{
public:
	bool parse_TS_packet(TSPHeader* header, AdaptationField* adapt);
	bool parse_adaptation_field(AdaptationField* adapt);
	bool parse_PES_packet(PesPacket* pes);

	auto is_PES() const -> bool;
	auto skip_pointer_field() -> uint8_t;
	auto get_current_byte() const -> uint8_t;
	auto get_current_byte_address() const -> uint8_t*;
	auto get_bit_index() const -> uint8_t;
	auto set_bit_index(const uint8_t _idx);
	auto skip_bit_index(const uint8_t _idx);

	TSPacket(uint8_t* _packet);
	~TSPacket() = default;

	uint8_t* data_byte;
	uint8_t  data_byte_length;

private:
	uint8_t* packet;
	uint8_t  bit_index;
};
