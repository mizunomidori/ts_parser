#pragma once

#include <memory>
#include <array>
#include <vector>

struct VideoStreamDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	int8_t  multiple_frame_rate_flag;
	uint8_t frame_rate_code;
	int8_t  MPEG_1_only_flag;
	int8_t  constrained_parameter_flag;
	int8_t  still_picture_flag;

	uint8_t profile_and_level_indication;
	uint8_t chroma_format;
	int8_t  frame_rate_extension_flag;

	bool parse(const uint8_t* p);
};

template<typename... T>
constexpr auto make_array(T... t)
-> std::array<typename std::common_type<T...>::type, sizeof...(T)>
{
	return std::array<typename std::common_type<T...>::type, sizeof...(T)>{ {t...} };
}

// ITU-T Rec. H.262 Table 6-4
struct FrameRateTable
{
	uint8_t frame_rate_code;
	double  frame_rate_value;
};

constexpr std::array<FrameRateTable, 8>
frame_rate_table = { {
	{ 0b0'001 , 24000. / 1001 },
	{ 0b0'010 , 24. },
	{ 0b0'011 , 25. },
	{ 0b0'100 , 30000. / 1001 },
	{ 0b0'101 , 30. },
	{ 0b0'110 , 50. },
	{ 0b0'111 , 60000. / 1001 },
	{ 0b1'000 , 60. }
} };

struct AudioStreamDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	int8_t  free_format_flag;
	int8_t  ID;
	int8_t  layer;
	int8_t  variable_rate_audio_indicator;

	bool parse(const uint8_t* p);
};

struct DataStreamAlignmentDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	uint8_t alignment_type;

	bool parse(const uint8_t* p);
};

struct TargetBackgroundGridDescriptor
{
	uint8_t  descriptor_tag;
	uint8_t  descriptor_length;
	uint16_t horizontal_size;
	uint16_t vertical_size;
	uint8_t  aspect_ratio_information;

	bool parse(const uint8_t* p);
};

struct VideoWindowDescriptor
{
	uint8_t  descriptor_tag;
	uint8_t  descriptor_length;
	uint16_t horizontal_offset;
	uint16_t vertical_offset;
	uint8_t  window_priority;

	bool parse(const uint8_t* p);
};

struct CADescriptor
{
	uint8_t  descriptor_tag;
	uint8_t  descriptor_length;
	uint16_t CA_system_ID;
	uint16_t CA_PID;
	std::vector<uint8_t> private_data_byte;

	bool parse(const uint8_t* p);
};

struct ISO639LanguageDescriptor
{
	struct ISO639LanguageInfo
	{
		uint32_t ISO_639_language_code;
		uint8_t  audio_type;
	};
	uint8_t  descriptor_tag;
	uint8_t  descriptor_length;
	std::vector<ISO639LanguageInfo> ISO639_info_list;

	bool parse(const uint8_t* p);
};

struct SystemClockDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	int8_t  external_clock_reference_indicator;
	uint8_t clock_accuracy_integer;
	uint8_t clock_accuracy_exponent;

	bool parse(const uint8_t* p);
};

struct MultiplexBufferUtilizationDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	int8_t   bound_valid_flag;
	uint16_t LTW_offset_lower_bound;
	uint16_t LTW_offset_upper_bound;

	bool parse(const uint8_t* p);
};

struct CopyrightDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	uint32_t copyright_indicator;
	std::vector<uint8_t> additional_copyright_info;

	bool parse(const uint8_t* p);
};

struct MaximumBitrateDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	uint32_t maximum_bitrate;

	bool parse(const uint8_t* p);
};

/* ARIB STD-B10v5_7 */
struct BouquetNameDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	std::vector<uint8_t> bouquet_name_chars;

	bool parse(const uint8_t* p);
};

struct CAIdentifierDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	std::vector<uint16_t> CA_system_ids;

	bool parse(const uint8_t* p);
};

struct ComponentDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;

	uint8_t stream_component;
	uint8_t component_type;
	uint8_t component_tag;
	int32_t ISO_639_language_code;

	std::vector<uint8_t> text_chars;

	bool parse(const uint8_t* p);
};

struct ComponentTable
{
	uint8_t stream_content;
	uint8_t component_type;
	wchar_t* statement;
};

