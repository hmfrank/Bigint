#ifndef BIGINT2_BIGINT_H
#define BIGINT2_BIGINT_H

#include <cstddef>
#include <cstdint>
#include <vector>

class Bigint
{
protected:
	std::vector<uint32_t> data;

	void normalize();

	uint32_t get(size_t index) const;

	void set(size_t index, uint32_t value, bool normalize = true, uint32_t mask = 0xFFFFFFFF);

public:
	Bigint();

	Bigint(uint32_t);

	Bigint(Bigint const &);

	Bigint(Bigint &&);

	bool is_negative() const;

	void negate();

	int compare_to(Bigint const &) const;


	Bigint operator -() const;

	Bigint &operator +=(Bigint const &);

	Bigint &operator -=(Bigint const &);

	Bigint operator +(Bigint const &) const;

	Bigint operator -(Bigint const &) const;

	Bigint &operator >>=(int);

	Bigint &operator <<=(int);

	bool operator ==(Bigint const &) const;

	bool operator <(Bigint const &) const;

	bool operator >(Bigint const &) const;

	bool operator <=(Bigint const &) const;

	bool operator >=(Bigint const &) const;
};


#endif //BIGINT2_BIGINT_H
