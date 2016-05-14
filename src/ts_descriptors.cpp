/*
 * Reference: ITU-T Rec. H.222.0 (05/2006)
 *            ARIB STD-B10 v5.7
 */

#include <memory>
#include <vector>
#include "ts_descriptors.h"
#include "ts_common_utils.h"
#include "char_decoder.h"

static inline std::string dec_text(const uint8_t* p, const uint16_t char_length)
{
	static CharDecoder cdec;
	static std::vector<uint8_t> chars;

	chars.assign(p, p + char_length);
	chars.push_back('\0');

	return cdec.decode(chars.data(), char_length);
}

// ITU-T Rec. H.222.0
bool VideoStreamDescriptor::parse(const uint8_t* p)
{
	descriptor_tag = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x02) {
		return false;
	}
	if (descriptor_length < 1) {
		return false;
	}

	multiple_frame_rate_flag   = read_bits<int8_t>(p, 0, 1);
	frame_rate_code            = read_bits<uint8_t>(p, 1, 4);
	MPEG_1_only_flag           = read_bits<int8_t>(p, 5, 1);
	constrained_parameter_flag = read_bits<int8_t>(p, 6, 1);
	still_picture_flag         = read_bits<int8_t>(p++, 7, 1);

	if (MPEG_1_only_flag == 0) {
		profile_and_level_indication = *p++;
		chroma_format             = read_bits<uint8_t>(p, 0, 2);
		frame_rate_extension_flag = read_bits<int8_t>(p++, 2, 1);
	}

	// test
	//for (auto& fr : frame_rate_table) {
	//	if (frame_rate_code == fr.frame_rate_code) {
	//		fprintf(stderr, "Framerate: %lf\n", fr.frame_rate_value);
	//		break;
	//	}
	//}

	return true;
}

bool AudioStreamDescriptor::parse(const uint8_t* p)
{
	descriptor_tag = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x03) {
		return false;
	}
	if (descriptor_length < 1) {
		return false;
	}

	free_format_flag              = read_bits<int8_t>(p, 0, 1);
	ID                            = read_bits<uint8_t>(p, 1, 1);
	layer                         = read_bits<int8_t>(p, 2, 2);
	variable_rate_audio_indicator = read_bits<int8_t>(p, 4, 1);

	return true;
}

bool DataStreamAlignmentDescriptor::parse(const uint8_t* p)
{
	descriptor_length = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x06) {
		return false;
	}
	if (descriptor_length != 1) {
		return false;
	}

	alignment_type    = *p;

	return true;
}

bool TargetBackgroundGridDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x07) {
		return false;
	}
	if (descriptor_length != 4) {
		return false;
	}

	horizontal_size = read_bits<uint16_t>(p++, 0, 14);
	vertical_size   = read_bits<uint16_t>(p, 6, 14);
	p += 2;
	aspect_ratio_information = read_bits<uint8_t>(p, 4, 4);

	return true;
}

bool VideoWindowDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x08) {
		return false;
	}
	if (descriptor_length != 4) {
		return false;
	}

	horizontal_offset = read_bits<uint16_t>(p++, 0, 14);
	vertical_offset   = read_bits<uint16_t>(p, 6, 14);
	p += 2;
	window_priority = read_bits<uint8_t>(p, 4, 4);

	return true;
}

bool CADescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x09) {
		return false;
	}
	if (descriptor_length < 4) {
		return false;
	}

	CA_system_ID = read_bits<uint16_t>(p, 0, 16);
	p += 2;
	CA_PID = read_bits<uint16_t>(p, 3, 13);
	p += 2;
	private_data_byte.assign(p, p + descriptor_length - 4);

	return true;
}

bool ISO639LanguageDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x0A) {
		return false;
	}
	if (!descriptor_length) {
		return false;
	}

	ISO639_info_list.resize(descriptor_length >> 2);
	for (auto& info : ISO639_info_list) {
		info.ISO_639_language_code = read_bits<uint32_t>(p, 0, 24);
		p += 3;
		info.audio_type = *p++;
	}

	return true;
}

bool SystemClockDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x0B) {
		return false;
	}
	if (descriptor_length != 2) {
		return false;
	}

	external_clock_reference_indicator = read_bits<int8_t>(p, 0, 1);
	clock_accuracy_integer  = read_bits<uint8_t>(p++, 2, 6);
	clock_accuracy_exponent = read_bits<uint8_t>(p, 0, 3);

	if (clock_accuracy_integer != 0 && external_clock_reference_indicator != 1) {
		const auto clock_frequency_accuracy =
			clock_accuracy_integer * std::pow(10., -clock_accuracy_exponent); // ppm
		if (clock_frequency_accuracy < 30.) {
			fprintf(stderr, "clock accuracy [%lf] < 30-ppm.\n", clock_frequency_accuracy);
		}
	}

	return true;
}

bool MultiplexBufferUtilizationDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x0C) {
		return false;
	}
	if (descriptor_length != 4) {
		return false;
	}

	bound_valid_flag = read_bits<int8_t>(p, 0, 1);
	LTW_offset_lower_bound = read_bits<uint16_t>(p, 1, 15);
	p += 2;
	LTW_offset_upper_bound = read_bits<uint16_t>(p, 1, 15);

	return true;
}

bool CopyrightDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x0D) {
		return false;
	}
	if (descriptor_length < 4) {
		return false;
	}

	copyright_indicator = read_bits<uint32_t>(p, 0, 32);
	p += 2;
	additional_copyright_info.assign(p, p + descriptor_length - 4);

	return true;
}

bool MaximumBitrateDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x0E) {
		return false;
	}
	if (descriptor_length != 3) {
		return false;
	}

	maximum_bitrate = read_bits<uint32_t>(p, 2, 22);

	return true;
}

// ARIB STD-B10 v5.7
bool BouquetNameDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x47) {
		return false;
	}
	if (!descriptor_length) {
		return false;
	}

	bouquet_name_chars = dec_text(p, descriptor_length);

	return true;
}

bool CAIdentifierDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x53) {
		return false;
	}
	if (!descriptor_length) {
		return false;
	}

	CA_system_ids.resize(descriptor_length >> 1);
	for (auto c : CA_system_ids) {
		c = *p << 8 | *(p + 1);
		p += 2;
	}

	return true;
}

bool ComponentDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x50) {
		return false;
	}
	if (descriptor_length < 6) {
		return false;
	}

	stream_component = *p++ & 0x0f;
	component_type = *p++;
	component_tag = *p++;
	ISO_639_language_code = (*p << 16) | (*(p + 1) << 8) | *(p + 2);
	p += 3;

	text_chars = dec_text(p, descriptor_length);

	// test
	//for (auto& component : component_table) {
	//	if (component.stream_content == stream_component &&
	//		component.component_type == component_type) {
	//		setlocale(LC_ALL, "Japanese");
	//		fwprintf(stderr, L"Component: %s\n", component.statement);
	//		break;
	//	}
	//}

	return true;
}

