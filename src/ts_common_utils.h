#pragma once

#include <cassert>
#include <ctime>

template<typename T>
static T
read_bits(const uint8_t* buffer, const int start_bit, const int bit_length)
{
	assert(0 <= start_bit && start_bit < 8);

	T value = 0;

	const auto mask = T(((uintmax_t)1 << bit_length) - 1);

	// The shift amount of buffer[0]
	auto shift = (bit_length - 1) - (7 - start_bit);

	while (shift >= 0) {
		value |= (T)*buffer++ << shift;
		shift -= 8;
	}

	if (-8 < shift && shift < 0) {
		value |= (T)*buffer >> -shift;
	}

	return value & mask;
}

template<typename T>
static std::unique_ptr<T[]> memcopy_to_unique(const T* start, const T* end)
{
	const std::vector<T> vec(start, end);
	std::unique_ptr<T[]> ret(new T[end - start]);
	if (!ret) throw std::runtime_error("Null pointer.");
	std::copy(vec.begin(), vec.end(), ret.get());
	return ret;
}

template<typename T>
static inline T BCD_to_dec(const uint8_t* buffer, const int digits)
{
	if (buffer == nullptr) return 0;

	T value = 0;
	for (auto i = 0; i < digits; i++) {
		value *= 10;
		if ((i & 1) == 0) { // even
			value += (buffer[i >> 1] & 0xf0) >> 4;
		}
		else { // odd
			value += buffer[i >> 1] & 0x0f;
		}
	}

	return value;
}

// 現在日付は2038年4月22日まで表現可能
static inline auto MJD_to_JTC(const uint16_t MJD) -> struct tm
{
	auto Y_prime = (int)((MJD - 15078.2) / 365.25);
	auto M_prime = (int)((MJD - 14956.1 - (int)(Y_prime * 365.25)) / 30.6001);
	auto D = MJD - 14956 - (int)(Y_prime * 365.25) - (int)(M_prime * 30.6001);
	auto K = (M_prime == 14 || M_prime == 15) ? 1 : 0;
	auto Y = Y_prime + K + 1900;
	auto M = M_prime - 1 - K * 12;

	return { 0, 0, 0, D, M, Y };
}
