/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Network.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/08 13:18:45 by pitriche          #+#    #+#             */
/*   Updated: 2021/05/17 14:09:31 by pitriche         ###   ########.fr       */
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

std::array<real_t, 2>	Network::execute(const Tuple &input)
{
	std::vector<real_t>		values(input.begin() + 0, input.end());
	std::array<real_t, 2>	result;

	values = this->layer[0].execute(values);
	values = this->layer[1].execute(values);
	values = this->layer[2].execute(values);
	result[0] = values[0];
	result[1] = values[1];
	return (result);
}
