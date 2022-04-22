#include <iostream>
#include "units.h"

enum class si_units
{
	metre,
	kilogram,
	ampere,
	kelvin,
	second,
	mole,
	candela,
	_count
};


using second = basic_unit<si_units, si_units::second>;
using metre = basic_unit<si_units, si_units::metre>;
using kilogram = basic_unit<si_units, si_units::kilogram>;

using metre_per_second = divided_unit<metre, second>;
using newton = divided_unit<multiplied_unit<kilogram, metre>, multiplied_unit<second, second>>;

int main()
{
	quantity<metre> l(2.1);
    quantity<second> t(0.9);
    auto v1(l / t);

    quantity<metre_per_second> v2{ 2.5 };

    std::cout << (v1 + v2).value() << std::endl;
};