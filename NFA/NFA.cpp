#include "NFA.h"
#include <algorithm>
#include <queue>
#include <iomanip>

NFA::~NFA()
{
	for (auto pair : nfa_states) 
	{
		delete pair.second;
	}
	nfa_states.clear(); 

	for (auto pair : dfa_states)
	{
		delete pair.second;
	}
	dfa_states.clear();
}

void NFA::read_alphabet(const std::string& file_name)
{
	strm.open(file_name, std::ios::in);  
	if (strm.is_open())
	{
		char letter;
		while (!strm.eof())
		{
			if (strm >> letter)
			{
				alphabet.add_letter(letter);
			}
		}
		// lambda
		alphabet.add_letter(LAMBDA);  

		std::cout << "Алфавит успешно записан!\n\n";
		strm.close();
	}
	else
	{
		std::cout << "Не удается открыть файл с алфавитом\n\n";
	}
}

const std::vector<std::string> NFA::_parse_transition_name(const std::string& str)
{
	std::vector<std::string> names;
	if (str[0] == '{')
	{
		std::string transit_name;
		for (size_t i = 1; str[i] != '}'; ++i)
		{
			char ch = str[i];
			if (ch != ',')
			{
				transit_name.append({ ch });
			}
			else
			{
				names.push_back(transit_name);
				transit_name.clear();
			}
		}
		names.push_back(transit_name);
	}
	else
	{
		names.push_back(str);
	}
	return names;
}

void NFA::read_states(const std::string& file_name)
{
	strm.open(file_name, std::ios::in);
	if (strm.is_open())
	{
		std::string name;
		std::string transition_name_set;
		bool init;
		bool final; 

		while (!strm.eof())
		{
			if (strm >> name)
			{
				if (nfa_states.find(name) == nfa_states.end())
				{
					_NFA_State* new_state = new _NFA_State(name);
					nfa_states.insert(std::make_pair(new_state->get_name(), new_state));
				}

				_NFA_State* state = nfa_states[name];

				strm >> init; 
				state->set_init(init);

				strm >> final;
				state->set_final(final);

				for (size_t i = 0; i < alphabet.size(); ++i)
				{
					strm >> transition_name_set;
					auto transit_names = _parse_transition_name(transition_name_set);

					size_t count = transit_names.size();
					for (size_t j = 0; j < count; ++j)
					{
						if (transit_names[j] != "")
						{
							if (nfa_states.find(transit_names[j]) == nfa_states.end())
							{
								_NFA_State* new_transition_state = new _NFA_State(transit_names[j]);
								nfa_states.insert(std::make_pair(new_transition_state->get_name(), new_transition_state));
							}
							state->add_transition(std::make_pair(std::string({ alphabet[i] }), nfa_states[transit_names[j]]));
						}
					}
				}
			}
		}
		std::cout << "Множество состояний NFA успешно записано!\n\n";
		strm.close();
	}
	else
	{
		std::cout << "Не удается открыть файл состояний\n\n";
	}
}

void NFA::generate_dfa_states()
{
	std::queue<StateSet*> new_states;

	 _NFA_State* init_nfa_state = (*std::find_if(nfa_states.begin(), nfa_states.end(), 
		[](std::pair<std::string, _NFA_State*> state) { return state.second->get_init(); })).second; 
	 
	 StateSet* initial_dfa_state = new StateSet(init_nfa_state, true); 
	 dfa_states.insert(std::make_pair(initial_dfa_state->get_name(), initial_dfa_state));  
	 new_states.push(initial_dfa_state);

	 while (!new_states.empty())  
	 {
		 StateSet* cur_state = new_states.front();
		 new_states.pop();

		 size_t state_size = cur_state->size();
		 size_t new_states_counter = 0;

		 size_t alphabet_size = alphabet.size() - 1; 
		 for (size_t i = 0; i < alphabet_size; ++i)
		 {
			 StateSet* state_set = new StateSet();
			 for (size_t j = 0; j < state_size; ++j)
			 {
				 auto transitions_by_cur_symb = cur_state->operator[](j)->get_states_by(std::string({ alphabet[i] }));
				 state_set->add_state(transitions_by_cur_symb.begin(), transitions_by_cur_symb.end());
			 }

			 std::string state_set_name = state_set->get_name();
			 if (dfa_states.find(state_set_name) == dfa_states.end())
			 {
				 dfa_states.insert(std::make_pair(state_set_name, state_set));
				 new_states.push(state_set);
			 }
			 else
			 {
				 delete state_set;
			 }
			 cur_state->add_transition(std::make_pair(std::string({ alphabet[i] }), (*dfa_states.find(state_set_name)).second)); 
		 }
	 }
}

