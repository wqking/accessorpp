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

#ifndef ACCESSOR_I_H_578722158669
#define ACCESSOR_I_H_578722158669

namespace accessorpp {


namespace internal_ {

template <typename CallbackType_>
class OnSetCallback
{
public:
	using CallbackType = CallbackType_;
	using ReturnType = CallbackType &;
	using ConstReturnType = const CallbackType &;

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
	template <typename AccessorType, typename ValueType, typename C>
	auto doOnSet(const AccessorType & /*accessor*/, const typename AccessorType::ValueType & /*newValue*/)
		-> typename std::enable_if<CanInvoke<C>::value, void>::type
	{
		callback();
	}

	template <typename AccessorType, typename ValueType, typename C>
	auto doOnSet(const AccessorType & /*accessor*/, const typename AccessorType::ValueType & newValue)
		-> typename std::enable_if<CanInvoke<C, ValueType>::value, void>::type
	{
		callback(newValue);
	}

	template <typename AccessorType, typename ValueType, typename C>
	auto doOnSet(const AccessorType & accessor, const typename AccessorType::ValueType & newValue)
		-> typename std::enable_if<CanInvoke<C, ValueType, ValueType>::value, void>::type
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
	using ReturnType = void;
	using ConstReturnType = void;

	template <typename AccessorType>
	void onSet(AccessorType & /*accessor*/, const typename AccessorType::ValueType & /*newValue*/)
	{
	}

};

template <typename CallbackType>
class OnChangingCallback : public OnSetCallback <CallbackType>
{
private:
	using super = OnSetCallback <CallbackType>;

public:
	using super::super;
};

template <typename CallbackType>
class OnChangedCallback : public OnSetCallback <CallbackType>
{
private:
	using super = OnSetCallback <CallbackType>;

public:
	using super::super;
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


} // namespace accessorpp


#endif
