#pragma once

#include <cinttypes>
#include <string>
#include <array>

 // G set
static constexpr uint8_t KANJI              = 0x42;
static constexpr uint8_t ASCII              = 0x4A;
static constexpr uint8_t HIRA               = 0x30;
static constexpr uint8_t KANA               = 0x31;
static constexpr uint8_t MOSAIC_A           = 0x32;
static constexpr uint8_t MOSAIC_B           = 0x33;
static constexpr uint8_t MOSAIC_C           = 0x34;
static constexpr uint8_t MOSAIC_D           = 0x35;
static constexpr uint8_t PROPORTIONAL_ASCII = 0x36;
static constexpr uint8_t PROPORTIONAL_HIRA  = 0x37;
static constexpr uint8_t PROPORTIONAL_KANA  = 0x38;
static constexpr uint8_t JIS_X0201_KANA     = 0x49;
static constexpr uint8_t JIS_KANJI_1        = 0x39;
static constexpr uint8_t JIS_KANJI_2        = 0x3A;
static constexpr uint8_t SYMBOL             = 0x3B;

// DRCS
static constexpr uint8_t DRCS_0  = 0x40;
static constexpr uint8_t DRCS_1  = 0x41;
static constexpr uint8_t DRCS_2  = 0x42;
static constexpr uint8_t DRCS_3  = 0x43;
static constexpr uint8_t DRCS_4  = 0x44;
static constexpr uint8_t DRCS_5  = 0x45;
static constexpr uint8_t DRCS_6  = 0x46;
static constexpr uint8_t DRCS_7  = 0x47;
static constexpr uint8_t DRCS_8  = 0x48;
static constexpr uint8_t DRCS_9  = 0x49;
static constexpr uint8_t DRCS_10 = 0x4A;
static constexpr uint8_t DRCS_11 = 0x4B;
static constexpr uint8_t DRCS_12 = 0x4C;
static constexpr uint8_t DRCS_13 = 0x4D;
static constexpr uint8_t DRCS_14 = 0x4E;
static constexpr uint8_t DRCS_15 = 0x4F;
static constexpr uint8_t MACRO   = 0x70;

// C0 control code
static constexpr uint8_t C0_NUL   = 0x00;
static constexpr uint8_t C0_BEL   = 0x07;
static constexpr uint8_t C0_APB   = 0x08;
static constexpr uint8_t C0_APF   = 0x09;
static constexpr uint8_t C0_APD   = 0x0A;
static constexpr uint8_t C0_APU   = 0x0B;
static constexpr uint8_t C0_CS    = 0x0C;
static constexpr uint8_t C0_APR   = 0x0D;
static constexpr uint8_t C0_LS1   = 0x0E; // Locking shift 0
static constexpr uint8_t C0_LS0   = 0x0F; // Locking shift 1
static constexpr uint8_t C0_PAPF  = 0x16;
static constexpr uint8_t C0_CAN   = 0x18;
static constexpr uint8_t C0_SS2   = 0x19; // Single shift 2
static constexpr uint8_t C0_ESC   = 0x1B; // Escape
static constexpr uint8_t C0_APS   = 0x1C;
static constexpr uint8_t C0_SS3   = 0x1D; // Single shift 3
static constexpr uint8_t C0_RS    = 0x1E;
static constexpr uint8_t C0_US    = 0x1F;

static constexpr uint8_t SP  = 0x20; // Space
static constexpr uint8_t DEL = 0x7F; // Delete

