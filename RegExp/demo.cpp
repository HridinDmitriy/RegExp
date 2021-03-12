#include "ConverterFromReToNfa.h"
#include <iostream>
#include "../NFA/NFA.h"
#include <conio.h>

int main()
{
	setlocale(LC_ALL, "rus");

	std::string regex;
	NFA automaton;

	std::cout << "Введите регулярное выражение: \n";
	std::getline(std::cin, regex);

	ConverterFromReToNfa converter(regex); 

	try 
	{
		converter.convert("nfa-states.txt");
		auto alphabet = converter.get_alphabet();
		automaton.set_alphabet(alphabet.begin(), alphabet.end());
		automaton.read_states("nfa-states.txt");
		automaton.generate_dfa_states();

		std::cout << "Минимизировать состояния DFA?(y)\n";
		switch (_getch())
		{
		case 121:
			std::cout << "\nМинимизирую...\n\n";
			automaton.to_minimize();
			break;
		default:
			std::cout << "\nМинимизация отключена\n\n";
		}
		automaton.write_dfa_states("dfa-states.txt");
		automaton.initialize_dfa("dfa-states.txt");

		std::string input_str;

		while (true)
		{
			std::cout << "Введите строку: ";
			std::cin >> input_str;
			automaton.process_string(input_str);
		}
	}
	catch (std::exception ex)
	{
		std::cout << ex.what(); 
		system("pause");
	}
}

