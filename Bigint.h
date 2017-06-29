#ifndef BIGINT2_BIGINT_H
#define BIGINT2_BIGINT_H

#include <cstddef>
#include <cstdint>
#include <vector>

class Bigint
{
protected:
	std::vector<int32_t> data;

	void normalize();

	int32_t get(size_t index) const;

	void set(size_t index, int32_t value);

public:
	Bigint();

	Bigint(int32_t);

	bool is_negative() const;
};


#endif //BIGINT2_BIGINT_H
