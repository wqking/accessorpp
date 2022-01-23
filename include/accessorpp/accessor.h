#ifndef ACCESSOR_H_578722158669
#define ACCESSOR_H_578722158669

#include "accessorpp/getter.h"
#include "accessorpp/setter.h"

#include <functional>
#include <type_traits>
#include <iostream> 

namespace accessorpp {

namespace internal_ {

template <typename CallbackType>
class OnSetCallback
{
public:
	template <typename AccessorType>
	void onSet(const AccessorType & accessor, const typename AccessorType::ValueType & newValue) {
		doOnSet<AccessorType, typename AccessorType::ValueType, CallbackType>(accessor, newValue);
	}

	CallbackType & getCallback() {
		return callback;
	}

	const CallbackType & getCallback() const {
		return callback;
	}

private:
	template <typename AccessorType, typename P2, typename C>
	auto doOnSet(const AccessorType & /*accessor*/, const typename AccessorType::ValueType & /*newValue*/)
		-> typename std::enable_if<CanInvoke<C>::value, void>::type
	{
		callback();
	}

	template <typename AccessorType, typename P2, typename C>
	auto doOnSet(const AccessorType & /*accessor*/, const typename AccessorType::ValueType & newValue)
		-> typename std::enable_if<CanInvoke<C, P2>::value, void>::type
	{
		callback(newValue);
	}

	template <typename AccessorType, typename P2, typename C>
	auto doOnSet(const AccessorType & accessor, const typename AccessorType::ValueType & newValue)
		-> typename std::enable_if<CanInvoke<C, P2, P2>::value, void>::type
	{
		callback(newValue, accessor.get());
	}

private:
	CallbackType callback;
};

template <>
class OnSetCallback <void>
{
public:
	template <typename AccessorType>
	void onSet(AccessorType & /*accessor*/, const typename AccessorType::ValueType & /*newValue*/)
	{
	}

};

template <typename Type, bool>
class AccessorStorage;

template <typename Type>
class AccessorStorage <Type, true>
{
public:
	using GetterType = Getter<Type>;
	using SetterType = Setter<Type>;
	using StorageValueType = typename std::remove_cv<typename std::remove_reference<Type>::type>::type;

	AccessorStorage()
		:
			storedValue(),
			getter(&AccessorStorage::storedValue, this),
			setter(&AccessorStorage::storedValue, this)
	{
	}

	AccessorStorage(const AccessorStorage & other)
		:
			storedValue(other.storedValue),
			getter(&AccessorStorage::storedValue, this),
			setter(&AccessorStorage::storedValue, this)
	{
	}

	// The functions getValue and setValue should be used to implement getter/setter,
	// don't use them to access the value directly outside of getter/setter.
	const StorageValueType & getValue() const {
		return storedValue;
	}

	void setValue(const StorageValueType & newValue) {
		storedValue = newValue;
	}

protected:
	StorageValueType storedValue;
	GetterType getter;
	SetterType setter;
};

template <typename Type>
class AccessorStorage <Type, false>
{
public:
	using GetterType = Getter<Type>;
	using SetterType = Setter<Type>;

	AccessorStorage()
		: getter(), setter()
	{
	}

	AccessorStorage(const AccessorStorage & other)
		: getter(other.getter),	setter(other.setter)
	{
	}

	template <typename P1>
	explicit AccessorStorage(P1 * p1) noexcept
		: getter(p1), setter(p1)
	{
	}

	template <typename P1, typename P2>
	AccessorStorage(P1 && p1, P2 && p2,
		typename std::enable_if<IsGetter<P1>::value && IsSetter<P2>::value, void>::type * = nullptr) noexcept
		: getter(std::forward<P1>(p1)), setter(std::forward<P2>(p2))
	{
	}

	template <typename P1, typename P2>
	AccessorStorage(P1 && p1, P2 && p2,
		typename std::enable_if<! (IsGetter<P1>::value && IsSetter<P2>::value), void>::type * = nullptr) noexcept
		: getter(std::forward<P1>(p1), std::forward<P2>(p2)), setter(std::forward<P1>(p1), std::forward<P2>(p2))
	{
	}

	template <typename P1, typename P2, typename P3, typename P4>
	AccessorStorage(P1 && p1, P2 && p2, P3 && p3, P4 && p4) noexcept
		: getter(std::forward<P1>(p1), std::forward<P2>(p2)), setter(std::forward<P3>(p3), std::forward<P4>(p4))
	{
	}

protected:
	GetterType getter;
	SetterType setter;
};


} // namespace internal_

struct UseStorage
{
	template <typename Type>
	using StorageType = internal_::AccessorStorage<Type, true>;
};

struct NoStorage
{
	template <typename Type>
	using StorageType = internal_::AccessorStorage<Type, false>;
};

template <
	typename Type,
	typename Storage = UseStorage,
	typename OnSetCallbackType = void
>
class Accessor : public Storage::template StorageType<Type>, public internal_::OnSetCallback<OnSetCallbackType>
{
private:
	using StorageType = typename Storage::template StorageType<Type>;
	using ThisType = Accessor<Type, Storage, OnSetCallbackType>;

public:
	using ValueType = Type;
	using GetterType = typename StorageType::GetterType;
	using SetterType = typename StorageType::SetterType;

	static constexpr bool useStorage = std::is_same<Storage, UseStorage>::value;

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
		this->onSet(*this, value);
		this->setter.set(value);
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
};

template <typename T>
struct IsAccessor : std::false_type
{
};

template <
	typename Type,
	typename Storage,
	typename OnSetCallbackType
>
struct IsAccessor <Accessor<Type, Storage, OnSetCallbackType> > : std::true_type
{
};

template <typename T>
struct AccessorValueType
{
	using Type = typename std::decay<T>::type;
};

template <
	typename T,
	typename Storage,
	typename OnSetCallbackType
>
struct AccessorValueType <Accessor<T, Storage, OnSetCallbackType> >
{
	using Type = typename Accessor<T, Storage, OnSetCallbackType>::ValueType;
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
