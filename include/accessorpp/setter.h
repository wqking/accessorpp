// accessorpp library
// Copyright (C) 2022 Wang Qi (wqking)
// Github: https://github.com/wqking/accessorpp
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//   http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef SETTER_H_578722158669
#define SETTER_H_578722158669

#include "internal/typeutil_i.h"

#include <functional>
#include <type_traits>
#include <istream> 

namespace accessorpp {

template <typename Type_>
class Setter
{
public:
	using Type = Type_;
	using ValueType = typename private_::GetUnderlyingType<Type>::Type;

public:
	Setter()
		: setterFunc([](const ValueType &) {})
	{
	}

	template <typename U>
	explicit Setter(U * address,
		typename std::enable_if<std::is_convertible<U, ValueType>::value>::type * = nullptr) noexcept
		: setterFunc([address](const ValueType & value) { *address = (U)(value); })
	{
	}

	template <typename U, typename C>
	Setter(U C::* address, C * instance,
		typename std::enable_if<std::is_convertible<U, ValueType>::value>::type * = nullptr) noexcept
		: setterFunc([address, instance](const ValueType & value) { instance->*address = (U)(value); })
	{
	}

	template <typename F>
	explicit Setter(F func,
		typename std::enable_if<private_::CanInvoke<F, ValueType>::value>::type * = nullptr) noexcept
		: setterFunc([func](const ValueType & value) { func(value); })
	{
	}

	template <typename F, typename C>
	Setter(F func, C * instance,
		typename std::enable_if<std::is_member_function_pointer<F>::value>::type * = nullptr) noexcept
		: setterFunc([func, instance](const ValueType & value) { (instance->*func)(value); })
	{
	}

	Setter(const Setter & other) noexcept
		: setterFunc(other.setterFunc)
	{
	}

	Setter(Setter && other) noexcept
		: setterFunc(std::move(other.setterFunc))
	{
	}

	Setter & operator = (const Setter & other) noexcept {
		setterFunc = other.setterFunc;
		return *this;
	}

	Setter & operator = (Setter && other) noexcept {
		setterFunc = std::move(other.setterFunc);
		return *this;
	}

	Setter & operator = (const ValueType & value) {
		set(value);
		return *this;
	}

	void set(const ValueType & value) {
		setterFunc(value);
	}

private:
	std::function<void (const ValueType &)> setterFunc;
};

template <typename T>
struct IsSetter : std::false_type
{
};

template <typename Type>
struct IsSetter <Setter<Type> > : std::true_type
{
};

template <typename Type>
std::istream & operator >> (std::istream & stream, Setter<Type> & setter)
{
	Type value;
	stream >> value;
	setter = value;
	return stream;
}


} // namespace accessorpp

#endif