bool ContentDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x54) {
		return false;
	}
	if (!descriptor_length) {
		return false;
	}

	auto list_length = descriptor_length >> 1;
	nibble_list.resize(list_length);
	for (auto i = 0; i < list_length; i++) {
		NibbleData nibble;
		nibble.content_nibble_level_1 = read_bits<uint8_t>(p, 0, 4);
		nibble.content_nibble_level_2 = read_bits<uint8_t>(p, 4, 4);
		nibble.user_nibble1 = read_bits<uint8_t>(p + 1, 0, 4);
		nibble.user_nibble2 = read_bits<uint8_t>(p + 1, 4, 4);

		nibble_list[i] = nibble;

		p += 2;
	}

	// test
	//for (auto& nibble : nibble_list) {
	//	for (auto n1 : nibble1_table) {
	//		if (nibble.content_nibble_level_1 == n1.content_nibble_level1) {
	//			setlocale(LC_ALL, "Japanese");
	//			fwprintf(stderr, L"Nibble: %s\n", n1.statement);
	//		}
	//	}
	//}

	return true;
}

bool CountryAvailabilityDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x49) {
		return false;
	}
	if (descriptor_length < 1) {
		return false;
	}

	country_availability_flag = read_bits<int8_t>(p++, 0, 1);

	country_code.resize((descriptor_length - 1) / 3);
	for (auto c : country_code) {
		c = *p << 16 | *(p + 1) << 8 | *(p + 2);
		p += 3;
	}

	return true;
}

bool SateliteDeliverySystemDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x43) {
		return false;
	}
	if (descriptor_length != 11) {
		return false;
	}

	frequency = BCD_to_dec<uint32_t>(p, 8) / 100000.;
	p += 4;
	orbital_position = BCD_to_dec<uint32_t>(p, 4) / 10.;
	p += 2;
	west_east_flag = read_bits<int8_t>(p, 0, 1);
	polarisation   = read_bits<int8_t>(p, 1, 2);
	modulation     = read_bits<int8_t>(p++, 3, 5);

	symbol_rate = BCD_to_dec<uint32_t>(p, 7) / 10000.;
	FEC_inner   = read_bits<int8_t>(p, 4, 4);

	return true;
}

bool ExtendedEventDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x4E) {
		return false;
	}
	if (descriptor_length < 6) {
		return false;
	}

	descriptor_number      = read_bits<uint8_t>(p, 0, 4);
	last_descriptor_number = read_bits<uint8_t>(p++, 4, 4);

	ISO_639_language_code = *p << 16 | *(p + 1) << 8 | *(p + 2);
	p += 3;
	length_of_items = *p++;

	for (int i = 0; i < length_of_items;) {
		ItemData item;
		item.item_description_length = *p++;

		item.item_description_chars = dec_text(p, item.item_description_length);
		p += item.item_description_length;

		item.item_length = *p++;

		item.item_chars = dec_text(p, item.item_length);
		p += item.item_length;

		item_list.push_back(item);

		i += 2 + item.item_description_length + item.item_length;
	}

	text_length = *p++;
	text_chars = dec_text(p, text_length);

	return true;
}

bool LinkageDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x4A) {
		return false;
	}
	if (descriptor_length < 7) {
		return false;
	}

	transport_stream_id = *p << 8 | *(p + 1);
	p += 2;
	original_network_id = *p << 8 | *(p + 1);
	p += 2;
	service_id = *p << 8 | *(p + 1);
	p += 2;
	linkage_type = *p++;

	private_data_bytes.assign(p, p + descriptor_length - 7);

	return true;
}

bool MosaicDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x51) {
		return false;
	}
	if (descriptor_length < 1) {
		return false;
	}

	mosaic_entry_point = read_bits<int8_t>(p, 0, 1);
	number_of_horizontal_elementary_cells = read_bits<uint8_t>(p, 1, 3);
	number_of_vertical_elementary_cells   = read_bits<uint8_t>(p++, 5, 3);

	for (auto i = 0; i < descriptor_length - 1; ++i) {
		CellData cell;

		cell.logical_cell_id                = read_bits<uint8_t>(p++, 0, 6);
		cell.logical_cell_presentation_info = read_bits<uint8_t>(p++, 5, 3);
		cell.elementary_cell_field_length   = *p++;
		cell.elementary_cell_id.resize(cell.elementary_cell_field_length);
		for (auto c : cell.elementary_cell_id) {
			c = read_bits<uint8_t>(p++, 2, 6);
		}

		cell.cell_linkage_info = *p++;
		if (cell.cell_linkage_info == 0x01) {
			cell.bouquet_id = *p << 8 | *(p + 1);
			p += 2;
		}
		if (cell.cell_linkage_info == 0x02 ||
			cell.cell_linkage_info == 0x03) {
			cell.original_network_id = *p << 8 | *(p + 1);
			p += 2;
			cell.transport_stream_id = *p << 8 | *(p + 1);
			p += 2;
			cell.service_id = *p << 8 | *(p + 1);
			p += 2;
		}
		if (cell.cell_linkage_info == 0x04) {
			cell.original_network_id = *p << 8 | *(p + 1);
			p += 2;
			cell.transport_stream_id = *p << 8 | *(p + 1);
			p += 2;
			cell.service_id = *p << 8 | *(p + 1);
			p += 2;
			cell.event_id = *p << 8 | *(p + 1);
			p += 2;
		}
		cell_list.push_back(cell);
	}

	return true;
}

bool NVODReferenceDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x4B) {
		return false;
	}
	if (!descriptor_length) {
		return false;
	}

	auto loop_count = descriptor_length / 6;
	service_list.resize(loop_count);
	for (int i = 0; i < loop_count; i++) {
		ServiceData service;
		service.transport_stream_id = *p << 8 | *(p + 1);
		p += 2;
		service.original_network_id = *p << 8 | *(p + 1);
		p += 2;
		service.service_id = *p << 8 | *(p + 1);
		p += 2;

		service_list[i] = service;
	}

	return true;
}

bool NetworkNameDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x40) {
		return false;
	}
	if (!descriptor_length) {
		return false;
	}

	network_name_chars = dec_text(p, descriptor_length);
	//fprintf(stderr, "%s\n", network_name_chars.c_str());

	return true;
}

bool ParentalRatingDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x55) {
		return false;
	}

	auto loop_count = descriptor_length >> 2;
	rating_list.resize(loop_count);
	for (int i = 0; i < loop_count; i++) {
		RatingData rating;
		rating.country_code = *p << 16 | *(p + 1) << 8 | *(p + 2);
		p += 3;
		rating.Rating = *p++;

		rating_list[i] = rating;
	}

	return true;
}

bool ServiceDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x48) {
		return false;
	}
	if (descriptor_length < 3) {
		return false;
	}

	service_type = *p++;
	service_provider_name_length = *p++;

	service_provider_name_chars = dec_text(p, service_provider_name_length);
	p += service_provider_name_length;

	service_name_length = *p++;
	service_name_chars = dec_text(p, service_name_length);

	return true;
}

bool ServiceListDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x41) {
		return false;
	}
	if (!descriptor_length) {
		return false;
	}

	auto loop_count = descriptor_length / 3;
	service_list.resize(loop_count);
	for (int i = 0; i < loop_count; i++) {
		ServiceData service;
		service.service_id = *p << 8 | *(p + 1);
		p += 2;
		service.service_type = *p++;

		service_list[i] = service;
	}

	return true;
}

bool ShortEventDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x4D) {
		return false;
	}
	if (descriptor_length < 5) {
		return false;
	}

	ISO_639_language_code = *p << 16 | *(p + 1) << 8 | *(p + 2);
	p += 3;

	event_name_length = *p++;

	event_name_chars = dec_text(p, event_name_length);
	p += event_name_length;

	text_length = *p++;

	text_chars = dec_text(p, text_length);

	return true;
}

