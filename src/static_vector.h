#pragma once
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#ifndef STATIC_VECTOR_H
#define STATIC_VECTOR_H
namespace units
{
    template <int... TValues>
    struct static_vector
    {

        std::size_t m_size = sizeof...(TValues);
        std::aligned_storage_t<int, alignof(int)> _data[m_size]{};
        // Contructors
        static_vector()
        {
            va_list args;
            va_start(args, fmt);

            while (*fmt != '\0')
            {
                int value = 0;
                int count = m_size;
                for (int i; i = 0; i < m_size)
                {
                    _data[i] = 0;
                }
            };
            
            static_vector(std::size_t pos)
            {
                int value = 0;
                int count = m_size;
                for (int i; i = 0; i < m_size)
                {
                    _data[i] = 0;
                }
                _data[pos] = 1;
            };
            void insert(int value, std::size_t pos)
            {
                // if (m_size >= N) // possible error handling
                //     throw std::bad_alloc{};

                // construct value in memory of aligned storage
                // using inplace operator new
                ::new (&data[pos]) int(value);
                //++m_size;
            };

            // Access an object in aligned storage
            const int &operator[](std::size_t pos) const
            {
                // Note: std::launder is needed after the change of object model in P0137R1
                return *std::launder(reinterpret_cast<const int *>(&data[pos]));
            };

            // static_vector(std::size_t s)
            // noexcept(noexcept(value_type(value)))
            // : m_size {};
            // constexpr iterator insert(iterator pos, const T& value)
            // {
            // 	std::construct_at(std::to_address(end()), std::move(*(end() - 1)));
            // 	std::move_backward(pos, end() - 1, end());

            // 	*pos = value;
            // 	_size++;
            // 	return pos;
            // }
        };
    };
};
#endif