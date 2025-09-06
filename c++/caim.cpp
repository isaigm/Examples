#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <map>
#include <set>

std::random_device rd;
std::mt19937 mt(1234);
float getRandom(float min, float max)
{
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(mt);
}

struct Sample
{
	int target{};
	float value{};
};
struct Interval
{
	enum class Type
	{
		Closed, //[x, y]
		LeftOpenRightClosed // (x, y]
	};
	Interval(float min_, float max_, Type type_ = Type::Closed) : min(min_), max(max_), type(type_)
	{

	}
	bool contains(float x)
	{
		switch (type)
		{
		case Type::Closed:
			return x >= min && x <= max;
		case Type::LeftOpenRightClosed:
			return x > min && x <= max;
		default:
			break;
		}
		return false;
	}
	float min;
	float max;
	Type type;
};
std::ostream& operator<<(std::ostream& os, const Interval& interval)
{
	switch (interval.type)
	{
	case Interval::Type::Closed:
		os << "[" << interval.min << ", " << interval.max << "]";
		break;
	case Interval::Type::LeftOpenRightClosed:
		os << "(" << interval.min << ", " << interval.max << "]";
		break;
	default:
		break;
	}

	return os;
}

std::vector<std::vector<int>> computeQuantaMatrix(size_t nClasses, const std::vector<Sample>& row, const std::vector<Interval>& intervals)
{
	std::vector<std::vector<int>> quantaMat(nClasses, (std::vector<int>(intervals.size(), 0)));
	for (size_t i = 0; i < intervals.size(); i++)
	{
		auto interval = intervals[i];
		for (size_t j = 0; j < row.size(); j++)
		{
			auto sample = row[j];
			int target = sample.target;
			float val = sample.value;
			if (interval.contains(val))
			{
				quantaMat[target][i]++;
			}
		}
	}
	return quantaMat;
}
float computeCAIM(const std::vector<std::vector<int>>& quantaMat)
{
	if (quantaMat.empty() || quantaMat[0].empty()) return 0.0f;
	auto cols = quantaMat[0].size(); //intervals
	auto rows = quantaMat.size(); //classes
	float sum = 0;
	for (size_t i = 0; i < cols; i++)
	{
		int maxr = 0;
		int total = 0;
		for (size_t j = 0; j < rows; j++)
		{
			total += quantaMat[j][i];
			maxr = std::max(maxr, quantaMat[j][i]);
		}
		if (total > 0) { 
			sum += float(maxr * maxr) / float(total);
		}
	}
	return sum / float(cols);
}

