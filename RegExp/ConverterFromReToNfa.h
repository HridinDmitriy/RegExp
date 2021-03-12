#pragma once
#include <fstream> 
#include <set>
#include <list> 
#include "../NFA/_NFA_State.h"
#include "../NFA/Lambda.h"

class ConverterFromReToNfa
{
public:
	ConverterFromReToNfa() = default;
	ConverterFromReToNfa(std::string regex);
	//~ConverterFromReToNfa();
	void convert(std::string nfa_file_name);
	const std::set<char>& get_alphabet() const;

private:
	std::string remove_spaces(std::string str) const;
	std::string add_concatenations(std::string str) const; 
	void build_transition(_NFA_State* left, _NFA_State* right, std::string transition);
	void build_union(_NFA_State* left, _NFA_State* right, std::string old_transition, const std::list<std::string>& new_transitions);
	void build_concatenation(_NFA_State* left, _NFA_State* right, std::string old_transition, const std::list<std::string>& new_transitions);
	void build_star(_NFA_State* left, _NFA_State* right, std::string old_transition, const std::list<std::string>& new_transitions);
	void alphabet_init(const std::string& str);
	std::list<std::string> get_parsed_regex(std::string regex) const;  
	void write_nfa_states(std::string nfa_file_name);
	std::string generate_transition_name(_NFA_State* state, std::string letter) const;

private:
	std::fstream strm; 
	std::set<char> alphabet;
	std::map<std::string, _NFA_State*> nfa_states;
	int state_id;
};

