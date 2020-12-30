#pragma once
#include <sstream>
#include <iostream>

#define NOMINMAX
#include <Windows.h>

class ColorChanger
{
public:

	static ColorChanger& getInstance()
	{
		static ColorChanger colorChanger;
		return colorChanger;
	}

	void changeColor(uint16_t color)
	{
		if (_currentColor != color) {
			_currentColor = color;
			SetConsoleTextAttribute(hConsole, color);
		}
	}

private:

	ColorChanger() :
		hConsole(GetStdHandle(STD_OUTPUT_HANDLE)),
		_currentColor(7)
	{}
	~ColorChanger()
	{
		changeColor(7);
	}

private:

	HANDLE hConsole;
	uint16_t _currentColor;

};

#define VLOG_CUSTOM(color, message) ColorChanger::getInstance().changeColor(color);  std::cout << message;
#define VLOG_TRACE(message) ColorChanger::getInstance().changeColor(7);  std::cout << message;
#define VLOG_DEBUG(message) ColorChanger::getInstance().changeColor(9);  std::cout << message;
#define VLOG_INFO(message)  ColorChanger::getInstance().changeColor(10); std::cout << message;
#define VLOG_WARN(message)  ColorChanger::getInstance().changeColor(14); std::cout << message;
#define VLOG_ERROR(message) ColorChanger::getInstance().changeColor(12); std::cout << message;
#define VLOG_FATAL(message) ColorChanger::getInstance().changeColor(4); std::cout << message;
