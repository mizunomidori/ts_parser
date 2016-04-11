#pragma once

#include <vector>
#include <memory>

// PSI: PAT, PMT, NIT, CAT, TSDT, ICIT
// SI: BIT, SDT, EIT, TDT, TOT

struct ProgramAssociationSection
{
	struct PMTInfo {
		uint16_t program_number;
		uint16_t program_map_PID;
		PMTInfo(const uint16_t pn, const uint16_t pid)
			: program_number(pn), program_map_PID(pid) {}
	};
	uint8_t  table_id;
	int8_t   section_syntax_indicator;
	uint16_t section_length;

	uint16_t transport_stream_id;
	uint8_t  version_number;
	int8_t   current_next_indicator;
	uint8_t  section_number;
	uint8_t  last_section_number;

	std::vector<PMTInfo> PMT_list;
	std::vector<uint16_t> network_PID;
	std::vector<uint16_t> program_map_PID;

	ProgramAssociationSection() = default;
	~ProgramAssociationSection() = default;

	bool parse(const uint8_t* p, uint16_t* read_length);
};

struct CASection
{
	uint8_t  table_id;
	int8_t   section_syntax_indicator;
	uint16_t section_length;

	uint8_t  version_number;
	int8_t   current_next_indicator;
	uint8_t  section_number;
	uint8_t  last_section_number;

	CASection() = default;
	~CASection() = default;

	bool parse(const uint8_t* p, uint16_t* read_length);
};

enum class StreamType : uint8_t
{
	/* ISO/IEC 13818-1 */
	STREAM_VIDEO_MPEG1      = 0x01,    // ISO/IEC 11172 Video
	STREAM_VIDEO_MPEG2      = 0x02,    // ITU-T Rec. H.262 | ISO/IEC 13818-2 Video or ISO/IEC 11172-2 constrained parameter video stream
	STREAM_AUDIO_MP1        = 0x03,    // ISO/IEC 11172 Audio
	STREAM_AUDIO_MP2        = 0x04,    // ISO/IEC 13818-3 Audio
	STREAM_PRIVATE_SECTION  = 0x05,    // ITU-T Rec. H.222.0 | ISO/IEC 13818-1 private_sections
	STREAM_PES_PRIVATE_DATA = 0x06,    // ITU-T Rec. H.222.0 | ISO/IEC 13818-1 PES packets containing private data
	STREAM_VIDEO_MPEG2_A    = 0x0A,    // ISO/IEC 13818-6 type A
	STREAM_VIDEO_MPEG2_B    = 0x0B,    // ISO/IEC 13818-6 type B
	STREAM_VIDEO_MPEG2_C    = 0x0C,    // ISO/IEC 13818-6 type C
	STREAM_VIDEO_MPEG2_D    = 0x0D,    // ISO/IEC 13818-6 type D
	STREAM_AUDIO_AAC        = 0x0F,    // ISO/IEC 13818-7 Audio with ADTS transport syntax
                                       // User Private
	STREAM_VIDEO_MP4        = 0x10,    // ISO/IEC 14496-2 Visual
	STREAM_AUDIO_MP4        = 0x11,    // ISO/IEC 14496-3 Audio with the LATM transport syntax as defined in ISO/IEC 14496-3 / AMD 1
	STREAM_VIDEO_AVC        = 0x1B,    // ISO/IEC 14496-10
	STREAM_VIDEO_PRIVATE    = 0x80,    // Private Video or Linear PCM
	STREAM_AUDIO_LPCM       = 0x80,
	STREAM_AUDIO_AC3_DTS    = 0x81,    // AC-3 or DTS
	STREAM_AUDIO_AC3        = 0x81,
	STREAM_AUDIO_DTS        = 0x82,    // DTS
	STREAM_AUDIO_MLP        = 0x83,    // MLP
	STREAM_AUDIO_DTS_HD     = 0x85,    // DTS-HD
	STREAM_AUDIO_DTS_HD_XLL = 0x86,    // DTS-HD with XLL
	STREAM_AUDIO_DDPLUS     = 0x87,    // DD+
	STREAM_AUDIO_DDPLUS_SUB = 0xA1,    // DD+ for secondary audio
	STREAM_AUDIO_DTS_HD_SUB = 0xA2,    // DTS-HD LBR for secondary audio
	STREAM_VIDEO_VC1        = 0xFD,    // VC-1
	STREAM_INVALID          = 0xFF
};

