#include "Alphabet.h"

Alphabet::Alphabet(std::initializer_list<char> value_list)
{
	for (auto letter : value_list)
	{
		alphabet.push_back(letter);
	}
}

void Alphabet::add_letter(char value) 
{
	alphabet.push_back(value);
}

const std::vector<char>& Alphabet::get_alphabet() const
{
	return alphabet;
}

char Alphabet::operator[](size_t index) const
{
	if (index < alphabet.size())
		return alphabet[index];
	else
		throw std::string("IndexOutOfRangeException");
}

size_t Alphabet::size() const
{
	return alphabet.size();
}