constexpr std::array<ComponentTable, 0xff>
component_table = { {
	{ 0x01, 0x01, L"映像480i(525i)、アスペクト比4:3" },
	{ 0x01, 0x02, L"映像480i(525i)、アスペクト比16:9 パンベクトルあり" },
	{ 0x01, 0x03, L"映像480i(525i)、アスペクト比16:9 パンベクトルなし" },
	{ 0x01, 0x04, L"映像480i(525i)、アスペクト比 > 16:9" },
	{ 0x01, 0x91, L"映像2160p、アスペクト比4:3" },
	{ 0x01, 0x92, L"映像2160p、アスペクト比16:9 パンベクトルあり" },
	{ 0x01, 0x93, L"映像2160p、アスペクト比16:9 パンベクトルなし" },
	{ 0x01, 0x94, L"映像2160p、アスペクト比 > 16:9" },
	{ 0x01, 0xA1, L"映像480p(525p)、アスペクト比4:3" },
	{ 0x01, 0xA2, L"映像480p(525p)、アスペクト比16:9 パンベクトルあり" },
	{ 0x01, 0xA3, L"映像480p(525p)、アスペクト比16:9 パンベクトルなし" },
	{ 0x01, 0xA4, L"映像480p(525p)、アスペクト比 > 16:9" },
	{ 0x01, 0xB1, L"映像1080i(1125i)、アスペクト比4:3" },
	{ 0x01, 0xB2, L"映像1080i(1125i)、アスペクト比16:9 パンベクトルあり" },
	{ 0x01, 0xB3, L"映像1080i(1125i)、アスペクト比16:9 パンベクトルなし" },
	{ 0x01, 0xB4, L"映像1080i(1125i)、アスペクト比 > 16:9" },
	{ 0x01, 0xC1, L"映像720p(750p)、アスペクト比4:3" },
	{ 0x01, 0xC2, L"映像720p(750p)、アスペクト比16:9 パンベクトルあり" },
	{ 0x01, 0xC3, L"映像720p(750p)、アスペクト比16:9 パンベクトルなし" },
	{ 0x01, 0xC4, L"映像720p(750p)、アスペクト比 > 16:9" },
	{ 0x01, 0xD1, L"映像240p アスペクト比4:3" },
	{ 0x01, 0xD2, L"映像240p アスペクト比16:9 パンベクトルあり" },
	{ 0x01, 0xD3, L"映像240p アスペクト比16:9 パンベクトルなし" },
	{ 0x01, 0xD4, L"映像240p アスペクト比 > 16:9" },
	{ 0x01, 0xE1, L"映像1080p(1125p)、アスペクト比4:3" },
	{ 0x01, 0xE2, L"映像1080p(1125p)、アスペクト比16:9 パンベクトルあり" },
	{ 0x01, 0xE3, L"映像1080p(1125p)、アスペクト比16:9 パンベクトルなし" },
	{ 0x01, 0xE4, L"映像1080p(1125p)、アスペクト比 > 16:9" },
	{ 0x02, 0x01, L"音声、1/0モード（シングルモノ）" },
	{ 0x02, 0x02, L"音声、1/0＋1/0モード（デュアルモノ）" },
	{ 0x02, 0x03, L"音声、2/0モード（ステレオ）" },
	{ 0x02, 0x04, L"音声、2/1モード" },
	{ 0x02, 0x05, L"音声、3/0モード" },
	{ 0x02, 0x06, L"音声、2/2モード" },
	{ 0x02, 0x07, L"音声、3/1モード" },
	{ 0x02, 0x08, L"音声、3/2モード" },
	{ 0x02, 0x09, L"音声、3/2＋LFEモード（3/2.1モード）" },
	{ 0x02, 0x0A, L"音声、3/3.1モード" },
	{ 0x02, 0x0B, L"音声、2/0/0-2/0/2-0.1モード" },
	{ 0x02, 0x0C, L"音声、5/2.1モード" },
	{ 0x02, 0x0D, L"音声、3/2/2.1モード" },
	{ 0x02, 0x0E, L"音声、2/0/0-3/0/2-0.1モード" },
	{ 0x02, 0x0F, L"音声、0/2/0-3/0/2-0.1モード" },
	{ 0x02, 0x10, L"音声、2/0/0-3/2/3-0.2モード" },
	{ 0x02, 0x11, L"音声、3/3/3-5/2/3-3/0/0.2モード" },
	{ 0x02, 0x40, L"視覚障害者用音声解説" },
	{ 0x02, 0x41, L"聴覚障害者用音声" },
	{ 0x05, 0x01, L"H.264|MPEG-4 AVC、映像480i(525i)、アスペクト比4:3" },
	{ 0x05, 0x02, L"H.264|MPEG-4 AVC、映像480i(525i)、アスペクト比16:9 パンベクトルあり" },
	{ 0x05, 0x03, L"H.264|MPEG-4 AVC、映像480i(525i)、アスペクト比16:9 パンベクトルなし" },
	{ 0x05, 0x04, L"H.264|MPEG-4 AVC、映像480i(525i)、アスペクト比 > 16:9" },
	{ 0x05, 0x91, L"H.264|MPEG-4 AVC、映像2160p、アスペクト比4:3" },
	{ 0x05, 0x92, L"H.264|MPEG-4 AVC、映像2160p、アスペクト比16:9 パンベクトルあり" },
	{ 0x05, 0x93, L"H.264|MPEG-4 AVC、映像2160p、アスペクト比16:9 パンベクトルなし" },
	{ 0x05, 0x94, L"H.264|MPEG-4 AVC、映像2160p、アスペクト比 > 16:9" },
	{ 0x05, 0xA1, L"H.264|MPEG-4 AVC、映像480p(525p)、アスペクト比4:3" },
	{ 0x05, 0xA2, L"H.264|MPEG-4 AVC、映像480p(525p)、アスペクト比16:9 パンベクトルあり" },
	{ 0x05, 0xA3, L"H.264|MPEG-4 AVC、映像480p(525p)、アスペクト比16:9 パンベクトルなし" },
	{ 0x05, 0xA4, L"H.264|MPEG-4 AVC、映像480p(525p)、アスペクト比 > 16:9" },
	{ 0x05, 0xB1, L"H.264|MPEG-4 AVC、映像1080i(1125i)、アスペクト比4:3" },
	{ 0x05, 0xB2, L"H.264|MPEG-4 AVC、映像1080i(1125i)、アスペクト比16:9 パンベクトルあり" },
	{ 0x05, 0xB3, L"H.264|MPEG-4 AVC、映像1080i(1125i)、アスペクト比16:9 パンベクトルなし" },
	{ 0x05, 0xB4, L"H.264|MPEG-4 AVC、映像1080i(1125i)、アスペクト比 > 16:9" },
	{ 0x05, 0xC1, L"H.264|MPEG-4 AVC、映像720p(750p)、アスペクト比4:3" },
	{ 0x05, 0xC2, L"H.264|MPEG-4 AVC、映像720p(750p)、アスペクト比16:9 パンベクトルあり" },
	{ 0x05, 0xC3, L"H.264|MPEG-4 AVC、映像720p(750p)、アスペクト比16:9 パンベクトルなし" },
	{ 0x05, 0xC4, L"H.264|MPEG-4 AVC、映像720p(750p)、アスペクト比 > 16:9" },
	{ 0x05, 0xD1, L"H.264|MPEG-4 AVC、映像240p アスペクト比4:3" },
	{ 0x05, 0xD2, L"H.264|MPEG-4 AVC、映像240p アスペクト比16:9 パンベクトルあり" },
	{ 0x05, 0xD3, L"H.264|MPEG-4 AVC、映像240p アスペクト比16:9 パンベクトルなし" },
	{ 0x05, 0xD4, L"H.264|MPEG-4 AVC、映像240p アスペクト比 > 16:9" },
	{ 0x05, 0xE1, L"H.264|MPEG-4 AVC、映像1080p(1125p)、アスペクト比4:3" },
	{ 0x05, 0xE2, L"H.264|MPEG-4 AVC、映像1080p(1125p)、アスペクト比16:9 パンベクトルあり" },
	{ 0x05, 0xE3, L"H.264|MPEG-4 AVC、映像1080p(1125p)、アスペクト比16:9 パンベクトルなし" },
	{ 0x05, 0xE4, L"H.264|MPEG-4 AVC、映像1080p(1125p)、アスペクト比 > 16:9" }
} };