// C1 control code
static constexpr uint8_t C1_BKF   = 0x80;
static constexpr uint8_t C1_RDF   = 0x81;
static constexpr uint8_t C1_GRF   = 0x82;
static constexpr uint8_t C1_YLF   = 0x83;
static constexpr uint8_t C1_BLF   = 0x84;
static constexpr uint8_t C1_MGF   = 0x85;
static constexpr uint8_t C1_CNF   = 0x86;
static constexpr uint8_t C1_WHF   = 0x87;
static constexpr uint8_t C1_SSZ   = 0x88;
static constexpr uint8_t C1_MSZ   = 0x89;
static constexpr uint8_t C1_NSZ   = 0x8A;
static constexpr uint8_t C1_SZX   = 0x8B;
static constexpr uint8_t C1_COL   = 0x90;
static constexpr uint8_t C1_FLC   = 0x91;
static constexpr uint8_t C1_CDC   = 0x92;
static constexpr uint8_t C1_POL   = 0x93;
static constexpr uint8_t C1_WMM   = 0x94;
static constexpr uint8_t C1_MACRO = 0x95;
static constexpr uint8_t C1_HLC   = 0x97;
static constexpr uint8_t C1_RPC   = 0x98;
static constexpr uint8_t C1_SPL   = 0x99;
static constexpr uint8_t C1_STL   = 0x9A;
static constexpr uint8_t C1_CSI   = 0x9B;
static constexpr uint8_t C1_TIME  = 0x9D;

static constexpr std::array<uint8_t, 16> DEFAULT_MACRO_CODE0 = {
	C0_ESC, 0x24, KANJI, C0_ESC, 0x29, ASCII, C0_ESC, 0x2A, HIRA, C0_ESC, 0x2B, 0x20, MACRO, C0_LS0, C0_ESC, 0x7D
};
static constexpr std::array<uint8_t, 16> DEFAULT_MACRO_CODE1 = {
	C0_ESC, 0x24, KANJI, C0_ESC, 0x29, KANA, C0_ESC, 0x2A, HIRA, C0_ESC, 0x2B, 0x20, MACRO, C0_LS0, C0_ESC, 0x7D
};
static constexpr std::array<uint8_t, 17> DEFAULT_MACRO_CODE2 = {
	C0_ESC, 0x24, KANJI, C0_ESC, 0x29, 0x20, DRCS_1, C0_ESC, 0x2A, HIRA, C0_ESC, 0x2B, 0x20, MACRO, C0_LS0, C0_ESC, 0x7D
};
static constexpr std::array<uint8_t, 16> DEFAULT_MACRO_CODE3 = {
	C0_ESC, 0x28, MOSAIC_A, C0_ESC, 0x29, MOSAIC_C, C0_ESC, 0x2A, MOSAIC_D, C0_ESC, 0x2B, 0x20, MACRO, C0_LS0, C0_ESC, 0x7D
};
static constexpr std::array<uint8_t, 16> DEFAULT_MACRO_CODE4 = {
	C0_ESC, 0x28, MOSAIC_A, C0_ESC, 0x29, MOSAIC_B, C0_ESC, 0x2A, MOSAIC_D, C0_ESC, 0x2B, 0x20, MACRO, C0_LS0, C0_ESC, 0x7D
};
static constexpr std::array<uint8_t, 17> DEFAULT_MACRO_CODE5 = {
	C0_ESC, 0x28, MOSAIC_A, C0_ESC, 0x29, 0x20, DRCS_1, C0_ESC, 0x2A, MOSAIC_D, C0_ESC, 0x2B, 0x20, MACRO, C0_LS0, C0_ESC, 0x7D
};
static constexpr std::array<uint8_t, 19> DEFAULT_MACRO_CODE6 = {
	C0_ESC, 0x28, 0x20, DRCS_1, C0_ESC, 0x29, 0x20, DRCS_2, C0_ESC, 0x2A, 0x20, DRCS_3, C0_ESC, 0x2B, 0x20, MACRO, C0_LS0, C0_ESC, 0x7D
};
static constexpr std::array<uint8_t, 19> DEFAULT_MACRO_CODE7 = {
	C0_ESC, 0x28, 0x20, DRCS_4, C0_ESC, 0x29, 0x20, DRCS_5, C0_ESC, 0x2A, 0x20, DRCS_6, C0_ESC, 0x2B, 0x20, MACRO, C0_LS0, C0_ESC, 0x7D
};
static constexpr std::array<uint8_t, 19> DEFAULT_MACRO_CODE8 = {
	C0_ESC, 0x28, 0x20, DRCS_7, C0_ESC, 0x29, 0x20, DRCS_8, C0_ESC, 0x2A, 0x20, DRCS_9, C0_ESC, 0x2B, 0x20, MACRO, C0_LS0, C0_ESC, 0x7D
};
static constexpr std::array<uint8_t, 19> DEFAULT_MACRO_CODE9 = {
	C0_ESC, 0x28, 0x20, DRCS_10, C0_ESC, 0x29, 0x20, DRCS_11, C0_ESC, 0x2A, 0x20, DRCS_12, C0_ESC, 0x2B, 0x20, MACRO, C0_LS0, C0_ESC, 0x7D
};
static constexpr std::array<uint8_t, 19> DEFAULT_MACRO_CODE10 = {
	C0_ESC, 0x28, 0x20, DRCS_13, C0_ESC, 0x29, 0x20, DRCS_14, C0_ESC, 0x2A, 0x20, DRCS_15, C0_ESC, 0x2B, 0x20, MACRO, C0_LS0, C0_ESC, 0x7D
};
static constexpr std::array<uint8_t, 17> DEFAULT_MACRO_CODE11 = {
	C0_ESC, 0x24, KANJI, C0_ESC, 0x29, 0x20, DRCS_2, C0_ESC, 0x2A, HIRA, C0_ESC, 0x2B, 0x20, MACRO, C0_LS0, C0_ESC, 0x7D
};
static constexpr std::array<uint8_t, 17> DEFAULT_MACRO_CODE12 = {
	C0_ESC, 0x24, KANJI, C0_ESC, 0x29, 0x20, DRCS_3, C0_ESC, 0x2A, HIRA, C0_ESC, 0x2B, 0x20, MACRO, C0_LS0, C0_ESC, 0x7D
};
static constexpr std::array<uint8_t, 17> DEFAULT_MACRO_CODE13 = {
	C0_ESC, 0x24, KANJI, C0_ESC, 0x29, 0x20, DRCS_4, C0_ESC, 0x2A, HIRA, C0_ESC, 0x2B, 0x20, MACRO, C0_LS0, C0_ESC, 0x7D
};
static constexpr std::array<uint8_t, 16> DEFAULT_MACRO_CODE14 = {
	C0_ESC, 0x28, KANA, C0_ESC, 0x29, HIRA, C0_ESC, 0x2A, ASCII, C0_ESC, 0x2B, 0x20, MACRO, C0_LS0, C0_ESC, 0x7D
};
static constexpr std::array<uint8_t, 17> DEFAULT_MACRO_CODE15 = {
	C0_ESC, 0x28, ASCII, C0_ESC, 0x29, MOSAIC_A, C0_ESC, 0x2A, 0x20, DRCS_1, C0_ESC, 0x2B, 0x20, MACRO, C0_LS0, C0_ESC, 0x7D
};

