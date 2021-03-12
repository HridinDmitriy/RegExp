#pragma once
#include <fstream> 
#include <iostream>
#include <vector> 
#include "StateSet.h"
#include "../DFA/DFA.h"

class NFA
{
public:
	using _Names_pair = std::pair<std::string, std::string>;
	using _Table_cells = std::map<_Names_pair, bool>;

	NFA() = default;
	~NFA();
	void read_alphabet(const std::string& file_name); 
	template<typename _Iter>
	void set_alphabet(_Iter _left, _Iter _right);
	void read_states(const std::string& file_name);
	void generate_dfa_states();
	void write_dfa_states(const std::string& file_name); 
	void initialize_dfa(const std::string& dfa_states_file_name); 
	void process_string(const std::string& str);
	void to_minimize();
private:
	const std::vector<std::string> _parse_transition_name(const std::string& str); 
	void _create_trivial_table(_Table_cells& table);
	void _find_all_equal_states(_Table_cells& table);
private:
	std::fstream strm; 
	Alphabet alphabet; 
	DFA dfa;
	std::map<std::string, _NFA_State*> nfa_states;
	std::map<std::string, StateSet*> dfa_states;
};

template<typename _Iter>
inline void NFA::set_alphabet(_Iter _left, _Iter _right)
{
	alphabet.add_letter(_left, _right);
}