void CAIM(size_t nClasses, const std::vector<Sample>& row)
{
	if (row.empty()) {
		std::cout << "Input data is empty." << std::endl;
		return;
	}
	float minValue = row[0].value;
	float maxValue = row[0].value;
	std::set<float> uniqueValues;
	for (auto sample : row)
	{
		minValue = std::min(minValue, sample.value);
		maxValue = std::max(maxValue, sample.value);
		uniqueValues.insert(sample.value);
	}
	std::vector<float> values;
	for (auto val : uniqueValues)
	{
		values.push_back(val);
	}
	std::vector<float> boundary;
	for (int i = 0; i < values.size() - 1; i++)
	{
		float midPoint = (values[i] + values[i + 1]) / 2.0f;
		boundary.push_back(midPoint);
	}

	float globalCAIM = 0;
	std::vector<Interval> scheme{ {minValue, maxValue} };
	std::set<size_t> usedIndex;
	size_t k = 1;
	while (true)
	{
		float currMax = 0;
		size_t selectedIndex{};
		std::vector<Interval> currIntervals;
		for (size_t i = 0; i < boundary.size(); i++)
		{
			if (usedIndex.contains(i)) continue;
			auto tentativeScheme = scheme;
			for (size_t j = 0; j < tentativeScheme.size(); j++)
			{
				auto& interval = tentativeScheme[j];
				if (interval.contains(boundary[i]))
				{
					float tempMax = interval.max;
					interval.max = boundary[i];
					Interval newInterval(boundary[i], tempMax, Interval::Type::LeftOpenRightClosed);
					tentativeScheme.insert(tentativeScheme.begin() + j + 1, newInterval);
					break;
				}
			}
			auto currQuant = computeQuantaMatrix(nClasses, row, tentativeScheme);
			float currCaim = computeCAIM(currQuant);
			if (currCaim > currMax)
			{
				selectedIndex = i;
				currMax = currCaim;
				currIntervals = tentativeScheme;
			}

		}
		if (currMax > globalCAIM || k < nClasses)
		{
			scheme = currIntervals;
			usedIndex.insert(selectedIndex);
			globalCAIM = currMax;
		}
		else break;
		k++;
	}
	for (auto& interval : scheme)
	{
		std::cout << interval << " ";
	}
	std::cout << "\n";
	auto quantaMatrix = computeQuantaMatrix(nClasses, row, scheme);
	auto caim = computeCAIM(quantaMatrix);
	for (size_t i = 0; i < quantaMatrix.size(); i++)
	{
		for (size_t j = 0; j < quantaMatrix[0].size(); j++)
		{
			std::cout << quantaMatrix[i][j] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "Caim: " << caim << "\n";
}

int main()
{
	size_t nClasses = 3; // 0: Setosa, 1: Versicolor, 2: Virginica

	
	std::vector<float> sepal_lengths = {
		5.1, 4.9, 4.7, 4.6, 5.0, 5.4, 4.6, 5.0, 4.4, 4.9, 5.4, 4.8, 4.8, 4.3, 5.8, 5.7, 5.4, 5.1, 5.7, 5.1, 5.4, 5.1, 4.6, 5.1, 4.8, 5.0, 5.0, 5.2, 5.2, 4.7, 4.8, 5.4, 5.2, 5.5, 4.9, 5.0, 5.5, 4.9, 4.4, 5.1, 5.0, 4.5, 4.4, 5.0, 5.1, 4.8, 5.1, 4.6, 5.3, 5.0,
		7.0, 6.4, 6.9, 5.5, 6.5, 5.7, 6.3, 4.9, 6.6, 5.2, 5.0, 5.9, 6.0, 6.1, 5.6, 6.7, 5.6, 5.8, 6.2, 5.6, 5.9, 6.1, 6.3, 6.1, 6.4, 6.6, 6.8, 6.7, 6.0, 5.7, 5.5, 5.5, 5.8, 6.0, 5.4, 6.0, 6.7, 6.3, 5.6, 5.5, 5.5, 6.1, 5.8, 5.0, 5.6, 5.7, 5.7, 6.2, 5.1, 5.7,
		6.3, 5.8, 7.1, 6.3, 6.5, 7.6, 4.9, 7.3, 6.7, 7.2, 6.5, 6.4, 6.8, 5.7, 5.8, 6.4, 6.5, 7.7, 7.7, 6.0, 6.9, 5.6, 7.7, 6.3, 6.7, 7.2, 6.2, 6.1, 6.4, 7.2, 7.4, 7.9, 6.4, 6.3, 6.1, 7.7, 6.3, 6.4, 6.0, 6.9, 6.7, 6.9, 5.8, 6.8, 6.7, 6.7, 6.3, 6.5, 6.2, 5.9
	};
	std::vector<int> species(150);
	std::fill(species.begin(), species.begin() + 50, 0);       // 50 Setosas
	std::fill(species.begin() + 50, species.begin() + 100, 1); // 50 Versicolors
	std::fill(species.begin() + 100, species.end(), 2);        // 50 Virginicas

	std::vector<Sample> row;
	for (size_t i = 0; i < sepal_lengths.size(); ++i) {
		row.push_back({ species[i], sepal_lengths[i] });
	}

	CAIM(nClasses, row);
	return 0;
}