void NFA::write_dfa_states(const std::string& file_name) 
{
	strm.open(file_name, std::ios::out | std::ios::trunc);  
	
	for (auto state_pair : dfa_states)
	{
		strm << state_pair.second->get_name() << std::setw(5) << "\t" << state_pair.second->get_init() <<
			std::setw(5) << "\t" << state_pair.second->get_final();

		size_t count = alphabet.size() - 1;
		for (size_t i = 0; i < count; ++i)
		{ 
			strm << std::setw(5) << "\t" << state_pair.second->get_state_by(std::string({ alphabet[i] }))->get_name();
		}
		strm << "\n";
	}
	strm.close();
}

void NFA::initialize_dfa(const std::string& dfa_states_file_name)
{
	auto alp = alphabet.get_alphabet();
	dfa.set_alphabet(alp.begin(), --alp.end()); 
	dfa.read_states(dfa_states_file_name);
}

void NFA::process_string(const std::string & str)
{
	dfa.process_string(str); 
}

void NFA::to_minimize()
{
	_Table_cells table; 
	std::set<StateSet*> to_delete;

	_create_trivial_table(table); 
	_find_all_equal_states(table);

	for (_Table_cells::iterator cell_it = table.begin(); cell_it != table.end(); ++cell_it) 
	{
		if (!cell_it->second) 
		{
			StateSet* left_state = dfa_states.at(cell_it->first.first);
			StateSet* right_state = dfa_states.at(cell_it->first.second);

			if (left_state != right_state) 
			{
				size_t alphabet_size = alphabet.size() - 1;
				for (std::map<std::string, StateSet*>::iterator pair = dfa_states.begin();
					pair != dfa_states.end(); ++pair)
				{
					for (size_t i = 0; i < alphabet_size; ++i)
					{
						std::string symb = std::string({ alphabet[i] }); 
						if (pair->second->get_state_by(symb) == right_state) 
						{
							pair->second->delete_transition(symb);
							pair->second->add_transition(std::make_pair(symb, left_state));
							to_delete.insert(right_state);
						}
					}
				}                   
			}
		}
	}

	for (auto it = to_delete.begin(); it != to_delete.end(); ++it)
	{
		dfa_states.erase((*it)->get_name());
		delete *it;  
	}
}
 
void NFA::_create_trivial_table(_Table_cells& table)
{
	for (std::map<std::string, StateSet*>::const_iterator left_it = dfa_states.begin();
		left_it != dfa_states.end(); ++left_it)
	{
		for (std::map<std::string, StateSet*>::const_iterator right_it = left_it;
			right_it != dfa_states.end(); ++right_it)
		{
			table[std::make_pair(left_it->second->get_name(), right_it->second->get_name())]
				= left_it->second->get_final() != right_it->second->get_final();
		}
	}
}

void NFA::_find_all_equal_states(_Table_cells& table)
{
	bool  there_are_new_diff_states = true;
	while (there_are_new_diff_states)
	{
		there_are_new_diff_states = false;
		for (_Table_cells::iterator cell_it = table.begin(); cell_it != table.end(); ++cell_it)
		{
			if (!cell_it->second)
			{
				std::string  left = cell_it->first.first;
				std::string  right = cell_it->first.second;

				size_t alphabet_size = alphabet.size() - 1; 
				for (size_t i = 0; i < alphabet_size; ++i)
				{
					std::string  left_transition_by_cur_letter = dfa_states.at(left)->get_state_by(std::string({ alphabet[i] }))->get_name();
					std::string  right_transition_by_cur_letter = dfa_states.at(right)->get_state_by(std::string({ alphabet[i] }))->get_name();

					// если ячейка с указанными координатами находится ниже главной диагонали таблицы...
					if (left_transition_by_cur_letter > right_transition_by_cur_letter)
					{
						std::swap(left_transition_by_cur_letter, right_transition_by_cur_letter);
					}

					if (table.find(std::make_pair(left_transition_by_cur_letter, right_transition_by_cur_letter))->second)
					{
						cell_it->second = true;
						there_are_new_diff_states = true;
						break;
					}                        
				}
			}
			if (there_are_new_diff_states)
			{
				break;
			}
		}
	} 
}
