#include "crossword.hpp"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <set>

Crossword::Crossword()
{
}

Crossword::Crossword(std::string path)
{
	load(std::move(path));
}

Crossword::~Crossword()
{
}

Crossword& Crossword::operator=(const Crossword& cpy)
{
	_name = cpy._name;
	_numRows = cpy._numRows;
	_numCols = cpy._numCols;
	_board = cpy._board;
	loadWords();
	return *this;
}

void Crossword::loadWords()
{
	_crosswordWords.clear();
	for (int i = 0; i < _numRows; ++i)
	{
		for (int j = 0; j < _numCols; ++j)
		{
			int start = j;
			while (j < _numCols && !isBox(i, j))
			{
				++j;
			}
			if (j - start <= 1)
			{
				continue;
			}
			CrosswordWord word;
			word.isHor = 1;
			for (; start < j; ++start)
			{
				word.letters.push_back(&_board[i][start]);
				word.letterIndices.push_back(i * _numCols + start);
			}
			_crosswordWords.emplace_back(std::move(word));
		}
	}

	for (int j = 0; j < _numCols; ++j)
	{
		for (int i = 0; i < _numRows; ++i)
		{
			int start = i;
			while (i < _numRows && !isBox(i, j))
			{
				++i;
			}
			if (i - start <= 1)
			{
				continue;
			}
			CrosswordWord word;
			word.isHor = 0;
			for (; start < i; ++start)
			{
				word.letters.push_back(&_board[start][j]); 
				word.letterIndices.push_back(start * _numCols + j);
			}
			_crosswordWords.emplace_back(std::move(word));
		}
	}
	std::sort(_crosswordWords.begin(), _crosswordWords.end(), CrosswordWord::sortHelpIndices);
}

void Crossword::printASCII()
{
	if (_name.empty())
	{
		return;
	}
	if (_numRows == 0 || _numCols == 0)
	{
		return;
	}
	for (int i = 0; i < _numRows; i++)
	{
		for (int j = 0; j < _numCols; j++)
		{
			if (isBox(i, j))
			{
				std::cout << (j != 0 ? " " : "") << "|";
			}
			else
			{
				std::cout << (j != 0 ? " " : "") << _board[i][j];
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
	{
		path[i] = tolower(path[i]);
	}
	if (path.substr(path.size() - 4, 4) != ".ctb")
	{
		path += ".ctb";
	}
	std::ifstream fin(path, std::ios::binary);
	while (!fin.good())
	{
		std::cout << "[ERROR]: Invalid path " << path << ". Try again:" << std::endl;
		std::cin >> path;
		if (path.substr(path.size() - 4, 4) != ".ctb")
		{
			path += ".ctb";
		}
		fin.open(path, std::ios::binary);
	}
	_name = path.substr(0, path.size() - 4);

	readCrosswordFile(fin);
	loadWords();
	printASCII();
}

void Crossword::readCrosswordFile(std::istream& in)
{
	in.get(_numRows);
	in.get(_numCols);

	_board = std::vector<std::vector<uc>>(_numRows, std::vector<uc>(_numCols));
	
	for (int i = 0; i < _numRows; i++)
	{
		for (int j = 0; j < _numCols; j++)
		{
			_board[i][j] = uc(in.get());
			if (uc(_board[i][j] + 64) >= CYRILLIC_A)
			{
				_board[i][j] += 64;
			}
		}
	}
}

void Crossword::save(std::string path)
{
	if (path.empty())
		path = _name + ".ctb";
	
	std::ofstream fout(path, std::ios::binary);

	fout << _numRows << _numCols;
	for (int i = 0; i < _numRows; i++)
	{
		for (int j = 0; j < _numCols; j++)
		{
			if (uc(_board[i][j]) >= CYRILLIC_A)
			{
				fout << uc(_board[i][j] - 64);
			}
			else 
			{
				fout << _board[i][j];
			}
		}
	}
	_name = path;
	std::cout << "[INFO]: Saved successfully at " << _name << "." << std::endl;
}

Crossword::CrosswordReport Crossword::generateReport() const
{
	CrosswordReport report;

	std::vector<std::string> strWords;
	std::set<std::string> uniqueWords;
	double lengthSum = 0.0;
	for (const auto& word : _crosswordWords)
	{
		strWords.emplace_back(word.toString());
		lengthSum += strWords.back().size();
	
		if (!uniqueWords.insert(strWords.back()).second)
		{
			report.repeatingWords.push_back(&word);
		}
	}

	report.crosswordName = _name;
	report.numWords = _crosswordWords.size();
	report.averageWordLength = lengthSum / report.numWords;
	report.cols = _numCols;
	report.rows = _numRows;
	report.numBoxes = 0;
	for (uint32_t i = 0; i < _numRows; ++i)
	{
		for (uint32_t j = 0; j < _numCols; ++j)
		{
			report.numBoxes += isBox(i, j);
		}
	}
	report.boxedAreaCoef = double(report.numBoxes) / double(report.cols * report.rows);
	
	return report;
}

bool Crossword::isValid(const Crossword& crossword)
{
	auto report = crossword.generateReport();
	return report.repeatingWords.empty();
}

const std::vector<const CrosswordWord*> Crossword::compare(const Crossword& crosswordA, const Crossword& crosswordB)
{
	std::map<std::string, const CrosswordWord*> wordsFromCrosswordA;
	for(const auto& word : crosswordA._crosswordWords)
	{
		wordsFromCrosswordA.emplace(word.toString(), &word);
	}

	std::vector<const CrosswordWord*> res;

	for(const auto& word : crosswordB._crosswordWords)
	{
		auto it = wordsFromCrosswordA.find(word.toString());
		if(it != wordsFromCrosswordA.end())
		{
			res.push_back(it->second);
		}
	}

	return res;
}

