#pragma once
#include <map>
#include <set>
#include "Lambda.h"
#include "_NFA_State.h"

struct Comparator
{
	bool operator()(_NFA_State* _left, _NFA_State* _right)
	{
		return _left->get_name() < _right->get_name();
	}
};

class StateSet
{
public:
	StateSet() = default;
	StateSet(_NFA_State* _state, bool _init = false);
	std::string get_name() const;
	bool get_final() const;
	bool get_init() const;
	void set_init(bool _init);
	void add_state(_NFA_State* _new_state);
	template<typename _Iter>
	void add_state(_Iter _left, _Iter _right);
	void add_transition(std::pair<std::string, StateSet*> _transition);
	void delete_transition(std::string _transition_key);
	_NFA_State* operator[] (size_t _pos) const;
	size_t size() const;
	const StateSet* get_state_by(std::string _key);
private: 
	void _update_name();
	void _add_lambdas_to_super_state(_NFA_State* _state);
	void _set_final(bool _final);
private:
	std::string name;
	bool is_final;
	bool is_init;
	std::set<_NFA_State*, Comparator> super_state;
	std::map<std::string, StateSet*> transitions;
};

template<typename _Iter>
inline void StateSet::add_state(_Iter _left, _Iter _right)
{
	for (auto it = _left; it != _right; ++it)
	{
		add_state(*it);
	}
}

