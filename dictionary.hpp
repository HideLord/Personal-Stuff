#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include <string>
#include <boost/property_tree/ini_parser.hpp>

#include "robin_hood.h"

namespace utils
{

	class Dictionary
	{
	public:

		const static uint8_t CYRILLIC_A = 224; // The first letter of the cyrillic alphabet
		const static uint8_t BOX_CHAR = 209; // Used to place explanations in it
		const static uint8_t SPECIAL_BOX_CHAR = 208; // Used to place images over it

		const static uint8_t ANY_CHAR = 0; // Used in patterns to indicate that any character can be placed there
		const static uint8_t LONGEST_WORD = 50;
		const static char* DEFAULT_DICTIONARY_PATH;
		const static char* DEFAULT_CONFIG_PATH;
	
	public:

		class Pattern
		{
		public:
			Pattern(size_t size, std::vector<uint16_t>::iterator nextWord, std::function<const std::string& (uint16_t)> callback) :
				size(size),
				_get(std::move(callback)),
				_nextWord(nextWord),
				_begin(std::move(nextWord))
			{}
			Pattern() : 
				size(0),
				_get([](uint32_t) -> const std::string& { assert(false && "Using uninitialized Pattern"); return ""; })
			{}
			Pattern(Pattern&& other) noexcept
			{
				*this = std::move(other);
			}

			void operator=(Pattern&& other) noexcept
			{
				size = other.size;
				_get = std::move(other._get);
				_nextWord = std::move(other._nextWord);
				_begin = std::move(other._begin);
			}

			void reset() { _nextWord = _begin; }

		public:
			size_t size; // The number of possible words
			std::pair<uint16_t, const std::string&> operator()() { return { *_nextWord, _get(*(_nextWord++)) }; } // Returns the next possible word and its index

		private:
			std::function<const std::string& (uint16_t)> _get; // callback to dictionary which returns word from index.
			std::vector<uint16_t>::iterator _nextWord, _begin; // iterator given at construction pointing from vector of possible words.

		};

	public:

		static int levenstein(std::string a, std::string b); // Returns the distance between word `a` and word `b`

		static std::string cleanString(const std::string& dirtyString);
		static std::string toupper(std::string word);
		static char toupper(char c);
		static bool isalpha(char c);
		static std::string dosToWinCode(std::string word);

	public:

		const std::string& getDirty(const std::string& clean) const;
		const std::string& getExplanation(const std::string& clean) const;
		Pattern findPossible(const std::string& pattern);
		void shuffle();

	public:

		Dictionary();
		Dictionary(const std::string& configFilePath);
		~Dictionary();

	private:
		
		static uint64_t getKey(uint32_t mask, const std::string& word);
		static uint64_t getKey(const std::string& word);
		static inline bool isPossible(const std::string& pattern, const std::string& contender, const std::vector<uint32_t>& filledIndices);

		void addToFastSearch(const std::string& newWord, uint16_t index);
		inline const std::string& getFromIndex(uint16_t index) const { return _allWords[index]; }

		void loadDictionary();
		void reset();

	private:

		robin_hood::unordered_map<std::string, std::string> _explanationDict; // Given a clean word it returns an explination.
		robin_hood::unordered_map<std::string, std::string> _dirtyDict; // Given a clean word it returns the original untouched word in the dictionary.
		std::vector<std::string> _allWords; // All words loaded from the dict

		robin_hood::unordered_map<std::string, std::vector<uint16_t>*> _cacheSearchMap; // Maps from pattern to cache
		robin_hood::unordered_map<uint64_t, std::vector<uint16_t>> _fastSearch[LONGEST_WORD]; // Maps from first six (or less if there aren't enough) letters to word indices. Divided by length.

		boost::property_tree::ptree _iniPropertyTree;

		std::string _dictionaryFilePath;

	};

}