bool StreamIdentifierDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x52) {
		return false;
	}
	if (descriptor_length != 1) {
		return false;
	}

	component_tag = *p++;

	return true;
}

bool SttuffingDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x42) {
		return false;
	}
	if (!descriptor_length) {
		return false;
	}

	stuffing_bytes.assign(p, p + descriptor_length);

	return true;
}

bool TimeShiftedEventDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x4F) {
		return false;
	}
	if (descriptor_length != 4) {
		return false;
	}

	reference_service_id = *p << 8 | *(p + 1);
	p += 2;
	reference_event_id = *p << 8 | *(p + 1);
	p += 2;

	return true;
}

bool TimeShiftedServiceDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x4C) {
		return false;
	}
	if (descriptor_length != 2) {
		return false;
	}

	reference_service_id = *p << 8 | *(p + 1);
	p += 2;

	return true;
}

bool DataComponentDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0xFD) {
		return false;
	}
	if (descriptor_length < 2) {
		return false;
	}

	data_component_id = *p << 8 | *(p + 1);
	p += 2;
	additional_data_component_info.assign(
		p, p + descriptor_length - 2);

	return true;
}

bool SystemManagementDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0xFE) {
		return false;
	}
	if (descriptor_length < 2) {
		return false;
	}

	system_management_id = *p << 8 | *(p + 1);
	p += 2;
	additional_identification_info.assign(
		p, p + descriptor_length - 2);

	return true;
}

bool HierarchicalTransmissionDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0xC0) {
		return false;
	}
	if (descriptor_length != 3) {
		return false;
	}

	quality_level     = *p++ & 0x01;
	reference_PID     = read_bits<uint16_t>(p, 3, 13);

	return true;
}

bool DigitalCopyControlDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0xC1) {
		return false;
	}
	if (descriptor_length < 1) {
		return false;
	}

	digital_recording_control_data = read_bits<int8_t>(p, 0, 2);
	maximum_bitrate_flag           = read_bits<int8_t>(p, 2, 1);
	component_control_flag         = read_bits<int8_t>(p++, 3, 1);
	if (maximum_bitrate_flag) {
		maximum_bitrate = *p++;
	}
	if (component_control_flag) {
		component_control_length = *p++;
		for (auto i = 0; i < component_control_length; i++) {
			ComponentControlData ctrl;
			ctrl.component_tag = *p++;
			ctrl.digital_recording_control_data = read_bits<int8_t>(p, 0, 2);
			ctrl.maximum_bitrate_flag           = read_bits<int8_t>(p, 2, 1);
			ctrl.user_defined                   = read_bits<int8_t>(p++, 3, 1);
			if (ctrl.maximum_bitrate_flag) {
				ctrl.maximum_bitrate = *p++;
				i += 1;
			}
			component_ctrl_list.push_back(ctrl);
		}
	}

	return true;
}

bool EmergencyInformationDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0xFC) {
		return false;
	}
	if (!descriptor_length) {
		return false;
	}

	for (auto i = 0; i < descriptor_length; i++) {
		EmergencyData emergency;
		emergency.service_id       = read_bits<uint16_t>(p, 0, 16);
		p += 2;
		emergency.start_end_flag   = read_bits<int8_t>(p, 0, 1);
		emergency.signal_level     = read_bits<int8_t>(p++, 1, 1);
		emergency.area_code_length = *p++;
		emergency.area_code.resize(emergency.area_code_length >> 1);
		for (auto code : emergency.area_code) {
			code = read_bits<uint16_t>(p, 0, 12);
			p += 2;
		}
	}

	return true;
}

bool LocalTimeOffsetDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x58) {
		return false;
	}
	if (!descriptor_length) {
		return false;
	}

	lto_list.resize(descriptor_length / 13);
	for (auto& lto : lto_list) {
		lto.country_code      = read_bits<int32_t>(p, 0, 24);
		p += 3;
		lto.country_region_id = read_bits<int8_t>(p, 0, 6);
		lto.local_time_offset_polarity = read_bits<int8_t>(p++, 7, 1);
		lto.local_time_offset = read_bits<int16_t>(p, 0, 16);
		p += 2;
		lto.time_of_change    = read_bits<uint64_t>(p, 0, 40);
		p += 5;
		lto.next_time_offset  = read_bits<int16_t>(p, 0, 16);
		p += 2;
	}

	return true;
}

bool AudioComponentDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0xC4) {
		return false;
	}
	if (descriptor_length < 9) {
		return false;
	}

	stream_content        = read_bits<uint8_t>(p++, 4, 4);
	component_type        = *p++;
	component_tag         = *p++;
	stream_type           = *p++;
	simulcast_group_tag   = *p++;
	ES_multi_lingual_flag = read_bits<int8_t>(p, 0, 1);
	main_component_flag   = read_bits<int8_t>(p, 1, 1);
	quality_indicator     = read_bits<int8_t>(p, 2, 2);
	sampling_rate         = read_bits<uint8_t>(p++, 4, 3);
	ISO_639_language_code = read_bits<int32_t>(p, 0, 24);
	p += 3;

	auto text_chars_length = 9;
	if (ES_multi_lingual_flag == 1) {
		ISO_639_language_code_2 = read_bits<int32_t>(p, 0, 24);
		p += 3;
		text_chars_length += 3;
	}
	text_chars = dec_text(p, text_chars_length);

	return true;
}

bool TargetRegionDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0xC6) {
		return false;
	}
	if (descriptor_length != 1) {
		return false;
	}

	region_spec_type  = *p++;

	// target_region_spec()

	return true;
}

bool DataContentDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0xC7) {
		return false;
	}
	if (descriptor_length < 9) {
		return false;
	}

	data_component_id = read_bits<uint16_t>(p, 0, 16);
	entry_component   = *p++;
	select_length     = *p++;
	selector_bytes.assign(p, p + select_length);
	p += select_length;

	num_of_component_ref = *p++;
	component_refs.assign(p, p + num_of_component_ref);
	p += num_of_component_ref;

	ISO_639_language_code = read_bits<int32_t>(p, 0, 24);
	p += 3;
	text_length = *p++;
	text_chars.assign(p, p + text_length);

	return true;
}

