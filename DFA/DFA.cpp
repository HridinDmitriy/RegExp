#include "DFA.h" 
#include <algorithm>

DFA::~DFA()
{
	for (auto state : states)
	{
		delete state.second; 
	}
	states.clear();
}

void DFA::read_alphabet(std::string file_name)
{
	strm.open(file_name);
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
		std::cout << "������� ������� �������!\n\n";
		strm.close(); 
	}
	else
	{
		std::cout << "�� ������� ������� ����\n\n";
	}
}

void DFA::set_alphabet(const Alphabet & _other_alphabet)
{
	alphabet = _other_alphabet; 
}

void DFA::read_states(std::string file_name)
{
	strm.open(file_name);
	if (strm.is_open())
	{
		std::string name;
		std::string transition_state_name;
		bool init;
		bool final; 

		while (!strm.eof())
		{
			if (strm >> name)
			{
				if (states.find(name) == states.end())
				{
					State* new_state = new State(name); 
					states.insert(std::make_pair(new_state->get_name(), new_state)); 
				}

				State* state = states[name]; 

				strm >> init;
				state->set_init(init); 

				strm >> final;
				state->set_final(final);

				for (size_t i = 0; i < alphabet.size(); ++i)
				{
					strm >> transition_state_name;
					if (states.find(transition_state_name) == states.end())
					{
						State* new_transition_state = new State(transition_state_name);
						states.insert(std::make_pair(new_transition_state->get_name(), new_transition_state));
					}
					state->add_transition(std::make_pair(alphabet[i], states[transition_state_name]));
				}
			}
		}
		std::cout << "��������� ��������� DFA ������� ��������!\n\n";       
		strm.close();
	}
	else
	{
		std::cout << "�� ������� ������� ����\n\n";
	}
}

void DFA::process_string(std::string str)
{
	State* current_state = (*std::find_if(states.cbegin(), states.cend(), 
		[](std::pair<std::string, State*> state) { return state.second->get_init(); })).second;

	std::cout << "\n - " << current_state->get_name();

	size_t str_len = str.length();
	for (size_t i = 0; i < str_len; ++i)
	{
		current_state = current_state->get_state_by(str[i]); 
		if (!current_state)
		{
			std::cout << "\n\n������ �������� �������, ������� ��� � ��������!\n\n";
			return;
		}

		std::cout << "\n - " << current_state->get_name();
		if (i == (str_len - 1))
		{
			if (current_state->get_final())
			{
				std::cout << "\n\n������ ������ ������������� �������� �����!\n\n";
			}
			else
			{
				std::cout << "\n\n������ ������ �� ����������� �����!\n\n";
			}
		}
	}
}