struct ContentDescriptor
{
	struct NibbleData
	{
		uint8_t content_nibble_level_1;
		uint8_t content_nibble_level_2;
		uint8_t user_nibble1;
		uint8_t user_nibble2;
	};

	uint8_t descriptor_tag;
	uint8_t descriptor_length;

	std::vector<NibbleData> nibble_list;

	bool parse(const uint8_t* p);
};

struct Nibble1Table
{
	uint8_t content_nibble_level1;
	wchar_t* statement;
};

constexpr std::array<Nibble1Table, 0x10>
nibble1_table = { {
	{ 0x00, L"ニュース／報道" },
	{ 0x01, L"スポーツ" },
	{ 0x02, L"情報／ワイドショー" },
	{ 0x03, L"ドラマ" },
	{ 0x04, L"音楽" },
	{ 0x05, L"バラエティ" },
	{ 0x06, L"映画" },
	{ 0x07, L"アニメ／特撮" },
	{ 0x08, L"ドキュメンタリー／教養" },
	{ 0x09, L"劇場／公演" },
	{ 0x0A, L"趣味／教育" },
	{ 0x0B, L"福祉" },
	{ 0x0E, L"拡張" },
	{ 0x0F, L"その他" },
} };

struct Nibble2Table
{
	uint8_t content_nibble_level1;
	uint8_t content_nibble_level2;
	wchar_t* statement;
};

constexpr std::array<Nibble2Table, 0x100>
nibble2_table = { {
	{ 0x00, 0x00, L"定時・総合" },
	{ 0x00, 0x01, L"天気" },
	{ 0x00, 0x02, L"特集・ドキュメント" },
	{ 0x00, 0x03, L"政治・国会" },
	{ 0x00, 0x04, L"経済・市況" },
	{ 0x00, 0x05, L"海外・国際" },
	{ 0x00, 0x06, L"解説" },
	{ 0x00, 0x07, L"討論・会談" },
	{ 0x00, 0x08, L"報道特番" },
	{ 0x00, 0x09, L"ローカル・地域" },
	{ 0x00, 0x0A, L"交通" },
	{ 0x00, 0x0F, L"その他" },
	{ 0x01, 0x00, L"スポーツニュース" },
	{ 0x01, 0x01, L"野球" },
	{ 0x01, 0x02, L"サッカー" },
	{ 0x01, 0x03, L"ゴルフ" },
	{ 0x01, 0x04, L"その他の球技" },
	{ 0x01, 0x05, L"相撲・格闘技" },
	{ 0x01, 0x06, L"オリンピック・国際大会" },
	{ 0x01, 0x07, L"マラソン・陸上・水泳" },
	{ 0x01, 0x08, L"モータースポーツ" },
	{ 0x01, 0x09, L"マリン・ウィンタースポーツ" },
	{ 0x01, 0x0A, L"競馬・公営競技" },
	{ 0x01, 0x0F, L"その他" },
	{ 0x02, 0x00, L"芸能・ワイドショー" },
	{ 0x02, 0x01, L"ファッション" },
	{ 0x02, 0x02, L"暮らし・住まい" },
	{ 0x02, 0x03, L"健康・医療" },
	{ 0x02, 0x04, L"ショッピング・通販" },
	{ 0x02, 0x05, L"グルメ・料理" },
	{ 0x02, 0x06, L"イベント" },
	{ 0x02, 0x07, L"番組紹介・お知らせ" },
	{ 0x02, 0x0F, L"その他" },
	{ 0x03, 0x00, L"国内ドラマ" },
	{ 0x03, 0x01, L"海外ドラマ" },
	{ 0x03, 0x02, L"時代劇" },
	{ 0x03, 0x0F, L"その他" },
	{ 0x04, 0x00, L"国内ロック・ポップス" },
	{ 0x04, 0x01, L"海外ロック・ポップス" },
	{ 0x04, 0x02, L"クラシック・オペラ" },
	{ 0x04, 0x03, L"ジャズ・フュージョン" },
	{ 0x04, 0x04, L"歌謡曲・演歌" },
	{ 0x04, 0x05, L"ライブ・コンサート" },
	{ 0x04, 0x06, L"ランキング・リクエスト" },
	{ 0x04, 0x07, L"カラオケ・のど自慢" },
	{ 0x04, 0x08, L"民謡・邦楽" },
	{ 0x04, 0x09, L"童謡・キッズ" },
	{ 0x04, 0x0A, L"民族音楽・ワールドミュージック" },
	{ 0x04, 0x0F, L"その他" },
	{ 0x05, 0x00, L"クイズ" },
	{ 0x05, 0x01, L"ゲーム" },
	{ 0x05, 0x02, L"トークバラエティ" },
	{ 0x05, 0x03, L"お笑い・コメディ" },
	{ 0x05, 0x04, L"音楽バラエティ" },
	{ 0x05, 0x05, L"旅バラエティ" },
	{ 0x05, 0x06, L"料理バラエティ" },
	{ 0x05, 0x0F, L"その他" },
	{ 0x06, 0x00, L"洋画" },
	{ 0x06, 0x01, L"邦画" },
	{ 0x06, 0x02, L"アニメ" },
	{ 0x06, 0x0F, L"その他" },
	{ 0x07, 0x00, L"国内アニメ" },
	{ 0x07, 0x01, L"海外アニメ" },
	{ 0x07, 0x02, L"特撮" },
	{ 0x07, 0x0F, L"その他" },
	{ 0x08, 0x00, L"社会・時事" },
	{ 0x08, 0x01, L"歴史・紀行" },
	{ 0x08, 0x02, L"自然・動物・環境" },
	{ 0x08, 0x03, L"宇宙・科学・医学" },
	{ 0x08, 0x04, L"カルチャー・伝統文化" },
	{ 0x08, 0x05, L"文学・文芸" },
	{ 0x08, 0x06, L"スポーツ" },
	{ 0x08, 0x07, L"ドキュメンタリー全般" },
	{ 0x08, 0x08, L"インタビュー・討論" },
	{ 0x08, 0x0F, L"その他" },
	{ 0x09, 0x00, L"現代劇・新劇" },
	{ 0x09, 0x01, L"ミュージカル" },
	{ 0x09, 0x02, L"ダンス・バレエ" },
	{ 0x09, 0x03, L"落語・演芸" },
	{ 0x09, 0x04, L"歌舞伎・古典" },
	{ 0x09, 0x0F, L"その他" },
	{ 0x0A, 0x00, L"旅・釣り・アウトドア" },
	{ 0x0A, 0x01, L"園芸・ペット・手芸" },
	{ 0x0A, 0x02, L"音楽・美術・工芸" },
	{ 0x0A, 0x03, L"囲碁・将棋" },
	{ 0x0A, 0x04, L"麻雀・パチンコ" },
	{ 0x0A, 0x05, L"車・オートバイ" },
	{ 0x0A, 0x06, L"コンピュータ・ＴＶゲーム" },
	{ 0x0A, 0x07, L"会話・語学" },
	{ 0x0A, 0x08, L"幼児・小学生" },
	{ 0x0A, 0x09, L"中学生・高校生" },
	{ 0x0A, 0x0A, L"大学生・受験" },
	{ 0x0A, 0x0B, L"生涯教育・資格" },
	{ 0x0A, 0x0C, L"教育問題" },
	{ 0x0A, 0x0F, L"その他" },
	{ 0x0B, 0x00, L"高齢者" },
	{ 0x0B, 0x01, L"障害者" },
	{ 0x0B, 0x02, L"社会福祉" },
	{ 0x0B, 0x03, L"ボランティア" },
	{ 0x0B, 0x04, L"手話" },
	{ 0x0B, 0x05, L"文字（字幕）" },
	{ 0x0B, 0x06, L"音声解説" },
	{ 0x0E, 0x0F, L"その他" },
	{ 0x0E, 0x00, L"BS/地上デジタル放送用番組付属情報" },
	{ 0x0E, 0x01, L"広帯域CS デジタル放送用拡張" },
	{ 0x0E, 0x02, L"衛星デジタル音声放送用拡張" },
	{ 0x0E, 0x03, L"サーバー型番組付属情報" },
	{ 0x0E, 0x04, L"IP 放送用番組付属情報" }
} };