bool HyperlinkDescriptor::parse(const uint8_t* p)
{
	descriptor_tag        = *p++;
	descriptor_length     = *p++;
	if (descriptor_tag != 0xC5) {
		return false;
	}
	if (descriptor_length < 3) {
		return false;
	}

	hyper_linkage_type    = *p++;
	link_destination_type = *p++;
	selector_length       = *p++;

	for (auto i = 0; i < selector_length;) {
		std::shared_ptr<SelectorData> data;
		// link_service_info()
		if (link_destination_type == 0x01) {
			data->original_network_id = read_bits<uint16_t>(p, 0, 16);
			data->transport_stream_id = read_bits<uint16_t>(p + 2, 0, 16);
			data->service_id          = read_bits<uint16_t>(p + 4, 0, 16);
			p += 6;
			i += 6;
		}
		// link_event_info()
		else if (link_destination_type == 0x02) {
			data->original_network_id = read_bits<uint16_t>(p, 0, 16);
			data->transport_stream_id = read_bits<uint16_t>(p + 2, 0, 16);
			data->service_id          = read_bits<uint16_t>(p + 4, 0, 16);
			data->event_id            = read_bits<uint16_t>(p + 6, 0, 16);
			p += 8;
			i += 8;
		}
		// link_module_info()
		else if (link_destination_type == 0x03) {
			data->original_network_id = read_bits<uint16_t>(p, 0, 16);
			data->transport_stream_id = read_bits<uint16_t>(p + 2, 0, 16);
			data->service_id          = read_bits<uint16_t>(p + 4, 0, 16);
			data->event_id            = read_bits<uint16_t>(p + 6, 0, 16);
			data->component_tag       = read_bits<uint8_t>(p + 8, 0, 8);
			data->moduleId            = read_bits<uint16_t>(p + 9, 0, 16);
			p += 11;
			i += 11;
		}
		// link_content_info()
		else if (link_destination_type == 0x04) {
			data->original_network_id = read_bits<uint16_t>(p, 0, 16);
			data->transport_stream_id = read_bits<uint16_t>(p + 2, 0, 16);
			data->service_id          = read_bits<uint16_t>(p + 4, 0, 16);
			data->content_id          = read_bits<uint32_t>(p + 6, 0, 32);
			p += 10;
			i += 10;
		}
		// link_destination_info()
		else if (link_destination_type == 0x05) {
			data->original_network_id = read_bits<uint16_t>(p, 0, 16);
			data->transport_stream_id = read_bits<uint16_t>(p + 2, 0, 16);
			data->service_id          = read_bits<uint16_t>(p + 4, 0, 16);
			data->content_id          = read_bits<uint32_t>(p + 6, 0, 32);
			data->component_tag       = read_bits<uint8_t>(p + 10, 0, 8);
			data->moduleId            = read_bits<uint16_t>(p + 11, 0, 16);
			p += 13;
			i += 13;
		}
		// link_ert_node_info()
		else if (link_destination_type == 0x06) {
			data->information_provider_id = read_bits<uint16_t>(p, 0, 16);
			data->event_relation_id       = read_bits<uint16_t>(p + 2, 0, 16);
			data->node_id                 = read_bits<uint16_t>(p + 4, 0, 16);
			p += 6;
			i += 6;
		}
		// link_stored_content_info()
		else if (link_destination_type == 0x07) {
			const auto uri_size = selector_length - i;
			// Ref. ARIB STD-B24 Part 2 Section 9
			data->uri_chars.assign(p, p + uri_size);
			p += uri_size;
			i += uri_size;
		}

		if (data) {
			selector_bytes.push_back(*data);
		}
	}

	const auto private_length = descriptor_length - 3 - selector_length;
	private_data.assign(p, p + private_length);

	return true;
}

bool VideoDecodeControlDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0xC8) {
		return false;
	}
	if (descriptor_length != 1) {
		return false;
	}

	still_picture_flag       = read_bits<int8_t>(p, 0, 1);
	sequence_end_code_flag   = read_bits<int8_t>(p, 1, 1);
	video_encode_format      = read_bits<int8_t>(p, 2, 4);
	transfer_characteristics = read_bits<int8_t>(p++, 6, 2);

	return true;
}

bool TerrestrialDeliberySystemDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0xFA) {
		return false;
	}
	if (descriptor_length < 2) {
		return false;
	}

	area_code         = read_bits<int16_t>(p++, 0, 12);
	guard_interval    = read_bits<int8_t>(p, 4, 2);
	transmission_mode = read_bits<int8_t>(p++, 6, 2);
	
	frequencies.resize((descriptor_length - 2) / 2);
	for (auto freq : frequencies) {
		freq = read_bits<uint16_t>(p, 0, 16);
		p += 2;
	}

	return true;
}

bool PartialReceptionDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0xFB) {
		return false;
	}
	if (!descriptor_length) {
		return false;
	}

	service_ids.resize(descriptor_length / 2);
	for (auto service_id : service_ids) {
		service_id = read_bits<uint16_t>(p, 0, 16);
		p += 2;
	}

	return true;
}

bool SeriesDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0xD5) {
		return false;
	}
	if (descriptor_length < 8) {
		return false;
	}

	series_id              = read_bits<uint16_t>(p, 0, 16);
	p += 2;
	repeat_label           = read_bits<uint8_t>(p, 0, 4);
	program_pattern        = read_bits<uint8_t>(p, 4, 3);
	expire_date_valid_flag = read_bits<uint8_t>(p++, 7, 1);
	expire_date            = read_bits<uint16_t>(p, 0, 16);
	p += 2;
	episode_number         = read_bits<uint16_t>(p++, 0, 12);
	last_episode_number    = read_bits<uint16_t>(p, 4, 12);
	p += 2;

	series_name_chars = dec_text(p, descriptor_length - 8);

	return true;
}

bool EventGroupDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0xD6) {
		return false;
	}
	if (descriptor_length < 1) {
		return false;
	}

	group_type  = read_bits<uint8_t>(p, 0, 4);
	event_count = read_bits<uint8_t>(p++, 4, 4);

	event_group_list.resize(event_count);
	for (auto event_group : event_group_list) {
		event_group.service_id = read_bits<uint16_t>(p, 0, 16);
		event_group.event_id   = read_bits<uint16_t>(p + 2, 0, 16);
		p += 4;
	}

	const auto rest_length = descriptor_length - 1 - event_count * 4;
	if (group_type == 4 || group_type == 5) {
		event_group2_list.resize(rest_length / 8);
		for (auto event_group2 : event_group2_list) {
			event_group2.original_network_id = read_bits<uint16_t>(p, 0, 16);
			event_group2.transport_stream_id = read_bits<uint16_t>(p + 2, 0, 16);
			event_group2.service_id = read_bits<uint16_t>(p + 4, 0, 16);
			event_group2.event_id   = read_bits<uint16_t>(p + 6, 0, 16);
			p += 8;
		}
	}
	else {
		private_data_bytes.assign(p,  p + rest_length);
	}

	return true;
}

auto MJD_to_JTC2(uint16_t MJD)
{
	auto Y_prime = (int)((MJD - 15078.2) / 365.25);
	auto M_prime = (int)((MJD - 14956.1 - (int)(Y_prime * 365.25)) / 30.6001);
	auto D = MJD - 14956 - (int)(Y_prime * 365.25) - (int)(M_prime * 30.6001);
	auto K = (M_prime == 14 || M_prime == 15) ? 1 : 0;
	auto Y = Y_prime + K + 1900;
	auto M = M_prime - 1 - K * 12;

	return static_cast<uint32_t>(Y * 10000 + M * 100 + D);
}

bool SIParameterDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0xD7) {
		return false;
	}
	if (descriptor_length < 3) {
		return false;
	}

	parameter_version = *p++;
	update_time       = read_bits<uint16_t>(p, 0, 16);
	const auto JTC = MJD_to_JTC(update_time);
	//fprintf(stderr, "SI param desc: [Update time: %02d:%02d:%02d]\n", JTC.tm_hour, JTC.tm_min, JTC.tm_sec);

	const auto loop_count = descriptor_length - 3;
	for (auto i = 0; i < loop_count;) {
		TableData table;
		table.table_id                 = read_bits<uint8_t>(p++, 0, 8);
		table.table_description_length = read_bits<uint8_t>(p++, 0, 8);
		table.table_description_bytes.assign(
			p, p + table.table_description_length);
		p += table.table_description_length;
		table_list.push_back(table);

		i += 2 + table.table_description_length;
	}

	return true;
}

