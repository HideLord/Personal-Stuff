#include "dictionary.h"


void dictionary::loadDict() {

	ifstream fin;
	fin.open("Z:\\Cross\\BIGDICT.TXT");
	if(!fin.good()) fin.open("BIGDICT.txt");
	if (!fin.good()) {
		cout << "Failed to load dictionary: could not open file!" << endl;
		return;
	}
	string w, expl;
	while (getline(fin, w, '\t')) {
		getline(fin, expl);
		string clean;
		for (size_t i = 0; i < w.size(); i++) {
			uc c = w[i];
			if (c >= cyrillicA - 96 && c < cyrillicA - 64)c += 32;
			if (c >= cyrillicA - 64 && c < cyrillicA - 32)c += 64, clean.push_back(c);
			w[i] = c;
		}
		for (size_t i = 0; i < expl.size(); i++) {
			uc c = expl[i];
			if (c >= cyrillicA - 96 && c < cyrillicA - 32)c += 64;
			expl[i] = c;
		}
		explanationDict[clean] = expl;
		allWords.push_back(clean);
	}
	fin.close();
}

dictionary::dictionary()
{
}


dictionary::~dictionary()
{
}
