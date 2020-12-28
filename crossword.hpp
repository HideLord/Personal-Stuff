#pragma once
#include <vector>
#include <string>
#include <cassert>

using uc = unsigned char;

class CrosswordWord
{
public: // member variables
	std::vector<uc*> letters;
	std::vector<uint32_t> letterIndices;
	bool isHor;

public: // member functions
	std::string toString() const
	{
		std::string res(letters.size(), 0);
		for (size_t i = 0; i < letters.size(); i++)
			res[i] = *letters[i];
		return res;
	}
	bool fill(const std::string& newWord)
	{
		assert(newWord.size() == letters.size() && "[LOGICAL ERROR]: The new word has to have the same length as the crossword word!");
		for (uint32_t i = 0; i < letters.size(); ++i)
			*letters[i] = newWord[i];
	}

public: // operators
	explicit operator std::string() const
	{
		return toString();
	}
	bool operator<(const CrosswordWord& other) const
	{
		return toString() < other.toString();
	}

public: // static functions
	// first word to appear in crossword. If they both start in the same square, than the horizontal has priority.
	static bool sortHelpIndices(const CrosswordWord &A, const CrosswordWord &B)
	{
		return A.letterIndices[0] == B.letterIndices[0] ? A.isHor : A.letterIndices[0] < B.letterIndices[0];
	}
	static bool sortHelpString(const CrosswordWord& A, const CrosswordWord& B)
	{
		return A.toString() < B.toString();
	}
};

class Crossword
{
public:

	struct CrosswordReport
	{
		std::vector<const CrosswordWord*> repeatingWords; // words which are counted twice or more in the same crossword
		std::string crosswordName;
		double averageWordLength;
		double boxedAreaCoef;
		uint32_t numBoxes;
		uint32_t numWords;
		uint32_t rows;
		uint32_t cols;
	};

private:

	std::string _name;
	char _numRows = 0;
	char _numCols = 0;

	std::vector<std::vector<uc>> _board;
	std::vector<CrosswordWord> _crosswordWords;

private:

	static const unsigned char CYRILLIC_A = 224;
	static const unsigned char BOX_CHAR = 209;
	static const unsigned char SBOX_CHAR = 208;

private:

	void loadWords();
	void readCrosswordFile(std::istream& inStream);

public:

	bool isBox(int i, int j) const { return _board[i][j] == BOX_CHAR || _board[i][j] == SBOX_CHAR; }

	void printASCII();
	void load(std::string path);
	void save(std::string path = "");
	CrosswordReport generateReport() const;
	bool isValid() { return Crossword::isValid(*this); }

	Crossword();
	Crossword(std::string path);
	~Crossword();

public:

	Crossword& operator=(const Crossword& cpy);

public:

	static bool isValid(const Crossword& crossword);
	static const std::vector<const CrosswordWord*> compare(const Crossword& crosswordA, const Crossword& crosswordB);

	static bool isBox(uc c) { return c == BOX_CHAR || c == SBOX_CHAR; }
};
