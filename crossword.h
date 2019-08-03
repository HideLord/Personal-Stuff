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

using namespace std;

using uc = unsigned char;

class position {
public:
	bool hor;
	vector<pair< uc*, int >> letters;
	string to_string() {
		string res(letters.size(),0);
		for (size_t i = 0; i < letters.size(); i++)
			res[i] = *letters[i].first;
		return res;
	}
	string operator=(string S) {
		assert(S.size() == letters.size());
		for (size_t i = 0; i < letters.size(); i++) {
			*letters[i].first = S[i];
		}
		return S;
	}
	static bool sortHelp(const position & A, const position & B) {
		return A.letters[0].second < B.letters[0].second;
	}
};

class crossword
{
	const unsigned char cyrillicA = 224;
	const unsigned char boxChar = 209;
	const unsigned char sboxChar = 208;


	void loadWords();
public:
	bool isBox(uc c) {
		return c == boxChar || c == sboxChar;
	}
	bool isBox(int i, int j) {
		return board[i][j] == boxChar || board[i][j] == sboxChar;
	}
	void printASCII();
	void load(string path);
	void save(string path = "");
	crossword();
	~crossword();

	string name;
	char N = 0, M = 0;

	vector<vector<uc>> board;
	vector<position> areas;

	crossword& operator=(const crossword& cpy) {
		this->name = cpy.name;
		this->N = cpy.N;
		this->M = cpy.M;
		this->board = cpy.board;
		this->areas = cpy.areas;
		return *this;
	}
};

