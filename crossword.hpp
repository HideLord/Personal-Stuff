#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <string>
#include <string.h>
#include <cassert>

using uc = unsigned char;

class CrosswordWord
{
public:

	bool isHor;

	std::vector<std::pair<uc *, int>> letters;

	std::string toString() const
	{
		std::string res(letters.size(), 0);
		for (size_t i = 0; i < letters.size(); i++)
			res[i] = *letters[i].first;
		return res;
	}
	std::string operator=(const std::string& S) const
	{
		assert(S.size() == letters.size());
		for (size_t i = 0; i < letters.size(); i++)
		{
			*letters[i].first = S[i];
		}
		return S;
	}
	static bool sortHelp(const CrosswordWord &A, const CrosswordWord &B)
	{
		return A.letters[0].second < B.letters[0].second;
	}
};

class Crossword
{
	const unsigned char CYRILLIC_A = 224;
	const unsigned char BOX_CHAR = 209;
	const unsigned char SBOX_CHAR = 208;

	void loadWords();
	void readCrosswordFile(std::istream& inStream);

public:

	bool isBox(uc c) { return c == BOX_CHAR || c == SBOX_CHAR; }
	bool isBox(int i, int j) { return board[i][j] == BOX_CHAR || board[i][j] == SBOX_CHAR; }

	void printASCII();
	void load(std::string path);
	void save(std::string path = "");

	Crossword();
	~Crossword();

public:

	std::string name;
	char numRows = 0;
	char numCols = 0;

	std::vector<std::vector<uc>> board;
	std::vector<CrosswordWord> crosswordWords;

	Crossword &operator=(const Crossword &cpy)
	{
		this->name = cpy.name;
		this->numRows = cpy.numRows;
		this->numCols = cpy.numCols;
		this->board = cpy.board;
		loadWords();
		return *this;
	}

public:

	static const std::vector<CrosswordWord> compare(const Crossword& crosswordA, const Crossword& crosswordB);
};
