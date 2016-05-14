/*
 * Reference: ARIB STD-B24 v6.2
 *            ARIB STD-B05 v1.0
 *            https://charatsoft.sakura.ne.jp/develop/toaru2/index.php?did=4
 *            http://kikakurui.com/x0/X0201-1997-01.html
 */

#include "char_decoder.h"
#include "ts_common_utils.h"

static constexpr uint8_t BITMASK_GL = 0xFF;
static constexpr uint8_t BITMASK_GR = 0x7F;

CharDecoder::CharDecoder()
	: middle_size_flag(false)
{
}

void CharDecoder::init()
{
	code_elements[0] = { CodeSetClass::GSet, KANJI, 2 }; // G0
	code_elements[1] = { CodeSetClass::GSet, ASCII, 1 }; // G1
	code_elements[2] = { CodeSetClass::GSet, HIRA,  1 }; // G2
	code_elements[3] = { CodeSetClass::GSet, KANA,  1 }; // G3
	GL = &code_elements[0]; // LS0  (G0)
	GR = &code_elements[2]; // LS2R (G2)
	chars = "";
}

template<typename T, typename I>
static void dump_hex_bytes(T* p, I length)
{
	fprintf(stderr, "+0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +A +B +C +D +E +F\n");
	for (auto i = 0; i < length; i++) {
		fprintf(stderr, "%02x ", p[i]);
		if (i && (i & 0xF) == 0xF)
			fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");
}

std::string CharDecoder::decode(const uint8_t* buffer, const uint16_t length)
{
	init();

	//dump_hex_bytes(buffer, length);
	auto it = buffer;
	while (it - buffer < length) {
		read_length = 0;
		if (*it <= 0x20) {
			decode_C0(it);
		}
		else if (*it < 0x7F) {
			decode_GLGR<BITMASK_GL>(it, *GL);
		}
		else if (*it <= 0xA0) {
			decode_C1(&(*it));
		}
		else if (*it < 0xFF) {
			decode_GLGR<BITMASK_GR>(it, *GR);
		}
		else {
			++it;
		}
		it += read_length;
	}

	//fprintf(stderr, "%s\n", chars.c_str());
	return chars;
}

void CharDecoder::decode_C0(const uint8_t* buffer)
{
	auto& read = read_length;

	switch (buffer[0]) {
	case C0_NUL:
	case C0_BEL:
	case C0_APB:
	case C0_APF:
	case C0_APD:
	case C0_APU:
		++read;
		break;
	case C0_APR:
		chars += "\r\n";
		++read;
		break;
	case C0_PAPF:
		read += 2;
		break;
	case C0_APS:
		read += 3;
		break;
	case C0_CS:
	case C0_CAN:
		++read;
		break;
	case C0_ESC:
		decode_ESC(buffer);
		break;
	case C0_LS1:
		GL = &code_elements[1];
		++read;
		break;
	case C0_LS0:
		GL = &code_elements[0];
		++read;
		break;
	case C0_SS2:
		decode_SS<2>(buffer);
		break;
	case C0_SS3:
		decode_SS<3>(buffer);
		break;
	case C0_RS:
	case C0_US:
		++read;
		break;
	case SP:
		if (middle_size_flag) {
			chars += " ";
		}
		else {
			chars += "Å@";
		}
	default:
		++read;
		break;
	}
}

void CharDecoder::decode_C1(const uint8_t* buffer)
{
	auto& read = read_length;

	switch (buffer[0]) {
	case C1_BKF:
	case C1_RDF:
	case C1_GRF:
	case C1_YLF:
	case C1_BLF:
	case C1_MGF:
	case C1_CNF:
	case C1_WHF:
		++read;
		break;
	case C1_COL:
		read += 2;
		break;
	case C1_POL:
		read += 2;
		break;
	case C1_SSZ:
		// Character size: small
		++read;
		break;
	case C1_MSZ:
		// Character size: middle
		middle_size_flag = true;
		++read;
		break;
	case C1_NSZ:
		// Character size: normal
		middle_size_flag = false;
		++read;
		break;
	case C1_SZX:
		read += 2;
		break;
	case C1_FLC:
		read += 2;
		break;
	case C1_CDC:
		if (buffer[1] == 0x40 || buffer[1] == 0x4F) {
			read += 2;
		}
		else {
			read += 3;
		}
		break;
	case C1_WMM:
		read += 2;
		break;
	case C1_TIME:
		if (buffer[1] == 0x20) {
			read += 3;
		}
		else {

		}
		break;
	case C1_MACRO: {
		for (auto i = 1; buffer[i] != 0x4F; ++i) {
			++read;
		}
		break;
	}
	case C1_RPC:
		read += 2;
		break;
	case C1_STL:
	case C1_SPL:
		++read;
		break;
	case C1_HLC:
		read += 2;
		break;
	case C1_CSI:
		decode_CSI(buffer);
		break;
	default:
		++read;
		break;
	}
}

template<uint8_t BitMask>
void CharDecoder::decode_GLGR(const uint8_t* buffer, const GStatus& GLGR)
{
	auto& read = read_length;

	if (GLGR.code_set_class == CodeSetClass::GSet) {
		switch (GLGR.char_code_set) {
		case KANJI:
		case JIS_KANJI_1:
		case JIS_KANJI_2:
		case SYMBOL: {
			jis_to_sjis(buffer[0] & BitMask, buffer[1] & BitMask);
			break;
		}
		case ASCII:
		case PROPORTIONAL_ASCII: {
			if (middle_size_flag) {
				chars += HALF_ASCII_TABLE[(buffer[0] & BitMask) - 0x21];
			}
			else {
				chars += ASCII_TABLE[(buffer[0] & BitMask) - 0x21];
			}
			break;
		}
		case HIRA:
		case PROPORTIONAL_HIRA: {
			chars += HIRA_TABLE[(buffer[0] & BitMask) - 0x21];
			break;
		}
		case KANA:
		case PROPORTIONAL_KANA: {
			chars += KANA_TABLE[(buffer[0] & BitMask) - 0x21];
			break;
		}
		case JIS_X0201_KANA: {
			chars += JIS_X0201_KANA_TABLE[(buffer[0] & BitMask) - 0x21];
			break;
		}
		case MOSAIC_A:
		case MOSAIC_B:
		case MOSAIC_C:
		case MOSAIC_D:
			break;
		default:
			fprintf(stderr, "Unknown char_code_set: %x\n", GLGR.char_code_set);
			break;
		}
	}
	else if (GLGR.code_set_class == CodeSetClass::DRCS) {
		// Not used in EPGs
		switch (buffer[0]) {
		case 0x60:
			decode(DEFAULT_MACRO_CODE0.data(), DEFAULT_MACRO_CODE0.size());
			break;
		case 0x61:
			decode(DEFAULT_MACRO_CODE1.data(), DEFAULT_MACRO_CODE1.size());
			break;
		case 0x62:
			decode(DEFAULT_MACRO_CODE2.data(), DEFAULT_MACRO_CODE2.size());
			break;
		case 0x63:
			decode(DEFAULT_MACRO_CODE3.data(), DEFAULT_MACRO_CODE3.size());
			break;
		case 0x64:
			decode(DEFAULT_MACRO_CODE4.data(), DEFAULT_MACRO_CODE4.size());
			break;
		case 0x65:
			decode(DEFAULT_MACRO_CODE5.data(), DEFAULT_MACRO_CODE5.size());
			break;
		case 0x66:
			decode(DEFAULT_MACRO_CODE6.data(), DEFAULT_MACRO_CODE6.size());
			break;
		case 0x67:
			decode(DEFAULT_MACRO_CODE7.data(), DEFAULT_MACRO_CODE7.size());
			break;
		case 0x68:
			decode(DEFAULT_MACRO_CODE8.data(), DEFAULT_MACRO_CODE8.size());
			break;
		case 0x69:
			decode(DEFAULT_MACRO_CODE9.data(), DEFAULT_MACRO_CODE9.size());
			break;
		case 0x6A:
			decode(DEFAULT_MACRO_CODE10.data(), DEFAULT_MACRO_CODE10.size());
			break;
		case 0x6B:
			decode(DEFAULT_MACRO_CODE11.data(), DEFAULT_MACRO_CODE11.size());
			break;
		case 0x6C:
			decode(DEFAULT_MACRO_CODE12.data(), DEFAULT_MACRO_CODE12.size());
			break;
		case 0x6D:
			decode(DEFAULT_MACRO_CODE13.data(), DEFAULT_MACRO_CODE13.size());
			break;
		case 0x6E:
			decode(DEFAULT_MACRO_CODE14.data(), DEFAULT_MACRO_CODE14.size());
			break;
		case 0x6F:
			decode(DEFAULT_MACRO_CODE15.data(), DEFAULT_MACRO_CODE15.size());
			break;
		default:
			break;
		}
	}
	else {
	}

	read += GLGR.num_of_bytes;
}

void CharDecoder::jis_to_sjis(uint8_t first, uint8_t second)
{
	// What happens in this function?
	// http://www.jiancool.com/article/21971527801/
	// http://www7a.biglobe.ne.jp/~tsuneoka/cgitech/4.html

	if (first >= 0x75 && second >= 0x21) {
		// Supplemental character (Gaiji)
		static const auto size = get_array_size(GAIJI_TABLE);
		const auto code = first << 8 | second;

		for (auto i = 0; i < size; ++i) {
			if (code == GAIJI_TABLE[i].code) {
				chars += GAIJI_TABLE[i].character;
				break;
			}
		}
	}
	else {
		// offset
		const auto cell = first & 1 ? 0x1F + (second > 0x5F) : 0x7E;

		second = (second + cell) & 0xFF;

		first = (first - 0x21) >> 1;
		const auto row = first >= 0x1F ? 0xC1 : 0x81;
		first += row;

		const char sjis_kanji[3]{ static_cast<char>(first), static_cast<char>(second) };
		chars += sjis_kanji;
	}
}

void CharDecoder::decode_ESC(const uint8_t* buffer)
{
	auto& read = read_length;

	switch (buffer[1]) {
		// Table 7-1  Invocation of code elements
	case 0x6E:
		// LS2
		GL = &code_elements[2];
		read += 2;
		break;
	case 0x6F:
		// LS3
		GL = &code_elements[3];
		read += 2;
		break;
	case 0x7E:
		// LS1R
		GR = &code_elements[1];
		read += 2;
		break;
	case 0x7D:
		// LS2R
		GR = &code_elements[2];
		read += 2;
		break;
	case 0x7C:
		// LS3R
		GR = &code_elements[3];
		read += 2;
		break;

		// Table 7-2  Designation of graphic sets
	case 0x28:
	case 0x29:
	case 0x2A:
	case 0x2B: {
		if (buffer[2] == 0x20) {
			// ESC [0x28] 0x20 F : 1-byte DRCS [G0]
			// ESC [0x29] 0x20 F : 1-byte DRCS [G1]
			// ESC [0x2A] 0x20 F : 1-byte DRCS [G2]
			// ESC [0x2B] 0x20 F : 1-byte DRCS [G3]
			const auto index = buffer[1] - 0x28;
			code_elements[index].char_code_set  = buffer[3];
			code_elements[index].code_set_class = CodeSetClass::DRCS;
			code_elements[index].num_of_bytes   = 1;

			read += 4;
		}
		else {
			// ESC [0x28] F : 1-byte G set [G0]
			// ESC [0x29] F : 1-byte G set [G1]
			// ESC [0x2A] F : 1-byte G set [G2]
			// ESC [0x2B] F : 1-byte G set [G3]
			const auto index = buffer[1] - 0x28;
			code_elements[index].char_code_set  = buffer[2];
			code_elements[index].code_set_class = CodeSetClass::GSet;
			code_elements[index].num_of_bytes   = 1;

			read += 3;
		}

		break;
	}
	case 0x24: {
		if (buffer[2] < 0x28 || 0x2B < buffer[2]) {
			// ESC 0x24 F : 2-byte G set [G0]
			code_elements[0].char_code_set  = buffer[2];
			code_elements[0].code_set_class = CodeSetClass::GSet;
			code_elements[0].num_of_bytes   = 2;
			read += 3;
		}
		else if (buffer[3] == 0x20) {
			// ESC 0x24 [0x28] 0x20 F : 2-byte DRCS [G0]
			// ESC 0x24 [0x29] 0x20 F : 2-byte DRCS [G1]
			// ESC 0x24 [0x2A] 0x20 F : 2-byte DRCS [G2]
			// ESC 0x24 [0x2B] 0x20 F : 2-byte DRCS [G3]
			const auto index = buffer[2] - 0x28;
			code_elements[index].char_code_set  = buffer[4];
			code_elements[index].code_set_class = CodeSetClass::DRCS;
			code_elements[index].num_of_bytes   = 2;

			read += 5;
		}
		else {
			// ESC 0x24 [0x29] F : 2-byte G set [G1]
			// ESC 0x24 [0x2A] F : 2-byte G set [G2]
			// ESC 0x24 [0x2B] F : 2-byte G set [G3]
			const auto index = buffer[2] - 0x28;
			code_elements[index].char_code_set  = buffer[3];
			code_elements[index].code_set_class = CodeSetClass::GSet;
			code_elements[index].num_of_bytes   = 2;

			read += 4;
		}
		break;
	}
	default:
		++read;
		break;
	}
}

template<uint8_t N>
void CharDecoder::decode_SS(const uint8_t* buffer)
{
	// Single shift
	const auto last_GL = GL;
	GL = &code_elements[N];

	++read_length;
	decode_GLGR<BITMASK_GL>(&buffer[1], *GL);

	GL = last_GL;
}

void CharDecoder::decode_CSI(const uint8_t* buffer)
{
	// CSI .... 0x20 F
	auto read = 1;
	for (; buffer[read] != 0x20; ++read);

	++read;

	// final character
	switch (buffer[read]) {
	case 0x53:
		// SWF
	case 0x54:
		// CCC
	case 0x6E:
		// RCS
	case 0x61:
		// ACPS
	case 0x56:
		// SDF
	case 0x5F:
		// SDP
	case 0x57:
		// SSM
	case 0x5B:
		// PLD
	case 0x5C:
		// PLU
	case 0x58:
		// SHS
	case 0x59:
		// SVS
	case 0x42:
		// GSM
	case 0x5D:
		// GAA
	case 0x5E:
		// SRC
	case 0x62:
		// TCC
	case 0x65:
		// CFS
	case 0x63:
		// MDF
	case 0x66:
		// XCS
	case 0x68:
		// PRA
	case 0x69:
		// ACS
	case 0x6A:
		// UED
	case 0x6F:
		// SCS
	default:
		break;
	}

	++read;

	read_length += read;
}
