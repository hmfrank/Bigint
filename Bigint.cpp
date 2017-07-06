#include <cstdio>
#include "Bigint.h"

static inline size_t max(size_t x, size_t y)
{
	return x > y ? x : y;
}

static inline size_t min(size_t x, size_t y)
{
	return x < y ? x : y;
}


static inline bool get_msb(uint32_t x)
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
		else
		{
			break;
		}
	}
}

uint32_t Bigint::get(size_t index) const
{
	if (index >= this->data.size())
	{
		return this->is_negative() * 0xFFFFFFFF;
	}
	else
	{
		return this->data[index];
	}
}

void Bigint::set(size_t index, uint32_t value, bool normalize, uint32_t mask)
{
	// expand array, if necessary
	if (index >= this->data.size())
	{
		this->data.reserve(index + 1);

		for (size_t i = this->data.size(); i <= index; i++)
		{
			this->data.push_back(this->is_negative() * 0xFFFFFFFF);
		}
	}

	if (mask == 0xFFFFFFFF) // This if is there for performance reasons, but it's actually logically redundant.
	{
		this->data[index] = value;
	}
	else
	{
		this->data[index] &= ~mask;
		this->data[index] |= value & mask;
	}

	if (normalize)
	{
		this->normalize();
	}
}


Bigint::Bigint() : Bigint(0)
{}

Bigint::Bigint(uint32_t value)
{
	if (value != 0)
	{
		this->data.push_back(value);
	}
}

Bigint::Bigint(Bigint const &x)
{
	this->data = x.data;
}

Bigint::Bigint(Bigint &&x)
{
	this->data = std::move(x.data);
}

bool Bigint::is_negative() const
{
	size_t length = this->data.size();

	return length ? get_msb(this->data[length - 1]) : false;
}

void Bigint::negate()
{
	bool sign = this->is_negative();
	uint64_t carry = 1;

	for (size_t i = 0; i < this->data.size(); i++)
	{
		carry += ~this->data[i];
		this->data[i] = static_cast<int32_t>(carry);
		carry >>= 32;
	}

	if (this->is_negative() == sign)
	{
		this->data.push_back(!sign * 0xFFFFFFFF);
	}

	this->normalize();
}

int Bigint::compare_to(Bigint const &x) const
{
	bool sign_t = this->is_negative();
	bool sign_x = x.is_negative();

	if (sign_t != sign_x)
	{
		return sign_t ? -1 : 1;
	}

	for (size_t i = max(this->data.size(), x.data.size()); i--;)
	{
		uint32_t t_value = this->get(i);
		uint32_t x_value = x.get(i);

		if (t_value < x_value)
		{
			return -1;
		}
		else if (t_value > x_value)
		{
			return 1;
		}
	}

	return 0;
}


Bigint Bigint::operator -() const
{
	Bigint copy(*this);
	copy.negate();
	return std::move(copy); // TODO: ann freych ob des su ghet
}

// TODO: Performance verbessen, indem ma am Anfang mit ää ma in ganzen Speicher reserviert
Bigint &Bigint::operator +=(Bigint const &x)
{
	size_t n = max(this->data.size(), x.data.size());
	bool equal_sign = this->is_negative() == x.is_negative();
	uint64_t sum = 0;

	for (size_t i = 0; i < n; i++)
	{
		sum += static_cast<uint64_t>(this->get(i)) + x.get(i);
		this->set(i, static_cast<uint32_t>(sum), false);
		sum >>= 32;
	}

	if (equal_sign && (this->is_negative() != x.is_negative()))
	{
		this->data.push_back(x.is_negative() * 0xFFFFFFFF);
	}

	this->normalize(); // just to be sure
	return *this;
}

Bigint &Bigint::operator -=(Bigint const &x)
{
	return *this += (-x);
}

Bigint Bigint::operator +(Bigint const &x) const
{
	Bigint copy(x);
	copy += x;
	return copy;
}

Bigint Bigint::operator -(Bigint const &x) const
{
	Bigint copy(x);
	copy -= x;
	return copy;
}

Bigint &Bigint::operator >>=(int x)
{
	if (x < 0)
		return *this <<= -x;

	size_t word_shift = static_cast<unsigned>(x) >> 5;
	size_t bit_shift = static_cast<unsigned>(x) & 0x1F;

	// shift words to the right
	size_t n = this->data.size();
	for (size_t i = word_shift; i <= n; i++)
	{
		int64_t word = this->get(i);
		word <<= 32;
		word >>= bit_shift;

		this->set(i - word_shift, static_cast<uint32_t>(word >> 32), false);
		if (i > word_shift)
		{
			this->set(i - word_shift - 1, static_cast<uint32_t>(word), false, ~(0xFFFFFFFF >> bit_shift));
		}
	}

	// cut off the left part
	this->data.resize(max(0, n - word_shift + 1));

	this->normalize(); // just to be sure
	return *this;
}

Bigint &Bigint::operator <<=(int x)
{
	if (x < 0)
		return *this >>= -x;

	size_t word_shift = static_cast<unsigned>(x) >> 5;
	size_t bit_shift = static_cast<unsigned>(x) & 0x1F;

	// shift words to the left
	for (size_t i = this->data.size(); i--;)
	{
		uint64_t word = this->get(i);
		word <<= bit_shift;

		this->set(i + word_shift, static_cast<uint32_t>(word), false);
		this->set(i + word_shift + 1, static_cast<uint32_t>(word >> 32), false, ~(0xFFFFFFFF << bit_shift));
	}

	// put in new zeros at the right
	for (size_t i = 0; i < x; i += 32)
	{
		this->set(i >> 5, 0, false, ~(x - i >= 32 ? 0 : 0xFFFFFFFF << x - i));
	}

	this->normalize(); // just to be sure
	return *this;
}

bool Bigint::operator ==(Bigint const &x) const
{
	return this->data == x.data;
}

bool Bigint::operator <(Bigint const &x) const
{
	return this->compare_to(x) < 0;
}

bool Bigint::operator >(Bigint const &x) const
{
	return this->compare_to(x) > 0;
}

bool Bigint::operator <=(Bigint const &x) const
{
	return this->compare_to(x) <= 0;
}

bool Bigint::operator >=(Bigint const &x) const
{
	return this->compare_to(x) >= 0;
}
