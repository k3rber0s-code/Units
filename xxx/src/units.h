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

/////////////////////////// PUSH BACK ///////////////////////////
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

/////////////////////////// STATIC CAT ///////////////////////////
template <typename, typename>
struct static_vector_cat;

template <int... E1, int... E2>
struct static_vector_cat<static_vector<E1...>, static_vector<E2...>>
{
	using type = static_vector<E1..., E2...>;
};

/////////////////////////// STATIC ADD ///////////////////////////

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

/////////////////////////// STATIC ADD MULTIPLE ///////////////////////////

template <typename... Vector>
struct static_vector_add_multiple;

template <int... E1, int... E2, typename... Vector>
struct static_vector_add_multiple<static_vector<E1...>, static_vector<E2...>, Vector...>
{
	using type = typename static_vector_add_multiple<typename static_vector_add<static_vector<E1...>, static_vector<E2...>>::type, Vector...>::type;
};

template <int... E1, int... E2>
struct static_vector_add_multiple<static_vector<E1...>, static_vector<E2...>>
{
	using type = typename static_vector_add<static_vector<E1...>, static_vector<E2...>>::type;
};

/////////////////////////// STATIC NEGATE ///////////////////////////
template <typename>
struct static_vector_negate;

template <int E1H, int... E1T>
struct static_vector_negate<static_vector<E1H, E1T...>>
{
	using type = typename static_vector_cat<static_vector<-E1H>, typename static_vector_negate<static_vector<E1T...>>::type>::type;
};

template <>
struct static_vector_negate<static_vector<0>>
{
	using type = static_vector<0>;
};

/////////////////////////// STATIC VECTOR SUBTRACT ///////////////////////////
template <typename, typename>
struct static_vector_subtract;

template <int E1H, int... E1T, int E2H, int... E2T>
struct static_vector_subtract<static_vector<E1H, E1T...>, static_vector<E2H, E2T...>>
{
	using type = typename static_vector_add<static_vector<E1H, E1T...>, typename static_vector_negate<static_vector<E2H, E2T...>>::type>::type;
};

/////////////////////////// STATIC VECTOR CREATE ///////////////////////////

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

template <typename TDividendUnit, typename TDivisorUnit>
//requires (std::is_same<typename TDividendUnit::type, typename TDivisorUnit::type>::value)
using divided_unit = unit<typename TDividendUnit::type,
						  typename static_vector_subtract<typename TDividendUnit::vector, typename TDivisorUnit::vector>::type>;

/////////////////////////// MULTIPLIED UNIT ///////////////////////////
template <typename TFirstUnit, typename... TOtherUnits>
using multiplied_unit = unit<typename TFirstUnit::type, typename static_vector_add_multiple<typename TFirstUnit::vector, typename TOtherUnits::vector...>::type>;

/////////////////////////// ARE SAME ///////////////////////////

template <typename U1, typename... Us>
struct are_same : std::conjunction<std::is_same<U1, Us>...>
{
};

/////////////////////////// QUANTITY ///////////////////////////

template <typename TUnit, typename TValue = double>
struct quantity
{
private:
	TValue value_;
public:
	using unit = TUnit;
	quantity(double _v) : value_(_v) {};
	TValue value() {return value_;};

	template <typename TUnit2>
	auto operator+(quantity<TUnit2> q) { return quantity<unit>((*this).value_ + q.value());};

	template <typename TUnit2>
	auto operator-(quantity<TUnit2> q) { return quantity<unit>((*this).value_ - q.value());};

	template <typename TUnit2>
	auto operator/(quantity<TUnit2> q) { return quantity<unit>((*this).value_ / q.value());};

	template <typename TUnit2>
	auto operator*(quantity<TUnit2> q) { return quantity<unit>((*this).value_ * q.value());};
};

#endif