struct CountryAvailabilityDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;

	int8_t country_availability_flag;
	std::vector<int32_t> country_code;

	bool parse(const uint8_t* p);
};

struct SateliteDeliverySystemDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;

	double frequency;
	double orbital_position;
	int8_t west_east_flag;
	int8_t polarisation;
	int8_t modulation;

	double symbol_rate;
	int8_t FEC_inner;

	bool parse(const uint8_t* p);
};

struct ExtendedEventDescriptor
{
	struct ItemData
	{
		uint8_t              item_description_length;
		std::vector<uint8_t> item_description_chars;
		uint8_t              item_length;
		std::vector<uint8_t> item_chars;
	};

	uint8_t descriptor_tag;
	uint8_t descriptor_length;

	uint8_t descriptor_number;
	uint8_t last_descriptor_number;
	uint8_t country_availability_flag;

	int32_t ISO_639_language_code;
	uint8_t length_of_items;

	std::vector<ItemData> item_list;

	uint8_t text_length;
	std::vector<uint8_t> text_chars;

	bool parse(const uint8_t* p);
};

struct LinkageDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;

	uint16_t transport_stream_id;
	uint16_t original_network_id;
	int16_t  service_id;
	uint8_t  linkage_type;

	std::vector<uint8_t> private_data_bytes;

	bool parse(const uint8_t* p);
};

struct MosaicDescriptor
{
	struct CellData
	{
		uint8_t  logical_cell_id;
		uint8_t  logical_cell_presentation_info;
		uint8_t  elementary_cell_field_length;
		std::vector<uint8_t> elementary_cell_id;
		uint8_t  cell_linkage_info;
		uint16_t bouquet_id;
		uint16_t original_network_id;
		uint16_t transport_stream_id;
		uint16_t service_id;
		uint16_t event_id;
	};

	uint8_t descriptor_tag;
	uint8_t descriptor_length;

	int8_t  mosaic_entry_point;
	uint8_t number_of_horizontal_elementary_cells;
	uint8_t number_of_vertical_elementary_cells;

	std::vector<CellData> cell_list;

	uint8_t text_length;
	std::vector<uint8_t> text_chars;

	bool parse(const uint8_t* p);
};

struct NVODReferenceDescriptor
{
	struct ServiceData
	{
		uint16_t transport_stream_id;
		uint16_t original_network_id;
		uint16_t service_id;
	};

	uint8_t descriptor_tag;
	uint8_t descriptor_length;

	std::vector<ServiceData> service_list;

	bool parse(const uint8_t* p);
};

struct NetworkNameDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;

	std::vector<uint16_t> network_name_chars;

	bool parse(const uint8_t* p);
};

struct ParentalRatingDescriptor
{
	struct RatingData
	{
		uint32_t country_code;
		uint8_t Rating;
	};
	uint8_t descriptor_tag;
	uint8_t descriptor_length;

	std::vector<RatingData> rating_list;

	bool parse(const uint8_t* p);
};

struct ServiceTypeTable
{
	uint8_t service_type;
	wchar_t* statement;
};

constexpr std::array<ServiceTypeTable, 0x100>
service_type_table = { {
	{ 0x00, L"未定義" },
	{ 0x01, L"デジタルTVサービス" },
	{ 0x02, L"デジタル音声サービス" },
	{ 0xA1, L"臨時映像サービス" },
	{ 0xA2, L"臨時音声サービス" },
	{ 0xA3, L"臨時データサービス" },
	{ 0xA4, L"エンジニアリングサービス" },
	{ 0xA5, L"プロモーション映像サービス" },
	{ 0xA6, L"プロモーション音声サービス" },
	{ 0xA7, L"プロモーションデータサービス" },
	{ 0xA8, L"事前蓄積用データサービス" },
	{ 0xA9, L"蓄積専用データサービス" },
	{ 0xAA, L"ブックマーク一覧データサービス" },
	{ 0xAD, L"超高精細度4K専用TVサービス" },
	{ 0xC0, L"データサービス" },
	{ 0xC1, L"TLVを用いた蓄積型サービス" },
	{ 0xC2, L"マルチメディアサービス" }
} };

struct ServiceDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	uint8_t service_type;
	uint8_t service_provider_name_length;
	std::vector<uint8_t> service_provider_name_chars;

	uint8_t service_name_length;
	std::vector<uint8_t> service_name_chars;

	bool parse(const uint8_t* p);
};

struct ServiceListDescriptor
{
	struct ServiceData
	{
		uint16_t service_id;
		uint8_t  service_type;
	};

