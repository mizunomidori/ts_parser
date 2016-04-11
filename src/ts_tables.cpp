/*
 * Reference: ITU-T Rec. H.222.0 (05/2006)
 *            ARIB STD-B10 v5.7
 */

#include "ts_tables.h"
#include "ts_descriptors.h"
#include "crc32.h"
#include "ts_common_utils.h"

template <typename Size>
auto check_CRC(const uint8_t* table_beg_ptr, const Size table_length) -> bool
{
	const auto CRC_32 =
		read_bits<uint32_t>(&table_beg_ptr[table_length - 4], 0, 32);
	if (CRC_32 !=
		crc::_crc32(table_beg_ptr, table_length - crc::CRC32_SIZE)) {
		fprintf(stderr, "CRC error.\n");
		return false;
	}
	return true;
}

/* ITU-T Rec. H.222.0 */
bool ProgramAssociationSection::parse(const uint8_t* p, uint16_t* read_length)
{
	table_id = p[0];
	if (table_id != 0x00) {
		return false;
	}
	section_syntax_indicator = (p[1] & 0x80) >> 7;
	if (section_syntax_indicator != 1) {
		return false;
	}
	section_length = (p[1] & 0x0f) << 8 | p[2];
	if (section_length > 0x3FD) {
		return false;
	}
	if (read_length)
		*read_length = 3 + section_length;

	if (!check_CRC(p, 3 + section_length)) {
		return false;
	}

	transport_stream_id    = p[3] << 8 | p[4];
	version_number         = (p[5] & 0x3e) >> 1;
	current_next_indicator = p[5] & 0x01; // 1: applicable, 0: not yet applicable
	if (!current_next_indicator) {
		return false;
	}
	section_number         = p[6];
	last_section_number    = p[7];

	p += 8;
	const auto rest_section_length = section_length - (5 + crc::CRC32_SIZE);

	for (auto i = 0; i < rest_section_length; i += 4) {
		// NOTE: The program_number may be used as a designation for a broadcast channel
		const uint16_t program_number_ = (p[i] << 8) | p[i + 1];
		if (program_number_ == 0x00) {
			const uint16_t network_PID_ = (p[i + 2] & 0x1f) << 8 | p[i + 3];
			network_PID.push_back(network_PID_);
		}
		else {
			const uint16_t program_map_PID_ = (p[i + 2] & 0x1f) << 8 | p[i + 3];
			auto duplicate_flag = false;
			for (auto pid : program_map_PID) {
				if (pid == program_map_PID_)
					duplicate_flag = true;
			}
			if (!duplicate_flag) {
				PMT_list.push_back(PMTInfo(program_number_, program_map_PID_));
			}
		}
	}

	return true;
}

bool CASection::parse(const uint8_t* p, uint16_t* read_length)
{
	table_id = p[0];
	if (table_id != 0x01) {
		return false;
	}
	section_syntax_indicator = (p[1] & 0x80) >> 7;
	if (section_syntax_indicator != 1) {
		return false;
	}
	section_length = (p[1] & 0x0f) << 8 | p[2];
	if (section_length > 0x3FD) {
		return false;
	}
	if (read_length)
		*read_length = 3 + section_length;

	if (!check_CRC(p, 3 + section_length)) {
		return false;
	}

	version_number = (p[5] & 0x3e) >> 1;
	current_next_indicator = p[5] & 0x01; // 1: applicable, 0: not yet applicable
	if (!current_next_indicator) {
		return false;
	}
	section_number = p[6];
	last_section_number = p[7];

	const auto descriptor_length = section_length - crc::CRC32_SIZE - 5;
	// descriptor()
	if (descriptor_length) {
		Descriptor desc;
		desc.decode(&p[8], descriptor_length);
	}

	return true;
}

