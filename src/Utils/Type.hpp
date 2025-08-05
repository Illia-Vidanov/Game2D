#ifndef FILE_TYPE_HPP
#define FILE_TYPE_HPP

#include <type_traits>
#include <stdint.h>


namespace tolik
{
namespace detail
{
template<bool Condition>
struct Warning
{};

template<>
struct __attribute__((deprecated)) Warning<false>
{ constexpr Warning() {} };
} // detail

#define StaticWarning(x, ...) ((void)Warning<x>())


namespace detail
{
template<typename T>
struct RemoveClassPointerImpl;

template<typename T, typename... Args>
struct RemoveClassPointerImpl<T(Args...)>
{ using type = T(Args...); };

template<typename T, typename U, typename... Args>
struct RemoveClassPointerImpl<T(U::*)(Args...)>
{ using type = T(Args...); };
} // detail

template<typename T>
struct RemoveClassPointer
{ using type = typename detail::RemoveClassPointerImpl<T>::type; };

template<typename T>
using RemoveClassPointerT = typename RemoveClassPointer<T>::type;


namespace detail
{
template<typename T, typename... Args>
struct FunctorTraitsBase
{
	using ReturnType = T;
	static constexpr inline std::size_t arg_count = sizeof...(Args);
	template<std::size_t I>
	using ArgType = std::tuple_element_t<I, std::tuple<Args...>>;
};
	
template<typename T, typename... Args>
struct FunctorTraitsImpl : FunctorTraitsImpl<decltype(&T::operator())>
{};

template<typename ClassType, typename ReturnType, typename... Args>
struct FunctorTraitsImpl<ReturnType(ClassType::*)(Args...) const> : FunctorTraitsBase<ReturnType, Args...>
{};

template<typename ClassType, typename ReturnType, typename... Args>
struct FunctorTraitsImpl<ReturnType(ClassType::*)(Args...)> : FunctorTraitsBase<ReturnType, Args...>
{};

template<typename ReturnType, typename... Args>
struct FunctorTraitsImpl<ReturnType(*)(Args...)> : FunctorTraitsBase<ReturnType, Args...>
{};

template<typename ReturnType, typename... Args>
struct FunctorTraitsImpl<ReturnType(&)(Args...)> : FunctorTraitsBase<ReturnType, Args...>
{};
} // detail

// ReturnType = return type of a functor
// arg_count = amount of arguments in functor
// ArgT<Index> = type of argument in index (Note: indexing starts at 0)
template<typename T>
struct FunctorTraits : detail::FunctorTraitsImpl<T>
{};


namespace detail
{
	template<typename T, std::size_t N, template<std::size_t> typename F, std::size_t Index = N, T... Args>
	struct GenerateArrayImpl : GenerateArrayImpl<T, N, F, Index - 1, Args..., F<N - Index>::value>
	{};

