#ifndef GAME_MATH_HPP
#define GAME_MATH_HPP

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"


namespace game
{
// If type is custom it's better to leave it as is
template<typename T>
using ReturnFloatType = std::conditional_t<std::is_integral_v<T>, DefaultFloatType, T>;

template<typename T>
[[nodiscard]] constexpr T NotNull(T t) noexcept { return t == 0 ? T{1} : t; }

template<typename T>
[[nodiscard]] constexpr ReturnFloatType<T> DegreesToRadians(T degrees) noexcept { return degrees * (kPI / 180); }

template<typename T>
[[nodiscard]] constexpr ReturnFloatType<T> RadiansToDegrees(T radians) noexcept { return radians * (180 / kPI); }


template<typename T, typename U>
[[nodiscard]] constexpr bool AreSame(T t, U u) noexcept { return gcem::abs(t - u) < kElipson; }

// Shorthand of writing (std::numeric_limits<T>::digits10 + 1)
template<typename T>
constexpr inline static int kMaxDigits = std::numeric_limits<T>::digits10 + 1;


namespace detail
{
template<typename T>
constexpr inline T kFastPower10PositiveLookup[20] =
{
    1, 10, 100, kMaxDigits<T> > 3 ? 1000 : 0,
    kMaxDigits<T> > 4 ?  gcem::pow<T, T>(10, 4) : 0,  kMaxDigits<T> > 5 ?  gcem::pow<T, T>(10, 5) : 0,  kMaxDigits<T> > 6 ?  gcem::pow<T, T>(10, 6) : 0,  kMaxDigits<T> > 7 ?  gcem::pow<T, T>(10, 7) : 0,
    kMaxDigits<T> > 8 ?  gcem::pow<T, T>(10, 8) : 0,  kMaxDigits<T> > 9 ?  gcem::pow<T, T>(10, 9) : 0,  kMaxDigits<T> > 10 ? gcem::pow<T, T>(10, 10) : 0, kMaxDigits<T> > 11 ? gcem::pow<T, T>(10, 11) : 0, 
    kMaxDigits<T> > 12 ? gcem::pow<T, T>(10, 12) : 0, kMaxDigits<T> > 13 ? gcem::pow<T, T>(10, 13) : 0, kMaxDigits<T> > 14 ? gcem::pow<T, T>(10, 14) : 0, kMaxDigits<T> > 15 ? gcem::pow<T, T>(10, 15) : 0,
    kMaxDigits<T> > 16 ? gcem::pow<T, T>(10, 16) : 0, kMaxDigits<T> > 17 ? gcem::pow<T, T>(10, 17) : 0, kMaxDigits<T> > 18 ? gcem::pow<T, T>(10, 18) : 0, kMaxDigits<T> > 19 ? gcem::pow<T, T>(10, 19) : 0
};

constexpr inline DefaultFloatType kFastPower10NegativeLookup[20] =
{
    static_cast<DefaultFloatType>(1), static_cast<DefaultFloatType>(1) / 10, static_cast<DefaultFloatType>(1) / 100, static_cast<DefaultFloatType>(1) / 1000,
    static_cast<DefaultFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 4),  static_cast<DefaultFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 5),  static_cast<DefaultFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 6),  static_cast<DefaultFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 7),
    static_cast<DefaultFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 8),  static_cast<DefaultFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 9),  static_cast<DefaultFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 10), static_cast<DefaultFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 11), 
    static_cast<DefaultFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 12), static_cast<DefaultFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 13), static_cast<DefaultFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 14), static_cast<DefaultFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 15),
    static_cast<DefaultFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 16), static_cast<DefaultFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 17), static_cast<DefaultFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 18), static_cast<DefaultFloatType>(1) / gcem::pow<uint64_t, uint64_t>(10, 19)
};
} // detail

// Function to get power of 10 with lookup
// If exponent might be floatin-point specialize return value
// Example: FastPower10<float>(2.3);
// If exp > 19 or exp < -19, 1 is returned
template<typename T = uint32_t, typename U>
[[nodiscard]] constexpr T FastPower10(const U exp) noexcept
{
    if constexpr(!std::is_integral_v<U>)
        return gcem::pow(T(10), exp);
    else if(std::is_signed_v<U>)
    {
        if(exp < 0)
        {
            if(exp < -19)
                return 1;

            return static_cast<T>(detail::kFastPower10NegativeLookup[-exp]);
        }
    }

    if(exp > 19)
        return 1;

    return detail::kFastPower10PositiveLookup<T>[exp];
}