template <typename T>
std::string get_stream_type(const T stream_type)
{
	auto stream_type_ = static_cast<StreamType>(stream_type);
	std::string str;
	switch (stream_type_) {
	case StreamType::STREAM_VIDEO_MPEG1:
		str = ".m1v";
		break;
	case StreamType::STREAM_VIDEO_MPEG2:
	case StreamType::STREAM_VIDEO_MPEG2_A:
	case StreamType::STREAM_VIDEO_MPEG2_B:
	case StreamType::STREAM_VIDEO_MPEG2_C:
	case StreamType::STREAM_VIDEO_MPEG2_D:
		str = ".m2v";
		break;
	case StreamType::STREAM_VIDEO_AVC:
		str = ".avc";
		break;
	case StreamType::STREAM_VIDEO_VC1:
		str = ".vc1";
		break;
	case StreamType::STREAM_AUDIO_MP1:
	case StreamType::STREAM_AUDIO_MP2:
		str = ".mpa";
		break;
	case StreamType::STREAM_AUDIO_AAC:
		str = ".aac";
		break;
	case StreamType::STREAM_AUDIO_LPCM:
		str = ".pcm";
		break;
	case StreamType::STREAM_AUDIO_AC3:
		str = ".ac3";
		break;
	case StreamType::STREAM_AUDIO_DTS:
	case StreamType::STREAM_AUDIO_MLP:
	case StreamType::STREAM_AUDIO_DTS_HD:
	case StreamType::STREAM_AUDIO_DTS_HD_SUB:
	case StreamType::STREAM_AUDIO_DDPLUS:
	case StreamType::STREAM_AUDIO_DDPLUS_SUB:
		str = ".ac3";
		break;
	default:
		str = "unknown";
		break;
	}

	return str;
}

/* ITU-T Rec. H.222.0 */
bool ProgramMapSection::parse(const uint8_t* p, uint16_t* read_length)
{
	table_id = p[0];
	if (table_id != 0x02) {
		return false;
	}
	section_syntax_indicator = (p[1] & 0x80) >> 7;
	if (section_syntax_indicator != 1) {
		return false;
	}
	section_length = (p[1] & 0x0f) << 8 | p[2];
	if (section_length > 0x3FD) {
		return false;
	}
	if (read_length)
		*read_length = 3 + section_length;

	if (!check_CRC(p, 3 + section_length)) {
		return false;
	}

	program_number = p[3] << 8 | p[4];
	version_number = (p[5] & 0x3e) >> 1;
	current_next_indicator = p[5] & 0x01; // 1: applicable, 0: not yet applicable
	if (!current_next_indicator) {
		return false;
	}
	section_number = p[6];
	last_section_number = p[7];

	PCR_PID = (p[8] & 0x1f) << 8 | p[9];
	program_info_length = (p[10] & 0x0f) << 8 | p[11];
	p += 12;

	// descriptor()
	if (program_info_length) {
		Descriptor desc;
		desc.decode(p, program_info_length);
	}

	p += program_info_length;
	const auto rest_section_length = section_length - (9 + program_info_length + crc::CRC32_SIZE);
	for (auto i = 0; i < rest_section_length;) {
		ESInfo info;
		info.stream_type = p[i];
		info.elementary_PID = (p[i + 1] & 0x1f) << 8 | p[i + 2];
		info.ES_info_length = (p[i + 3] & 0x0f) << 8 | p[i + 4];

		ES_list.push_back(info);

		auto pid_type = get_stream_type(info.stream_type);
		//fprintf(stderr, "pid_type: %s\n", pid_type.c_str());
		//fprintf(stderr, "stream_type: %x\n", info.stream_type);
		//fprintf(stderr, "PID: %x\n", info.elementary_PID);

		// descriptor()
		if (info.ES_info_length) {
			Descriptor desc2;
			desc2.decode(&p[i + 5], info.ES_info_length);
		}

		i += 5 + info.ES_info_length;
	}

	return true;
}

/* ITU-T Rec. H.222.0: 2.4.4.10 Syntax of the Private section */
bool PrivateSection::parse(const uint8_t* p, uint16_t* read_length)
{
	table_id                 = p[0];
	section_syntax_indicator = (p[1] & 0x80) >> 7;
	private_section_length   = (p[1] & 0x0f) << 8 | p[2];
	if (private_section_length > 0xFFD) {
		return false;
	}
	if (read_length)
		*read_length = 3 + private_section_length;

	if (section_syntax_indicator == 0) {
		private_data_byte.resize(private_section_length - 3);
		for (auto i = 0; i < private_section_length - 3; ++i) {
			private_data_byte[i] = p[i + 3];
		}
	}
	else {
		table_id_extension     = p[3] << 8 | p[4];
		version_number         = (p[5] & 0x3e) >> 1;
		current_next_indicator = p[5] & 0x01; // 1: applicable, 0: not yet applicable
		if (!current_next_indicator) {
			return false;
		}
		section_number         = p[6];
		last_section_number    = p[7];
		private_data_byte.resize(private_section_length - 8 - 4);
		for (auto i = 0; i < private_section_length - 4; ++i) {
			private_data_byte[i] = p[i + 8];
		}

		if (!check_CRC(p, 3 + private_section_length)) {
			return false;
		}
	}

	return true;
}