static constexpr std::array<uint8_t, 94> HALF_ASCII_TABLE = {
	     '!', '\"', '#', '$', '%', '&', '\'', '(', ')', '*', '+',  ',', '-', '.', '/',
	'0', '1',  '2', '3', '4', '5', '6',  '7', '8', '9', ':', ';',  '<', '=', '>', '?',
	'@', 'A',  'B', 'C', 'D', 'E', 'F',  'G', 'H', 'I', 'J', 'K',  'L', 'M', 'N', 'O',
	'P', 'Q',  'R', 'S', 'T', 'U', 'V',  'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_',
	'`', 'a',  'b', 'c', 'd', 'e', 'f',  'g', 'h', 'i', 'j', 'k',  'l', 'm', 'n', 'o',
	'p', 'q',  'r', 's', 't', 'u', 'v',  'w', 'x', 'y', 'z', '{',  '|', '}', 'ÅP'
};

// JIS X0201-1997
static constexpr char ASCII_TABLE[][3] = {
	      "ÅI", "Åh", "Åî", "Åê", "Åì", "Åï", "Åf", "Åi", "Åj", "Åñ", "Å{", "ÅC", "Å|", ".", "Å^",
	"ÇO", "ÇP", "ÇQ", "ÇR", "ÇS", "ÇT", "ÇU", "ÇV", "ÇW", "ÇX", "ÅF", "ÅG", "ÅÉ", "ÅÅ", "ÅÑ", "ÅH",
	"Åó", "Ç`", "Ça", "Çb", "Çc", "Çd", "Çe", "Çf", "Çg", "Çh", "Çi", "Çj", "Çk", "Çl", "Çm", "Çn",
	"Ço", "Çp", "Çq", "Çr", "Çs", "Çt", "Çu", "Çv", "Çw", "Çx", "Çy", "Åm", "Åè", "Ån", "ÅO", "ÅQ",
	"Åe", "ÇÅ", "ÇÇ", "ÇÉ", "ÇÑ", "ÇÖ", "ÇÜ", "Çá", "Çà", "Çâ", "Çä", "Çã", "Çå", "Çç", "Çé", "Çè",
	"Çê", "Çë", "Çí", "Çì", "Çî", "Çï", "Çñ", "Çó", "Çò", "Çô", "Çö", "Åo", "Åb", "Åp", "ÅP"
};

