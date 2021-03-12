#include "ConverterFromReToNfa.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <string>

ConverterFromReToNfa::ConverterFromReToNfa(std::string regex)
{
	state_id = 1;

	regex = remove_spaces(regex);
	regex = add_concatenations(regex);
	alphabet_init(regex);

	_NFA_State* begin = new _NFA_State("st", true, false);
	_NFA_State* end = new _NFA_State("fin", false, true);

	begin->add_transition(std::make_pair(regex, end)); 
	nfa_states.insert(std::make_pair(begin->get_name(), begin));
	nfa_states.insert(std::make_pair(end->get_name(), end));
}

void ConverterFromReToNfa::convert(std::string nfa_file_name)
{
	_NFA_State* begin = nfa_states.at("st");
	_NFA_State* end = nfa_states.at("fin");

	auto regex = begin->get_alphabet().front();

	if (regex.length() > 1)
	{
		build_transition(begin, end, regex);
	}

	write_nfa_states(nfa_file_name);
}

void ConverterFromReToNfa::build_transition(_NFA_State* left, _NFA_State* right, std::string transition)
{
	auto transition_parts = get_parsed_regex(transition); 
	auto begin = transition_parts.begin();
	auto end = transition_parts.end(); 

	
	std::list<std::string> operands; 
	std::string operation;
	std::list<std::string>::iterator found_operation;
	void(ConverterFromReToNfa::*build_operation)(_NFA_State*, _NFA_State*, std::string, const std::list<std::string>&) = nullptr;

	if (found_operation = std::find(begin, end, "+"), found_operation != end)
	{
		operation = "+";
		build_operation = &ConverterFromReToNfa::build_union;
	}
	else if (found_operation = std::find(begin, end, "&"), found_operation != end)
	{
		operation = "&"; 
		build_operation = &ConverterFromReToNfa::build_concatenation;
	}
	else if (found_operation = std::find(begin, end, "*"), found_operation != end)
	{
		operation = "*"; 
		build_operation = &ConverterFromReToNfa::build_star;
	}

	auto operator_begin = begin;
	while (operator_begin != end)
	{
		std::string operand;

		for (; operator_begin != found_operation; ++operator_begin)
		{
			operand.append(*operator_begin);
		}
		operands.push_back(operand); 

		if (found_operation != end)
		{
			operator_begin = ++found_operation;
			found_operation = std::find(found_operation, end, operation);
		}
	}

	(this->*build_operation)(left, right, transition, operands); 
}

void ConverterFromReToNfa::build_union(_NFA_State* left, _NFA_State* right, std::string old_transition, const std::list<std::string>& new_transitions)
{ 
	for (auto new_transition : new_transitions)
	{
		left->add_transition(std::make_pair(new_transition, right)); 
		if (new_transition.length() > 1)
		{
			build_transition(left, right, new_transition);
		}
	}
	left->delete_transition(old_transition); 
}

void ConverterFromReToNfa::build_concatenation(_NFA_State* left, _NFA_State* right, std::string old_transition, const std::list<std::string>& new_transitions)
{
	auto left_state = left;
	for (auto it = new_transitions.begin(); it != --new_transitions.end(); ++it)
	{
		_NFA_State* new_state = new _NFA_State("q" + std::to_string(state_id++)); 
		nfa_states.insert(std::make_pair(new_state->get_name(), new_state));
		left_state->add_transition(std::make_pair(*it, new_state));
		if ((*it).length() > 1) 
		{
			build_transition(left_state, new_state, *it); 
		}
		left_state = new_state;
	}

	left_state->add_transition(std::make_pair(new_transitions.back(), right));
	if (new_transitions.back().length() > 1)
	{
		build_transition(left_state, right, new_transitions.back()); 
	}

	left->delete_transition(old_transition);
}

void ConverterFromReToNfa::build_star(_NFA_State* left, _NFA_State* right, std::string old_transition, const std::list<std::string>& new_transitions)
{
	_NFA_State* new_state = new _NFA_State("q" + std::to_string(state_id++));
	nfa_states.insert(std::make_pair(new_state->get_name(), new_state));

	left->add_transition(std::make_pair(std::string({ LAMBDA }), new_state));
	new_state->add_transition(std::make_pair(std::string({ LAMBDA }), right));

	auto new_transition = new_transitions.front();
	new_state->add_transition(std::make_pair(new_transition, new_state));

	if (new_transition.length() > 1)
	{
		build_transition(new_state, new_state, new_transition);
	}

	left->delete_transition(old_transition);
}