/* ITU-T Rec. H.222.0: 2.4.4.12 Syntax of the Transport Stream section */
bool TSDescriptionSection::parse(const uint8_t* p, uint16_t* read_length)
{
	table_id = p[0];
	if (table_id != 0x03) {
		return false;
	}
	section_syntax_indicator = (p[1] & 0x80) >> 7;
	section_length           = (p[1] & 0x0f) << 8 | p[2];
	if (section_length > 0x3FD) {
		return false;
	}
	if (read_length)
		*read_length = 3 + section_length;

	if (!check_CRC(p, 3 + section_length)) {
		return false;
	}

	version_number         = (p[5] & 0x3e) >> 1;
	current_next_indicator = p[5] & 0x01; // 1: applicable, 0: not yet applicable
	if (!current_next_indicator) {
		return false;
	}
	section_number         = p[6];
	last_section_number    = p[7];

	return true;
}

/* ARIB STD-B10v5_7 */
bool NetworkInformationSection::parse(const uint8_t* p, uint16_t* read_length)
{
	table_id = p[0];
	if (table_id != 0x40 && table_id != 0x41) {
		return false;
	}
	section_syntax_indicator = (p[1] & 0x80) >> 7;
	if (section_syntax_indicator != 1) {
		return false;
	}
	section_length = (p[1] & 0x0f) << 8 | p[2];
	if (section_length > 0x3fd) {
		return false;
	}
	if (read_length)
		*read_length = 3 + section_length;

	if (!check_CRC(p, 3 + section_length)) {
		return false;
	}

	network_id                 = p[3] << 8 | p[4];
	version_number             = (p[5] & 0x3e) >> 1;
	current_next_indicator     = p[5] & 0x01;
	if (!current_next_indicator) {
		return false;
	}
	section_number             = p[6];
	last_section_number        = p[7];
	network_descriptors_length = (p[8] & 0x0f) << 8 | p[9];

	p += 10;

	// descriptor()
	if (network_descriptors_length) {
		Descriptor desc;
		desc.decode(p, network_descriptors_length);
	}

	p += network_descriptors_length;

	transport_stream_loop_length = (p[0] & 0x0f) << 8 | p[1];
	p += 2;

	for (auto i = 0; i < transport_stream_loop_length;) {
		TSInfo info;
		info.transport_stream_id = p[i] << 8 | p[i + 1];
		info.original_network_id = p[i + 2] << 8 | p[i + 3];
		info.transport_descriptors_length = (p[i + 4] & 0x0f) << 8 | p[i + 5];

		TS_list.push_back(info);

		// descriptor()
		if (info.transport_descriptors_length) {
			Descriptor desc;
			desc.decode(&p[i + 6], info.transport_descriptors_length);
		}

		i += 6 + info.transport_descriptors_length;
	}

	return true;
}

/* ARIB STD-B10v5_7 */
bool BouquetAssociationSection::parse(const uint8_t* p, uint16_t* read_length)
{
	table_id = p[0];
	if (table_id != 0x4A) {
		return false;
	}
	section_syntax_indicator = (p[1] & 0x80) >> 7;
	if (section_syntax_indicator != 1) {
		return false;
	}
	section_length = (p[1] & 0x0f) << 8 | p[2];
	if (section_length > 0x3fd) {
		return false;
	}
	if (read_length)
		*read_length = 3 + section_length;

	if (!check_CRC(p, 3 + section_length)) {
		return false;
	}

	bouquet_id                 = p[3] << 8 | p[4];
	version_number             = (p[5] & 0x3e) >> 1;
	current_next_indicator     = p[5] & 0x01;
	if (!current_next_indicator) {
		return false;
	}
	section_number             = p[6];
	last_section_number        = p[7];
	bouquet_descriptors_length = (p[8] & 0x0f) << 8 | p[9];

	p += 10;

	// descriptor()
	if (bouquet_descriptors_length) {
		Descriptor desc;
		desc.decode(p, bouquet_descriptors_length);
	}

	p += bouquet_descriptors_length;

	transport_stream_loop_length = (p[0] & 0x0f) << 8 | p[1];
	p += 2;

	for (auto i = 0; i < transport_stream_loop_length;) {
		TSInfo info;
		info.transport_stream_id = p[i] << 8 | p[i + 1];
		info.original_network_id = p[i + 2] << 8 | p[i + 3];
		info.transport_descriptors_length =
			(p[i + 4] & 0x0f) << 8 | p[i + 5];

		TS_list.push_back(info);

		// descriptor()
		if (info.transport_descriptors_length) {
			Descriptor desc;
			desc.decode(&p[i + 6], info.transport_descriptors_length);
		}

		i += 6 + info.transport_descriptors_length;
	}

	return true;
}