	uint8_t descriptor_tag;
	uint8_t descriptor_length;

	std::vector<ServiceData> service_list;

	bool parse(const uint8_t* p);
};

struct ShortEventDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	int32_t ISO_639_language_code;
	uint8_t event_name_length;

	std::vector<uint8_t> event_name_chars;

	uint8_t text_length;
	std::vector<uint8_t> text_chars;

	bool parse(const uint8_t* p);
};

struct StreamIdentifierDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	uint8_t component_tag;

	bool parse(const uint8_t* p);
};

struct SttuffingDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	std::vector<uint8_t> stuffing_bytes;

	bool parse(const uint8_t* p);
};

struct TimeShiftedEventDescriptor
{
	uint8_t  descriptor_tag;
	uint8_t  descriptor_length;
	uint16_t reference_service_id;
	uint16_t reference_event_id;

	bool parse(const uint8_t* p);
};

struct TimeShiftedServiceDescriptor
{
	uint8_t  descriptor_tag;
	uint8_t  descriptor_length;
	uint16_t reference_service_id;

	bool parse(const uint8_t* p);
};

struct DataComponentDescriptor
{
	uint8_t  descriptor_tag;
	uint8_t  descriptor_length;
	uint16_t data_component_id;
	std::vector<uint8_t> additional_data_component_info;

	bool parse(const uint8_t* p);
};

struct SystemManagementDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	uint16_t system_management_id;
	std::vector<uint8_t> additional_identification_info;

	bool parse(const uint8_t* p);
};

struct HierarchicalTransmissionDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	uint8_t  quality_level;
	uint16_t reference_PID;

	bool parse(const uint8_t* p);
};

struct DigitalCopyControlDescriptor
{
	struct ComponentControlData
	{
		uint8_t component_tag;
		int8_t  digital_recording_control_data;
		int8_t  maximum_bitrate_flag;
		int8_t  user_defined;
		uint8_t maximum_bitrate;
	};

	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	int8_t  digital_recording_control_data;
	int8_t  maximum_bitrate_flag;
	int8_t  component_control_flag;
	uint8_t maximum_bitrate;
	uint8_t component_control_length;
	std::vector<ComponentControlData> component_ctrl_list;

	bool parse(const uint8_t* p);
};

struct EmergencyInformationDescriptor
{
	struct EmergencyData
	{
		uint16_t service_id;
		int8_t   start_end_flag;
		int8_t   signal_level;
		uint8_t  area_code_length;

		std::vector<uint16_t> area_code;
	};

	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	std::vector<EmergencyData> emergency_list;

	bool parse(const uint8_t* p);
};

struct LocalTimeOffsetDescriptor
{
	struct LocalTimeOffsetData
	{
		int32_t country_code;
		int8_t  country_region_id;
		int8_t  local_time_offset_polarity;
		int16_t local_time_offset;
		int64_t time_of_change;
		int16_t next_time_offset;
	};

	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	std::vector<LocalTimeOffsetData> lto_list;

	bool parse(const uint8_t* p);
};

struct AudioComponentDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	uint8_t stream_content;
	uint8_t component_type;
	uint8_t component_tag;
	uint8_t stream_type;
	int8_t  simulcast_group_tag;
	int8_t  ES_multi_lingual_flag;
	int8_t  main_component_flag;
	int8_t  quality_indicator;
	uint8_t sampling_rate;
	int32_t ISO_639_language_code;
	int32_t ISO_639_language_code_2;
	std::vector<uint8_t> text_chars;

	bool parse(const uint8_t* p);
};

constexpr struct
{
	uint8_t component_type_b7;
	wchar_t* statement;
} dialog_ctrl_table[] = {
	{ 0b0, L"音声ストリームがダイアログ制御情報を含まない" },
	{ 0b1, L"音声ストリームがダイアログ制御情報を含む" }
};

constexpr struct
{
	int8_t quality_indicator;
	wchar_t* statement;
} quality_indicator_table[] = {
	{ 0b00, L"将来使用のためリザーブ" },
	{ 0b01, L"モード１" },
	{ 0b10, L"モード２" },
	{ 0b11, L"モード３" }
};

constexpr struct
{
	uint8_t sampling_rate;
	wchar_t* statement;
} sampling_rate_table[] = {
	{ 0b000, L"将来使用のためリザーブ" },
	{ 0b001, L"16kHz" },
	{ 0b010, L"22.05kHz" },
	{ 0b011, L"24kHz" },
	{ 0b100, L"リザーブ" },
	{ 0b101, L"32kHz" },
	{ 0b110, L"44.1kHz" },
	{ 0b111, L"48kHz" }
};

struct TargetRegionDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	uint8_t region_spec_type;

	bool parse(const uint8_t* p);
};

struct DataContentDescriptor
{
	uint8_t  descriptor_tag;
	uint8_t  descriptor_length;
	uint16_t data_component_id;
	uint8_t  entry_component;
	uint8_t  select_length;
	std::vector<uint8_t> selector_bytes;
	uint8_t num_of_component_ref;
	std::vector<uint8_t> component_refs;
	int32_t ISO_639_language_code;
	uint8_t text_length;
	std::vector<uint8_t> text_chars;

	bool parse(const uint8_t* p);
};

struct HyperlinkDescriptor
{
	struct SelectorData
	{
		uint16_t original_network_id;
		uint16_t transport_stream_id;
		uint16_t service_id;
		uint16_t event_id;
		uint8_t  component_tag;
		uint16_t moduleId;
		uint32_t content_id;
		uint16_t information_provider_id;
		uint16_t event_relation_id;
		uint16_t node_id;
		std::vector<uint8_t> uri_chars;
	};

	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	uint8_t hyper_linkage_type;
	uint8_t link_destination_type;
	uint8_t selector_length;
	std::vector<SelectorData> selector_bytes;
	std::vector<uint8_t> private_data;

	bool parse(const uint8_t* p);
};

struct VideoDecodeControlDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	int8_t  still_picture_flag;
	int8_t  sequence_end_code_flag;
	int8_t  video_encode_format;
	int8_t  transfer_characteristics;

	bool parse(const uint8_t* p);
};

struct TerrestrialDeliberySystemDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	int16_t area_code;
	int8_t  guard_interval;
	int8_t  transmission_mode;
	std::vector<uint16_t> frequencies;

	bool parse(const uint8_t* p);
};

