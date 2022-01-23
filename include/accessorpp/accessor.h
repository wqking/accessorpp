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

#ifndef ACCESSOR_H_578722158669
#define ACCESSOR_H_578722158669

#include "accessorpp/getter.h"
#include "accessorpp/setter.h"
#include "accessorpp/internal/accessor_i.h"

#include <functional>
#include <type_traits>
#include <iostream> 

namespace accessorpp {

struct DefaultPolicies
{
};

template <
	typename Type,
	typename PoliciesType = DefaultPolicies
>
class Accessor :
	public internal_::AccessorStorage<
			Type, internal_::SelectHoldValue<PoliciesType, internal_::HasFieldHoldValue<PoliciesType>::value>::value
		>,
	public internal_::OnChangingCallback<
			typename internal_::SelectOnChangingCallback<PoliciesType, internal_::HasTypeOnChangingCallback<PoliciesType>::value>::Type,
			typename internal_::SelectCallbackData<PoliciesType, internal_::HasTypeCallbackData<PoliciesType>::value>::Type
		>,
	public internal_::OnChangedCallback<
			typename internal_::SelectOnChangedCallback<PoliciesType, internal_::HasTypeOnChangedCallback<PoliciesType>::value>::Type,
			typename internal_::SelectCallbackData<PoliciesType, internal_::HasTypeCallbackData<PoliciesType>::value>::Type
		>
{
private:
	using StorageType = internal_::AccessorStorage<
			Type, internal_::SelectHoldValue<PoliciesType, internal_::HasFieldHoldValue<PoliciesType>::value>::value
		>;
	using OnChangingCallbackType = internal_::OnChangingCallback<
			typename internal_::SelectOnChangingCallback<PoliciesType, internal_::HasTypeOnChangingCallback<PoliciesType>::value>::Type,
			typename internal_::SelectCallbackData<PoliciesType, internal_::HasTypeCallbackData<PoliciesType>::value>::Type
		>;
	using OnChangedCallbackType = internal_::OnChangedCallback<
			typename internal_::SelectOnChangedCallback<PoliciesType, internal_::HasTypeOnChangedCallback<PoliciesType>::value>::Type,
			typename internal_::SelectCallbackData<PoliciesType, internal_::HasTypeCallbackData<PoliciesType>::value>::Type
		>;

public:
	using ValueType = Type;
	using GetterType = typename StorageType::GetterType;
	using SetterType = typename StorageType::SetterType;

	static constexpr bool useStorage = internal_::SelectHoldValue<PoliciesType, internal_::HasFieldHoldValue<PoliciesType>::value>::value;

public:
	Accessor() noexcept
		: StorageType()
	{
	}

	// The explict static_cast is required, otherwise it will call
	// template <typename P1> explicit AccessorStorage(P1 && p2)
	Accessor(const Accessor & other) noexcept
		: StorageType(static_cast<const StorageType &>(other)) {
	}

	using StorageType::StorageType;

	Accessor & operator = (const Accessor & other) {
		*this = other.get();
		return *this;
	}

	Accessor & operator = (const ValueType & value) {
		return this->set(value);
	}

	Accessor & set(const ValueType & value) {
		this->OnChangingCallbackType::invokeCallback(value);
		this->setter.set(value);
		this->OnChangedCallbackType::invokeCallback(value);
		return *this;
	}

	template <typename CD>
	Accessor & set(const ValueType & value, CD && callbackData) {
		this->OnChangingCallbackType::invokeCallback(value, std::forward<CD>(callbackData));
		this->setter.set(value);
		this->OnChangedCallbackType::invokeCallback(value, std::forward<CD>(callbackData));
		return *this;
	}

	ValueType get() const {
		return this->getter.get();
	}

	operator ValueType() const {
		return get();
	}

	template <typename F>
	void setGetter(const F & newGetter) {
		this->getter = GetterType(newGetter);
	}

	template <typename F>
	void setSetter(const F & newSetter) {
		this->setter = SetterType(newSetter);
	}

private:
};

template <typename T>
struct IsAccessor : std::false_type
{
};

template <
	typename Type,
	typename PoliciesType
>
struct IsAccessor <Accessor<Type, PoliciesType> > : std::true_type
{
};

template <typename T>
struct AccessorValueType
{
	using Type = typename std::decay<T>::type;
};

template <
	typename T,
	typename PoliciesType
>
struct AccessorValueType <Accessor<T, PoliciesType> >
{
	using Type = typename Accessor<T, PoliciesType>::ValueType;
};



template <typename T>
auto operator << (std::ostream & stream, const T & accessor)
	-> typename std::enable_if<IsAccessor<T>::value, std::ostream &>::type
{
	stream << accessor.get();
	return stream;
}

template <typename T>
auto operator >> (std::istream & stream, T & accessor)
	-> typename std::enable_if<IsAccessor<T>::value, std::istream &>::type
{
	typename T::ValueType value;
	stream >> value;
	accessor = value;
	return stream;
}


// Unary operators

template <typename T>
auto operator ++ (T & a)
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	a += 1;
	return a;
}

template <typename T>
auto operator ++ (T & a, int)
	-> typename std::enable_if<IsAccessor<T>::value && T::useStorage, T>::type
{
	T result;
	result = a;
	++a;
	return result;
}

template <typename T>
auto operator -- (T & a)
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	a -= 1;
	return a;
}

template <typename T>
auto operator -- (T & a, int)
	-> typename std::enable_if<IsAccessor<T>::value && T::useStorage, T>::type
{
	T result;
	result = a;
	--a;
	return result;
}