enum class RunnigStatus
{
	UNDEFINED  = 0,
	NOT_RUNNIG = 1,
	START      = 2,
	STOP       = 3,
	RUNNIG     = 4,
};

/* ARIB STD-B10v5_7 */
bool ServiceDescriptionSection::parse(const uint8_t* p, uint16_t* read_length)
{
	table_id = p[0];
	if (table_id != 0x42 && table_id != 0x46) {
		return false;
	}
	section_syntax_indicator = (p[1] & 0x80) >> 7;
	if (section_syntax_indicator != 1) {
		return false;
	}
	section_length = (p[1] & 0x0f) << 8 | p[2];
	if (section_length > 0x3fd) {
		return false;
	}
	if (read_length)
		*read_length = 3 + section_length;

	if (!check_CRC(p, 3 + section_length)) {
		return false;
	}

	transport_stream_id    = p[3] << 8 | p[4];
	version_number         = (p[5] & 0x3e) >> 1;
	current_next_indicator = p[5] & 0x01;
	if (!current_next_indicator) {
		return false;
	}
	section_number         = p[6];
	last_section_number    = p[7];
	original_network_id    = (p[8] & 0x0f) << 8 | p[9];

	p += 11;
	const auto rest_section_length = section_length - (8 + crc::CRC32_SIZE);
	for (auto i = 0; i < rest_section_length;) {
		ServiceInfo info;
		info.service_id                 =  p[i] << 8 | p[i + 1];
		info.EIT_user_defined_flags     = (p[i + 2] & 0x1c) >> 2;
		info.EIT_schedule_flag          = (p[i + 2] & 0x02) >> 1;
		info.EIT_present_following_flag =  p[i + 2] & 0x01;
		info.running_status             = (p[i + 3] & 0xe0) >> 5;
		info.free_CA_mode               = (p[i + 3] & 0x10) >> 4;
		info.descriptors_loop_length    = (p[i + 3] & 0x0f) << 8 | p[i + 4];

		service_info_list.push_back(info);

		// descriptor
		Descriptor desc;
		desc.decode(&p[i + 5], info.descriptors_loop_length);

		i += 5 + info.descriptors_loop_length;
	}

	return true;
}

/* ARIB STD-B10v5_7 */
bool EventInformationSection::parse(const uint8_t* p, uint16_t* read_length)
{
	table_id = p[0];
	if (table_id != 0x4E && table_id != 0x4F && // p/f
		!(0x50 <= table_id && table_id <= 0x6f)) { // schedule
		return false;
	}
	section_syntax_indicator = (p[1] & 0x80) >> 7;
	if (section_syntax_indicator != 1) {
		return false;
	}
	section_length = (p[1] & 0x0f) << 8 | p[2];
	if (section_length < 7 || section_length > 0xffd) {
		return false;
	}
	if (read_length)
		*read_length = 3 + section_length;

	if (!check_CRC(p, 3 + section_length)) {
		return false;
	}

	service_id                  = p[3] << 8 | p[4];
	version_number              = (p[5] & 0x3e) >> 1;
	current_next_indicator      = p[5] & 0x01;
	if (!current_next_indicator) {
		return false;
	}
	section_number              = p[6];
	last_section_number         = p[7];
	transport_stream_id         = p[8] << 8 | p[9];
	original_network_id         = p[10] << 8 | p[11];
	segment_last_section_number = p[12];
	last_table_id               = p[13];

	p += 14;
	const auto rest_section_length = section_length - (11 + crc::CRC32_SIZE);
	for (auto i = 0; i < rest_section_length;) {
		EventInfo info;
		info.event_id   = read_bits<uint16_t>(&p[i], 0, 16);
		info.start_time = read_bits<int64_t> (&p[i + 2], 0, 40);
		info.duration   = read_bits<uint32_t>(&p[i + 7], 0, 24);
		if (info.duration == 0xFFFFFF) {
			//fprintf(stderr, "duration: undef.\n");
		}
		info.running_status = read_bits<uint8_t>(&p[i + 10], 0, 3);
		info.free_CA_mode   = read_bits<int8_t>(&p[i + 10], 3, 1);
		info.descriptors_loop_length = read_bits<uint16_t>(&p[i + 10], 4, 12);

		event_info_list.push_back(info);

		//const auto mjd = MJD_to_JTC(read_bits<uint16_t>(&p[i + 2], 0, 16));
		//const auto jtc = BCD_to_dec<uint32_t>(&p[i + 4], 6);
		//const auto dur = BCD_to_dec<uint32_t>(reinterpret_cast<uint8_t*>(&info.duration), 6);
		//fprintf(stderr, "start: %08d %08d, duration: %06d\n", mjd, jtc, dur);

		// descriptor
		Descriptor desc;
		desc.decode(&p[i + 12], info.descriptors_loop_length);

		i += 12 + info.descriptors_loop_length;
	}

	return true;
}

