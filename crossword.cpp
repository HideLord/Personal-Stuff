#include "crossword.hpp"

Crossword::Crossword()
{
}

Crossword::~Crossword()
{
}

void Crossword::loadWords()
{
	crosswordWords.clear();
	for (int i = 0; i < numRows; ++i)
	{
		for (int j = 0; j < numCols; ++j)
		{
			int start = j;
			for (; j < numCols && !isBox(i, j); ++j)
				;
			if (j - start <= 1)
				continue;
			CrosswordWord newPos;
			newPos.isHor = 1;
			for (; start < j; ++start)
				newPos.letters.push_back({&board[i][start], i * numCols + start});
			crosswordWords.push_back(newPos);
		}
	}

	for (int j = 0; j < numCols; ++j)
	{
		for (int i = 0; i < numRows; ++i)
		{
			int start = i;
			for (; i < numRows && !isBox(i, j); ++i)
				;
			if (i - start <= 1)
				continue;
			CrosswordWord newPos;
			newPos.isHor = 0;
			for (; start < i; ++start)
				newPos.letters.push_back({&board[start][j], start * numCols + j});
			crosswordWords.push_back(newPos);
		}
	}
	sort(crosswordWords.begin(), crosswordWords.end(), CrosswordWord::sortHelp);
}

void Crossword::printASCII()
{
	if (name.empty())
		return;
	if (numRows == 0 || numCols == 0)
		return;
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			if (isBox(i, j))
			{
				std::cout << (j != 0 ? " " : "") << "|";
			}
			else
			{
				std::cout << (j != 0 ? " " : "") << board[i][j];
			}
		}
		std::cout << std::endl;
	}
}

void Crossword::load(std::string path)
{
	std::cout << "[INFO]: Crossword::load: loading " << path << "..." << std::endl;
	while (path.size() < 5)
	{
		std::cout << "[ERROR]: Invalid path. Try again:" << std::endl;
		std::cin >> path;
	}
	for (size_t i = 0; i < path.size(); i++)
		path[i] = tolower(path[i]);
	if (path.substr(path.size() - 4, 4) != ".ctb")
		path += ".ctb";
	std::ifstream fin(path, std::ios::binary);
	while (!fin.good())
	{
		std::cout << "[ERROR]: Invalid path " << path << ". Try again:" << std::endl;
		std::cin >> path;
		if (path.substr(path.size() - 4, 4) != ".ctb")
			path += ".ctb";
		fin.open(path, std::ios::binary);
	}
	name = path.substr(0, path.size() - 4);

	readCrosswordFile(fin);
	loadWords();
	printASCII();
}

void Crossword::readCrosswordFile(std::istream& in)
{
	in.get(numRows);
	in.get(numCols);

	board = std::vector<std::vector<uc>>(numRows, std::vector<uc>(numCols));
	
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			board[i][j] = uc(in.get());
			if (uc(board[i][j] + 64) >= CYRILLIC_A)
				board[i][j] += 64;
		}
	}
}

void Crossword::save(std::string path)
{
	if (path.empty())
	{
		path = name + ".ctb";
	}
	std::ofstream fout(path, std::ios::binary);

	fout << numRows << numCols;
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			if (uc(board[i][j]) >= CYRILLIC_A)
				fout << uc(board[i][j] - 64);
			else
				fout << board[i][j];
		}
	}
	name = path;
	std::cout << "Saved successfully at " << name << "." << std::endl;
}

const std::vector<CrosswordWord> Crossword::compare(const Crossword& crosswordA, const Crossword& crosswordB)
{
	std::map<std::string, CrosswordWord> wordsFromCrosswordA;
	for(const auto& word : crosswordA.crosswordWords)
	{
		wordsFromCrosswordA.emplace(word.toString(), word);
	}

	std::vector<CrosswordWord> res;

	for(const auto& word : crosswordB.crosswordWords)
	{
		auto it = wordsFromCrosswordA.find(word.toString());
		if(it != wordsFromCrosswordA.end())
		{
			res.push_back(it->second);
		}
	}

	return res;
}