bool BroadcasterNameDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0xD8) {
		return false;
	}
	if (!descriptor_length) {
		return false;
	}

	broadcaster_name_chars = dec_text(p, descriptor_length);

	return true;
}

bool ComponentGroupDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0xD9) {
		return false;
	}
	if (descriptor_length < 1) {
		return false;
	}

	component_group_type = read_bits<uint8_t>(p, 0, 3);
	total_bit_rate_flag  = read_bits<uint8_t>(p, 3, 1);
	num_of_group         = read_bits<uint8_t>(p++, 4, 4);

	group_list.resize(num_of_group);
	for (auto& group : group_list) {
		group.component_group_id = read_bits<uint8_t>(p, 0, 4);
		group.num_of_CA_unit     = read_bits<uint8_t>(p++, 4, 4);

		group.CA_unit_list.resize(group.num_of_CA_unit);
		for (auto& CA_unit : group.CA_unit_list) {
			CA_unit.CA_unit_id       = read_bits<uint8_t>(p, 0, 4);
			CA_unit.num_of_component = read_bits<uint8_t>(p++, 4, 4);

			CA_unit.component_tag.assign(
				p, p + CA_unit.num_of_component);
			p += CA_unit.num_of_component;
		}
		if (total_bit_rate_flag == 1) {
			group.total_bitrate = *p++;
		}
		group.text_length = *p++;

		group.text_chars = dec_text(p, group.text_length);
		p += group.text_length;
	}

	return true;
}

bool SIPrimeTsDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0xDA) {
		return false;
	}
	if (descriptor_length < 7) {
		return false;
	}

	parameter_version = *p++;
	update_time                  = read_bits<uint16_t>(p, 0, 16);
	SI_prime_ts_network_id       = read_bits<uint16_t>(p + 2, 0, 16);
	SI_prime_transport_stream_id = read_bits<uint16_t>(p + 4, 0, 16);
	p += 6;

	for (auto i = 0; i < descriptor_length - 7;) {
		TableDescriptionData desc;
		desc.table_id = *p++;
		desc.table_description_length = *p++;
		desc.table_description_bytes.assign(
			p, p + desc.table_description_length);
		p += desc.table_description_length;
		table_desc_list.push_back(desc);

		i += 2 + desc.table_description_length;
	}

	return true;
}

bool BoardInformationDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0xDB) {
		return false;
	}
	if (descriptor_length < 2) {
		return false;
	}

	title_length = *p++;
	title_chars = dec_text(p, title_length);
	p += title_length;

	text_length = *p++;
	text_chars = dec_text(p, text_length);

	return true;
}

bool LDTLinkageDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0xDC) {
		return false;
	}
	if (descriptor_length < 6) {
		return false;
	}

	original_service_id = read_bits<uint16_t>(p, 0, 16);
	transport_stream_id = read_bits<uint16_t>(p + 2, 0, 16);
	original_network_id = read_bits<uint16_t>(p + 4, 0, 16);
	p += 6;

	LDT_list.resize((descriptor_length - 6) >> 2);
	for (auto& ldt : LDT_list) {
		ldt.description_id   = read_bits<uint16_t>(p, 0, 16);
		ldt.description_type = read_bits<uint8_t>(p + 2, 4, 4);
		p += 4;
	}

	return true;
}

bool ConnectedTransmissionDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0xDD) {
		return false;
	}
	if (descriptor_length < 3) {
		return false;
	}

	connected_transmission_group_id = read_bits<uint16_t>(p, 0, 16);
	p += 2;
	segment_type      = read_bits<int8_t>(p, 0, 2);
	modulation_type_A = read_bits<int8_t>(p, 2, 2);
	modulation_type_B = read_bits<int8_t>(p, 4, 2);
	modulation_type_C = read_bits<int8_t>(p, 6, 2);

	additional_connected_transmission_info.assign(
		p, p + descriptor_length - 3);

	return true;
}

bool TSInformationDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0xCD) {
		return false;
	}
	if (descriptor_length < 2) {
		return false;
	}

	remote_control_key_id   = *p++;
	length_of_ts_name       = read_bits<uint8_t>(p, 0, 6);
	transmission_type_count = read_bits<uint8_t>(p++, 6, 2);
	ts_name_chars = dec_text(p, length_of_ts_name);
	p += length_of_ts_name;
	TS_info_list.resize(transmission_type_count);
	for (auto info : TS_info_list) {
		info.transmission_type_info = *p++;
		info.num_of_service = *p++;
		info.service_ids.resize(info.num_of_service);
		for (auto id : info.service_ids) {
			read_bits<uint16_t>(p, 0, 16);
			p += 2;
		}
	}

	return true;
}

bool ExtendedBroadcasterDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0xCE) {
		return false;
	}
	if (descriptor_length < 1) {
		return false;
	}

	broadcaster_type = read_bits<uint8_t>(p++, 0, 4);

	if (broadcaster_type == 0x1) {
		terrestrial_broadcaster_id = read_bits<uint16_t>(p, 0, 16);
		p += 2;
		number_of_affiliation_id_loop = read_bits<uint8_t>(p, 0, 4);
		number_of_broadcaster_id_loop = read_bits<uint8_t>(p++, 4, 4);

		affiliation_ids.assign(
			p, p + number_of_affiliation_id_loop);
		p += number_of_affiliation_id_loop;

		broadcaster_ids.resize(number_of_broadcaster_id_loop);
		for (auto& id : broadcaster_ids) {
			id.original_network_id = read_bits<uint16_t>(p, 0, 16);
			p += 2;
			id.broadcaster_id = *p++;
		}

		const auto byte_length =
			descriptor_length
			- 4
			- number_of_affiliation_id_loop
			- number_of_broadcaster_id_loop * 3;
		private_data_bytes.assign(p, p + byte_length);
	}
	else if (broadcaster_type == 0x2) {
		terrestrial_broadcaster_id = read_bits<uint16_t>(p, 0, 16);
		p += 2;
		number_of_sound_broadcast_affiliation_id_loop = read_bits<uint8_t>(p, 0, 4);
		number_of_broadcaster_id_loop = read_bits<uint8_t>(p++, 4, 4);

		sound_broadcast_affiliation_ids.assign(
			p, p + number_of_sound_broadcast_affiliation_id_loop);
		p += number_of_sound_broadcast_affiliation_id_loop;

		broadcaster_ids.resize(number_of_broadcaster_id_loop);
		for (auto& id : broadcaster_ids) {
			id.original_network_id = read_bits<uint16_t>(p, 0, 16);
			p += 2;
			id.broadcaster_id = *p++;
		}

		const auto byte_length =
			descriptor_length
			- 4
			- number_of_affiliation_id_loop
			- number_of_broadcaster_id_loop * 3;
		private_data_bytes.assign(p, p + byte_length);
	}
	else {}

	return true;
}