/* ARIB STD-B10v5_7 */
bool TimeDateSection::parse(const uint8_t* p, uint16_t* read_length)
{
	table_id = p[0];
	if (table_id != 0x70) {
		return false;
	}
	section_syntax_indicator = (p[1] & 0x80) >> 7;
	if (section_syntax_indicator != 0) {
		return false;
	}
	section_length = (p[1] & 0x0f) << 8 | p[2];
	if (section_length != 0x005) {
		return false;
	}

	int16_t MJD = p[3] << 8 | p[4];
	JST_time = MJD_to_JTC(MJD);
	JST_time.tm_hour = BCD_to_dec<int>(&p[5], 2);
	JST_time.tm_min  = BCD_to_dec<int>(&p[6], 2);
	JST_time.tm_sec  = BCD_to_dec<int>(&p[7], 2);
	//fprintf(stderr, "TDT: [%d.%02d.%02d %02d:%02d.%02d]\n", JST_time.tm_year, JST_time.tm_mon, JST_time.tm_mday, JST_time.tm_hour, JST_time.tm_min, JST_time.tm_sec);

	if (read_length)
		*read_length = 3 + section_length;
	return true;
}

/* ARIB STD-B10v5_7 */
bool TimeOffsetSection::parse(const uint8_t* p, uint16_t* read_length)
{
	table_id = p[0];
	if (table_id != 0x73) {
		return false;
	}
	section_syntax_indicator = (p[1] & 0x80) >> 7;
	if (section_syntax_indicator != 0) {
		return false;
	}
	section_length = (p[1] & 0x0f) << 8 | p[2];
	if (section_length > 0xffd) {
		return false;
	}
	if (read_length)
		*read_length = 3 + section_length;

	if (!check_CRC(p, 3 + section_length)) {
		return false;
	}

	//int64_t JST_time = read_bits<int64_t>(&p[3], 0, 40);

	const uint16_t MJD = p[3] << 8 | p[4];
	JST_time = MJD_to_JTC(MJD);
	JST_time.tm_hour = BCD_to_dec<int>(&p[5], 2);
	JST_time.tm_min  = BCD_to_dec<int>(&p[6], 2);
	JST_time.tm_sec  = BCD_to_dec<int>(&p[7], 2);
	//fprintf(stderr, "TOT: [%d.%02d.%02d %02d:%02d.%02d]\n", JST_time.tm_year, JST_time.tm_mon, JST_time.tm_mday, JST_time.tm_hour, JST_time.tm_min, JST_time.tm_sec);

	descriptors_loop_length = (p[8] & 0x0f) << 8 | p[9];

	// descriptor()
	if (descriptors_loop_length) {
		Descriptor desc;
		desc.decode(&p[10], descriptors_loop_length);
	}

	return true;
}

