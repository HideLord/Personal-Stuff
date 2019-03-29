#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <string>
#include <string.h>
#include <unordered_map>
using namespace std;

class dictionary
{
	const unsigned char cyrillicA = 224;
	const unsigned char boxChar = 209;
	const unsigned char sboxChar = 208;
	using uc = unsigned char;
public:
	unordered_map<string, string> explanationDict;
	vector<string> allWords;
	void loadDict();
	dictionary();
	~dictionary();
};

