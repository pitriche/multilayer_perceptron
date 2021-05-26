/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DataPack.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 16:04:13 by pitriche          #+#    #+#             */
/*   Updated: 2021/05/26 15:05:16 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cmath>
#include <cstdlib>

#include "DataPack.hpp"

static Tuple	_parse_example(std::string &line)
{
	Tuple				tup;
	std::stringstream	sst;
	std::string			tmp;
	unsigned			element;
	unsigned			feature_tup;

	sst.str(line);
	element = 0;
	feature_tup = 1;
	while (sst.good())
	{
		tmp.clear();
		std::getline(sst, tmp, ',');
		if (element == 1)
		{
			if (tmp == "B")
				tup[0] = 0.0f;
			else if (tmp == "M")
				tup[0] = 1.0f;
			else
				throw std::logic_error("I HATE YOU : >" + tmp + "<");
		}
		else if (element >= 2)	// select only useful features
		{
			if (!tmp.empty())
				tup[feature_tup++] = (real_t)std::stold(tmp);
			else
				throw std::logic_error("Missing data");
		}
		element++;
	}
	if (element != 32)
		throw std::logic_error("Invalid example (" + std::to_string(element) + " elements)");
	return (tup);
}

void	DataPack::parse(std::istream &is)
{
	std::string	tmp;

	while (is.good())
	{
		std::getline(is, tmp);
		if (tmp.empty()) // skip empty lines including the last one
			continue ;
		try { this->push_back(_parse_example(tmp)); }
		catch (std::exception &e)
		{
			std::cerr << "Parsing failed: " << e.what() << std::endl;
			throw e;
		}
	}
	std::cout << "There are " << this->size() << " elements in dataset" <<
	std::endl;
}

/* ########################################################################## */

void	DataPack::normalize(void)
{
	Tuple		min;
	Tuple		max;

	min.fill(INFINITY);
	max.fill(-INFINITY);
	for (const Tuple &example : *this)
		for (unsigned feature = 1; feature < TUPLE_SIZE; ++feature)
		{
			if (example[feature] < min[feature])
				min[feature] = example[feature];
			if (example[feature] > max[feature])
				max[feature] = example[feature];
		}
	for (Tuple &example : *this)
		for (unsigned feature = 1; feature < TUPLE_SIZE; ++feature)
		{
			example[feature] -= min[feature];
			example[feature] /= max[feature] - min[feature];
		}
}

/* ########################################################################## */

DataPack				DataPack::split(double ratio)
{
	DataPack		pack;
	unsigned		pack_size;
	size_t			swap_id;

	if (ratio <= 0.0 || ratio >= 1.0)
		throw (std::logic_error("You donkey"));
	pack_size = (unsigned)((1.0 - ratio) * (double)this->size());
	for (unsigned i = 0; i < pack_size; ++i)
	{
		swap_id = (size_t)std::rand() % this->size();
		pack.push_back((*this)[swap_id]);
		this->erase(this->begin() + (long)swap_id);
	}
	return (pack);
}

// std::vector<DataPack>	DataPack::batch(unsigned n)
// {
// 	std::vector<DataPack> batches;
// 	DataPack		pack;
// 	unsigned		pack_size;
// 	size_t			swap_id;

// 	if (n < 1)// || n > (this->size() / 2))
// 		throw (std::logic_error("You donkey"));
// 	pack_nbr = (unsigned)((1.0 - ratio) * (double)this->size());
// 	for (unsigned i = 0; i < pack_size; ++i)
// 	{
// 		swap_id = (size_t)std::rand() % this->size();
// 		pack.push_back((*this)[swap_id]);
// 		this->erase(this->begin() + (long)swap_id);
// 	}
// 	return (pack);
// }
