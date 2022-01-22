#ifndef GETTER_H_578722158669
#define GETTER_H_578722158669

#include "internal/util.h"

#include <functional>
#include <type_traits>
#include <ostream> 

namespace accessorpp {

template <typename Type>
class Getter
{
public:
	using ValueType = Type;
	using RawType = typename internal_::GetRawType<Type>::Type;

public:
	Getter()
		: getterFunc([](){ return ValueType(); })
	{
	}

	template <typename U>
	explicit Getter(const U * address,
		typename std::enable_if<std::is_convertible<U, RawType>::value>::type * = nullptr) noexcept
		: getterFunc([address]()->ValueType { return (ValueType)*address; })
	{
	}

	template <typename U, typename C>
	explicit Getter(const U C::* address, const C * instance,
		typename std::enable_if<std::is_convertible<U, RawType>::value>::type * = nullptr) noexcept
		: getterFunc([address, instance]()->ValueType { return (ValueType)(instance->*address); })
	{
	}

	template <typename U>
	explicit Getter(U && value,
		typename std::enable_if<std::is_convertible<U, RawType>::value>::type * = nullptr) noexcept
		: getterFunc([value]()->ValueType { return (ValueType)value; })
	{
	}

	template <typename F>
	explicit Getter(F func,
		typename std::enable_if<internal_::CanInvoke<F>::value>::type * = nullptr) noexcept
		: getterFunc([func]()->ValueType { return (ValueType)func(); })
	{
	}

	template <typename F, typename C>
	explicit Getter(const F & func, const C * instance,
		typename std::enable_if<std::is_member_function_pointer<F>::value>::type * = nullptr) noexcept
		: getterFunc(std::bind(func, instance))
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

	ValueType get() const {
		return getterFunc();
	}

	operator ValueType() const {
		return get();
	}

private:
	std::function<ValueType ()> getterFunc;
};

template <typename Type>
std::ostream & operator << (std::ostream & stream, const Getter<Type> & getter)
{
	stream << getter.get();
	return stream;
}


} // namespace accessorpp

#endif