// Gets digit at index from number
// If index is invalid 0 returned
// If value is floating point, fraction part is ignored, by casting value to long long
// Indexing starts at 0 and equals the same as (x / 10^index) % 10
template<typename T = uint8_t, typename U, typename V, std::enable_if_t<!std::is_floating_point_v<U>, bool> = true>
[[nodiscard]] constexpr T GetDigit(const U u, const V index) noexcept
{
    // We might get division by 0 if we skip this line
    if(index < 0)
        return 0;
    return (gcem::abs(u) / FastPower10<uint64_t>(index)) % U(10);
}

template<typename T = uint8_t, typename U, typename V, std::enable_if_t<std::is_floating_point_v<U>, bool> = true>
[[nodiscard]] constexpr T GetDigit(const U u, const V index) noexcept
{
    return GetDigit(static_cast<long long>(u), index);
}


// Get substring from number
// Same as (number / 10^first) % 10^last
// first number inclusive, last exclusive
// Example: first = 1, last = 5, number = 6543210 => 4321;
//          first = 4, last = 8, number = -6543210 => 654;
//          first = -10, last = 5, number = -6543210 => 43210;
// If number is floating point, fraction part is ignored by casting value to long long
template<typename T, typename U, std::enable_if_t<!std::is_floating_point_v<T>, bool> = true>
[[nodiscard]] constexpr T GetDigitSubstring(const T number, U first, const U last) noexcept
{
    // I am not sure about this line.
    // It might be better to throw an error, but it means we need to check for it anyway, so I would prefer doing it this way
    // If we'll ignore the fact of negative numbers we'll get arithmetic exception that is hard to debug
    if(first < 0)
        first = 0;
    if(last < 0)
        return 0;
    if(first >= last)
        return 0;

    return (number / IntegralPower(10, first)) % IntegralPower(10, last - first);
}

template<typename T, typename U, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
[[nodiscard]] constexpr T GetDigitSubstring(const T number, const U first, const U last) noexcept
{
    return GetDigitSubstring(static_cast<long long>(number), first, last);
}


