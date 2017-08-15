#include "catch.hpp"

#include "Bigint.h"

TEST_CASE("Bigint Default Constructor", "Bigint::Bigint()")
{
	Bigint i;

	REQUIRE(i.get_data().size() == 0);
}

TEST_CASE("Bigint uint32_t Constructor", "Bigint::Bigint(uint32_t)")
{
	Bigint i(0);
	Bigint j(187);

	REQUIRE(i.get_data().size() == 0);
	REQUIRE(j.get_data().size() == 1);
	REQUIRE(j.get_data()[0] == 187);
}

TEST_CASE("Bigint Copy Constructor", "Bigint::Bigint(Bigint const &)")
{
	Bigint i(420);
	Bigint j(i);

	REQUIRE(i.get_data() == j.get_data());
	REQUIRE(i == j);
}

TEST_CASE("Bigint Move Constructor", "Bigint::Bigint(Bigint &&)")
{
	Bigint i(31);
	Bigint j = std::move(i);

	REQUIRE(i.get_data().size() == 0);
	REQUIRE(j.get_data().size() == 1);
	REQUIRE(j.get_data()[0] == 31);
}

// TODO: test Bigint::operator <<=
// TODO: test Bigint::operator +=

TEST_CASE("Bigint is negative", "Bigint::is_negative()")
{
	Bigint i(0);
	Bigint j(5);
	Bigint k(-5);
	Bigint l(j);
	Bigint m(k);

	l <<= 32;
	l += k;
	m <<= 32;
	m += j;

	REQUIRE(!i.is_negative());
	REQUIRE(!j.is_negative());
	REQUIRE(k.is_negative());
	REQUIRE(!l.is_negative());
	REQUIRE(m.is_negative());
}

TEST_CASE("Bigint negation", "Bigint::negate()")
{
	Bigint i(0);
	Bigint j(20);
	Bigint k(1);

	k <<= 31; // k = 2^31

	i.negate();
	REQUIRE(i.get_data().size() == 0);

	j.negate();
	REQUIRE(j.get_data().size() == 1);
	REQUIRE(j.get_data()[0] == -20);

	j.negate();
	REQUIRE(j.get_data().size() == 1);
	REQUIRE(j.get_data()[0] == 20);

	k.negate();
	REQUIRE(k.get_data().size() == 1);
	REQUIRE(k.get_data()[0] == 2147483648);

	k.negate();
	REQUIRE(k.get_data().size() == 2);
	REQUIRE(k.get_data()[0] == 2147483648);
	REQUIRE(k.get_data()[1] == 0);
}