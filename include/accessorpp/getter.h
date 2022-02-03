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

#ifndef GETTER_H_578722158669
#define GETTER_H_578722158669

#include "internal/typeutil_i.h"

#include <functional>
#include <type_traits>
#include <ostream> 

namespace accessorpp {

template <typename Type_>
class Getter
{
public:
	using Type = Type_;
	using ValueType = typename private_::GetUnderlyingType<Type>::Type;

public:
	Getter()
		: getterFunc([](){ return ValueType(); })
	{
	}

	template <typename U>
	explicit Getter(const U * address,
		typename std::enable_if<std::is_convertible<U, ValueType>::value>::type * = nullptr) noexcept
		: getterFunc([address]()->Type { return (Type)*address; })
	{
	}

	template <typename U, typename C>
	Getter(const U C::* address, const C * instance,
		typename std::enable_if<std::is_convertible<U, ValueType>::value>::type * = nullptr) noexcept
		: getterFunc([address, instance]()->Type { return (Type)(instance->*address); })
	{
	}

	explicit Getter(const Type & value) noexcept
		: getterFunc([value]()->Type { return value; })
	{
	}

	template <typename F>
	explicit Getter(F func,
		typename std::enable_if<private_::CanInvoke<F>::value>::type * = nullptr) noexcept
		: getterFunc([func]()->Type { return (Type)func(); })
	{
	}

	template <typename F, typename C>
	Getter(F func, C * instance,
		typename std::enable_if<std::is_member_function_pointer<F>::value>::type * = nullptr) noexcept
		: getterFunc([func, instance]()->Type { return (Type)((instance->*func)()); })
	{
	}

	Getter(const Getter & other) noexcept
		: getterFunc(other.getterFunc)
	{
	}

	Getter(Getter && other) noexcept
		: getterFunc(std::move(other.getterFunc))
	{
	}

	Getter & operator = (const Getter & other) noexcept {
		getterFunc = other.getterFunc;
		return *this;
	}

	Getter & operator = (Getter && other) noexcept {
		getterFunc = std::move(other.getterFunc);
		return *this;
	}

	Type get() const {
		return getterFunc();
	}

	operator Type() const {
		return get();
	}

private:
	std::function<Type ()> getterFunc;
};

template <typename T>
struct IsGetter : std::false_type
{
};

template <typename Type>
struct IsGetter <Getter<Type> > : std::true_type
{
};

template <typename Type>
std::ostream & operator << (std::ostream & stream, const Getter<Type> & getter)
{
	stream << getter.get();
	return stream;
}


} // namespace accessorpp

#endif