std::list<std::string> ConverterFromReToNfa::get_parsed_regex(std::string regex) const
{
	std::list<std::string> regex_parts;
	int depth_level = 0;
	int operand_start = 0;

	for (size_t i = 0; i < regex.length(); ++i)
	{
		if (regex[i] == '(')
		{
			if (depth_level == 0)
				operand_start = i;
			++depth_level;
		}
		else if (regex[i] == ')')
		{
			--depth_level;
			if (depth_level < 0)
			{
				throw std::runtime_error("Ќеверна€ скобочна€ последовательность\n"); 
			}
			if (depth_level == 0)
			{
				regex_parts.push_back(regex.substr(operand_start, i - operand_start + 1));
			}
		}
		else if (depth_level == 0)
		{
			regex_parts.push_back(regex.substr(i, 1));
		}
	}

	if (depth_level > 0)
	{
		throw std::runtime_error("Ќеверна€ скобочна€ последовательность\n");
	}

	if (regex_parts.size() == 1)
	{
		auto part = regex_parts.front();
		if (part[0] == '(' && part[part.length() - 1] == ')')
		{
			regex_parts.front() = part.substr(1, part.length() - 2);     
		}
		if (regex_parts.front().length() > 1)
		{
			return get_parsed_regex(regex_parts.front());
		}
	}

	return regex_parts;
}

std::string ConverterFromReToNfa::remove_spaces(std::string str) const
{
	size_t pos = str.find(" ");
	while (pos < str.length())
	{
		str.erase(pos, 1);
		pos = str.find(" ");
	}
	return str;
}

std::string ConverterFromReToNfa::add_concatenations(std::string str) const
{
	std::set<char> spec_symb({'(', ')', '*', '+', '&', ' '});

	size_t pos = str.find(")(");  
	while (pos < str.length())
	{
		str.insert(pos + 1, 1, '&');
		pos = str.find(")(");
	}

	pos = str.find("*(");
	while (pos < str.length())
	{
		str.insert(pos + 1, 1, '&');
		pos = str.find("*(");
	}

	for (size_t i = 0; i < str.length(); ++i)
	{
		if (i > 0)
		{
			bool is_letter = spec_symb.find(str[i]) == spec_symb.end();
			bool has_letter_before = spec_symb.find(str[i - 1]) == spec_symb.end();
			bool has_star_or_closed_bracket_before = str[i - 1] == '*' || str[i - 1] == ')';

			bool is_open_bracket = str[i] == '(';

			if ((is_letter && (has_letter_before || has_star_or_closed_bracket_before)) ||
				(is_open_bracket && has_letter_before))
			{
				str.insert(i++, 1, '&');
			}
		}
	}
	return str;
}

void ConverterFromReToNfa::alphabet_init(const std::string& str)
{
	std::set<char> spec_symb({ '(', ')', '*', '+', '&', ' ' });

	for (size_t i = 0; i < str.length(); ++i)
	{
		if (spec_symb.find(str[i]) == spec_symb.end())
		{
			alphabet.insert(str[i]);
		}
	}
	alphabet.insert(LAMBDA);
}

void ConverterFromReToNfa::write_nfa_states(std::string nfa_file_name) 
{
	strm.open(nfa_file_name, std::ios::out | std::ios::trunc);

	for (auto state : nfa_states)
	{
		strm << state.second->get_name() << std::setw(5) << "\t" << state.second->get_init() << std::setw(5) << "\t" << state.second->get_final();

		for (auto letter : alphabet)
		{
			strm << std::setw(5) << "\t\t" << generate_transition_name(state.second, std::string({ letter }));
		}
		strm << "\n";
	}
	strm.close();
}

std::string ConverterFromReToNfa::generate_transition_name(_NFA_State* state, std::string letter) const
{
	auto transitions_by_letter = state->get_states_by(letter);

	if (transitions_by_letter.size() == 0)
		return std::string("{}"); 
	if (transitions_by_letter.size() == 1)
		return transitions_by_letter.front()->get_name();

	std::string full_transition_name("{");
	for (auto state : transitions_by_letter)
	{
		full_transition_name += state->get_name() + std::string(",");
	}
	full_transition_name.back() = '}';

	return full_transition_name;
}

const std::set<char>& ConverterFromReToNfa::get_alphabet() const
{
	return alphabet;
}