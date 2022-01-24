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

#include <cstddef>
#include <stdexcept>

namespace accessorpp {

namespace internal_ {

template <typename CallbackType>
struct ChangeCallbackBase
{
	CallbackType & getCallback() {
		return callback;
	}

	const CallbackType & getCallback() const {
		return callback;
	}

	CallbackType callback;
};

template <typename CallbackType, typename CallbackDataType>
class ChangeCallback : protected ChangeCallbackBase <CallbackType>
{
protected:
	template <typename ValueType>
	void invokeCallback(
			const ValueType & newValue
		) {
		doInvokeCallback<ValueType, CallbackType>(newValue, CallbackDataType());
	}

	template <typename ValueType>
	void invokeCallback(
			const ValueType & newValue,
			const CallbackDataType & data
		) {
		doInvokeCallback<ValueType, CallbackType>(newValue, data);
	}

private:
	template <typename ValueType, typename C>
	auto doInvokeCallback(
			const ValueType & /*newValue*/,
			const CallbackDataType & /*data*/
		)
		-> typename std::enable_if<CanInvoke<C>::value, void>::type
	{
		this->callback();
	}

	template <typename ValueType, typename C>
	auto doInvokeCallback(
			const ValueType & newValue,
			const CallbackDataType & /*data*/
		)
		-> typename std::enable_if<CanInvoke<C, const ValueType &>::value, void>::type
	{
		this->callback(newValue);
	}

	template <typename ValueType, typename C>
	auto doInvokeCallback(
			const ValueType & newValue,
			const CallbackDataType & data
		)
		-> typename std::enable_if<CanInvoke<C, const ValueType &, const CallbackDataType &>::value, void>::type
	{
		this->callback(newValue, data);
	}
};

template <typename CallbackType>
class ChangeCallback <CallbackType, void> : protected ChangeCallbackBase <CallbackType>
{
protected:
	template <typename ValueType>
	void invokeCallback(
			const ValueType & newValue
		) {
		doInvokeCallback<ValueType, CallbackType>(newValue);
	}

private:
	template <typename ValueType, typename C>
	auto doInvokeCallback(
			const ValueType & /*newValue*/
		)
		-> typename std::enable_if<CanInvoke<C>::value, void>::type
	{
		this->callback();
	}

	template <typename ValueType, typename C>
	auto doInvokeCallback(
			const ValueType & newValue
		)
		-> typename std::enable_if<CanInvoke<C, const ValueType &>::value, void>::type
	{
		this->callback(newValue);
	}
};

class DummyChangeCallback
{
protected:
	template <typename ValueType>
	void invokeCallback(const ValueType & /*newValue*/)
	{
	}

};

template <typename CallbackType, typename CallbackDataType>
class OnChangingCallback : public ChangeCallback <CallbackType, CallbackDataType>
{
private:
	using super = ChangeCallback <CallbackType, CallbackDataType>;

public:
	using super::super;

	const CallbackType & onChanging() const {
		return super::getCallback();
	}

	CallbackType & onChanging() {
		return super::getCallback();
	}
};

template <typename CallbackDataType>
class OnChangingCallback <void, CallbackDataType> : public DummyChangeCallback
{
};

template <typename CallbackType, typename CallbackDataType>
class OnChangedCallback : public ChangeCallback <CallbackType, CallbackDataType>
{
private:
	using super = ChangeCallback <CallbackType, CallbackDataType>;

public:
	using super::super;

	const CallbackType & onChanged() const {
		return super::getCallback();
	}

	CallbackType & onChanged() {
		return super::getCallback();
	}
};

template <typename CallbackDataType>
class OnChangedCallback <void, CallbackDataType> : public DummyChangeCallback
{
};

template <typename Type>
class AccessorStorageBase
{
public:
	using GetterType = Getter<Type>;
	using SetterType = Setter<Type>;

public:
	AccessorStorageBase()
		:
			getter(),
			setter(),
			readOnly(false)
	{
	}

	AccessorStorageBase(const AccessorStorageBase & other)
		:
			getter(other.getter),
			setter(other.setter),
			readOnly(other.readOnly)
	{
	}

	AccessorStorageBase(AccessorStorageBase && other)
		:
			getter(std::move(other.getter)),
			setter(std::move(other.setter)),
			readOnly(std::move(other.readOnly))
	{
	}