bool LogoTransmissionDescriptor::parse(const uint8_t* p)
{
	descriptor_tag         = *p++;
	descriptor_length      = *p++;
	if (descriptor_tag != 0xCF) {
		return false;
	}
	if (descriptor_length < 1) {
		return false;
	}

	logo_transmission_type = *p++;
	if (logo_transmission_type == 0x01) {
		logo_id          = read_bits<uint16_t>(p, 7, 9);
		logo_version     = read_bits<uint16_t>(p + 2, 4, 12);
		download_data_id = read_bits<uint16_t>(p + 4, 0, 16);
	}
	else if (logo_transmission_type == 0x02) {
		logo_id = read_bits<uint16_t>(p, 7, 9);
	}
	else if (logo_transmission_type == 0x03) {
		// 8-unit code character string
		logo_chars.assign(p, p + descriptor_length - 1);
		logo_chars.push_back('\0');

		CharDecoder cdec;
		cdec.decode(&logo_chars[0], logo_chars.size());
	}
	else {}

	return true;
}

bool ContentAvailabilityDescriptor::parse(const uint8_t* p)
{
	descriptor_tag         = *p++;
	descriptor_length      = *p++;
	if (descriptor_tag != 0xDE) {
		return false;
	}
	if (descriptor_length < 1) {
		return false;
	}

	copy_restriction_mode  = read_bits<int8_t>(p, 1, 1);
	image_constraint_token = read_bits<int8_t>(p, 2, 1);
	retention_mode         = read_bits<int8_t>(p, 3, 1);
	retention_state        = read_bits<int8_t>(p, 4, 3);
	encryption_mode        = read_bits<int8_t>(p, 7, 1);

	return true;
}

bool CarouselCompatibleCompositeDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0xF7) {
		return false;
	}

	// sub_descriptor()

	return true;
}

bool AVCVideoDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x28) {
		return false;
	}
	if (descriptor_length != 4) {
		return false;
	}

	profile_idc       = *p++;

	constraint_set0_flag = read_bits<int8_t>(p, 0, 1);
	constraint_set1_flag = read_bits<int8_t>(p, 1, 1);
	constraint_set2_flag = read_bits<int8_t>(p, 2, 1);
	constraint_set3_flag = read_bits<int8_t>(p, 3, 1);
	constraint_set4_flag = read_bits<int8_t>(p, 4, 1);
	constraint_set5_flag = read_bits<int8_t>(p, 5, 1);
	AVC_compatible_flag  = read_bits<int8_t>(p, 6, 2);
	level_idc = *p++;
	AVC_still_present        = read_bits<int8_t>(p, 0, 1);
	AVC_24_hour_picture_flag = read_bits<int8_t>(p, 1, 1);
	Frame_Packing_SEI_not_present_flag = read_bits<int8_t>(p, 2, 1);

	return true;
}

bool AVCTimingAndHRFDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x2A) {
		return false;
	}
	if (descriptor_length < 2) {
		return false;
	}

	hrd_management_valid_flag       = read_bits<int8_t>(p, 0, 1);
	picture_and_timing_info_present = read_bits<int8_t>(p++, 7, 1);
	if (picture_and_timing_info_present == 1) {
		_90kHz_flag = read_bits<int8_t>(p++, 0, 1);
		if (_90kHz_flag == 0) {
			N = read_bits<uint32_t>(p, 0, 32);
			K = read_bits<uint32_t>(p + 4, 0, 32);
			p += 8;
		}
		num_units_in_tick = read_bits<uint32_t>(p, 0, 32);
	}
	fixed_frame_rate_flag = read_bits<int8_t>(p, 0, 1);
	temporal_poc_flag     = read_bits<int8_t>(p, 1, 1);
	picture_to_display_conversion_flag = read_bits<int8_t>(p, 2, 1);

	return true;
}

bool ServiceGroupDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0xE0) {
		return false;
	}
	if (descriptor_length < 1) {
		return false;
	}

	service_group_type = read_bits<uint8_t>(p++, 0, 4);

	if (service_group_type == 1) {
		const auto list_size = (descriptor_length - 1) / 4;
		service_id_list.resize(list_size);
		for (auto& id : service_id_list) {
			id.primary_service_id   = read_bits<uint16_t>(p, 0, 16);
			id.secondary_service_id = read_bits<uint16_t>(p + 2, 0, 16);
			p += 4;
		}
	}
	else {
		const auto byte_size = descriptor_length - 1;
		private_data_bytes.assign(p, p + byte_size);
	}

	return true;
}

bool MPEG4AudioDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x1C) {
		return false;
	}
	if (descriptor_length != 1) {
		return false;
	}

	MPEG4_audio_profile_and_level = *p++;

	return true;
}

bool MPEG4AudioExtensionDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x2E) {
		return false;
	}
	if (descriptor_length < 1) {
		return false;
	}

	ASC_flag     = read_bits<int8_t>(p, 0, 1);
	num_of_loops = read_bits<uint8_t>(p++, 4, 4);
	audioProfileLevelIndication.resize(num_of_loops);
	for (auto indication : audioProfileLevelIndication) {
		indication = *p++;
	}
	if (ASC_flag == 1) {
		ASC_size = *p++;
		// audioSpecificConfig()
	}

	return true;
}

bool RegistrationDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x05) {
		return false;
	}
	if (descriptor_length < 4) {
		return false;
	}

	format_indentifier = read_bits<uint32_t>(p, 0, 32);
	p += 4;

	additional_identification_info.assign(p, p + descriptor_length - 4);

	return true;
}

bool DataBroadcastIdDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x66) {
		return false;
	}
	if (descriptor_length < 2) {
		return false;
	}

	data_broadcast_id = read_bits<uint16_t>(p, 0, 16);
	p += 2;
	if (data_broadcast_id == 0x000B) {
		// IP/MAC_notification_info()s
		INT_info = std::unique_ptr<INTIdSelectorData>(new INTIdSelectorData);
		INT_info->platform_id_data_length = *p++;
		for (auto j = 0; j < INT_info->platform_id_data_length; j += 5) {
			INTData INT;
			INT.platform_id         = read_bits<uint32_t>(p, 0, 32);
			INT.action_type         = read_bits<uint8_t>(p + 4, 0, 8);
			INT.INT_versioning_flag = read_bits<int8_t>(p + 5, 2, 1);
			INT.INT_version         = read_bits<uint8_t>(p + 5, 3, 5);
			p += 5;

			INT_info->INT_list.push_back(INT);
		}
		const auto byte_size = descriptor_length - 3 - INT_info->platform_id_data_length;
		INT_info->private_data_bytes.assign(p, p + byte_size);
	}
	else {
		const auto byte_size = descriptor_length - 2;
		id_selector_bytes.assign(p, p + byte_size);
	}

	return true;
}

bool AccessControlDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0xF6) {
		return false;
	}
	if (descriptor_length < 4) {
		return false;
	}

	CA_system_id      = read_bits<uint16_t>(p, 0, 16);
	p += 2;
	transmission_type = read_bits<int8_t>(p, 0, 3);
	PID               = read_bits<uint16_t>(p, 3, 13);
	p += 2;

	const auto byte_size = descriptor_length - 4;
	private_data_bytes.assign(p, p + byte_size);

	return true;
}

bool AreaBroadcastingInformationDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0xE1) {
		return false;
	}
	if (descriptor_length < 1) {
		return false;
	}

	num_of_station_point = *p++;
	station_list.resize(num_of_station_point);
	for (auto& item : station_list) {
		item.station_id                     = read_bits<uint32_t>(p, 0, 24);
		item.location_code                  = read_bits<uint16_t>(p + 3, 0, 16);
		item.broadcast_signal_format        = read_bits<uint8_t>(p + 5, 0, 8);
		item.additional_station_info_length = read_bits<uint8_t>(p + 6, 0, 8);
		p += 7;

		item.additional_station_info.assign(
			p, p + item.additional_station_info_length);
		p += item.additional_station_info_length;
	}

	return true;
}

