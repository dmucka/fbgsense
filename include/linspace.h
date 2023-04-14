#pragma once
// https://gist.github.com/lorenzoriano/5414671

#include <vector>

/**
 * @brief Creates a vector of linearly spaced elements.
*/
template <typename T>
std::vector<double> linspace(T start, T end, int num)
{
	std::vector<double> linspaced;

	if (0 != num)
	{
		if (1 == num)
		{
			linspaced.push_back(start);
		}
		else
		{
			double len = num - 1;
			double delta = (end - start) / len;

			for (size_t i = 0; i < len; ++i)
			{
				linspaced.push_back(start + (delta * i));
			}

			// ensure that start and end are exactly the same as the input
			linspaced.push_back(end);
		}
	}

	return linspaced;
}