// JIS X0213:2004
static constexpr char KANA_TABLE[][3] = {
	      "É@", "ÉA", "ÉB", "ÉC", "ÉD", "ÉE", "ÉF", "ÉG", "ÉH", "ÉI", "ÉJ", "ÉK", "ÉL", "ÉM", "ÉN",
	"ÉO", "ÉP", "ÉQ", "ÉR", "ÉS", "ÉT", "ÉU", "ÉV", "ÉW", "ÉX", "ÉY", "ÉZ", "É[", "É\", "É]", "É^",
	"É_", "É`", "Éa", "Éb", "Éc", "Éd", "Ée", "Éf", "Ég", "Éh", "Éi", "Éj", "Ék", "Él", "Ém", "Én",
	"Éo", "Ép", "Éq", "Ér", "És", "Ét", "Éu", "Év", "Éw", "Éx", "Éy", "Éz", "É{", "É|", "É}", "É~",
	"ÉÄ", "ÉÅ", "ÉÇ", "ÉÉ", "ÉÑ", "ÉÖ", "ÉÜ", "Éá", "Éà", "Éâ", "Éä", "Éã", "Éå", "Éç", "Éé", "Éè",
	"Éê", "Éë", "Éí", "Éì", "Éî", "Éï", "Éñ", "ÅT", "ÅU", "Å[", "ÅB", "Åu", "Åv", "ÅA", "ÅE"
};

// JIS X0213:2004
static constexpr char HIRA_TABLE[][3] = {
	      "Çü", "Ç†", "Ç°", "Ç¢", "Ç£", "Ç§", "Ç•", "Ç¶", "Çß", "Ç®", "Ç©", "Ç™", "Ç´", "Ç¨", "Ç≠",
	"ÇÆ", "ÇØ", "Ç∞", "Ç±", "Ç≤", "Ç≥", "Ç¥", "Çµ", "Ç∂", "Ç∑", "Ç∏", "Çπ", "Ç∫", "Çª", "Çº", "ÇΩ",
	"Çæ", "Çø", "Ç¿", "Ç¡", "Ç¬", "Ç√", "Çƒ", "Ç≈", "Ç∆", "Ç«", "Ç»", "Ç…", "Ç ", "ÇÀ", "ÇÃ", "ÇÕ",
	"ÇŒ", "Çœ", "Ç–", "Ç—", "Ç“", "Ç”", "Ç‘", "Ç’", "Ç÷", "Ç◊", "Çÿ", "ÇŸ", "Ç⁄", "Ç€", "Ç‹", "Ç›",
	"Çﬁ", "Çﬂ", "Ç‡", "Ç·", "Ç‚", "Ç„", "Ç‰", "ÇÂ", "ÇÊ", "ÇÁ", "ÇË", "ÇÈ", "ÇÍ", "ÇÎ", "ÇÏ", "ÇÌ",
	"ÇÓ", "ÇÔ", "Ç", "ÇÒ", "Å@", "Å@", "Å@", "ÅT", "ÅU", "Å[", "ÅB", "Åu", "Åv", "ÅA", "ÅE"
};

