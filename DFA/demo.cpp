#include <iostream>
#include "DFA.h"

int main()
{
	setlocale(LC_ALL, "rus");

	DFA dfa; 
	dfa.read_alphabet("alphabet.txt");
	dfa.read_states("states.txt");

	std::string input_str;

	while (true)
	{
		std::cout << "¬ведите строку: ";
		std::cin >> input_str;
		dfa.process_string(input_str);
	}
}