struct ProgramMapSection
{
	struct ESInfo {
		uint8_t  stream_type;
		uint16_t elementary_PID;
		uint16_t ES_info_length;
	};

	uint8_t  table_id;
	int8_t   section_syntax_indicator;
	uint16_t section_length;

	uint16_t program_number;
	uint8_t  version_number;
	int8_t   current_next_indicator;
	uint8_t  section_number;
	uint8_t  last_section_number;

	uint16_t PCR_PID;
	uint16_t program_info_length;

	std::vector<ESInfo>  ES_list;

	ProgramMapSection() = default;
	~ProgramMapSection() = default;

	bool parse(const uint8_t* p, uint16_t* read_length);
};

struct PrivateSection
{
	uint8_t  table_id;
	int8_t   section_syntax_indicator;
	uint16_t private_section_length;
	std::vector<uint8_t> private_data_byte;

	uint16_t table_id_extension;
	uint8_t  version_number;
	int8_t   current_next_indicator;
	uint8_t  section_number;
	uint8_t  last_section_number;

	PrivateSection() = default;
	~PrivateSection() = default;

	bool parse(const uint8_t* p, uint16_t* read_length);
};

struct TSDescriptionSection
{
	uint8_t  table_id;
	int8_t   section_syntax_indicator;
	uint16_t section_length;
	
	uint8_t version_number;
	int8_t  current_next_indicator;
	uint8_t section_number;
	uint8_t last_section_number;

	TSDescriptionSection() = default;
	~TSDescriptionSection() = default;

	bool parse(const uint8_t* p, uint16_t* read_length);
};

struct NetworkInformationSection
{
	struct TSInfo {
		uint16_t transport_stream_id;
		uint16_t original_network_id;
		uint16_t transport_descriptors_length;
	};

	uint8_t  table_id;
	int8_t   section_syntax_indicator;
	uint16_t section_length;

	uint16_t network_id;
	uint8_t  version_number;
	int8_t   current_next_indicator;
	uint8_t  section_number;
	uint8_t  last_section_number;
	uint16_t network_descriptors_length;

	uint16_t transport_stream_loop_length;
	std::vector<TSInfo> TS_list;

	NetworkInformationSection() = default;
	~NetworkInformationSection() = default;

	bool parse(const uint8_t* p, uint16_t* read_length);
};

/* ARIB STD-B10v5_7 */
struct BouquetAssociationSection
{
	struct TSInfo {
		uint16_t transport_stream_id;
		uint16_t original_network_id;
		uint16_t transport_descriptors_length;
	};

	uint8_t  table_id;
	int8_t   section_syntax_indicator;
	uint16_t section_length;

	uint16_t bouquet_id;
	uint8_t  version_number;
	int8_t   current_next_indicator;
	uint8_t  section_number;
	uint8_t  last_section_number;
	uint16_t bouquet_descriptors_length;

	uint16_t transport_stream_loop_length;
	std::vector<TSInfo> TS_list;

	BouquetAssociationSection() = default;
	~BouquetAssociationSection() = default;

	bool parse(const uint8_t* p, uint16_t* read_length);
};

struct ServiceDescriptionSection
{
	struct ServiceInfo {
		uint16_t service_id;
		int8_t   EIT_user_defined_flags;
		int8_t   EIT_schedule_flag;
		int8_t   EIT_present_following_flag;
		uint8_t  running_status;
		uint8_t  free_CA_mode;
		uint16_t descriptors_loop_length;
	};

	uint8_t  table_id;
	int8_t   section_syntax_indicator;
	uint16_t section_length;

	uint16_t transport_stream_id;
	uint8_t  version_number;
	int8_t   current_next_indicator;
	uint8_t  section_number;
	uint8_t  last_section_number;
	uint16_t original_network_id;

	std::vector<ServiceInfo> service_info_list;

	ServiceDescriptionSection() = default;
	~ServiceDescriptionSection() = default;

	bool parse(const uint8_t* p, uint16_t* read_length);
};

