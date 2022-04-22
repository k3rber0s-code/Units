#pragma once
#ifndef UNITS_H
#define UNITS_H

#include <iostream>

/////////////////////////// STATIC VECTOR ///////////////////////////
template <int... Values>
struct static_vector
{
	static constexpr size_t s = sizeof...(Values);
};

/////////////////////////// ARE SAME ///////////////////////////
/*
Compares multiple units and checks if they are all of identical type.
@tparam U1,...Us units
*/

template <typename U1, typename... Us>
struct are_same : std::conjunction<std::is_same<U1, Us>...>
{
};

/////////////////////////// PUSH BACK ///////////////////////////
/*
Adds a value to the parameters of static_vector<>.
*/
template <typename Vector, int Added>
struct push_back
{
};

template <int Added, int... Values>
struct push_back<static_vector<Values...>, Added>
{
	using type = static_vector<Values..., Added>;
};

/////////////////////////// AT ///////////////////////////
/*
@returns parameter at given index from static_vector.
*/
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

/////////////////////////// STATIC VEC CAT ///////////////////////////
/*
Concatenates two static_vectors.
@tparam Values1..., Values2... => variadic list of parameters.
*/
template <typename, typename>
struct static_vector_cat;

template <int... Values1, int... Values2>
struct static_vector_cat<static_vector<Values1...>, static_vector<Values2...>>
{
	using type = static_vector<Values1..., Values2...>;
};

/////////////////////////// STATIC VECTOR ADD ///////////////////////////
/*
Adds parameters at the same position from two vectors.
@tparam V1..., V2... => parameter packs
@tparam V1H, V2H => first parameter (head)
@tparam V1t, V2T => rest of parameters (tail)
*/

template <typename, typename>
struct static_vector_add;

template <int V1H, int... V1T, int V2H, int... V2T>
struct static_vector_add<static_vector<V1H, V1T...>, static_vector<V2H, V2T...>>
{
	using type = typename static_vector_cat<static_vector<V1H + V2H>, typename static_vector_add<static_vector<V1T...>, static_vector<V2T...>>::type>::type;
};

template <int... V2>
struct static_vector_add<static_vector<0>, static_vector<V2...>>
{
	using type = static_vector<V2...>;
};

template <int... V1>
struct static_vector_add<static_vector<V1...>, static_vector<0>>
{
	using type = static_vector<V1...>;
};

template <>
struct static_vector_add<static_vector<0>, static_vector<0>>
{
	using type = static_vector<0>;
};

/////////////////////////// STATIC ADD MULTIPLE ///////////////////////////
/*
Adds parameters at the same position from multiple vectors.
@tparam V1, V2, Vector... static_vector<>;
*/

template <typename... Vector>
struct static_vector_add_multiple;

template <int... V1, int... V2, typename... Vector>
struct static_vector_add_multiple<static_vector<V1...>, static_vector<V2...>, Vector...>
{
	using type = typename static_vector_add_multiple<typename static_vector_add<static_vector<V1...>, static_vector<V2...>>::type, Vector...>::type;
};

template <int... V1, int... V2>
struct static_vector_add_multiple<static_vector<V1...>, static_vector<V2...>>
{
	using type = typename static_vector_add<static_vector<V1...>, static_vector<V2...>>::type;
};

template <int... V1>
struct static_vector_add_multiple<static_vector<V1...>>
{
	using type = static_vector<V1...>;
};
/////////////////////////// STATIC VECTOR NEGATE ///////////////////////////
/*
Negates parameters of static_vector.
@tparam V1H => first value (head)
@tparam V1T => rest (tail)
*/
template <typename>
struct static_vector_negate;

template <int V1H, int... V1T>
struct static_vector_negate<static_vector<V1H, V1T...>>
{
	using type = typename static_vector_cat<static_vector<-V1H>, typename static_vector_negate<static_vector<V1T...>>::type>::type;
};

template <>
struct static_vector_negate<static_vector<0>>
{
	using type = static_vector<0>;
};

