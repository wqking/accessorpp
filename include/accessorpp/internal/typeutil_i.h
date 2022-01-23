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

#ifndef TYPEUTIL_I_H_582750282985
#define TYPEUTIL_I_H_582750282985

namespace accessorpp {

namespace internal_ {

template <typename F, typename ...Args>
struct CanInvoke
{
	template <typename U, typename ...X>
	static auto invoke(int) -> decltype(std::declval<U>()(std::declval<X>()...), std::true_type());

	template <typename U, typename ...X>
	static auto invoke(...)->std::false_type;

	enum {
		value = !! decltype(invoke<F, Args...>(0))()
	};
};

template <typename T>
struct GetRawType
{
	using Type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
};

template <typename T>
struct HasTypeOnChangingCallback
{
	template <typename C> static std::true_type test(typename C::OnChangingCallback *) ;
	template <typename C> static std::false_type test(...);    

	enum { value = !! decltype(test<T>(0))() };
};
template <typename T, bool> struct SelectOnChangingCallback { using Type = typename T::OnChangingCallback; };
template <typename T> struct SelectOnChangingCallback <T, false> { using Type = void; };

template <typename T>
struct HasTypeOnChangedCallback
{
	template <typename C> static std::true_type test(typename C::OnChangedCallback *) ;
	template <typename C> static std::false_type test(...);    

	enum { value = !! decltype(test<T>(0))() };
};
template <typename T, bool> struct SelectOnChangedCallback { using Type = typename T::OnChangedCallback; };
template <typename T> struct SelectOnChangedCallback <T, false> { using Type = void; };

template <typename T>
struct HasFieldHoldValue
{
	template <typename C> static std::true_type test(decltype(C::holdValue) *) ;
	template <typename C> static std::false_type test(...);    

	enum { value = !! decltype(test<T>(0))() };
};
template <typename T, bool> struct SelectHoldValue { static constexpr bool value = T::holdValue; };
template <typename T> struct SelectHoldValue <T, false> { static constexpr bool value = true; };


} // namespace internal_

} // namespace accessorpp

#endif