/* ARIB STD-B10v5_7 */
struct EventInformationSection
{
	struct EventInfo {
		uint16_t event_id;
		int64_t  start_time;
		uint32_t duration;
		uint8_t  running_status;
		int8_t   free_CA_mode;
		uint16_t descriptors_loop_length;
	};

	uint8_t  table_id;
	int8_t   section_syntax_indicator;
	uint16_t section_length;

	uint16_t service_id;
	uint8_t  version_number;
	int8_t   current_next_indicator;
	uint8_t  section_number;
	uint8_t  last_section_number;
	uint16_t transport_stream_id;
	uint16_t original_network_id;
	uint8_t  segment_last_section_number;
	uint8_t  last_table_id;

	std::vector<EventInfo> event_info_list;

	EventInformationSection() = default;
	~EventInformationSection() = default;

	bool parse(const uint8_t* p, uint16_t* read_length);
};

struct TimeDateSection
{
	uint8_t  table_id;
	int8_t   section_syntax_indicator;
	uint16_t section_length;

	struct tm JST_time;

	TimeDateSection() = default;
	~TimeDateSection() = default;

	bool parse(const uint8_t* p, uint16_t* read_length);
};

/* ARIB STD-B10v5_7 */
struct TimeOffsetSection
{
	uint8_t  table_id;
	int8_t   section_syntax_indicator;
	uint16_t section_length;

	struct tm JST_time;
	uint16_t descriptors_loop_length;

	TimeOffsetSection() = default;
	~TimeOffsetSection() = default;

	bool parse(const uint8_t* p, uint16_t* read_length);
};

/* ARIB STD-B10v5_7 */
struct BroadcasterInformationSection
{
	struct BroadcasterInfo {
		uint8_t  broadcaster_id;
		uint16_t broadcaster_descriptors_length;
	};
	uint8_t  table_id;
	int8_t   section_syntax_indicator;
	uint16_t section_length;

	uint16_t original_network_id;
	uint8_t  version_number;
	int8_t   current_next_indicator;
	uint8_t  section_number;
	uint8_t  last_section_number;

	int8_t   broadcast_view_propriety;
	uint16_t first_descriptors_length;

	std::vector<BroadcasterInfo> broadcaster_info_list;

	BroadcasterInformationSection() = default;
	~BroadcasterInformationSection() = default;

	bool parse(const uint8_t* p, uint16_t* read_length);
};

enum class TableID
{
	PAT            = 0x00,
	CAT            = 0x01,
	PMT            = 0x02,
	TSDT           = 0x03,
	NIT_actual     = 0x40,
	NIT_other      = 0x41,
	SDT_actual     = 0x42,
	SDT_other      = 0x46,
	BAT            = 0x4A,
	EIT_pf_actual  = 0x4E,
	EIT_pf_other   = 0x4F,
	TDT            = 0x70,
	TOT            = 0x73,
	BIT            = 0xC4,
};

struct TableData
{
	std::unique_ptr<ProgramAssociationSection>     PAT;
	std::unique_ptr<CASection>                     CAT;
	std::unique_ptr<ProgramMapSection>             PMT;
	std::unique_ptr<TSDescriptionSection>          TSDT;
	std::unique_ptr<NetworkInformationSection>     NIT;
	std::unique_ptr<BouquetAssociationSection>     BAT;
	std::unique_ptr<ServiceDescriptionSection>     SDT;
	std::unique_ptr<EventInformationSection>       EIT;
	std::unique_ptr<TimeDateSection>               TDT;
	std::unique_ptr<TimeOffsetSection>             TOT;
	std::unique_ptr<BroadcasterInformationSection> BIT;

	TableID id;

	TableData() :
		PAT(nullptr),
		CAT(nullptr),
		PMT(nullptr),
		TSDT(nullptr),
		NIT(nullptr),
		BAT(nullptr),
		SDT(nullptr),
		EIT(nullptr),
		TDT(nullptr),
		TOT(nullptr),
		BIT(nullptr) {}
};

class PsiTable
{
public:
	bool decode(const uint8_t* data, const uint16_t data_length);

	PsiTable() = default;
	virtual ~PsiTable() = default;

private:
	std::vector<uint8_t> table_id_list;
	std::vector<TableData> table_list;
};
