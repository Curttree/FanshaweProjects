// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <conio.h>
#include <fstream>
#include <string>

int main()
{
	//Read the text file one line at a time and write to console.
	std::ifstream inputFile("hello.txt");
	std::string line;
	std::size_t colonValue;
	while (std::getline(inputFile, line)) {
		std::cout << line << std::endl;
	}
	std::cout << "-----------------------" << std::endl;
	std::cout << "<Press any key to exit>" << std::endl;

	// Check for any key being hit.
	bool shouldExit = false;
	while (!shouldExit) {
		if (_kbhit()) {
			shouldExit = true;
		}
	}
}
