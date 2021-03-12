#include <iostream>
#include "NFA.h"
#include <conio.h>
using namespace std;

int main()
{
	setlocale(LC_ALL, "rus");

	NFA automaton;
	automaton.read_alphabet("alphabet.txt");
	automaton.read_states("nfa-states.txt");
	automaton.generate_dfa_states();

	std::cout << "�������������� ��������� DFA?(y)\n";
	switch (_getch())
	{
	case 121:
		std::cout << "\n�����������...\n\n";
		automaton.to_minimize();
		break;
	default:
		std::cout << "\n����������� ���������\n\n";
	}
	automaton.write_dfa_states("dfa-states.txt");
	automaton.initialize_dfa("dfa-states.txt");

	std::string input_str;

	while (true)
	{
		std::cout << "������� ������: ";
		std::cin >> input_str;
		automaton.process_string(input_str);
	}
}