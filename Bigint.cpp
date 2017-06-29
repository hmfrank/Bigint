#include <cstdio>
#include "Bigint.h"

static inline bool get_msb(int32_t x)
{
	return (x & 0x80000000) != 0;
}

void Bigint::normalize()
{
	bool sign = this->is_negative();

	for (size_t i = this->data.size(); i--;)
	{
		if (this->data[i] == sign * -1 && ((i ? get_msb(this->data[i - 1]) : false) == sign))
		{
			this->data.pop_back();
		}
	}
}

int32_t Bigint::get(size_t index) const
{
	if (index >= this->data.size())
	{
		return this->is_negative() * -1;
	}
	else
	{
		return this->data[index];
	}
}

void Bigint::set(size_t index, int32_t value)
{
	// expand array, if necessary
	if (index >= this->data.size())
	{
		this->data.reserve(index + 1);

		for (size_t i = this->data.size(); i <= index; i++)
		{
			this->data.push_back(this->is_negative() * -1);
		}
	}

	this->data[index] = value;
	this->normalize();
}



Bigint::Bigint() : Bigint(0)
{ }

Bigint::Bigint(int32_t value)
{
	if (value != 0)
	{
		this->data.push_back(value);
	}
}

bool Bigint::is_negative() const
{
	size_t length = this->data.size();

	return length ? get_msb(this->data[length - 1]) : false;
}
