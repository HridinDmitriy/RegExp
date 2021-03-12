#pragma once
#include <string>
#include <map> 
#include <vector>

class _NFA_State
{
public:
	_NFA_State() = default; 
	_NFA_State(const std::string& _name, bool _init = false, bool _fin = false); 
	void set_name(const std::string& _name);
	const std::string& get_name() const;
	void set_init(bool _init);
	bool get_init() const;
	void set_final(bool _fin); 
	bool get_final() const; 
	void add_transition(std::pair<std::string, _NFA_State*> _transition);
	void delete_transition(std::string _transition_key);
	const std::vector<_NFA_State*>& get_states_by(std::string key);  
	const std::vector<std::string> get_alphabet() const;
private: 
	std::string name;
	bool is_init;
	bool is_final;   
	std::map<std::string, std::vector<_NFA_State*>> transitions;
};