	template<typename T, std::size_t N, template<std::size_t> typename F, T... Args>
	struct GenerateArrayImpl<T, N, F, 0, Args...>
	{ inline static constexpr std::array<T, N> value = std::array<T, N>{ Args... }; };
} // detail

// Generate std::array<T, N> with value of F<Index>::value
template<typename T, std::size_t N, template<std::size_t> typename F>
struct GenerateArray : detail::GenerateArrayImpl<T, N, F>
{};

// Alias for GenerateArray struct
template<typename T, std::size_t N, template<std::size_t> typename F>
std::array<T, N> kGenerateArray = GenerateArray<T, N, F>::value;

// Used for GenerateArray struct
template<std::size_t N>
struct ArrayConstant
{
	template<std::size_t>
	struct type
	{
		static constexpr inline std::size_t value = N;
	};
};


template<typename... Args>
struct MakeVoid
{ using type = void; };

template<typename... Args>
using MakeVoidT = typename MakeVoid<Args...>::type;


enum class OperatorType : uint8_t
{
  kModule,
  kPointerToMemberDot,
  kPointerToMemberArrow,
  kMultiplication,
  kDivision,
  kAddition,
  kSubtraction,
  kBitwiseLeftShift,
  kBitwiseRightShift,
  kLessThen,
  kLessEqualThen,
  kMoreThen,
  kMoreEqualThen,
  kEqual,
  kNotEqual,
  kBitwiseAnd, 
  kBitwiseXor,
  kBitwiseOr,
  kLogicalAnd,
  kLogicalOr,
  kAssigment,
  kAddAssign,
  kSubtractAssign,
  kMultiplyAssign,
  kDivideAssign,
  kModuleAssign,
  kBitwiseLeftShiftAssign,
  kBitwiseRightShiftAssign,
  kBitwiseAndAssign,
  kBitwiseXorAssign,
  kBitwiseOrAssign,
  kComma
};

namespace detail
{
template<OperatorType Operator, typename T, typename U, typename = void>
struct OperatorDefinedImpl : std::false_type
{ static_assert(Operator >= OperatorType::kModule && Operator <= OperatorType::kComma); };

template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kModule, T, U, MakeVoidT<decltype(std::declval<T>() % std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kPointerToMemberDot, T, U, MakeVoidT<decltype(std::declval<T>() .* std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kPointerToMemberArrow, T, U, MakeVoidT<decltype(std::declval<T>() ->* std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kMultiplication, T, U, MakeVoidT<decltype(std::declval<T>() * std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kDivision, T, U, MakeVoidT<decltype(std::declval<T>() / std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kAddition, T, U, MakeVoidT<decltype(std::declval<T>() + std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kSubtraction, T, U, MakeVoidT<decltype(std::declval<T>() - std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kBitwiseLeftShift, T, U, MakeVoidT<decltype(std::declval<T>() << std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kBitwiseRightShift, T, U, MakeVoidT<decltype(std::declval<T>() >> std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kLessThen, T, U, MakeVoidT<decltype(std::declval<T>() < std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kLessEqualThen, T, U, MakeVoidT<decltype(std::declval<T>() <= std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kMoreThen, T, U, MakeVoidT<decltype(std::declval<T>() > std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kMoreEqualThen, T, U, MakeVoidT<decltype(std::declval<T>() >= std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kEqual, T, U, MakeVoidT<decltype(std::declval<T>() == std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kNotEqual, T, U, MakeVoidT<decltype(std::declval<T>() != std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kBitwiseAnd, T, U, MakeVoidT<decltype(std::declval<T>() & std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kBitwiseXor, T, U, MakeVoidT<decltype(std::declval<T>() ^ std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kBitwiseOr, T, U, MakeVoidT<decltype(std::declval<T>() | std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kLogicalAnd, T, U, MakeVoidT<decltype(std::declval<T>() && std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kLogicalOr, T, U, MakeVoidT<decltype(std::declval<T>() || std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kAssigment, T, U, MakeVoidT<decltype(std::declval<T>() = std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kAddAssign, T, U, MakeVoidT<decltype(std::declval<T>() += std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kSubtractAssign, T, U, MakeVoidT<decltype(std::declval<T>() -= std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kMultiplyAssign, T, U, MakeVoidT<decltype(std::declval<T>() *= std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kDivideAssign, T, U, MakeVoidT<decltype(std::declval<T>() /= std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kModuleAssign, T, U, MakeVoidT<decltype(std::declval<T>() %= std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kBitwiseLeftShiftAssign, T, U, MakeVoidT<decltype(std::declval<T>() <<= std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kBitwiseRightShiftAssign, T, U, MakeVoidT<decltype(std::declval<T>() >>= std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kBitwiseAndAssign, T, U, MakeVoidT<decltype(std::declval<T>() &= std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kBitwiseXorAssign, T, U, MakeVoidT<decltype(std::declval<T>() ^= std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kBitwiseOrAssign, T, U, MakeVoidT<decltype(std::declval<T>() |= std::declval<U>())>> : std::true_type
{};
template<typename T, typename U>
struct OperatorDefinedImpl<OperatorType::kComma, T, U, MakeVoidT<decltype(std::declval<T>() , std::declval<U>())>> : std::true_type
{};
} // detail

template<OperatorType Operator, typename T, typename U>
struct OperatorDefined : detail::OperatorDefinedImpl<Operator, T, U>
{};

template<OperatorType Operator, typename T, typename U>
constexpr inline bool kOperatorDefined = OperatorDefined<Operator, T, U>::value;


template<typename... Args>
struct TypePack
{
  constexpr TypePack() noexcept = default;
};
} // tolik

#endif // FILE_TYPE_HPP