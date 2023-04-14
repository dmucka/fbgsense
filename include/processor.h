/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#pragma once

#include <memory>

/**
 * @brief Provides support for custom computation implementations.
 * @tparam T Processor type
*/
template<typename T>
class processor
{
protected:
	/**
	 * @brief Java string hashing function.
	 * http://hg.openjdk.java.net/jdk8/jdk8/jdk/file/687fd7c7986d/src/share/classes/java/lang/String.java#l1452
	 * @param str String to hash.
	*/
	constexpr int get_hash(const char str[])
	{
		int h = 0;
		size_t len = strlen(str);

		for (int i = 0; i < len; ++i)
			h = 31 * h + str[i];

		return h;
	}
public:
	/**
	 * @brief Gets the unique identifier of the concrete subclass. Used for equal-type checking.
	 * The implementation should use the protected function ::get_hash
	*/
	constexpr virtual int get_type() = 0;

	/**
	 * @brief Factory for the current concrete subclass.
	*/
	virtual std::unique_ptr<T> create() = 0;

	virtual ~processor() = default;
};
