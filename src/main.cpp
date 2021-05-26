/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 15:14:08 by pitriche          #+#    #+#             */
/*   Updated: 2021/05/26 14:48:11 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <fstream>

#include "DataPack.hpp"
#include "Network.hpp"
#include "Defines.hpp"

static int	_is_correct(const std::array<real_t, 2> &res, real_t type)
{
	if (res[0] >= res[1])
		return (type == 0.0f);
	return (type == 1.0f);
}

int			main(int ac, char **av)
{
	std::srand(SEED);

	std::ifstream	ifs;
	std::string		filename;
	DataPack		train;
	DataPack		test;

	if (ac < 2)
	{
		std::cout << "Default file: data.csv" << std::endl;
		filename = "data.csv";
	}
	else
		filename = std::string(av[1]);
	ifs.open(filename);
	if (!ifs.is_open())
	{
		std::cout << "Impossible to open file" << std::endl;
		return (0);
	}
	try { train.parse(ifs); }
	catch (std::exception &e) { return (0); }
	train.normalize();
	test = train.split(TRAIN_TEST_RATIO);
	std::cout << "train set size: " << train.size() << std::endl;
	std::cout << "test set size: " << test.size() << std::endl;

	/* ###################################################################### */
	/* #########################	Execution	############################# */
	/* ###################################################################### */

	Network net;
	net.initialize();
	for (unsigned i = 0; i < LEARNING_CYCLES; ++i)
	{
		if (i % (LEARNING_CYCLES / 12) == 0)
			std::cout << "Epoch : " << i << '/' << LEARNING_CYCLES <<
			"  \t- Test cost : " << net.cost(test) << " - Train cost : " <<
			net.cost(train) << std::endl;
		net.learning_cycle(train);
	}
	std::cout << "Final Epoch : " << LEARNING_CYCLES <<
			"  \t- Test cost : " << net.cost(test) << " - Train cost : " <<
			net.cost(train) << std::endl;

	/* ###################################################################### */
	/* #########################	Accuracy	############################# */
	/* ###################################################################### */

	int correct_b = 0;
	int correct_m = 0;
	int total_b = 0;
	for (const auto &ex : test)
		if (ex[0] == 0.0f)
		{
			correct_b += _is_correct(net.execute(ex), ex[0]);
			++total_b;
		}
		else
			correct_m += _is_correct(net.execute(ex), ex[0]);
	std::cout << std::endl << "Validation:" << std::endl;
	std::cout << "B:   " << correct_b << '/' << total_b << std::endl;
	std::cout << "M:   " << correct_m << '/' << (int)test.size() - total_b << std::endl;
	std::cout << "Acc: " << (correct_m + correct_b) / (float)test.size() * 100.0f << '%' << std::endl;

	correct_b = 0;
	correct_m = 0;
	total_b = 0;
	for (const auto &ex : train)
		if (ex[0] == 0.0f)
		{
			correct_b += _is_correct(net.execute(ex), ex[0]);
			++total_b;
		}
		else
			correct_m += _is_correct(net.execute(ex), ex[0]);
	std::cout << std::endl << "Train:" << std::endl;
	std::cout << "B:   " << correct_b << '/' << total_b << std::endl;
	std::cout << "M:   " << correct_m << '/' << (int)train.size() - total_b << std::endl;
	std::cout << "Acc: " << (correct_m + correct_b) / (float)train.size() * 100.0f << '%' << std::endl;
	return (0);
}
