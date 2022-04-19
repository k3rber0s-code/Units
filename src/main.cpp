#include <iostream>

template <int... Values>
struct static_vector
{
	size_t s = sizeof...(Values);
};

template <typename Vector, int Added>
struct push_back
{
};

template <int Added, int... Values>
struct push_back<static_vector<Values...>, Added>
{
	using type = static_vector<Values..., Added>;
};

template <typename Vector, size_t Index>
struct at
{
};

template <int Head, int... Tail>
struct at<static_vector<Head, Tail...>, 0>
{
	static constexpr auto value = Head;
};

template <size_t Index, int Head, int... Tail>
struct at<static_vector<Head, Tail...>, Index>
{
	static constexpr auto value = at<static_vector<Tail...>, Index - 1>::value;
};

template <typename, typename>
struct static_vector_cat;

template <int... E1, int... E2>
struct static_vector_cat<static_vector<E1...>, static_vector<E2...>>
{
	using type = static_vector<E1..., E2...>;
};

template <typename, typename>
struct static_vector_add;

template <int E1H, int... E1T, int E2H, int... E2T>
struct static_vector_add<static_vector<E1H, E1T...>, static_vector<E2H, E2T...>>
{
	using type = typename static_vector_cat<static_vector<E1H + E2H>, typename static_vector_add<static_vector<E1T...>, static_vector<E2T...>>::type>::type;
};

template <int... E2>
struct static_vector_add<static_vector<0>, static_vector<E2...>>
{
	using type = static_vector<E2...>;
};

template <int... E1>
struct static_vector_add<static_vector<E1...>, static_vector<0>>
{
	using type = static_vector<E1...>;
};

template <>
struct static_vector_add<static_vector<0>, static_vector<0>>
{
	using type = static_vector<0>;
};

template <typename, size_t Capacity, int Index>
struct static_vector_create
{
};

template <int Index, size_t Capacity>
struct static_vector_create<static_vector<>, Capacity, Index>
{
	using type = typename static_vector_create<push_back<static_vector<>, 0>::type, Capacity - 1, Index - 1>::type;
};

template <int Index, size_t Capacity, int Head, int... Tail>
struct static_vector_create<static_vector<Head, Tail...>, Capacity, Index>
{
	using type = typename static_vector_create<typename push_back<static_vector<Head, Tail...>, 0>::type, Capacity - 1, Index - 1>::type;
};

template <size_t Capacity, int Head, int... Tail>
struct static_vector_create<static_vector<Head, Tail...>, Capacity, 0>
{
	using type = typename static_vector_create<typename push_back<static_vector<Head, Tail...>, 1>::type, Capacity - 1, -1>::type;
};

template <int Index, int Head, int... Tail>
struct static_vector_create<static_vector<Head, Tail...>, 0, Index>
{
	using type = static_vector<Head, Tail...>;
};
template <typename TEnum, typename TPowers>
struct unit;

template <typename TEnum, TEnum Index>
using basic_unit = unit<TEnum, typename static_vector_create<static_vector<>, (int)TEnum::_count, (int)Index>::type>;

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

int main()
{
	using second = basic_unit<si_units, si_units::second>;
	using v = static_vector<4, 3, 2, 1, 0>;
	using v2 = static_vector<4, 4, 2, 1, 0>;
	using v3 = push_back<v2, 9>::type;
	using v4 = static_vector<at<v, 0>::value + at<v, 0>::value>;
	// std::cout << at<v, 0>::value << std::endl;
	using v7 = typename static_vector_create<static_vector<>, 3, 1>::type;
	std::cout << at<v7, 0>::value << " ";
	std::cout << at<v7, 1>::value << " ";
	std::cout << at<v7, 2>::value << " ";

	std::cout << second << std::endl;

	using v5 = typename static_vector_add<v, v2>::type;
	using v6 = static_vector<0>;
	// std::cout << at<v6, 0>::value << "v6" << std::endl;

	// std::cout << at<v5, 0>::value << std::endl;
	// std::cout << at<v5, 1>::value << std::endl;
	// std::cout << at<v5, 2>::value << std::endl;
	// std::cout << at<v5, 3>::value << std::endl;
	// std::cout << at<v5, 4>::value << std::endl;
}
