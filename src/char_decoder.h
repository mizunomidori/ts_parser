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
	char character[12];
};

static constexpr Gaiji GAIJI_TABLE[] = {
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
	{ 0x7A67, "[îNóÓêßå¿]" },
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

	// 92 ãÊ 1ì_Å`
	{ 0x7C21, "Å®" },
	{ 0x7C22, "Å©" },
	{ 0x7C23, "Å™" },
	{ 0x7C24, "Å´" },
	{ 0x7C25, "Åú" },
	{ 0x7C26, "Åõ" },
	{ 0x7C27, "îN" },
	{ 0x7C28, "åé" },
	{ 0x7C29, "ì˙" },
	{ 0x7C2A, "â~" },
	{ 0x7C2B, "áu" },
	{ 0x7C2C, "m^3" },
	{ 0x7C2D, "áp" },
	{ 0x7C2E, "áp^2" },
	{ 0x7C2F, "áp^3" },
	{ 0x7C30, "ÇO." },
	{ 0x7C31, "ÇP." },
	{ 0x7C32, "ÇQ." },
	{ 0x7C33, "ÇR." },
	{ 0x7C34, "ÇS." },
	{ 0x7C35, "ÇT." },
	{ 0x7C36, "ÇU." },
	{ 0x7C37, "ÇV." },
	{ 0x7C38, "ÇW." },
	{ 0x7C39, "ÇX." },
	{ 0x7C3A, "éÅ" },
	{ 0x7C3B, "ïõ" },
	{ 0x7C3C, "å≥" },
	{ 0x7C3D, "åÃ" },
	{ 0x7C3E, "ëO" },
	{ 0x7C3F, "å„" },
	{ 0x7C40, "ÇO," },
	{ 0x7C41, "ÇP," },
	{ 0x7C42, "ÇQ," },
	{ 0x7C43, "ÇR," },
	{ 0x7C44, "ÇS," },
	{ 0x7C45, "ÇT," },
	{ 0x7C46, "ÇU," },
	{ 0x7C47, "ÇV," },
	{ 0x7C48, "ÇW," },
	{ 0x7C49, "ÇX," },
	{ 0x7C4A, "(é–)" },
	{ 0x7C4B, "(ç‡)" },
	{ 0x7C4C, "(óL)" },
	{ 0x7C4D, "(äî)" },
	{ 0x7C4E, "(ë„)" },
	{ 0x7C4F, "(ñ‚)" },

	{ 0x7C55, "^2" },
	{ 0x7C56, "^3" },
	{ 0x7C57, "(CD)" },
	{ 0x7C58, "(vn)" },
	{ 0x7C59, "(ob)" },
	{ 0x7C5A, "(cb)" },
	{ 0x7C5B, "(ce" },
	{ 0x7C5C, "mb)" },
	{ 0x7C5D, "(hp)" },
	{ 0x7C5E, "(br)" },
	{ 0x7C5F, "(p)" },
	{ 0x7C60, "(s)" },
	{ 0x7C61, "(ms)" },
	{ 0x7C62, "(t)" },
	{ 0x7C63, "(bs)" },
	{ 0x7C64, "(b)" },
	{ 0x7C65, "(tb)" },
	{ 0x7C66, "(tp)" },
	{ 0x7C67, "(ds)" },
	{ 0x7C68, "(ag)" },
	{ 0x7C69, "(eg)" },
	{ 0x7C6A, "(vo)" },
	{ 0x7C6B, "(fl)" },
	{ 0x7C6C, "(ke" },
	{ 0x7C6D, "y)" },
	{ 0x7C6E, "(sa" },
	{ 0x7C6F, "x)" },
	{ 0x7C70, "(sy" },
	{ 0x7C71, "n)" },
	{ 0x7C72, "(or" },
	{ 0x7C73, "g)" },
	{ 0x7C74, "(pe" },
	{ 0x7C75, "r)" },
	{ 0x7C76, "(R)" },
	{ 0x7C77, "(C)" },
	{ 0x7C78, "(‚∂)" },
	{ 0x7C79, "DJ" },
	{ 0x7C7A, "[ââ]" },
	{ 0x7C7B, "Fax" },

	// 93 ãÊ 1ì_Å`
	{ 0x7D21, "(åé)" },
	{ 0x7D22, "(âŒ)" },
	{ 0x7D23, "(êÖ)" },
	{ 0x7D24, "(ñÿ)" },
	{ 0x7D25, "(ã‡)" },
	{ 0x7D26, "(ìy)" },
	{ 0x7D27, "(ì˙)" },
	{ 0x7D28, "(èj)" },
	{ 0x7D29, "ñæé°" },
	{ 0x7D2A, "ëÂê≥" },
	{ 0x7D2B, "è∫òa" },
	{ 0x7D2C, "ïΩê¨" },
	{ 0x7D2D, "No." },
	{ 0x7D2E, "Tel" },
	{ 0x7D2F, "(Åß)" },
	{ 0x7D30, "()()" },
	{ 0x7D31, "[ñ{]" },
	{ 0x7D32, "[éO]" },
	{ 0x7D33, "[ìÒ]" },
	{ 0x7D34, "[à¿]" },
	{ 0x7D35, "[ì_]" },
	{ 0x7D36, "[ë≈]" },
	{ 0x7D37, "[ìê]" },
	{ 0x7D38, "[èü]" },
	{ 0x7D39, "[îs]" },
	{ 0x7D3A, "[S]" },
	{ 0x7D3B, "[ìä]" },
	{ 0x7D3C, "[ïﬂ]" },
	{ 0x7D3D, "[àÍ]" },
	{ 0x7D3E, "[ìÒ]" },
	{ 0x7D3F, "[éO]" },
	{ 0x7D40, "[óV]" },
	{ 0x7D41, "[ç∂]" },
	{ 0x7D42, "[íÜ]" },
	{ 0x7D43, "[âE]" },
	{ 0x7D44, "[éw]" },
	{ 0x7D45, "[ëñ]" },
	{ 0x7D46, "[ë≈]" },
	{ 0x7D47, "ág" },
	{ 0x7D48, "kg" },
	{ 0x7D49, "Hz" },
	{ 0x7D4A, "ha" },
	{ 0x7D4B, "km" },
	{ 0x7D4C, "km^2" },
	{ 0x7D4D, "hPa" },
		
	{ 0x7D50, "1/2" },
	{ 0x7D51, "0/3" },
	{ 0x7D52, "1/3" },
	{ 0x7D53, "2/3" },
	{ 0x7D54, "1/4" },
	{ 0x7D55, "3/4" },
	{ 0x7D56, "1/5" },
	{ 0x7D57, "2/5" },
	{ 0x7D58, "3/5" },
	{ 0x7D59, "4/5" },
	{ 0x7D5A, "1/6" },
	{ 0x7D5B, "5/6" },
	{ 0x7D5C, "1/7" },
	{ 0x7D5D, "1/8" },
	{ 0x7D5E, "1/9" },
	{ 0x7D5F, "1/10" },

	{ 0x7D6E, "!!" },
	{ 0x7D6F, "!?" },

	// 94 ãÊ 1ì_Å`
	{ 0x7E21, "áT" },
	{ 0x7E22, "áU" },
	{ 0x7E23, "áV" },
	{ 0x7E24, "áW" },
	{ 0x7E25, "áX" },
	{ 0x7E26, "áY" },
	{ 0x7E27, "áZ" },
	{ 0x7E28, "á[" },
	{ 0x7E29, "á\" },
	{ 0x7E2A, "á]" },
	{ 0x7E2B, "XI" },
	{ 0x7E2C, "XII" },
	{ 0x7E2D, "áP" },
	{ 0x7E2E, "áQ" },
	{ 0x7E2F, "áR" },
	{ 0x7E30, "áS" },
	{ 0x7E31, "(1)" },
	{ 0x7E32, "(2)" },
	{ 0x7E33, "(3)" },
	{ 0x7E34, "(4)" },
	{ 0x7E35, "(5)" },
	{ 0x7E36, "(6)" },
	{ 0x7E37, "(7)" },
	{ 0x7E38, "(8)" },
	{ 0x7E39, "(9)" },
	{ 0x7E3A, "(10)" },
	{ 0x7E3B, "(11)" },
	{ 0x7E3C, "(12)" },
	{ 0x7E3D, "(21)" },
	{ 0x7E3E, "(22)" },
	{ 0x7E3F, "(23)" },
	{ 0x7E40, "(24)" },
	{ 0x7E41, "(A)" },
	{ 0x7E42, "(B)" },
	{ 0x7E43, "(C)" },
	{ 0x7E44, "(D)" },
	{ 0x7E45, "(E)" },
	{ 0x7E46, "(F)" },
	{ 0x7E47, "(G)" },
	{ 0x7E48, "(H)" },
	{ 0x7E49, "(I)" },
	{ 0x7E4A, "(J)" },
	{ 0x7E4B, "(K)" },
	{ 0x7E4C, "(L)" },
	{ 0x7E4D, "(M)" },
	{ 0x7E4E, "(N)" },
	{ 0x7E4F, "(O)" },
	{ 0x7E50, "(P)" },
	{ 0x7E51, "(Q)" },
	{ 0x7E52, "(R)" },
	{ 0x7E53, "(S)" },
	{ 0x7E54, "(T)" },
	{ 0x7E55, "(U)" },
	{ 0x7E56, "(V)" },
	{ 0x7E57, "(W)" },
	{ 0x7E58, "(X)" },
	{ 0x7E59, "(Y)" },
	{ 0x7E5A, "(Z)" },
	{ 0x7E5B, "(25)" },
	{ 0x7E5C, "(26)" },
	{ 0x7E5D, "(27)" },
	{ 0x7E5E, "(28)" },
	{ 0x7E5F, "(29)" },
	{ 0x7E60, "(30)" },
	{ 0x7E61, "á@" },
	{ 0x7E62, "áA" },
	{ 0x7E63, "áB" },
	{ 0x7E64, "áC" },
	{ 0x7E65, "áD" },
	{ 0x7E66, "áE" },
	{ 0x7E67, "áF" },
	{ 0x7E68, "áG" },
	{ 0x7E69, "áH" },
	{ 0x7E6A, "áI" },
	{ 0x7E6B, "áJ" },
	{ 0x7E6C, "áK" },
	{ 0x7E6D, "áL" },
	{ 0x7E6E, "áM" },
	{ 0x7E6F, "áN" },
	{ 0x7E70, "áO" },
	{ 0x7E71, "(1)" },
	{ 0x7E72, "(2)" },
	{ 0x7E73, "(3)" },
	{ 0x7E74, "(4)" },
	{ 0x7E75, "(5)" },
	{ 0x7E76, "(6)" },
	{ 0x7E77, "(7)" },
	{ 0x7E78, "(8)" },
	{ 0x7E79, "(9)" },
	{ 0x7E7A, "(10)" },
	{ 0x7E7B, "(11)" },
	{ 0x7E7C, "(12)" },
	{ 0x7E7D, "(31)" },

	{ 0x7525, "˙q" },
	{ 0x7526, "˙a" },
	{ 0x752B, "˙ä" },
	{ 0x752E, "˚•" },
	{ 0x7533, "˙ì" },
	{ 0x753A, "˙ú" },
	{ 0x753F, "˙™" },
	{ 0x7540, "˙±" },
	{ 0x7543, "˙∏" },
	{ 0x7544, "˙g" },
	{ 0x7545, "˙∫" },
	{ 0x7548, "˙≈" },
	{ 0x7549, "˙‘" },
	{ 0x754B, "èå" },
	{ 0x754C, "˙ﬁ" },
	{ 0x754D, "˙f" },
	{ 0x7565, "˚W" },
	{ 0x7565, "˚Y" },
	{ 0x756A, "˚a" },
	{ 0x756B, "˚b" },
	{ 0x756F, "˚g" },
	{ 0x7570, "˚h" },
	{ 0x757B, "ã_" },
	{ 0x757C, "‚X" },
	{ 0x7628, "˚ë" },
	{ 0x762D, "äã" },
	{ 0x762E, "˙`" },
	{ 0x762F, "ñH" },
	{ 0x7630, "˚õ" },
	{ 0x7632, "êI" },
	{ 0x7635, "˚¢" },
	{ 0x7637, "˚™" },
	{ 0x7639, "í“" },
	{ 0x763C, "˚π" },
	{ 0x763D, "ìA" },
	{ 0x7640, "˙_" },
	{ 0x7641, "˚ÿ" },
	{ 0x7642, "˙^" },
	{ 0x7643, "˚Ë" },
	{ 0x7645, "ÈL" },
	{ 0x7647, "˚¸" },
	{ 0x7648, "éI" }
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

	std::string decode(const uint8_t* buffer, const uint16_t length);
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
	GStatus *GL;
	GStatus *GR;

private:
	uint16_t read_length;
	std::string chars;

	bool middle_size_flag;
};

