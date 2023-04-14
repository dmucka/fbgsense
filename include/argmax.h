/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#pragma once

#include <vector>
/**
 * @brief Returns argmax of vector.
*/
template <typename T>
int argmax(std::vector<T> vec)
{
	if (vec.size() == 0)
		return -1;

	T localMax = vec[0];
	int localMaxIndex = 0;

	// find local argmax
	for (int z = 0; z < vec.size(); ++z)
	{
		if (vec[z] > localMax)
		{
			localMax = vec[z];
			localMaxIndex = z;
		}
	}

	return localMaxIndex;
}
