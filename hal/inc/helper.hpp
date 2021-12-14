/**************************************************************************//**
 * @file helper.hpp
 * @date 15.04.2016
 * @author vlunyov
 * @brief Helpers
 *
 * Last modified by: $Author: vlunyov $
 * $Revision: 1690 $
 * $Date: 2017-06-05 20:10:49 +0300 (Пн, 05 июн 2017) $
 *
 * @note
 * <h2><center>&copy; Copyright KW-Systems</center></h2>
 * <h2><center><a href="https://kwsystems.ru/">KW-Systems</a></center></h2>
 *
 *****************************************************************************/

#ifndef HELPER_H
#define HELPER_H

#include <cstdint>
#include <type_traits>
#include <initializer_list>
#include <iterator>
#include <utility>

namespace kip5 {
namespace helper {

/**
 * @brief Линейное преобразование
 * @details
 * Преобразует x по формуле:<br/>
 * \f$y(x)=\frac{(x-x_{min})*(y_{max}-y_{min})}{x_{max}-x_{min}}\f$<br/>
 * Если x равно iMin, то значение будет равно oMin.<br/>
 * Если x равно iMax, то значение будет равно oMax.
 * @tparam I Тип входных параметров
 * @tparam O Тип выходных параметров
 * @param x Преобразуемая величина
 * @param iMin Минимальное значение входной величины
 * @param iMax Максимальное значение входной величины
 * @param oMin Минимальное значение выходной величины
 * @param oMax Максимальное значение выходной величины
 * @return Преобразованная величина
 */
template<typename I, typename O>
inline O linearMapper(I x, I iMin, I iMax, O oMin, O oMax)
{
	using T = typename std::conditional<std::is_floating_point<I>::value, I, typename std::conditional<sizeof(I) < sizeof(long), long, long long>::type>::type;
	return static_cast<O>((static_cast<T>(x - iMin) * (oMax - oMin)) / (iMax - iMin)) + oMin;
}

/**
 * @brief Ограничение входной величины границами [min,max]
 * @tparam T Тип параметров
 * @param val Входная величина
 * @param min Минимум
 * @param max Максимум
 * @return Ограниченная величина
 */
template<class T>
inline T limit(T val, T min, T max)
{
	if(val > max)
		val = max;
	if(val < min)
		val = min;
	return val;
}

/**
 * @brief Ограничение входной величины границами [min,max] с учетом порядка min и max
 * @tparam T Тип параметров
 * @param val Входная величина
 * @param min Минимум (максимум)
 * @param max Максимум (минимум)
 * @return Ограниченная величина
 */
template<class T>
inline T orderedLimit(T val, T min, T max)
{
	if(min <= max)
		return limit(val, min, max);
	else
		return limit(val, max, min);
}

/**
 * @brief Повторение функции
 * @tparam N Ко-во повторений
 * @param f Указатель на повторяемую функцию
 */
template<unsigned N>
void repeat(void (*f)())
{
	for(unsigned i = 0; i < N; ++i)
		f();
}

/**
 * @brief Вычисляет размер массива
 * @tparam T Тип массива
 * @param array Ссылка на массив
 * @return Размер массива
 */
template<typename T>
size_t array_size(const T& array)
{
	return std::distance(std::begin(array), std::end(array));
}

/**
 * @brief Вычисляет ко-во элементов массива
 * @tparam T Тип элементов массива
 * @tparam N Ко-во элементов массива
 * @return Ко-во элементов массива
 */
template<typename T, size_t N>
constexpr size_t array_count(const T (&array)[N])
{
	(void)array;
	return N;
}

/**
 * @brief Шаблонный класс инкапсулирующий массив который можно возвращать из constexpr функций
 * @details
 * Документ:
 * <a href="https://raw.githubusercontent.com/boostcon/cppnow_presentations_2015/master/files/CppNow2015_ConstexprSlides.pdf">
 * CppNow2015_ConstexprSlides.pdf</a>
 * @tparam T Тип хранимый массивом
 * @tparam N Размер массива
 */
template<typename T, size_t N>
class array_result
{
private:
	constexpr static size_t size_ = N;
	T data_[N] {};	// T default constructor essential!
public:
	constexpr size_t size() const {return N;}
#if __cplusplus >= 201402L
	constexpr T& operator[](size_t n) {return data_[n];}
#endif
	constexpr const T& operator[](size_t n) const {return data_[n];}
	using iterator = T*;
	constexpr iterator begin() {return &data_[0];}
	constexpr iterator end() {return &data_[N];}
};

/**
 * @brief Массив инициализируемый списком инициализации
 * @tparam T Тип элементов массива
 */
template<typename T>
struct array : public std::initializer_list<T>
{
	constexpr array(std::initializer_list<T> il) : std::initializer_list<T>(il) {}
	constexpr const T& operator[](size_t n) const noexcept {return *(this->begin() + n);}
};

/**
 * @brief Односторонний преобразователь
 * @tparam T1 Тип 1
 * @tparam T2 Тип 2
 */
template<typename T1, typename T2>
struct map : public array<std::pair<T1, T2>>
{
	/**
	 * Конструирует односторонний преобразователь из списка инициализации
	 * @param il Список инициализации
	 */
	constexpr map(std::initializer_list<std::pair<T1, T2>> il) : array<std::pair<T1, T2>>(il) {}