namespace detail
{
template <typename T>
[[nodiscard]] constexpr uint32_t DigitCountImpl(T t) noexcept
{
    if(t < T(0))
        return DigitCountImpl(-t);
    uint32_t digits = 0;
    while(T(0) < t)
    {
        digits++;
        t = t / 10;
    }
    return digits;
}

constexpr unsigned char kCharCountLookup[256] = { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };

template<>
[[nodiscard]] constexpr inline uint32_t DigitCountImpl<char>(char t) noexcept
{
    if(t < 0)
        return DigitCountImpl<char>(-t);
    
    return kCharCountLookup[static_cast<unsigned char>(t)];
}

template<>
[[nodiscard]] constexpr inline uint32_t DigitCountImpl<unsigned char>(unsigned char t) noexcept
{
  return kCharCountLookup[t];
}

template<>
[[nodiscard]] constexpr inline uint32_t DigitCountImpl<short>(short t) noexcept
{
    if(t < 0)
        return DigitCountImpl<short>(-t);
    
    if(t >= 100)
    {
        if(t >= 1000)
        {
            if(t > 10000)
                return 5;
            return 4;
        }
        return 3;
    }
    else
    {
        if(t >= 10)
            return 2;
        return 1;
    }
}
template<>
[[nodiscard]] constexpr inline uint32_t DigitCountImpl<unsigned short>(unsigned short t) noexcept
{
    if(t >= 100)
    {
        if(t >= 1000)
        {
            if(t > 10000)
                return 5;
            return 4;
        }
        return 3;
    }
    else
    {
        if(t >= 10)
            return 2;
        return 1;
    }
}

template<>
[[nodiscard]] constexpr inline uint32_t DigitCountImpl<int>(int t) noexcept
{
    if(t < 0) return DigitCountImpl<int>(-t);

    if(t >= 100000)
    {
        if(t >= 10000000)
        {
            if(t >= 100000000)
            {
                if(t > 1000000000)
                    return 10;
                return 9;
            }
            return 8;
        }
        if(t >= 1000000)
            return 7;
        return 6;
    }
    if(t >= 100)
    {
        if(t >= 1000)
        {
            if(t >= 10000)
                return 5;
            return 4;
        }
        return 3;
    }
    if(t >= 10)
        return 2;
    return 1;
}

template<>
[[nodiscard]] constexpr inline uint32_t DigitCountImpl<unsigned int>(unsigned int t) noexcept
{
    if(t >= 100000)
    {
        if(t >= 10000000)
        {
            if(t >= 100000000)
            {
                if(t > 1000000000)
                    return 10;
                return 9;
            }
            return 8;
        }
        if(t >= 1000000)
            return 7;
        return 6;
    }
    if(t >= 100)
    {
        if(t >= 1000)
        {
            if(t >= 10000)
                return 5;
            return 4;
        }
        return 3;
    }
    if(t >= 10)
        return 2;
    return 1;
}

template<>
[[nodiscard]] constexpr inline uint32_t DigitCountImpl<long long>(long long t) noexcept
{
    if(t < 0)
        return DigitCountImpl<long long>(-t);

    if(t >= 10000000000)
    {
        if(t >= 100000000000000)
        {
            if(t >= 10000000000000000)
            {
                if(t >= 100000000000000000)
                {
                    if(t >= 1000000000000000000)
                        return 19;
                    return 18;
                }
                return 17;
            }
            if(t >= 1000000000000000)
                return 16;
            return 15;
        } 
        if(t >= 1000000000000)
        {
            if(t >= 10000000000000)
                return 14;
            return 13;
        }
        if(t >= 100000000000)
            return 12;
        return 11;
    }
    if(t >= 100000)
    {
        if(t >= 10000000)
        {
            if(t >= 100000000)
            {
                if(t >= 1000000000)
                    return 10;
                return 9;
            }
            return 8;
        }
        if (t >= 1000000)
            return 7;
        return 6;
    }
    if(t >= 100)
    {
        if(t >= 1000)
        {
            if(t >= 10000)
                return 5;
            return 4;
        }
        return 3;
    }
    if(t >= 10)
        return 2;
    return 1;
}
template<>
[[nodiscard]] constexpr inline uint32_t DigitCountImpl<unsigned long long>(unsigned long long t) noexcept
{
    if(t >= 10000000000)
    {
        if(t >= 1000000000000000)
        {
            if(t >= 100000000000000000)
            {
                if(t >= 1000000000000000000)
                {
                    if(t >= 10000000000000000000u)
                        return 20;
                    return 19;
                }
                return 18;
            }
            if(t >= 10000000000000000)
                return 17;
            return 16;
        } 
        if(t >= 1000000000000)
        {
            if(t >= 10000000000000)
            {
                if(t > 100000000000000)
                    return 15;
                return 14;
            }
            return 13;
        }
        if(t >= 100000000000)
            return 12;
        return 11;
    }
    if(t >= 100000)
    {
        if(t >= 10000000)
        {
            if(t >= 100000000)
            {
                if(t >= 1000000000)
                    return 10;
                return 9;
            }
            return 8;
        }
        if (t >= 1000000)
            return 7;
        return 6;
    }
    if(t >= 100)
    {
        if(t >= 1000)
        {
            if(t >= 10000)
                return 5;
            return 4;
        }
        return 3;
    }
    if(t >= 10)
        return 2;
    return 1;
}
} // detail

// Get number of whole digits in any type
// 0 = 0 digits
template<typename T = uint32_t, typename U>
[[nodiscard]] constexpr T DigitCount(U number) noexcept
{
    return number == 0 ? 0 : static_cast<T>(detail::DigitCountImpl(number));
}

template<typename T = int, typename U>
[[nodiscard]] constexpr T Bitmask(U bit) noexcept { GAME_ASSERT(bit < (sizeof(T) * CHAR_BIT)) << "Too large bit: " << bit; return static_cast<T>(1) << bit; }

template<typename T = int, typename U>
[[nodiscard]] constexpr T Sign(U value) noexcept { return value < U{0} ? static_cast<T>(-1) : static_cast<T>(1); } // return static_cast<T>(static_cast<int>(value < 0) * -2 + 1)    branchless approach

[[nodiscard]] inline auto Tob2Vec2(const Vector2 &vector) noexcept -> b2Vec2 { return b2Vec2{vector.x(), vector.y()}; }
[[nodiscard]] inline auto Tob2Rot(DefaultFloatType angle) noexcept -> b2Rot { return b2Rot{std::cos(angle), std::sin(angle)}; }
[[nodiscard]] inline auto Tob2Rot(const Vector2 &sin_and_cos) noexcept -> b2Rot { return b2Rot{sin_and_cos.y(), sin_and_cos.x()}; }
[[nodiscard]] inline auto ToNormalVector2(const b2Vec2 &vector) noexcept -> Vector2 { return Vector2{vector.x, vector.y}; }
[[nodiscard]] inline auto ToRotationAngle(const b2Rot &rotation) noexcept -> DefaultFloatType { return std::atan2(rotation.s, rotation.c); }
} // game

#endif // GAME_MATH_HPP