struct PartialReceptionDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	std::vector<uint16_t> service_ids;

	bool parse(const uint8_t* p);
};

struct SeriesDescriptor
{
	uint8_t  descriptor_tag;
	uint8_t  descriptor_length;
	uint16_t series_id;
	uint8_t  repeat_label;
	uint8_t  program_pattern;
	uint8_t  expire_date_valid_flag;
	uint16_t expire_date;
	uint16_t episode_number;
	uint16_t last_episode_number;

	std::vector<uint16_t> series_name_chars;

	bool parse(const uint8_t* p);
};

struct EventGroupDescriptor
{
	struct EventGroupData
	{
		uint16_t service_id;
		uint16_t event_id;
	};
	struct EventGroupData2
	{
		uint16_t original_network_id;
		uint16_t transport_stream_id;
		uint16_t service_id;
		uint16_t event_id;
	};

	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	uint8_t group_type;
	uint8_t event_count;

	std::vector<EventGroupData>  event_group_list;
	std::vector<EventGroupData2> event_group2_list;

	std::vector<uint8_t> private_data_bytes;

	bool parse(const uint8_t* p);
};

struct SIParameterDescriptor
{
	struct TableData
	{
		uint8_t table_id;
		uint8_t table_description_length;
		std::vector<uint8_t> table_description_bytes;
	};

	uint8_t  descriptor_tag;
	uint8_t  descriptor_length;
	uint8_t  parameter_version;
	uint16_t update_time;

	std::vector<TableData> table_list;

	bool parse(const uint8_t* p);
};

struct BroadcasterNameDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;

	std::vector<uint8_t> broadcaster_name_chars;

	bool parse(const uint8_t* p);
};

struct ComponentGroupDescriptor
{
	struct CAUnitData
	{
		uint8_t CA_unit_id;
		uint8_t num_of_component;
		std::vector<uint8_t> component_tag;
	};

	struct GroupData
	{
		uint8_t component_group_id;
		uint8_t num_of_CA_unit;
		std::vector<CAUnitData> CA_unit_list;
		uint8_t total_bitrate;
		uint8_t text_length;
		std::vector<uint8_t> text_chars;
	};

	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	uint8_t component_group_type;
	uint8_t total_bit_rate_flag;
	uint8_t num_of_group;

	std::vector<GroupData> group_list;

	bool parse(const uint8_t* p);
};

struct SIPrimeTsDescriptor
{
	struct TableDescriptionData
	{
		uint8_t table_id;
		uint8_t table_description_length;
		std::vector<uint8_t> table_description_bytes;
	};

	uint8_t  descriptor_tag;
	uint8_t  descriptor_length;
	uint8_t  parameter_version;
	uint16_t update_time;
	uint16_t SI_prime_ts_network_id;
	uint16_t SI_prime_transport_stream_id;

	std::vector<TableDescriptionData> table_desc_list;

	bool parse(const uint8_t* p);
};

struct BoardInformationDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	uint8_t title_length;
	std::vector<uint8_t> title_chars;
	uint8_t text_length;
	std::vector<uint8_t> text_chars;

	bool parse(const uint8_t* p);
};

struct LDTLinkageDescriptor
{
	struct LDTData
	{
		uint16_t description_id;
		uint8_t description_type;
	};

	uint8_t  descriptor_tag;
	uint8_t  descriptor_length;
	uint16_t original_service_id;
	uint16_t transport_stream_id;
	uint16_t original_network_id;

	std::vector<LDTData> LDT_list;

	bool parse(const uint8_t* p);
};

struct ConnectedTransmissionDescriptor
{
	uint8_t  descriptor_tag;
	uint8_t  descriptor_length;
	uint16_t connected_transmission_group_id;
	int8_t   segment_type;
	int8_t   modulation_type_A;
	int8_t   modulation_type_B;
	int8_t   modulation_type_C;

	std::vector<uint8_t> additional_connected_transmission_info;

	bool parse(const uint8_t* p);
};

struct TSInformationDescriptor
{
	struct TSInfoData
	{
		uint8_t transmission_type_info;
		uint8_t num_of_service;
		std::vector<uint16_t> service_ids;
	};

	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	uint8_t remote_control_key_id;
	uint8_t length_of_ts_name;
	uint8_t transmission_type_count;
	std::vector<uint8_t> ts_name_chars;

	std::vector<TSInfoData> TS_info_list;

	bool parse(const uint8_t* p);
};

struct ExtendedBroadcasterDescriptor
{
	struct BroadcasterData
	{
		uint16_t original_network_id;
		uint8_t  broadcaster_id;
	};

	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	uint8_t broadcaster_type;

	uint16_t terrestrial_broadcaster_id;
	uint8_t  number_of_affiliation_id_loop;
	uint8_t  number_of_broadcaster_id_loop;
	std::vector<uint8_t> affiliation_ids;
	std::vector<BroadcasterData> broadcaster_ids;
	std::vector<uint8_t> private_data_bytes;

	uint16_t terrestrial_sound_broadcaster_id;
	uint8_t  number_of_sound_broadcast_affiliation_id_loop;
	std::vector<uint8_t> sound_broadcast_affiliation_ids;

	bool parse(const uint8_t* p);
};

struct LogoTransmissionDescriptor
{
	uint8_t  descriptor_tag;
	uint8_t  descriptor_length;
	uint8_t  logo_transmission_type;
	uint16_t logo_id;
	uint16_t logo_version;
	uint16_t download_data_id;
	std::vector<uint8_t> logo_chars;

	bool parse(const uint8_t* p);
};

struct ContentAvailabilityDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	int8_t  copy_restriction_mode;
	int8_t  image_constraint_token;
	int8_t  retention_mode;
	int8_t  retention_state;
	int8_t  encryption_mode;

	bool parse(const uint8_t* p);
};

struct CarouselCompatibleCompositeDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	// sub_descriptor()

	bool parse(const uint8_t* p);
};

struct AVCVideoDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	int8_t  profile_idc;
	int8_t  constraint_set0_flag;
	int8_t  constraint_set1_flag;
	int8_t  constraint_set2_flag;
	int8_t  constraint_set3_flag;
	int8_t  constraint_set4_flag;
	int8_t  constraint_set5_flag;
	int8_t  AVC_compatible_flag;
	uint8_t level_idc;
	int8_t  AVC_still_present;
	int8_t  AVC_24_hour_picture_flag;
	int8_t  Frame_Packing_SEI_not_present_flag;

	bool parse(const uint8_t* p);
};

