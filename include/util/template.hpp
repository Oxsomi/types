#pragma once
#include <type_traits>

//HAS_FUNC_NAMED_RET detects if a member function signature is present

//Example:
//HAS_FUNC_NAMED_RET(HasFunc, func, void, T*)
//Would generate HasFunc<T> that checks if the member function "void func(T*)" is available
//This means that you can also check for functions that take itself as a parameter

//NOTE: Can give false positives if a type can implicitly cast (like mistake ints for floats)
//		So please keep in mind that this won't give perfect results when casting is at play

//Thanks to /LagMeester4000 for help

#define HAS_FUNC_NAMED_RET(funcName, name, ret, ...)																\
	template<typename, typename, typename = int, typename ...>														\
	struct T##funcName : std::false_type { };																		\
																													\
	template<typename T, typename Ret, typename ...args>															\
	struct T##funcName<																								\
		T, Ret,																										\
		std::enable_if_t<std::is_same_v<Ret, decltype(std::declval<T>().name(std::declval<args>()...))>, int>,		\
		args...																										\
	> : std::true_type { };																							\
																													\
	template<typename T>																							\
	static constexpr bool funcName = T##funcName<T, ret, int, __VA_ARGS__>::value;

//HAS_FUNC_NAMED detects if a member function signature is present

//Example:
//HAS_FUNC_NAMED(HasBegin, begin)
//Would generate HasBegin<T> that checks if the member function "auto begin()" is available

//NOTE: Can give false positives if a type can implicitly cast (like mistake ints for floats)
//		So please keep in mind that this won't give perfect results when casting is at play

#define HAS_FUNC_NAMED(funcName, name, ...)																				\
	template<typename T, typename = int, typename ...args>																\
	struct T##funcName : std::false_type {};																			\
																														\
	template<typename T, typename ...args>																				\
	struct T##funcName<T, decltype(std::declval<T>().name(std::declval<args>()...), 0), args...> : std::true_type {};	\
																														\
	template<typename T>																								\
	static constexpr bool funcName = T##funcName<T, int, __VA_ARGS__>::value;

//HAS_FIELD_NAMED_T detects if a member variable with type is present (even if it's protected or private)

//Example:
//HAS_FIELD_NAMED_T(HasFieldTest, test, u32)
//Would generate HasFieldTest<T> that checks if the member variable "u32 test" is available
//This means that you can also check for a T* for example (like having a parent)

#define HAS_FIELD_NAMED_T(funcName, name, type)															\
	template<typename, typename, typename = int>														\
	struct T##funcName : std::false_type {};															\
																										\
	template<typename T, typename Ret>																	\
	struct T##funcName<T, Ret, 																			\
		std::enable_if_t<std::is_same_v<Ret, decltype(std::declval<T>().name)>, int>					\
	> : std::true_type {};																				\
																										\
	template<typename T>																				\
	static constexpr bool funcName = T##funcName<T, type, int>::value;

//HAS_FIELD_NAMED detects if a member variable is present (even if it's protected or private)

//Example:
//HAS_FIELD_NAMED(HasFieldTest, test)
//Would generate HasFieldTest<T> that checks if the member variable "test" is available
//This means that you can also check for a T* for example (like having a parent)

#define HAS_FIELD_NAMED(funcName, name)																	\
	template<typename, typename = int>																	\
	struct T##funcName : std::false_type {};															\
																										\
	template<typename T>																				\
	struct T##funcName<T, decltype(std::declval<T>().name, 0)> : std::true_type {};						\
																										\
	template<typename T>																				\
	static constexpr bool funcName = T##funcName<T, int>::value;

namespace otc {

	template <typename T, typename C, typename = void>
	struct CanCastSafely : std::false_type {};

	template <typename T, typename C>
	struct CanCastSafely<T, C, std::void_t<decltype(static_cast<T>(std::declval<C>()))>> : std::true_type {};

	template<typename Target, typename Current>
	static constexpr bool canCastSafely = CanCastSafely<Target, Current>::value;

	template<typename T, typename ...args>
	struct TTypeList {
		static constexpr usz size = 1 + sizeof...(args);
	};

	struct TypeList {

		template<typename ...args>
		static auto make(const args &...) {
			return TTypeList<args...>();
		}

	};

	static constexpr bool Is64Bit = sizeof(usz) == 8;


}