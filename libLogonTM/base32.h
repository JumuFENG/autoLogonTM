#pragma once
#include <string>

namespace base32{
	static const std::string base32_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdef";

	static inline bool is_base32(unsigned char c) 
	{
		return (c <= 'Z' && c>= 'A') || (c>='a'&&c<='f');
	}

	static std::string base32_encode(unsigned char const* bytes_to_encode, unsigned int in_len)
	{
		std::string ret;
		int i = 0;
		int j = 0;
		unsigned char char_array_5[5];
		unsigned char char_array_8[8];

		while (in_len--) {
			char_array_5[i++] = *(bytes_to_encode++);
			if (i == 5) {
				char_array_8[0] = (char_array_5[0] & 0xf8) >> 3;
				char_array_8[1] = ((char_array_5[0] & 0x07) << 2) + ((char_array_5[1] & 0xc0) >> 6);
				char_array_8[2] = (char_array_5[1] & 0x3e) >> 1;
				char_array_8[3] = ((char_array_5[1] & 0x01) << 4) + ((char_array_5[2] & 0xf0)>>4);
				char_array_8[4] = ((char_array_5[2] & 0x0f) << 1) + ((char_array_5[3] & 0x80)>>7);
				char_array_8[5] = (char_array_5[3] & 0x7c) >> 2;
				char_array_8[6] = ((char_array_5[3] & 0x03) << 3) + ((char_array_5[4] & 0xe0)>>5);
				char_array_8[7] = (char_array_5[4] & 0x1f);

				for(i = 0; (i < 8) ; i++)
					ret += base32_chars[char_array_8[i]];
				i = 0;
			}
		}

		if (i)
		{
			for(j = i; j < 5; j++)
				char_array_5[j] = '\0';

			char_array_8[0] = (char_array_5[0] & 0xf8) >> 3;
			char_array_8[1] = ((char_array_5[0] & 0x07) << 2) + ((char_array_5[1] & 0xc0) >> 6);
			char_array_8[2] = (char_array_5[1] & 0x3e) >> 1;
			char_array_8[3] = ((char_array_5[1] & 0x01) << 4) + ((char_array_5[2] & 0xf0)>>4);
			char_array_8[4] = ((char_array_5[2] & 0x0f) << 1) + ((char_array_5[3] & 0x80)>>7);
			char_array_8[5] = (char_array_5[3] & 0x7c) >> 2;
			char_array_8[6] = ((char_array_5[3] & 0x03) << 3) + ((char_array_5[4] & 0xe0)>>5);
			char_array_8[7] = (char_array_5[4] & 0x1f);

			for (j = 0; (j < 8 - (5-i)*8/5); j++)
				ret += base32_chars[char_array_8[j]];

			ret += ( 5 - i ) + 'w' - 1;

		}

		return ret.c_str();

	}

	static std::string base32_decode(std::string const& encoded_string) 
	{
		int in_len = encoded_string.size();
		int i = 0;
		int j = 0;
		int in_ = 0;
		unsigned char char_array_8[8], char_array_5[5];
		std::string ret;

		while (in_len-- && is_base32(encoded_string[in_])) {
			char_array_8[i++] = encoded_string[in_]; in_++;
			if (i ==8) {
				for (i = 0; i <8; i++)
					char_array_8[i] = base32_chars.find(char_array_8[i]);

				char_array_5[0] = (char_array_8[0] << 3) + ((char_array_8[1] & 0x1c) >> 2);
				char_array_5[1] = ((char_array_8[1] & 0x03) << 6) + ((char_array_8[2] & 0x1f) << 1) + ((char_array_8[3] & 0x10) >> 4);
				char_array_5[2] = ((char_array_8[3] & 0x0f) << 4) + ((char_array_8[4] & 0x1e) >> 1);
				char_array_5[3] = ((char_array_8[4] & 0x01) << 7) + ((char_array_8[5] & 0x1f) << 2) + ((char_array_8[6] & 0x18) >> 3);
				char_array_5[4] = ((char_array_8[6] & 0x07) << 5) + (char_array_8[7] & 0x1f);

				for (i = 0; (i < 5); i++)
					ret += char_array_5[i];
				i = 0;
			}
		}

		if (i) {
			for (j = i ; j <8; j++)
				char_array_8[j] = 0;

			for (j = 0; j <i; j++)
				char_array_8[j] = base32_chars.find(char_array_8[j]);

			char_array_5[0] = (char_array_8[0] << 3) + ((char_array_8[1] & 0x1c) >> 2);
			char_array_5[1] = ((char_array_8[1] & 0x03) << 6) + ((char_array_8[2] & 0x1f) << 1) + ((char_array_8[3] & 0x10) >> 4);
			char_array_5[2] = ((char_array_8[3] & 0x0f) << 4) + ((char_array_8[4] & 0x1e) >> 1);
			char_array_5[3] = ((char_array_8[4] & 0x01) << 7) + ((char_array_8[5] & 0x1f) << 2) + ((char_array_8[6] & 0x18) >> 3);
			char_array_5[4] = ((char_array_8[6] & 0x07) << 5) + (char_array_8[7] & 0x1f);

			int maxLen = (encoded_string[in_]>='w'&&encoded_string[in_]<='z')?
				4  + 'w' - encoded_string[in_]:5;
			for (j = 0; j < maxLen; j++)
			{
				ret += char_array_5[j];
			}
		}

		return ret.c_str();
	}

	static std::string base32_encode(std::string const& str_to_encode)
	{
		return base32_encode((unsigned char const*)(str_to_encode.c_str()),str_to_encode.length());
	}
};