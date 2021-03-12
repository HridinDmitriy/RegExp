#pragma once
#include <fstream> 
#include <iostream>
#include "Alphabet.h"
#include "State.h"

class DFA 
{
public:
	DFA() = default; 
	~DFA();
	void read_alphabet(std::string file_name);
	void set_alphabet(const Alphabet& _other_alphabet); 
	template<typename _Iter>
	void set_alphabet(_Iter _left, _Iter _right);
	void read_states(std::string file_name);
	void process_string(std::string str);
private:
	std::ifstream strm;
	Alphabet alphabet; 
	std::map<std::string, State*> states; 
};

template<typename _Iter>
inline void DFA::set_alphabet(_Iter _left, _Iter _right)
{
	alphabet.add_letter(_left, _right);
}