	/**
	 * Поиск параметра по ключу
	 * @param key Ключ
	 * @return Значения найденного параметра или -1 при его отсутствии
	 */
	T2 find(T1 key) const
	{
		for(auto i : *this)
			if(i.first == key)
				return i.second;
		return (T2)-1;
	}
};

/**
 * @brief Двусторонний преобразователь
 * @tparam T1 Тип 1
 * @tparam T2 Тип 2
 */
template<typename T1, typename T2>
struct bimap : public map<T1, T2>
{
	/**
	 * Конструирует двусторонний преобразователь из списка инициализации
	 * @param il Список инициализации
	 */
	constexpr bimap(std::initializer_list<std::pair<T1, T2>> il) : map<T1, T2>(il) {}

	T2 find(T1 key) const = delete;

	/**
	 * Поиск второго параметра по первому ключу
	 * @param key Ключ
	 * @return Значения найденного параметра или -1 при его отсутствии
	 */
	T2 findSecond(T1 key) const
	{
		return map<T1, T2>::find(key);
	}

	/**
	 * Поиск первого параметра по второму ключу
	 * @param key Ключ
	 * @return Значения найденного параметра или -1 при его отсутствии
	 */
	T1 findFirst(T2 key) const
	{
		for(auto i : *this)
			if(i.second == key)
				return i.first;
		return (T1)-1;
	}
};

/**
 * @brief Байтовое объединение.
 * @tparam B Структура с побитовым доступом
 * @tparam d Значение по-умолчанию
 */
template<typename B, uint8_t d = 0>
union ByteUnion
{
	static_assert(sizeof(B) == sizeof(uint8_t), "Size of T must be 1 byte");
	operator uint8_t() const {return byte;}
	uint8_t byte {d};			///< Общее значение
	B bits;						///< Битовая структура
};

/**
 * @brief Обертка для объединений.
 * Предоставляет побайтный доступ к объединению.
 * @tparam T Тип общего значения
 * @tparam B Структура с побитовым доступом
 * @tparam d Значение по-умолчанию
 */
template<typename T, typename B, T d = 0>
union Union
{
	static_assert(std::is_literal_type<T>::value, "Type of T must be a literal type");
	static_assert(sizeof(T) == sizeof(B), "Size of T must be equal to size of B");
	operator T() const {return value;}
	uint8_t operator[](int i) const {return bytes[i];}
	uint8_t& operator[](int i) {return bytes[i];}
	T value {d};				///< Общее значение
	B bits;						///< Битовая структура
	uint8_t bytes[sizeof(T)];	///< Массив байтов
};

struct Bits8
{
	uint8_t bit0 : 1;	///< Бит 0, маска 0x01 = 0b00000001
	uint8_t bit1 : 1;	///< Бит 1, маска 0x02 = 0b00000010
	uint8_t bit2 : 1;	///< Бит 2, маска 0x04 = 0b00000100
	uint8_t bit3 : 1;	///< Бит 3, маска 0x08 = 0b00001000
	uint8_t bit4 : 1;	///< Бит 4, маска 0x10 = 0b00010000
	uint8_t bit5 : 1;	///< Бит 5, маска 0x20 = 0b00100000
	uint8_t bit6 : 1;	///< Бит 6, маска 0x40 = 0b01000000
	uint8_t bit7 : 1;	///< Бит 7, маска 0x80 = 0b10000000
};

struct Bits16 : Bits8
{
	uint8_t bit8  : 1;	///< Бит  8, маска 0x0100
	uint8_t bit9  : 1;	///< Бит  9, маска 0x0200
	uint8_t bit10 : 1;	///< Бит 10, маска 0x0400
	uint8_t bit11 : 1;	///< Бит 11, маска 0x0800
	uint8_t bit12 : 1;	///< Бит 12, маска 0x1000
	uint8_t bit13 : 1;	///< Бит 13, маска 0x2000
	uint8_t bit14 : 1;	///< Бит 14, маска 0x4000
	uint8_t bit15 : 1;	///< Бит 15, маска 0x8000
};

struct Bits32 : Bits16
{
	uint8_t bit16 : 1;	///< Бит 16, маска 0x00010000
	uint8_t bit17 : 1;	///< Бит 17, маска 0x00020000
	uint8_t bit18 : 1;	///< Бит 17, маска 0x00040000
	uint8_t bit19 : 1;	///< Бит 19, маска 0x00080000
	uint8_t bit20 : 1;	///< Бит 20, маска 0x00100000
	uint8_t bit21 : 1;	///< Бит 21, маска 0x00200000
	uint8_t bit22 : 1;	///< Бит 22, маска 0x00400000
	uint8_t bit23 : 1;	///< Бит 23, маска 0x00800000
	uint8_t bit24 : 1;	///< Бит 24, маска 0x01000000
	uint8_t bit25 : 1;	///< Бит 25, маска 0x02000000
	uint8_t bit26 : 1;	///< Бит 26, маска 0x04000000
	uint8_t bit27 : 1;	///< Бит 27, маска 0x08000000
	uint8_t bit28 : 1;	///< Бит 28, маска 0x10000000
	uint8_t bit29 : 1;	///< Бит 29, маска 0x20000000
	uint8_t bit30 : 1;	///< Бит 30, маска 0x40000000
	uint8_t bit31 : 1;	///< Бит 31, маска 0x80000000
};

constexpr uint8_t endian(uint8_t x)
{
	return x;
}

constexpr uint16_t endian(uint16_t x)
{
	return x << 8 | x >> 8;
}

constexpr uint32_t endian(uint32_t x)
{
#if __cplusplus >= 201402L
	x = x << 16 | x >> 16;
	x = (x & 0x00FF00FF) << 8 | (x & 0xFF00FF00) >> 8;
	return x;
#else
	return ((x << 16 | x >> 16) & 0x00FF00FF) << 8 | ((x << 16 | x >> 16) & 0xFF00FF00) >> 8;
#endif
}

constexpr uint64_t endian(uint64_t x)
{
	return (uint64_t)endian((uint32_t)x) << 32 | endian((uint32_t)(x >> 32));
}

static_assert(endian((uint8_t)0x11) == 0x11, "endian");
static_assert(endian((uint16_t)0x1122) == 0x2211, "endian");
static_assert(endian((uint32_t)0x11223344) == 0x44332211, "endian");
static_assert(endian((uint64_t)0x1122334455667788) == 0x8877665544332211, "endian");

} // namespace helper
} // namespace kws

#endif // HELPER_H
