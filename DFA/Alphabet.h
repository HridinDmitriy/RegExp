#pragma once
#include <vector>
#include <string>

class Alphabet
{
public:
	Alphabet() = default; 
	Alphabet(std::initializer_list<char> value_list);
	void add_letter(char letter); 
	template<typename _Iter>
	void add_letter(_Iter _left, _Iter _right); 
	const std::vector<char>& get_alphabet() const;
	char operator[](size_t index) const;
	size_t size() const;
private:
	std::vector<char> alphabet; 
};

template<typename _Iter>
inline void Alphabet::add_letter(_Iter _left, _Iter _right)
{
	for (auto it = _left; it != _right; ++it)
	{
		add_letter(*it); 
	}
}