struct AVCTimingAndHRFDescriptor
{
	uint8_t   descriptor_tag;
	uint8_t   descriptor_length;
	int8_t    hrd_management_valid_flag;
	int8_t    picture_and_timing_info_present;
	int8_t    _90kHz_flag;
	uint32_t  N;
	uint32_t  K;
	uint32_t  num_units_in_tick;
	int8_t  fixed_frame_rate_flag;
	int8_t  temporal_poc_flag;
	int8_t  picture_to_display_conversion_flag;

	bool parse(const uint8_t* p);
};

struct ServiceGroupDescriptor
{
	struct ServiceIdData
	{
		uint16_t  primary_service_id;
		uint16_t  secondary_service_id;
	};

	uint8_t  descriptor_tag;
	uint8_t  descriptor_length;
	uint8_t  service_group_type;
	std::vector<ServiceIdData> service_id_list;
	std::vector<uint8_t> private_data_bytes;

	bool parse(const uint8_t* p);
};

struct MPEG4AudioDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	uint8_t MPEG4_audio_profile_and_level;

	bool parse(const uint8_t* p);
};

struct MPEG4AudioExtensionDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	int8_t  ASC_flag;
	uint8_t num_of_loops;
	std::vector<uint8_t> audioProfileLevelIndication;
	uint8_t ASC_size;

	bool parse(const uint8_t* p);
};

struct RegistrationDescriptor
{
	uint8_t  descriptor_tag;
	uint8_t  descriptor_length;
	uint32_t format_indentifier;
	std::vector<uint8_t> additional_identification_info;

	bool parse(const uint8_t* p);
};

struct DataBroadcastIdDescriptor
{
	struct INTData
	{
		uint32_t platform_id;
		uint8_t  action_type;
		int8_t   INT_versioning_flag;
		uint8_t  INT_version;
	};

	struct INTIdSelectorData
	{
		uint8_t platform_id_data_length;
		std::vector<INTData> INT_list;
		std::vector<uint8_t> private_data_bytes;
	};

	uint8_t  descriptor_tag;
	uint8_t  descriptor_length;
	uint16_t data_broadcast_id;
	std::vector<uint8_t> id_selector_bytes;
	std::unique_ptr<INTIdSelectorData> INT_info;

	bool parse(const uint8_t* p);
};

struct AccessControlDescriptor
{
	uint8_t  descriptor_tag;
	uint8_t  descriptor_length;
	uint16_t CA_system_id;
	int8_t   transmission_type;
	uint16_t PID;
	std::vector<uint8_t> private_data_bytes;

	bool parse(const uint8_t* p);
};

struct AreaBroadcastingInformationDescriptor
{
	struct StationData
	{
		uint32_t station_id;
		uint16_t location_code;
		uint8_t  broadcast_signal_format;
		uint8_t  additional_station_info_length;
		std::vector<uint8_t> additional_station_info;
	};

	uint8_t descriptor_tag;
	uint8_t descriptor_length;
	uint8_t num_of_station_point;
	std::vector<StationData> station_list;

	bool parse(const uint8_t* p);
};

struct HEVCDescriptor
{
	uint8_t  descriptor_tag;
	uint8_t  descriptor_length;
	uint8_t  profile_space;
	int8_t   tier_flag;
	uint8_t  profile_idc;
	uint32_t profile_compatibility_indication;
	int8_t   progressive_source_flag;
	int8_t   interlaced_source_flag;
	int8_t   non_packed_constraint_flag;
	int8_t   frame_only_constraint_flag;
	int64_t  reserved_zero_44bits;
	uint8_t  level_idc;
	int8_t   temporal_layer_subset_flag;
	int8_t   HEVC_still_present_flag;
	int8_t   HEVC_24hr_picture_present_flg;

	uint8_t  temporal_id_min;
	uint8_t  temporal_id_max;

	bool parse(const uint8_t* p);
};

struct HierarchyDescriptor
{
	uint8_t descriptor_tag;
	uint8_t descriptor_length;

	int8_t  temporal_scalability_flag;
	int8_t  spatial_scalability_flag;
	int8_t  quality_scalability_flag;
	uint8_t hierarchy_type;

	uint8_t hierarchy_layer_index;
	int8_t  tref_present_flag;

	uint8_t hierarchy_embedded_layer_index;
	uint8_t hierarchy_channel;

	bool parse(const uint8_t* p);
};

struct HybridInformationDescriptor
{
	uint8_t  descriptor_tag;
	uint8_t  descriptor_length;
	int8_t   has_location;
	int8_t   location_type;
	uint8_t  format;

	uint8_t  component_tag;
	uint16_t module_id;

	uint8_t  URL_length;
	std::vector<uint8_t> URL_bytes;

	bool parse(const uint8_t* p);
};

struct DescriptorData
{
	std::unique_ptr<VideoStreamDescriptor>                 video_stream;
	std::unique_ptr<AudioStreamDescriptor>                 audio_stream;
	std::unique_ptr<DataStreamAlignmentDescriptor>         data_stream;
	std::unique_ptr<TargetBackgroundGridDescriptor>        target_bk_grid;
	std::unique_ptr<VideoWindowDescriptor>                 video_window;
	std::unique_ptr<CADescriptor>                          CA;
	std::unique_ptr<ISO639LanguageDescriptor>              ISO_639_language;
	std::unique_ptr<SystemClockDescriptor>                 sys_clock;
	std::unique_ptr<MultiplexBufferUtilizationDescriptor>  mpx_buf_util;
	std::unique_ptr<CopyrightDescriptor>                   copyright;
	std::unique_ptr<MaximumBitrateDescriptor>              max_bitrate;

