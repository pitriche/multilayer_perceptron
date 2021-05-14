/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Network.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/08 13:18:45 by pitriche          #+#    #+#             */
/*   Updated: 2021/05/12 14:40:29 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "Network.hpp"
#include "Types.hpp"

Network::Network(void) : layer({ Layer(TUPLE_SIZE - 1, HIDDEN_LAYER_1),
	Layer(HIDDEN_LAYER_1, HIDDEN_LAYER_2),
	Layer(HIDDEN_LAYER_2, 2)}) { }

Network::~Network(void) { }

void					Network::initialize(void)
{
	for (Layer &l : this->layer)
		l.initialize();
}

std::array<real_t, 2>	Network::execute(Tuple input)
{
	std::vector<real_t> result(input.begin() + 1, input.end());

	for (auto i : result)
		std::cout << i << std::endl;
}
