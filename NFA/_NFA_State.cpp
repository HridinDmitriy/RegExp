#include "_NFA_State.h"

_NFA_State::_NFA_State(const std::string& _name, bool _init, bool _fin)
	: name(_name), is_init(_init), is_final(_fin)
{ }

void _NFA_State::set_name(const std::string& _name)
{
	name = _name; 
}

const std::string& _NFA_State::get_name() const
{
	return name;
}

void _NFA_State::set_final(bool _fin)
{
	is_final = _fin;       
}

bool _NFA_State::get_final() const
{
	return is_final;    
}

void _NFA_State::set_init(bool _init)
{
	is_init = _init;
}

bool _NFA_State::get_init() const
{
	return is_init; 
}

void _NFA_State::add_transition(std::pair<std::string, _NFA_State*> transition)
{
	if (transitions.find(transition.first) == transitions.end()) 
	{
		transitions.insert(std::make_pair(transition.first, std::vector<_NFA_State*>())); 
	}
	transitions[transition.first].push_back(transition.second); 
}

void _NFA_State::delete_transition(std::string _transition_key)
{
	transitions.erase(_transition_key);
}

const std::vector<_NFA_State*>& _NFA_State::get_states_by(std::string key)
{
	return transitions[key];
}

const std::vector<std::string> _NFA_State::get_alphabet() const
{
	std::vector<std::string> keys;
	for (auto trans : transitions)
	{
		keys.push_back(trans.first);
	}
	return keys; 
}