	std::unique_ptr<BouquetNameDescriptor>                 bouquet_name;
	std::unique_ptr<CAIdentifierDescriptor>                CA_id;
	std::unique_ptr<ComponentDescriptor>                   component;
	std::unique_ptr<ContentDescriptor>                     content;
	std::unique_ptr<CountryAvailabilityDescriptor>         country;
	std::unique_ptr<SateliteDeliverySystemDescriptor>      satelite;
	std::unique_ptr<ExtendedEventDescriptor>               ext_event;
	std::unique_ptr<LinkageDescriptor>                     link;
	std::unique_ptr<MosaicDescriptor>                      mosaic;
	std::unique_ptr<NVODReferenceDescriptor>               NVOD_ref;
	std::unique_ptr<NetworkNameDescriptor>                 network_name;
	std::unique_ptr<ParentalRatingDescriptor>              parental_rating;
	std::unique_ptr<ServiceDescriptor>                     service;
	std::unique_ptr<ServiceListDescriptor>                 service_list;
	std::unique_ptr<ShortEventDescriptor>                  short_event;
	std::unique_ptr<StreamIdentifierDescriptor>            stream_id;
	std::unique_ptr<SttuffingDescriptor>                   stuffing;
	std::unique_ptr<TimeShiftedEventDescriptor>            time_shifted_event;
	std::unique_ptr<TimeShiftedServiceDescriptor>          time_shifted_service;
	std::unique_ptr<DataComponentDescriptor>               data_component;
	std::unique_ptr<SystemManagementDescriptor>            system_management;
	std::unique_ptr<HierarchicalTransmissionDescriptor>    hierarchical_trans;
	std::unique_ptr<DigitalCopyControlDescriptor>          digital_copy_ctrl;
	std::unique_ptr<EmergencyInformationDescriptor>        emergency;
	std::unique_ptr<LocalTimeOffsetDescriptor>             local_time_offset;
	std::unique_ptr<AudioComponentDescriptor>              audio_component;
	std::unique_ptr<TargetRegionDescriptor>                target_region;
	std::unique_ptr<DataContentDescriptor>                 data_content;
	std::unique_ptr<HyperlinkDescriptor>                   hyperlink;
	std::unique_ptr<VideoDecodeControlDescriptor>          video_dec_ctrl;
	std::unique_ptr<TerrestrialDeliberySystemDescriptor>   terrestrial;
	std::unique_ptr<PartialReceptionDescriptor>            partial_reception;
	std::unique_ptr<SeriesDescriptor>                      series;
	std::unique_ptr<EventGroupDescriptor>                  event_group;
	std::unique_ptr<SIParameterDescriptor>                 SI_param;
	std::unique_ptr<BroadcasterNameDescriptor>             broadcaster_name;
	std::unique_ptr<ComponentGroupDescriptor>              component_group;
	std::unique_ptr<SIPrimeTsDescriptor>                   SI_prime_ts;
	std::unique_ptr<BoardInformationDescriptor>            board_info;
	std::unique_ptr<LDTLinkageDescriptor>                  LDT_link;
	std::unique_ptr<ConnectedTransmissionDescriptor>       connected_trans;
	std::unique_ptr<TSInformationDescriptor>               TS_info;
	std::unique_ptr<ExtendedBroadcasterDescriptor>         ext_broadcaster;
	std::unique_ptr<LogoTransmissionDescriptor>            logo_trans;
	std::unique_ptr<ContentAvailabilityDescriptor>         content_availability;
	std::unique_ptr<CarouselCompatibleCompositeDescriptor> carousel;
	std::unique_ptr<AVCVideoDescriptor>                    AVC_video;
	std::unique_ptr<AVCTimingAndHRFDescriptor>             AVC_timing;
	std::unique_ptr<ServiceGroupDescriptor>                service_group;
	std::unique_ptr<MPEG4AudioDescriptor>                  MPEG4_audio;
	std::unique_ptr<MPEG4AudioExtensionDescriptor>         MPEG4_audio_ext;
	std::unique_ptr<RegistrationDescriptor>                registration;
	std::unique_ptr<DataBroadcastIdDescriptor>             data_broadcast_id;
	std::unique_ptr<AccessControlDescriptor>               access_ctrl;
	std::unique_ptr<AreaBroadcastingInformationDescriptor> area;
	std::unique_ptr<HEVCDescriptor>                        HEVC;
	std::unique_ptr<HierarchyDescriptor>                   hierarchy;
	std::unique_ptr<HybridInformationDescriptor>           hybrid;

	DescriptorData() :
		video_stream(nullptr),
		audio_stream(nullptr),
		data_stream(nullptr),
		target_bk_grid(nullptr),
		video_window(nullptr),
		CA(nullptr),
		ISO_639_language(nullptr),
		sys_clock(nullptr),
		mpx_buf_util(nullptr),
		copyright(nullptr),
		max_bitrate(nullptr),
		bouquet_name(nullptr),
		CA_id(nullptr),
		component(nullptr),
		content(nullptr),
		country(nullptr),
		satelite(nullptr),
		ext_event(nullptr),
		link(nullptr),
		mosaic(nullptr),
		NVOD_ref(nullptr),
		network_name(nullptr),
		parental_rating(nullptr),
		service(nullptr),
		service_list(nullptr),
		short_event(nullptr),
		stream_id(nullptr),
		stuffing(nullptr),
		time_shifted_event(nullptr),
		time_shifted_service(nullptr),
		data_component(nullptr),
		system_management(nullptr),
		hierarchical_trans(nullptr),
		digital_copy_ctrl(nullptr),
		emergency(nullptr),
		local_time_offset(nullptr),
		audio_component(nullptr),
		target_region(nullptr),
		data_content(nullptr),
		hyperlink(nullptr),
		video_dec_ctrl(nullptr),
		terrestrial(nullptr),
		partial_reception(nullptr),
		series(nullptr),
		event_group(nullptr),
		SI_param(nullptr),
		broadcaster_name(nullptr),
		component_group(nullptr),
		SI_prime_ts(nullptr),
		board_info(nullptr),
		LDT_link(nullptr),
		connected_trans(nullptr),
		TS_info(nullptr),
		ext_broadcaster(nullptr),
		logo_trans(nullptr),
		content_availability(nullptr),
		carousel(nullptr),
		AVC_video(nullptr),
		AVC_timing(nullptr),
		service_group(nullptr),
		MPEG4_audio(nullptr),
		MPEG4_audio_ext(nullptr),
		registration(nullptr),
		data_broadcast_id(nullptr),
		access_ctrl(nullptr),
		area(nullptr),
		HEVC(nullptr),
		hierarchy(nullptr),
		hybrid(nullptr) {}
};

class Descriptor
{
public:
	bool decode(const uint8_t* data, const uint16_t data_size);

	Descriptor() = default;
	virtual ~Descriptor() = default;

private:
	std::vector<DescriptorData> desc_list;
};

