/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 15:14:08 by pitriche          #+#    #+#             */
/*   Updated: 2021/06/17 10:40:24 by pitriche         ###   ########.fr       */
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

static void	_export_costs(std::vector<real_t> costs_test,
	std::vector<real_t> costs_train)
{
	std::ofstream	ofs;
	
	ofs.open("costs_test.irc");
	if (!ofs.is_open())
	{
		std::cout << "Impossible to open file" << std::endl;
		exit(0);
	}
	for (real_t r : costs_test)
		ofs << r << ' ';
	ofs.close();

	ofs.open("costs_train.irc");
	if (!ofs.is_open())
	{
		std::cout << "Impossible to open file" << std::endl;
		exit(0);
	}
	for (real_t r : costs_train)
		ofs << r << ' ';
	ofs.close();
}

/* ########################################################################## */

void		_train(DataPack &train)
{
	DataPack			save = train;
	DataPack			test;

	Network				net;
	Network				net_save;

	std::vector<real_t>	costs_test;
	std::vector<real_t>	costs_train;
	real_t				old_cost;
	real_t				new_cost;

	unsigned			i;

	net.initialize();
	old_cost = INFINITY;
	test = train.split(TRAIN_TEST_RATIO);
	std::cout << "train set size: " << train.size() << std::endl;
	std::cout << "test set size: " << test.size() << std::endl;
	for (i = 0; i < LEARNING_CYCLES; ++i)
	{
		if (i % (LEARNING_CYCLES / 12) == 0)
		{
			std::cout << "Epoch : " << i << '/' << LEARNING_CYCLES <<
			"  \t- Test cost : " << net.cost(test) << " - Train cost : " <<
			net.cost(train) << std::endl;
		}
		net.learning_cycle_regular(train);

		/* save costs for graphs */
		if (i % (LEARNING_CYCLES / 200) == 0)
		{
			costs_test.push_back(net.cost(test));
			costs_train.push_back(net.cost(train));
		}

		/* early stopping check */
		if (i % EARLY_STOP_CHECK == 0)
		{
			new_cost = net.cost(test);
			if (new_cost < old_cost)
			{
				old_cost = new_cost;
				net_save = net;
			}
			else
			{
				std::cout << "Early stopping !" << std::endl;
				net = net_save;
				break ;
			}
		}
	}
	std::cout << "Final Epoch : " << i <<
	"  \t- Test cost : " << net.cost(test) << " - Train cost : " <<
	net.cost(train) << std::endl;
	costs_test.push_back(net.cost(test));
	costs_train.push_back(net.cost(train));

	net.export_file("network.irc");
	_export_costs(costs_test, costs_train);

	/* ###################################################################### */
	/* #########################	Accuracy	############################# */
	/* ###################################################################### */

	// int correct_b = 0;
	// int correct_m = 0;
	// int total_b = 0;
	// for (const auto &ex : test)
	// 	if (ex[0] == 0.0f)
	// 	{
	// 		correct_b += _is_correct(net.execute(ex), ex[0]);
	// 		++total_b;
	// 	}
	// 	else
	// 		correct_m += _is_correct(net.execute(ex), ex[0]);
	// std::cout << std::endl << "Validation:" << std::endl;
	// std::cout << "B:   " << correct_b << '/' << total_b << std::endl;
	// std::cout << "M:   " << correct_m << '/' << (int)test.size() - total_b << std::endl;
	// std::cout << "Acc: " << (correct_m + correct_b) / (float)test.size() * 100.0f << '%' << std::endl;

	// correct_b = 0;
	// correct_m = 0;
	// total_b = 0;
	// for (const auto &ex : train)
	// 	if (ex[0] == 0.0f)
	// 	{
	// 		correct_b += _is_correct(net.execute(ex), ex[0]);
	// 		++total_b;
	// 	}
	// 	else
	// 		correct_m += _is_correct(net.execute(ex), ex[0]);
	// std::cout << std::endl << "Train:" << std::endl;
	// std::cout << "B:   " << correct_b << '/' << total_b << std::endl;
	// std::cout << "M:   " << correct_m << '/' << (int)train.size() - total_b << std::endl;
	// std::cout << "Acc: " << (correct_m + correct_b) / (float)train.size() * 100.0f << '%' << std::endl;
}

void		_execute(const std::string &filename)
{
	std::ifstream	ifs;
	DataPack		data;
	Network			net;
	int				correct_b;
	int				correct_m;
	int				total_b;

	ifs.open(filename);
	if (!ifs.is_open())
	{
		std::cout << "Impossible to open file" << std::endl;
		exit(0);
	}
	try { data.parse(ifs); }
	catch (std::exception &e) { exit(0); }
	ifs.close();
	data.normalize();

	net.import_file("network.irc");

	correct_b = 0;
	correct_m = 0;
	total_b = 0;
	for (const auto &ex : data)
		if (ex[0] == 0.0f)
		{
			correct_b += _is_correct(net.execute(ex), ex[0]);
			++total_b;
		}
		else
			correct_m += _is_correct(net.execute(ex), ex[0]);
	std::cout << "Total cost: " << net.cost(data) << std::endl;
	std::cout << "B:   " << correct_b << '/' << total_b << std::endl;
	std::cout << "M:   " << correct_m << '/' << (int)data.size() - total_b << std::endl;
	std::cout << "Acc: " << (float)(correct_m + correct_b) / (float)data.size() * 100.0f << '%' << std::endl;
}

/* ########################################################################## */

int			main(int ac, char **av)
{
	std::srand(SEED);

	std::ifstream	ifs;
	DataPack		data;

	if (ac < 2)
	{
		ifs.open("data.csv");
		if (!ifs.is_open())
		{
			std::cout << "Impossible to open file" << std::endl;
			return (0);
		}
		try { data.parse(ifs); }
		catch (std::exception &e) { return (0); }
		ifs.close();
		data.normalize();

		_train(data);
	}
	else
		_execute(std::string(av[1]));
	return (0);
}
