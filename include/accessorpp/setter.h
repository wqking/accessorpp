#ifndef SETTER_H_578722158669
#define SETTER_H_578722158669

#include "internal/util.h"

#include <functional>
#include <type_traits>
#include <istream> 

namespace accessorpp {

template <typename Type>
class Setter
{
public:
	using ValueType = Type;
	using RawType = typename internal_::GetRawType<Type>::Type;

public:
	Setter()
		: setterFunc([](const ValueType &) {})
	{
	}

	template <typename U>
	explicit Setter(U * address,
		typename std::enable_if<std::is_convertible<U, RawType>::value>::type * = nullptr) noexcept
		: setterFunc([address](const ValueType & value) { *address = (U)(value); })
	{
	}

	template <typename U, typename C>
	explicit Setter(U C::* address, C * instance,
		typename std::enable_if<std::is_convertible<U, RawType>::value>::type * = nullptr) noexcept
		: setterFunc([address, instance](const ValueType & value) { instance->*address = (U)(value); })
	{
	}

	template <typename F>
	explicit Setter(F func,
		typename std::enable_if<internal_::CanInvoke<F, RawType>::value>::type * = nullptr) noexcept
		: setterFunc([func](const ValueType & value) { func(value); })
	{
	}

	template <typename F, typename C>
	explicit Setter(const F & func, C * instance,
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
