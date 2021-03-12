#include "StateSet.h"

StateSet::StateSet(_NFA_State * _state, bool _init)
{
	add_state(_state);
	set_init(_init);
}

std::string StateSet::get_name() const
{
	return ("{" + name + "}");  
}

bool StateSet::get_final() const
{
	return is_final;  
}

bool StateSet::get_init() const
{
	return is_init;
}

void StateSet::set_init(bool _init)
{
	is_init = _init;
}

void StateSet::_set_final(bool _final)
{
	is_final = _final;
}

void StateSet::_update_name() 
{
	name.clear();
	for (auto state : super_state)
	{
		name += state->get_name() + ",";  
	}

	auto it = name.end();
	name.erase(--it);  
}

void StateSet::add_transition(std::pair<std::string, StateSet*> _transition)
{
	transitions.insert(_transition); 
}

void StateSet::delete_transition(std::string _transition_key)
{
	transitions.erase(_transition_key);
}

void StateSet::_add_lambdas_to_super_state(_NFA_State* _state)
{
	auto lambda_transitions = _state->get_states_by(std::string({ LAMBDA }));
	size_t count = lambda_transitions.size();

	for (size_t i = 0; i < count; ++i)
	{
		add_state(lambda_transitions[i]); 
	}
}

void StateSet::add_state(_NFA_State* _new_state)  
{
	if (super_state.insert(_new_state).second) 
	{
		if (!get_final() && _new_state->get_final())
		{
			_set_final(true);
		}

		_update_name();
		
		_add_lambdas_to_super_state(_new_state);
	} 
}

_NFA_State* StateSet::operator[](size_t _pos) const 
{
	auto it = super_state.begin();
	for (size_t i = 0; i < _pos; ++i)
	{
		++it; 
	}
	return *it; 
}

size_t StateSet::size() const
{
	return super_state.size();
}

const StateSet* StateSet::get_state_by(std::string _key)
{
	return transitions[_key]; 
}

