#include "distribution.h"
#include <iostream>
#include <fstream>
#include <string>
#include <memory>

int main()
{
	std::vector<double> data;

	std::ifstream file("data.txt");
	std::string line;

	while (std::getline(file, line))
	{
		data.push_back(std::stof(line));
	}

	file.close();

	double max = *std::max_element(data.begin(), data.end()) + 5;
	double min = *std::min_element(data.begin(), data.end()) - 5;

	std::sort(data.begin(), data.end());
	
	std::unique_ptr<KDE> distribution = std::make_unique<KDE>(data.size(), &data[0], max, min);



	std::cout << "Hello World!" << std::endl;
	return 0;
}