bool HEVCDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x38) {
		return false;
	}
	if (descriptor_length < 13) {
		return false;
	}

	profile_space = read_bits<uint8_t>(p, 0, 2);
	tier_flag     = read_bits<int8_t>(p, 2, 1);
	profile_idc   = read_bits<uint8_t>(p++, 3, 5);
	profile_compatibility_indication = read_bits<int32_t>(p, 0, 32);
	p += 4;
	progressive_source_flag    = read_bits<int8_t>(p, 0, 1);
	interlaced_source_flag     = read_bits<int8_t>(p, 1, 1);
	non_packed_constraint_flag = read_bits<int8_t>(p, 2, 1);
	frame_only_constraint_flag = read_bits<int8_t>(p, 3, 1);
	reserved_zero_44bits = read_bits<int64_t>(p, 4, 44);
	p += 6;
	level_idc = *p++;
	temporal_layer_subset_flag    = read_bits<int8_t>(p, 0, 1);
	HEVC_still_present_flag       = read_bits<int8_t>(p, 1, 1);
	HEVC_24hr_picture_present_flg = read_bits<int8_t>(p++, 2, 1);

	if (temporal_layer_subset_flag == 1) {
		temporal_id_min = read_bits<uint8_t>(p++, 5, 3);
		temporal_id_max = read_bits<uint8_t>(p++, 5, 3);
	}

	return true;
}

bool HierarchyDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x04) {
		return false;
	}
	if (descriptor_length < 4) {
		return false;
	}

	temporal_scalability_flag = read_bits<int8_t>(p, 1, 1);
	spatial_scalability_flag  = read_bits<int8_t>(p, 2, 1);
	quality_scalability_flag  = read_bits<int8_t>(p, 3, 1);
	hierarchy_type            = read_bits<uint8_t>(p++, 4, 4);

	hierarchy_layer_index          = read_bits<uint8_t>(p++, 2, 6);
	tref_present_flag              = read_bits<int8_t>(p, 0, 1);

	hierarchy_embedded_layer_index = read_bits<uint8_t>(p++, 2, 6);

	hierarchy_channel              = read_bits<uint8_t>(p++, 2, 6);

	return true;
}

bool HybridInformationDescriptor::parse(const uint8_t* p)
{
	descriptor_tag    = *p++;
	descriptor_length = *p++;
	if (descriptor_tag != 0x68) {
		return false;
	}
	if (descriptor_length < 1) {
		return false;
	}

	has_location  = read_bits<int8_t>(p, 0, 1);
	location_type = read_bits<int8_t>(p, 1, 1);
	format        = read_bits<uint8_t>(p++, 2, 4);

	if (has_location) {
		if (location_type == 0) {
			component_tag = *p++;
			module_id     = read_bits<uint16_t>(p, 0, 16);
			p += 2;
		}
		else {
			URL_length = *p++;
			URL_bytes.assign(p, p + URL_length);
		}
	}

	return true;
}