template <typename T>
auto operator ! (const T & a)
	-> typename std::enable_if<IsAccessor<T>::value && T::useStorage, T>::type
{
	T result;
	result = a;
	result = ! a.get();
	return result;
}

template <typename T>
auto operator + (T & a)
	-> typename std::enable_if<IsAccessor<T>::value && T::useStorage, T>::type
{
	T result;
	result = a;
	result = +(typename AccessorValueType<T>::Type)(a);
	return result;
}

template <typename T>
auto operator - (T & a)
	-> typename std::enable_if<IsAccessor<T>::value && T::useStorage, T>::type
{
	T result;
	result = a;
	result = -(typename AccessorValueType<T>::Type)(a);
	return result;
}




// Below operators are generated from tool generateops.py.
// Using C style cast because static_cast may fail on some case, such as char[]



// Logic operators

template <typename T, typename U>
auto operator == (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, bool>::type
{
	return (typename AccessorValueType<T>::Type)(a) == (typename AccessorValueType<U>::Type)(b);
}

template <typename T, typename U>
auto operator != (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, bool>::type
{
	return (typename AccessorValueType<T>::Type)(a) != (typename AccessorValueType<U>::Type)(b);
}

template <typename T, typename U>
auto operator > (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, bool>::type
{
	return (typename AccessorValueType<T>::Type)(a) > (typename AccessorValueType<U>::Type)(b);
}

template <typename T, typename U>
auto operator >= (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, bool>::type
{
	return (typename AccessorValueType<T>::Type)(a) >= (typename AccessorValueType<U>::Type)(b);
}

template <typename T, typename U>
auto operator < (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, bool>::type
{
	return (typename AccessorValueType<T>::Type)(a) < (typename AccessorValueType<U>::Type)(b);
}

template <typename T, typename U>
auto operator <= (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, bool>::type
{
	return (typename AccessorValueType<T>::Type)(a) <= (typename AccessorValueType<U>::Type)(b);
}

template <typename T, typename U>
auto operator && (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, bool>::type
{
	return (typename AccessorValueType<T>::Type)(a) && (typename AccessorValueType<U>::Type)(b);
}

template <typename T, typename U>
auto operator || (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, bool>::type
{
	return (typename AccessorValueType<T>::Type)(a) || (typename AccessorValueType<U>::Type)(b);
}
// Binary operators

template <typename T, typename U>
auto operator + (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value && T::useStorage, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) + (typename AccessorValueType<U>::Type)(b);
	return result;
}

template <typename T, typename U>
auto operator - (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value && T::useStorage, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) - (typename AccessorValueType<U>::Type)(b);
	return result;
}

template <typename T, typename U>
auto operator * (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value && T::useStorage, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) * (typename AccessorValueType<U>::Type)(b);
	return result;
}

template <typename T, typename U>
auto operator / (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value && T::useStorage, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) / (typename AccessorValueType<U>::Type)(b);
	return result;
}

template <typename T, typename U>
auto operator % (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value && T::useStorage, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) % (typename AccessorValueType<U>::Type)(b);
	return result;
}

template <typename T, typename U>
auto operator & (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value && T::useStorage, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) & (typename AccessorValueType<U>::Type)(b);
	return result;
}

template <typename T, typename U>
auto operator | (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value && T::useStorage, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) | (typename AccessorValueType<U>::Type)(b);
	return result;
}

template <typename T, typename U>
auto operator ^ (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value && T::useStorage, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) ^ (typename AccessorValueType<U>::Type)(b);
	return result;
}

template <typename T, typename U>
auto operator << (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value && T::useStorage, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) << (typename AccessorValueType<U>::Type)(b);
	return result;
}

template <typename T, typename U>
auto operator >> (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value && T::useStorage, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) >> (typename AccessorValueType<U>::Type)(b);
	return result;
}
// Binary assignment operators

template <typename T, typename U>
auto operator += (T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	a = (typename AccessorValueType<T>::Type)(a) + (typename AccessorValueType<U>::Type)(b);
	return a;
}

template <typename T, typename U>
auto operator -= (T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	a = (typename AccessorValueType<T>::Type)(a) - (typename AccessorValueType<U>::Type)(b);
	return a;
}

template <typename T, typename U>
auto operator *= (T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	a = (typename AccessorValueType<T>::Type)(a) * (typename AccessorValueType<U>::Type)(b);
	return a;
}

template <typename T, typename U>
auto operator /= (T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	a = (typename AccessorValueType<T>::Type)(a) / (typename AccessorValueType<U>::Type)(b);
	return a;
}

template <typename T, typename U>
auto operator %= (T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	a = (typename AccessorValueType<T>::Type)(a) % (typename AccessorValueType<U>::Type)(b);
	return a;
}

template <typename T, typename U>
auto operator &= (T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	a = (typename AccessorValueType<T>::Type)(a) & (typename AccessorValueType<U>::Type)(b);
	return a;
}

template <typename T, typename U>
auto operator |= (T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	a = (typename AccessorValueType<T>::Type)(a) | (typename AccessorValueType<U>::Type)(b);
	return a;
}

template <typename T, typename U>
auto operator ^= (T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	a = (typename AccessorValueType<T>::Type)(a) ^ (typename AccessorValueType<U>::Type)(b);
	return a;
}

template <typename T, typename U>
auto operator <<= (T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	a = (typename AccessorValueType<T>::Type)(a) << (typename AccessorValueType<U>::Type)(b);
	return a;
}

template <typename T, typename U>
auto operator >>= (T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	a = (typename AccessorValueType<T>::Type)(a) >> (typename AccessorValueType<U>::Type)(b);
	return a;
}


} // namespace accessorpp

#endif
