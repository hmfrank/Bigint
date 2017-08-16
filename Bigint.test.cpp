#include "catch.hpp"

#include "Bigint.h"

TEST_CASE("Bigint Default Constructor", "Bigint::Bigint()")
{
	Bigint i;

	REQUIRE(i.get_data().size() == 0);
}

TEST_CASE("Bigint::Bigint(uint32_t)")
{
	Bigint i(0);
	Bigint j(187);

	REQUIRE(i.get_data().size() == 0);
	REQUIRE(j.get_data().size() == 1);
	REQUIRE(j.get_data()[0] == 187);
}

// NOT TESTED: Bigint::operator ==(Bigint const &)
// NOT TESTED: Bigint::operator !=(Bigint const &)

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

TEST_CASE("Bigint::operator >>=(int)")
{
	Bigint i(12);
	Bigint j(12);
	Bigint k(1);
	Bigint l(-30);
	Bigint m(-8);

	REQUIRE(&(i >>= 0) == &i);
	REQUIRE(i == j);

	REQUIRE(&(i >>= 3) == &i);
	REQUIRE(i == k);

	REQUIRE(&(l >>= 2) == &l);
	REQUIRE(l == m);

	k <<= 64;
	REQUIRE(&(k >>= 33) == &k);
	REQUIRE(k.get_data().size() == 2);
	REQUIRE(k.get_data()[0] == 2147483648);
	REQUIRE(k.get_data()[1] == 0);

	REQUIRE(&(k >>= 1) == &k);
	REQUIRE(k.get_data().size() == 1);
	REQUIRE(k.get_data()[0] == 1073741824);

	REQUIRE(&(k >>= 31) == &k);
	REQUIRE(k.get_data().size() == 0);
}

TEST_CASE("Bigint::operator +=(Bigint const &)")
{
	Bigint i(12);
	Bigint j(3456);
	Bigint k(-78);
	Bigint l(-9012);
	Bigint m(2000000000);
	Bigint n(-2000000000);


	j <<= 32;
	l <<= 32;

	REQUIRE(&(j += i) == &j);
	REQUIRE(j.get_data().size() == 2);
	REQUIRE(j.get_data()[0] == 12);
	REQUIRE(j.get_data()[1] == 3456);

	REQUIRE(&(j += k) == &j);
	REQUIRE(j.get_data().size() == 2);
	REQUIRE(j.get_data()[0] == 4294967230);
	REQUIRE(j.get_data()[1] == 3455);

	REQUIRE(&(j += l) == &j);
	REQUIRE(j.get_data().size() == 2);
	REQUIRE(j.get_data()[0] == 4294967230);
	REQUIRE(j.get_data()[1] == -5557);

	REQUIRE(&(j += l) == &j);
	REQUIRE(j.get_data().size() == 2);
	REQUIRE(j.get_data()[0] == 4294967230);
	REQUIRE(j.get_data()[1] == -14569);

	REQUIRE(&(m += m) == &m);
	REQUIRE(m.get_data().size() == 2);
	REQUIRE(m.get_data()[0] == 4000000000);
	REQUIRE(m.get_data()[1] == 0);

	REQUIRE(&(m += n) == &m);
	REQUIRE(m.get_data().size() == 1);
	REQUIRE(m.get_data()[0] == 2000000000);

	REQUIRE(&(m += Bigint(2000000000)) == &m);
	REQUIRE(&(m += Bigint(2000000000)) == &m);
	REQUIRE(m.get_data().size() == 2);
	REQUIRE(m.get_data()[0] == 1705032704);
	REQUIRE(m.get_data()[1] == 1);

	REQUIRE(&(n += m) == &n);
	REQUIRE(n.get_data().size() == 2);
	REQUIRE(n.get_data()[0] == 4000000000);
	REQUIRE(n.get_data()[1] == 0);
}

TEST_CASE("Bigint::is_negative()")
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

TEST_CASE("Bigint::negate()")
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

TEST_CASE("Bigint::compare_to(Bigint const &)")
{
	Bigint i(0);
	Bigint j(42);
	Bigint k(-37);

	k <<= 32;

	REQUIRE(i.compare_to(i) == 0);
	REQUIRE(i.compare_to(j) < 0);
	REQUIRE(i.compare_to(k) > 0);
	REQUIRE(j.compare_to(i) > 0);
	REQUIRE(j.compare_to(j) == 0);
	REQUIRE(j.compare_to(k) > 0);
	REQUIRE(k.compare_to(i) < 0);
	REQUIRE(k.compare_to(j) < 0);
	REQUIRE(k.compare_to(k) == 0);
}

TEST_CASE("Bigint::operator -()")
{
	Bigint i(9);
	Bigint j = -i;

	REQUIRE(i != j);

	i.negate();

	REQUIRE(i == j);
}

// NOT TESTED: Bigint::operator -=(Bigint const &)
// NOT TESTED: Bigint::operator +(Bigint const &)
// NOT TESTED: Bigint::operator -(Bigint const &)