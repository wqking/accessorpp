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
	template <typename AccessorType, typename V, typename C>
	auto doOnSet(const AccessorType & /*accessor*/, const typename AccessorType::ValueType & /*newValue*/)
		-> typename std::enable_if<CanInvoke<C>::value, void>::type
	{
		callback();
	}

	template <typename AccessorType, typename V, typename C>
	auto doOnSet(const AccessorType & /*accessor*/, const typename AccessorType::ValueType & newValue)
		-> typename std::enable_if<CanInvoke<C, V>::value, void>::type
	{
		callback(newValue);
	}

	template <typename AccessorType, typename V, typename C>
	auto doOnSet(const AccessorType & accessor, const typename AccessorType::ValueType & newValue)
		-> typename std::enable_if<CanInvoke<C, V, V>::value, void>::type
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
struct AccessorStorage;

template <typename Type>
struct AccessorStorage <Type, true>
{
	using GetterType = Getter<Type>;
	using SetterType = Setter<Type>;
	using StorageValueType = typename std::remove_cv<typename std::remove_reference<Type>::type>::type;

	AccessorStorage()
		: storedValue(),
			getter(&AccessorStorage::storedValue, this),
			setter(&AccessorStorage::storedValue, this)
	{
	}

	AccessorStorage(const AccessorStorage & other)
		: storedValue(other.storedValue),
			getter(other.getter),
			setter(other.setter)
	{
	}

	template <typename U>
	explicit AccessorStorage(U && v)
		: getter(std::forward<U>(v)), setter(std::forward<U>(v))
	{
	}

	template <typename G, typename S>
	AccessorStorage(G && getter, S && setter)
		: getter(std::forward<G>(getter)), setter(std::forward<S>(setter))
	{
	}

	StorageValueType storedValue;
	Getter<Type> getter;
	Setter<Type> setter;
};

template <typename Type>
struct AccessorStorage <Type, false>
{
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

	template <typename U>
	explicit AccessorStorage(U && v)
		: getter(std::forward<U>(v)), setter(std::forward<U>(v))
	{
	}

	template <typename G, typename S>
	AccessorStorage(G && getter, S && setter)
		: getter(std::forward<G>(getter)), setter(std::forward<S>(setter))
	{
	}

	Getter<Type> getter;
	Setter<Type> setter;
};


} // namespace internal_

struct StoreValue
{
	template <typename Type>
	using StorageType = internal_::AccessorStorage<Type, true>;
};

struct NoStoreValue
{
	template <typename Type>
	using StorageType = internal_::AccessorStorage<Type, false>;
};

template <
	typename Type,
	typename Storage = StoreValue,
	typename OnSetCallbackType = void
>
class Accessor : private Storage::template StorageType<Type>, public internal_::OnSetCallback<OnSetCallbackType>
{
public:
	using ValueType = Type;

private:
	using StorageType = typename Storage::template StorageType<Type>;

public:
	Accessor()
		: StorageType()
	{
	}

	// The explict static_cast is required, otherwise it will call
	// template <typename U> explicit AccessorStorage(U && v)
	Accessor(const Accessor & other)
		: StorageType(static_cast<const StorageType &>(other)) {
	}

	template <typename U>
	explicit Accessor(U && v)
		: StorageType(std::forward<U>(v))
	{
	}

	template <typename G, typename S>
	Accessor(G && getter, S && setter)
		: StorageType(std::forward<G>(getter), std::forward<S>(setter))
	{
	}

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
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	T result(a);
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
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	T result(a);
	--a;
	return result;
}

template <typename T>
auto operator ! (const T & a)
	-> typename std::enable_if<IsAccessor<T>::value, T>::type
{
	T result(a);
	result = ! a.get();
	return result;
}

template <typename T>
auto operator + (T & a)
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	T result(a);
	result = +(typename AccessorValueType<T>::Type)(a);
	return result;
}

template <typename T>
auto operator - (T & a)
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	T result(a);
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
	-> typename std::enable_if<IsAccessor<T>::value, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) + (typename AccessorValueType<U>::Type)(b);
	return result;
}

template <typename T, typename U>
auto operator - (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) - (typename AccessorValueType<U>::Type)(b);
	return result;
}

template <typename T, typename U>
auto operator * (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) * (typename AccessorValueType<U>::Type)(b);
	return result;
}

template <typename T, typename U>
auto operator / (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) / (typename AccessorValueType<U>::Type)(b);
	return result;
}

template <typename T, typename U>
auto operator % (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) % (typename AccessorValueType<U>::Type)(b);
	return result;
}

template <typename T, typename U>
auto operator & (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) & (typename AccessorValueType<U>::Type)(b);
	return result;
}

template <typename T, typename U>
auto operator | (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) | (typename AccessorValueType<U>::Type)(b);
	return result;
}

template <typename T, typename U>
auto operator ^ (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) ^ (typename AccessorValueType<U>::Type)(b);
	return result;
}

template <typename T, typename U>
auto operator << (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) << (typename AccessorValueType<U>::Type)(b);
	return result;
}

template <typename T, typename U>
auto operator >> (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T>::type
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