	explicit AccessorStorageBase(std::nullptr_t)
		:
			getter(),
			setter(),
			readOnly(true)
	{
	}

	template <typename P1>
	explicit AccessorStorageBase(P1 * p1) noexcept
		:
			getter(p1),
			setter(p1),
			readOnly(false)
	{
	}

	template <typename P1, typename P2>
	AccessorStorageBase(P1 && p1, P2 && p2) noexcept
		:
			getter(std::forward<P1>(p1)),
			setter(std::forward<P2>(p2)),
			readOnly(false)
	{
	}

	template <typename P1>
	AccessorStorageBase(P1 && p1, std::nullptr_t) noexcept
		:
			getter(std::forward<P1>(p1)),
			setter(),
			readOnly(true)
	{
	}

	template <typename P1, typename P2, typename P3, typename P4>
	AccessorStorageBase(P1 && p1, P2 && p2, P3 && p3, P4 && p4) noexcept
		:
			getter(std::forward<P1>(p1), std::forward<P2>(p2)),
			setter(std::forward<P3>(p3), std::forward<P4>(p4)),
			readOnly(false)
	{
	}

	template <typename P1, typename P2>
	AccessorStorageBase(P1 && p1, P2 && p2, std::nullptr_t) noexcept
		:
			getter(std::forward<P1>(p1), std::forward<P2>(p2)),
			setter(),
			readOnly(true)
	{
	}

	constexpr bool isReadOnly() const {
		return readOnly;
	}

protected:
	void doCheckWritable() const {
		if(readOnly) {
			throw std::logic_error("Can't set to read-only accessor.");
		}
	}

protected:
	GetterType getter;
	SetterType setter;
	const bool readOnly;
};

template <typename Type, bool>
class AccessorStorage;

template <typename Type>
class AccessorStorage <Type, true> : public AccessorStorageBase<Type>
{
private:
	using super = AccessorStorageBase<Type>;

public:
	using StorageValueType = typename std::remove_cv<typename std::remove_reference<Type>::type>::type;

	AccessorStorage()
		:
			super(&AccessorStorage::storedValue, this, &AccessorStorage::storedValue, this),
			storedValue()
	{
	}

	explicit AccessorStorage(std::nullptr_t)
		:
			super(&AccessorStorage::storedValue, this, nullptr),
			storedValue()
	{
	}

	AccessorStorage(const AccessorStorage & other)
		:
			super(&AccessorStorage::storedValue, this, &AccessorStorage::storedValue, this),
			storedValue(other.storedValue)
	{
	}

	AccessorStorage(AccessorStorage && other)
		:
			super(static_cast<super &&>(other)),
			storedValue(std::move(other.storedValue))
	{
	}

	template <typename P1>
	explicit AccessorStorage(P1 * p1) noexcept
		:
			super(p1),
			storedValue()
	{
	}

	template <typename P1, typename P2>
	AccessorStorage(P1 && p1, P2 && p2) noexcept
		:
			super(std::forward<P1>(p1), std::forward<P2>(p2)),
			storedValue()
	{
	}

	template <typename P1>
	AccessorStorage(P1 && p1, std::nullptr_t) noexcept
		:
			super(std::forward<P1>(p1), nullptr),
			storedValue()
	{
	}

	template <typename P1, typename P2, typename P3, typename P4>
	AccessorStorage(P1 && p1, P2 && p2, P3 && p3, P4 && p4) noexcept
		:
			super(std::forward<P1>(p1), std::forward<P2>(p2), std::forward<P3>(p3), std::forward<P4>(p4)),
			storedValue()
	{
	}

	template <typename P1, typename P2>
	AccessorStorage(P1 && p1, P2 && p2, std::nullptr_t) noexcept
		:
			super(std::forward<P1>(p1), std::forward<P2>(p2), nullptr),
			storedValue()
	{
	}

	// The functions getValue and setValue should be used to implement getter/setter,
	// don't use them to access the value directly outside of getter/setter.
	const StorageValueType & getValue() const {
		return storedValue;
	}

	// This doesn't respect "readOnly".
	void setValue(const StorageValueType & newValue) {
		storedValue = newValue;
	}

private:
	StorageValueType storedValue;
};

template <typename Type>
class AccessorStorage <Type, false> : public AccessorStorageBase<Type>
{
private:
	using super = AccessorStorageBase<Type>;

public:
	using super::super;
};


} // namespace internal_


} // namespace accessorpp


#endif