/* ARIB STD-B10v5 */
bool BroadcasterInformationSection::parse(const uint8_t* p, uint16_t* read_length)
{
	table_id = p[0];
	if (table_id != 0xc4) {
		return false;
	}
	section_syntax_indicator = (p[1] & 0x80) >> 7;
	if (section_syntax_indicator != 1) {
		return false;
	}
	section_length = (p[1] & 0x0f) << 8 | p[2];
	if (section_length > 0xffd) {
		return false;
	}
	if (read_length)
		*read_length = 3 + section_length;

	if (!check_CRC(p, 3 + section_length)) {
		return false;
	}

	original_network_id      = p[3] << 8 | p[4];
	version_number           = (p[5] & 0x3e) >> 1;
	current_next_indicator   = p[5] & 0x01;
	if (!current_next_indicator) {
		return false;
	}
	section_number           = p[6];
	last_section_number      = p[7];
	broadcast_view_propriety = (p[8] & 0x10) >> 4;
	first_descriptors_length = (p[8] & 0x0f) << 8 | p[9];

	p += 3 + 7;

	// descriptor()
	if (first_descriptors_length) {
		Descriptor desc;
		desc.decode(p, first_descriptors_length);
	}

	p += first_descriptors_length;

	const auto loop_length = section_length - 7 - first_descriptors_length - crc::CRC32_SIZE;
	for (auto i = 0; i < loop_length;) {
		BroadcasterInfo info;
		info.broadcaster_id = p[i];
		info.broadcaster_descriptors_length = (p[i + 1] & 0x0f) << 8 | p[i + 2];

		broadcaster_info_list.push_back(info);

		// descriptor()
		if (info.broadcaster_descriptors_length) {
			Descriptor desc;
			desc.decode(&p[i + 3], info.broadcaster_descriptors_length);
		}

		i += 3 + info.broadcaster_descriptors_length;
	}

	return true;
}

bool PsiTable::decode(const uint8_t* data, const uint16_t data_length)
{
	if (!data || !data_length)
		return false;

	uint16_t decoded_length = 0;

	do {
		TableData table;
		uint16_t curr_read_length;
		bool success;

		const auto& table_id = data[decoded_length];
		//fprintf(stderr, "table_id: %02x\n", table_id);

		switch (table_id) {
		case 0x00:
			// Program Association Table
			table.PAT = std::make_unique<ProgramAssociationSection>();
			success = table.PAT->parse(data + decoded_length, &curr_read_length);
			break;
		case 0x01:
			// Conditional Access Table
			table.CAT = std::make_unique<CASection>();
			success = table.CAT->parse(data + decoded_length, &curr_read_length);
			break;
		case 0x02:
			// Program Map Table
			table.PMT = std::make_unique<ProgramMapSection>();
			success = table.PMT->parse(data + decoded_length, &curr_read_length);
			break;
		case 0x03:
			// TS Description Table
			table.TSDT = std::make_unique<TSDescriptionSection>();
			success = table.TSDT->parse(data + decoded_length, &curr_read_length);
			break;
		case 0x40:
		case 0x41:
			// Network Information Table
			table.NIT = std::make_unique<NetworkInformationSection>();
			success = table.NIT->parse(data + decoded_length, &curr_read_length);
			break;
		case 0x42:
		case 0x46:
			// Service Description Table
			table.SDT = std::make_unique<ServiceDescriptionSection>();
			success = table.SDT->parse(data + decoded_length, &curr_read_length);
			break;
		case 0x4A:
			// Bouquet Association Table
			table.BAT = std::make_unique<BouquetAssociationSection>();
			success = table.BAT->parse(data + decoded_length, &curr_read_length);
			break;
		case 0x70:
			// Time Date Table
			table.TDT = std::make_unique<TimeDateSection>();
			success = table.TDT->parse(data + decoded_length, &curr_read_length);
			break;
		case 0x73:
			// Time Offset Table
			table.TOT = std::make_unique<TimeOffsetSection>();
			success = table.TOT->parse(data + decoded_length, &curr_read_length);
			break;
		case 0xC4:
			// Broadcaster Information Table
			table.BIT = std::make_unique<BroadcasterInformationSection>();
			success = table.BIT->parse(data + decoded_length, &curr_read_length);
			break;
		case 0xFF:
			// Forbidden
			success = false;
			curr_read_length = data_length;
			break;
		default:
			if (0x4E <= table_id && table_id <= 0x6F) {
				// Event Information Table
				table.EIT = std::make_unique<EventInformationSection>();
				success = table.EIT->parse(data + decoded_length, &curr_read_length);
			}
			else {
				//fprintf(stderr, "Unknown table [table_id: %x]\n", table_id);
				return true;
			}
			break;
		}

		if (curr_read_length != data_length) {
			printf("Mismatch table length: %d, %d\n", curr_read_length, data_length);
		}
		if (success) {
			table_list.push_back(std::move(table));
			table_id_list.push_back(table_id);
		}
		else {
			return false;
		}

		decoded_length += curr_read_length;

	} while (decoded_length < data_length);

	return true;
}