/////////////////////////// STATIC VECTOR SUBTRACT ///////////////////////////
/*
Subtracts parameters at the same position from two vectors.
@tparam V1..., V2... = > parameter packs
@tparam V1H, V2H = > first parameter(head)
@tparam V1t, V2T = > rest of parameters(tail)
*/

template <typename, typename>
struct static_vector_subtract;

template <int V1H, int... V1T, int V2H, int... V2T>
struct static_vector_subtract<static_vector<V1H, V1T...>, static_vector<V2H, V2T...>>
{
	using type = typename static_vector_add<static_vector<V1H, V1T...>, typename static_vector_negate<static_vector<V2H, V2T...>>::type>::type;
};

/////////////////////////// STATIC VECTOR CREATE ///////////////////////////
/*
Creates a static_vector with #Capacity parameters, '0' at all positions except '1' at position Index.
*/

template <typename, size_t Capacity, int Index>
struct static_vector_create
{
};

template <int Index, size_t Capacity>
struct static_vector_create<static_vector<>, Capacity, Index>
{
	using type = typename static_vector_create<push_back<static_vector<>, 0>::type, Capacity - 1, Index - 1>::type;
};

template <size_t Capacity>
struct static_vector_create<static_vector<>, Capacity, 0>
{
	using type = typename static_vector_create<push_back<static_vector<>, 1>::type, Capacity - 1, -1>::type;
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

/////////////////////////// UNIT ///////////////////////////
/*
@tparam TEnum => enum to take the units from.
@tparam TPowers => static_vector with powers of given units.
*/
template <typename TEnum, typename TPowers>
struct unit
{
	typedef TEnum type;
	using vector = TPowers;
};

/////////////////////////// BASIC UNIT ///////////////////////////

template <typename TEnum, TEnum Index>
using basic_unit = unit<TEnum, typename static_vector_create<static_vector<>, (int)TEnum::_count, (int)Index>::type>;

/////////////////////////// DIVIDED UNIT ///////////////////////////
/*
Unit consisting of two divided units.
*/
template <typename TDividendUnit, typename TDivisorUnit>
requires (std::is_same<typename TDividendUnit::type, typename TDivisorUnit::type>::value)
using divided_unit = unit<typename TDividendUnit::type,
	typename static_vector_subtract<typename TDividendUnit::vector, typename TDivisorUnit::vector>::type>;

/////////////////////////// MULTIPLIED UNIT ///////////////////////////
/*
Unit of multiplied # units.
*/
template <typename TFirstUnit, typename... TOtherUnits>
requires(are_same<typename TFirstUnit::type,typename TOtherUnits::type...>::value)
using multiplied_unit = unit<typename TFirstUnit::type, typename static_vector_add_multiple<typename TFirstUnit::vector, typename TOtherUnits::vector...>::type>;


/////////////////////////// QUANTITY ///////////////////////////
/*
Quantity of type=double value and of given unit.
*/

template <typename TUnit, typename TValue = double>
struct quantity
{
private:
	TValue value_;
public:
	using unit = TUnit;
	using value_type = TValue;
	explicit quantity(double _v) : value_(_v) {};
	TValue value() { return value_; };

	template <typename TUnit2>
		requires(std::conjunction<std::is_same<unit, TUnit2>, std::is_same<value_type, typename quantity<TUnit2>::value_type>> ::value)
	auto operator+(quantity<TUnit2> q) { return quantity<unit>((*this).value_ + q.value()); };

	template <typename TUnit2>
		requires(std::conjunction<std::is_same<unit, TUnit2>, std::is_same<value_type, typename quantity<TUnit2>::value_type>> ::value)
	auto operator-(quantity<TUnit2> q) { return quantity<unit>((*this).value_ - q.value()); };

	template <typename TUnit2>
	requires(std::is_same<typename TUnit::type, typename TUnit2::type>::value)
	auto operator/(quantity<TUnit2> q) { return quantity<divided_unit<unit, TUnit2>>((*this).value_ / q.value()); };

	template <typename TUnit2>
		requires(std::is_same<typename TUnit::type, typename TUnit2::type>::value)
	auto operator*(quantity<TUnit2> q) { return quantity<multiplied_unit<unit, TUnit2>>((*this).value_ * q.value()); };
};

#endif