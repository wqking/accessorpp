#ifndef UTIL_H_582750282985
#define UTIL_H_582750282985

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

// This is used in SFINAE to avoid ambiguous overload
struct Dummy {};

} // namespace internal_

} // namespace accessorpp

#endif
