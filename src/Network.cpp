/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Network.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunomartin <brunomartin@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/08 13:18:45 by pitriche          #+#    #+#             */
/*   Updated: 2021/05/09 10:54:52 by brunomartin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Network.hpp"

Layer::Layer(unsigned input, unsigned output) : n_input(input),
	n_output(output)
{
	if (input == 0 || output == 0)
		throw (std::logic_error("Invalid layer size"));
	this->bias.resize(output, 0.0f);
	this->weight.resize(output, std::vector<real_t>(input, 0.0f));
}

Layer::~Layer(void) { }

inline static real_t	_rand()
{ return (((real_t)std::rand() / (real_t)INT_MAX) * 2.0f - 1.0f); }

void	Layer::initialize(void)
{
	for (real_t &n : this->bias)
		n = _rand();
	for (std::vector<real_t> &vec : this->weight)
		for (real_t &n : vec)
			n = _rand();

}

Layer	&Layer::operator=(const Layer &rhs)
{
	if (this->n_input != rhs.n_input || this->n_output != rhs.n_output)
		throw (std::logic_error("Invalid layer assignation: size differ"));
	this->weight = rhs.weight;
	this->bias = rhs.bias;
	return (*this);
}
