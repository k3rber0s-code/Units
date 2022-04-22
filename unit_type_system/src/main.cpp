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
#include "units.h"
#include "tests_common.h"
#include <iostream>

    using namespace std;

    int main()
    {
        // Basic logic of quantity<...>

        quantity<second> t(4.2);
        quantity<second, int32_t> t_int32(42);

        quantity<metre> l(666);

        cout << t.value() << endl;
        cout << t_int32.value() << endl;

        cout << is_same<
            decltype(t.value()),
            double
        >::value << endl;

        cout << is_same<
            decltype(t_int32.value()),
            int32_t
        >::value << endl;

        cout << (t + t).value() << endl;
        cout << (t - t).value() << endl;
        cout << (l * t).value() << endl;
        cout << (l / t).value() << endl;

        cout << is_same<
            decltype(t + t),
            decltype(t)
        >::value << endl;

        cout << is_same<
            decltype(t - t),
            decltype(t)
        >::value << endl;

        cout << is_same<
            decltype(l* l* l),
            cubic_metre
        >::value << endl;

        cout << is_same<
            decltype(l / t),
            metre_per_second
        >::value << endl;

        // Different units, types or scalar are not implicitly convertible


        cout << is_convertible<
            decltype(l),
            decltype(t)
        >::value << endl;

        cout << is_convertible<
            decltype(t),
            decltype(t_int32)
        >::value << endl;

        cout << is_convertible<
            decltype(l.value()),
            decltype(l)
        >::value << endl;
};