static constexpr char JIS_X0201_KANA_TABLE[][3] = {
	      "ÅB", "Åu", "Åv", "ÅA", "ÅE", "Éí", "É@", "ÉB", "ÉD", "ÉF", "ÉH", "ÉÉ", "ÉÖ", "Éá", "Éb",
	"Å[", "ÉA", "ÉC", "ÉE", "ÉG", "ÉI", "ÉJ", "ÉL", "ÉN", "ÉP", "ÉR", "ÉT", "ÉV", "ÉX", "ÉZ", "É\",
	"É^", "É`", "Éc", "Ée", "Ég", "Éi", "Éj", "Ék", "Él", "Ém", "Én", "Éq", "Ét", "Éw", "Éz", "É}",
	"É~", "ÉÄ", "ÉÅ", "ÉÇ", "ÉÑ", "ÉÜ", "Éà", "Éâ", "Éä", "Éã", "Éå", "Éç", "Éè", "Éì", "ÅJ", "ÅK"
};

struct Gaiji
{
	uint16_t code;
	char character[6];
};

static constexpr Gaiji GAIJI[] = {
	// 90 ãÊ 45ì_Å`63ì_
	{ 0x7A4D, "10." },
	{ 0x7A4E, "11." },
	{ 0x7A4F, "12." },
	{ 0x7A50, "[HV]" },
	{ 0x7A51, "[SD]" },
	{ 0x7A52, "[P]" },
	{ 0x7A53, "[W]" },
	{ 0x7A54, "[MV]" },
	{ 0x7A55, "[éË]" },
	{ 0x7A56, "[éö]" },
	{ 0x7A57, "[ëo]" },
	{ 0x7A58, "[Éf]" },
	{ 0x7A59, "[S]" },
	{ 0x7A5A, "[ìÒ]" },
	{ 0x7A5B, "[ëΩ]" },
	{ 0x7A5C, "[â]" },
	{ 0x7A5D, "[SS]" },
	{ 0x7A5E, "[B]" },
	{ 0x7A5F, "[N]" },
	// 90 ãÊ 64ì_Å`66ì_
	{ 0x7A60, "Å°" },
	{ 0x7A61, "Åú" },
	// 90 ãÊ 66ì_Å`84ì_
	{ 0x7A62, "[ìV]" },
	{ 0x7A63, "[å]" },
	{ 0x7A64, "[âf]" },
	{ 0x7A65, "[ñ≥]" },
	{ 0x7A66, "[óø]" },
	{ 0x7A67, "[ÅE]" },
	{ 0x7A68, "[ëO]" },
	{ 0x7A69, "[å„]" },
	{ 0x7A6A, "[çƒ]" },
	{ 0x7A6B, "[êV]" },
	{ 0x7A6C, "[èâ]" },
	{ 0x7A6D, "[èI]" },
	{ 0x7A6E, "[ê∂]" },
	{ 0x7A6F, "[îÃ]" },
	{ 0x7A70, "[ê∫]" },
	{ 0x7A71, "[êÅ]" },
	{ 0x7A72, "[PPV]" },
	{ 0x7A73, "(îÈ)" },
	{ 0x7A74, "ÇŸÇ©" },
	{ 0x7A75, "[MV]" },
};

class CharDecoder
{
public:
	enum class CodeSetClass
	{
		GSet,
		DRCS,
		unknown
	};

	struct GStatus
	{
		CodeSetClass code_set_class;
		uint8_t      char_code_set;
		uint8_t      num_of_bytes;
	};

	CharDecoder();
	virtual ~CharDecoder() = default;

	void init();

	void decode(const uint8_t* buffer, const uint16_t length);
	void decode_C0(const uint8_t* buffer);
	void decode_C1(const uint8_t* buffer);

	template<uint8_t BitMask>
	void decode_GLGR(const uint8_t* buffer, const GStatus& GLGR);

	void decode_ESC(const uint8_t* buffer);
	template<uint8_t N>
	void decode_SS(const uint8_t* buffer);
	void decode_CSI(const uint8_t* buffer);

	void jis_to_sjis(uint8_t first, uint8_t second);

	GStatus code_elements[4];
	GStatus GL;
	GStatus GR;

private:
	uint16_t read_length;
	std::string chars;
};