bool Descriptor::decode(const uint8_t* data, const uint16_t data_size)
{
	if (!data || !data_size)
		return false;

	auto decode_size = 0;

	while (decode_size < data_size) {
		DescriptorData desc;
		bool success;

		desc.descriptor_tag = data[decode_size];
		const auto& descriptor_length = data[decode_size + 1];
		//fprintf(stderr, "desc_tag: %02x, length: %4d -now: %4d, entire: %4d\n", desc.descriptor_tag, descriptor_length, decode_size, data_size);

		switch (desc.descriptor_tag) {
		case 0x00:
		case 0x01:
			// Reserved
			success = false;
			break;
		case 0x02:
			desc.video_stream = std::make_shared<VideoStreamDescriptor>();
			success = desc.video_stream->parse(data + decode_size);
			break;
		case 0x03:
			desc.audio_stream = std::make_shared<AudioStreamDescriptor>();
			success = desc.audio_stream->parse(data + decode_size);
			break;
		case 0x04:
			desc.hierarchy = std::make_shared<HierarchyDescriptor>();
			success = desc.hierarchy->parse(data + decode_size);
			break;
		case 0x05:
			desc.registration = std::make_shared<RegistrationDescriptor>();
			success = desc.registration->parse(data + decode_size);
			break;
		case 0x06:
			desc.data_stream = std::make_shared<DataStreamAlignmentDescriptor>();
			success = desc.data_stream->parse(data + decode_size);
			break;
		case 0x07:
			desc.target_bk_grid = std::make_shared<TargetBackgroundGridDescriptor>();
			success = desc.target_bk_grid->parse(data + decode_size);
			break;
		case 0x08:
			desc.video_window = std::make_shared<VideoWindowDescriptor>();
			success = desc.video_window->parse(data + decode_size);
			break;
		case 0x09:
			desc.CA = std::make_shared<CADescriptor>();
			success = desc.CA->parse(data + decode_size);
			break;
		case 0x0A:
			desc.ISO_639_language = std::make_shared<ISO639LanguageDescriptor>();
			success = desc.ISO_639_language->parse(data + decode_size);
			break;
		case 0x0B:
			desc.sys_clock = std::make_shared<SystemClockDescriptor>();
			success = desc.sys_clock->parse(data + decode_size);
			break;
		case 0x0C:
			desc.mpx_buf_util = std::make_shared<MultiplexBufferUtilizationDescriptor>();
			success = desc.mpx_buf_util->parse(data + decode_size);
			break;
		case 0x0D:
			desc.carousel= std::make_shared<CarouselCompatibleCompositeDescriptor>();
			success = desc.carousel->parse(data + decode_size);
			break;
		case 0x0E:
			desc.max_bitrate = std::make_shared<MaximumBitrateDescriptor>();
			success = desc.max_bitrate->parse(data + decode_size);
			break;
		case 0x1C:
			desc.MPEG4_audio = std::make_shared<MPEG4AudioDescriptor>();
			success = desc.MPEG4_audio->parse(data + decode_size);
			break;
		case 0x28:
			desc.AVC_video = std::make_shared<AVCVideoDescriptor>();
			success = desc.AVC_video->parse(data + decode_size);
			break;
		case 0x2A:
			desc.AVC_timing = std::make_shared<AVCTimingAndHRFDescriptor>();
			success = desc.AVC_timing->parse(data + decode_size);
			break;
		case 0x2E:
			desc.MPEG4_audio_ext = std::make_shared<MPEG4AudioExtensionDescriptor>();
			success = desc.MPEG4_audio_ext->parse(data + decode_size);
			break;
		case 0x38:
			desc.HEVC = std::make_shared<HEVCDescriptor>();
			success = desc.HEVC->parse(data + decode_size);
			break;
		case 0x40:
			desc.network_name = std::make_shared<NetworkNameDescriptor>();
			success = desc.network_name->parse(data + decode_size);
			break;
		case 0x41:
			desc.service_list = std::make_shared<ServiceListDescriptor>();
			success = desc.service_list->parse(data + decode_size);
			break;
		case 0x42:
			desc.stuffing = std::make_shared<SttuffingDescriptor>();
			success = desc.stuffing->parse(data + decode_size);
			break;
		case 0x43:
			desc.satelite = std::make_shared<SateliteDeliverySystemDescriptor>();
			success = desc.satelite->parse(data + decode_size);
			break;
		case 0x47:
			desc.bouquet_name = std::make_shared<BouquetNameDescriptor>();
			success = desc.bouquet_name->parse(data + decode_size);
			break;
		case 0x48:
			desc.service = std::make_shared<ServiceDescriptor>();
			success = desc.service->parse(data + decode_size);
			break;
		case 0x49:
			desc.country = std::make_shared<CountryAvailabilityDescriptor>();
			success = desc.country->parse(data + decode_size);
			break;
		case 0x4A:
			desc.link = std::make_shared<LinkageDescriptor>();
			success = desc.link->parse(data + decode_size);
			break;
		case 0x4B:
			desc.NVOD_ref = std::make_shared<NVODReferenceDescriptor>();
			success = desc.NVOD_ref->parse(data + decode_size);
			break;
		case 0x4C:
			desc.time_shifted_service = std::make_shared<TimeShiftedServiceDescriptor>();
			success = desc.time_shifted_service->parse(data + decode_size);
			break;
		case 0x4D:
			desc.short_event = std::make_shared<ShortEventDescriptor>();
			success = desc.short_event->parse(data + decode_size);
			break;
		case 0x4E:
			desc.ext_event = std::make_shared<ExtendedEventDescriptor>();
			success = desc.ext_event->parse(data + decode_size);
			break;
		case 0x4F:
			desc.time_shifted_event = std::make_shared<TimeShiftedEventDescriptor>();
			success = desc.time_shifted_event->parse(data + decode_size);
			break;
		case 0x50:
			desc.component = std::make_shared<ComponentDescriptor>();
			success = desc.component->parse(data + decode_size);
			break;
		case 0x51:
			desc.mosaic = std::make_shared<MosaicDescriptor>();
			success = desc.mosaic->parse(data + decode_size);
			break;
		case 0x52:
			desc.stream_id = std::make_shared<StreamIdentifierDescriptor>();
			success = desc.stream_id->parse(data + decode_size);
			break;
		case 0x53:
			desc.CA_id = std::make_shared<CAIdentifierDescriptor>();
			success = desc.CA_id->parse(data + decode_size);
			break;
		case 0x54:
			desc.content = std::make_shared<ContentDescriptor>();
			success = desc.content->parse(data + decode_size);
			break;
		case 0x55:
			desc.parental_rating = std::make_shared<ParentalRatingDescriptor>();
			success = desc.parental_rating->parse(data + decode_size);
			break;
		case 0x58:
			desc.local_time_offset = std::make_shared<LocalTimeOffsetDescriptor>();
			success = desc.local_time_offset->parse(data + decode_size);
			break;
		case 0x66:
			desc.data_broadcast_id = std::make_shared<DataBroadcastIdDescriptor>();
			success = desc.data_broadcast_id->parse(data + decode_size);
			break;
		case 0x68:
			desc.hybrid = std::make_shared<HybridInformationDescriptor>();
			success = desc.hybrid->parse(data + decode_size);
			break;
		case 0xC0:
			desc.hierarchical_trans = std::make_shared<HierarchicalTransmissionDescriptor>();
			success = desc.hierarchical_trans->parse(data + decode_size);
			break;
		case 0xC1:
			desc.digital_copy_ctrl = std::make_shared<DigitalCopyControlDescriptor>();
			success = desc.digital_copy_ctrl->parse(data + decode_size);
			break;
		case 0xC4:
			desc.audio_component = std::make_shared<AudioComponentDescriptor>();
			success = desc.audio_component->parse(data + decode_size);
			break;
		case 0xC5:
			desc.hyperlink = std::make_shared<HyperlinkDescriptor>();
			success = desc.hyperlink->parse(data + decode_size);
			break;
		case 0xC6:
			desc.target_region = std::make_shared<TargetRegionDescriptor>();
			success = desc.target_region->parse(data + decode_size);
			break;
		case 0xC7:
			desc.data_content = std::make_shared<DataContentDescriptor>();
			success = desc.data_content->parse(data + decode_size);
			break;
		case 0xC8:
			desc.video_dec_ctrl = std::make_shared<VideoDecodeControlDescriptor>();
			success = desc.video_dec_ctrl->parse(data + decode_size);
			break;
		case 0xCD:
			desc.TS_info = std::make_shared<TSInformationDescriptor>();
			success = desc.TS_info->parse(data + decode_size);
			break;
		case 0xCE:
			desc.ext_broadcaster = std::make_shared<ExtendedBroadcasterDescriptor>();
			success = desc.ext_broadcaster->parse(data + decode_size);
			break;
		case 0xCF:
			desc.logo_trans = std::make_shared<LogoTransmissionDescriptor>();
			success = desc.logo_trans->parse(data + decode_size);
			break;
		case 0xD5:
			desc.series = std::make_shared<SeriesDescriptor>();
			success = desc.series->parse(data + decode_size);
			break;
		case 0xD6:
			desc.event_group = std::make_shared<EventGroupDescriptor>();
			success = desc.event_group->parse(data + decode_size);
			break;
		case 0xD7:
			desc.SI_param = std::make_shared<SIParameterDescriptor>();
			success = desc.SI_param->parse(data + decode_size);
			break;
		case 0xD8:
			desc.broadcaster_name = std::make_shared<BroadcasterNameDescriptor>();
			success = desc.broadcaster_name->parse(data + decode_size);
			break;
		case 0xD9:
			desc.component_group = std::make_shared<ComponentGroupDescriptor>();
			success = desc.component_group->parse(data + decode_size);
			break;
		case 0xDA:
			desc.SI_prime_ts = std::make_shared<SIPrimeTsDescriptor>();
			success = desc.SI_prime_ts->parse(data + decode_size);
			break;
		case 0xDB:
			desc.board_info = std::make_shared<BoardInformationDescriptor>();
			success = desc.board_info->parse(data + decode_size);
			break;
		case 0xDC:
			desc.LDT_link = std::make_shared<LDTLinkageDescriptor>();
			success = desc.LDT_link->parse(data + decode_size);
			break;
		case 0xE0:
			desc.service_group = std::make_shared<ServiceGroupDescriptor>();
			success = desc.service_group->parse(data + decode_size);
			break;
		case 0xE1:
			desc.area = std::make_shared<AreaBroadcastingInformationDescriptor>();
			success = desc.area->parse(data + decode_size);
			break;
		case 0xF6:
			desc.access_ctrl = std::make_shared<AccessControlDescriptor>();
			success = desc.access_ctrl->parse(data + decode_size);
			break;
		case 0xFB:
			desc.partial_reception = std::make_shared<PartialReceptionDescriptor>();
			success = desc.partial_reception->parse(data + decode_size);
			break;
		case 0xFC:
			desc.emergency = std::make_shared<EmergencyInformationDescriptor>();
			success = desc.emergency->parse(data + decode_size);
			break;
		case 0xFE:
			desc.system_management = std::make_shared<SystemManagementDescriptor>();
			success = desc.system_management->parse(data + decode_size);
			break;
		default:
			success = false;
			break;
		}

		if (success) {
			desc_list.push_back(std::move(desc));
		}

		decode_size += 2 + descriptor_length;
	}

	if (data_size != decode_size) {
		fprintf(stderr, "descriptor(): invalid descriptor size -> "
			"data: %d, read: %d\n", data_size, decode_size);
		return false;
	}

	return true;
}
