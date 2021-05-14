/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 15:14:08 by pitriche          #+#    #+#             */
/*   Updated: 2021/05/12 14:18:05 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <fstream>

#include "DataPack.hpp"
#include "Network.hpp"

#define SEED				460
#define TRAIN_TEST_RATIO	0.7

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
	Network net;

	net.initialize();
	net.execute(train[0]);
	return (0);
}
