#pragma once
#include <inttypes.h>
#include <string>

namespace utils
{
	const uint8_t CYRILLIC_A = 224; // The first letter of the cyrillic alphabet

	const uint8_t DOS_BOX_CHAR = 209; // Used to place explanations in it (before conversion to win)
	const uint8_t DOS_SPECIAL_BOX_CHAR = 208; // Used to place images over it (before conversion to win)
	
	const uint8_t BOX_CHAR = 17; // Used to place explanations in it
	const uint8_t SPECIAL_BOX_CHAR = 16; // Used to place images over it

	inline bool isBox(uint8_t c) { return c == BOX_CHAR || c == SPECIAL_BOX_CHAR; }
	inline bool isDosBox(uint8_t c) { return c == DOS_BOX_CHAR || c == DOS_SPECIAL_BOX_CHAR; }

	inline bool isCyrillicChar(uint8_t c) { return c >= CYRILLIC_A - 32 && c < CYRILLIC_A + 32; }
	inline bool isDosCyrillicChar(uint8_t c) { return c >= CYRILLIC_A - 96 && c < CYRILLIC_A - 32; }

	/* Converts the dos code page to win 1251 cyrillic code page */
	inline std::string dosToWinCode(std::string winWord)
	{
		for (auto& c : winWord)
			if (isDosCyrillicChar(c) || isDosBox(c))
				c += 64;

		return winWord;
	}
	inline uint8_t dosToWinCode(uint8_t c)
	{
		if (isDosCyrillicChar(c) || isDosBox(c))
			return c + 64;

		return c;
	}

	/* Converts the win 1251 code page to the dos code page */
	inline std::string winToDosCode(std::string winWord)
	{
		for (auto& c : winWord)
			if (isCyrillicChar(c) || isBox(c))
				c -= 64;

		return winWord;
	}
	inline uint8_t winToDosCode(uint8_t c)
	{
		if (isCyrillicChar(c) || isBox(c))
			return c - 64;

		return c;
	}
}