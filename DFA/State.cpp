#include "State.h"

State::State(std::string _name, bool _init, bool _fin)  
	: name(_name), is_init(_init), is_final(_fin)
{ }

void State::set_name(std::string _name)
{
	name = _name;
}

void State::set_init(bool er)
{
	is_init = er;
}

void State::set_final(bool fin)
{
	is_final = fin;
}

std::string State::get_name() const
{
	return name;
}

bool State::get_init() const
{
	return is_init;
}

bool State::get_final() const
{
	return is_final;
}

void State::add_transition(std::pair<char, State*> transition)
{
	transitions.insert(transition); 
}

State* State::get_state_by(char key) 
{
	return transitions[key];      
}