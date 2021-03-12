#pragma once
#include <string>
#include <map> 

class State
{
public:
	State() = default; 
	State(std::string _name, bool _er = false, bool _fin = false);  
	std::string get_name() const;
	void set_name(std::string _name);
	void set_init(bool er);
	void set_final(bool fin);
	bool get_init() const; 
	bool get_final() const;
	void add_transition(std::pair<char, State*> transition);
	State* get_state_by(char key);  
private: 
	std::string name; 
	bool is_init;
	bool is_final;
	std::map<char, State*> transitions;  
};

