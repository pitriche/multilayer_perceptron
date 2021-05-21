/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Network.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/08 13:18:45 by pitriche          #+#    #+#             */
/*   Updated: 2021/05/21 18:24:34 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "Network.hpp"
#include "Types.hpp"
#include "Defines.hpp"
#include <cmath>	// pow, M_E


/* ########################################################################## */

static inline real_t	_softmax_1(std::array<real_t, 2> res)
{ return (std::exp(res[1]) / (std::exp(res[0]) + std::exp(res[1]))); }

/* ########################################################################## */

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
	Vector		values(input.begin() + 1, input.end());
	std::array<real_t, 2>	result;

	values = this->layer[0].execute(values);
	values = this->layer[1].execute(values);
	values = this->layer[2].execute(values);
	result[0] = values[0];
	result[1] = values[1];
	return (result);
}

real_t					Network::cost(const DataPack &test)
{
	real_t	result;
	real_t	cost;

	cost = 0.0f;
	for (const Tuple &tup : test)
	{
		result = _softmax_1(this->execute(tup));
		cost += (real_t)std::pow((tup[0] - result), 2);
	}
	return (cost / test.size());
}

void					Network::learning_cycle(const DataPack &train)
{
	Layer	global_deriv_l2(HIDDEN_LAYER_2, 2);	// average derivatives

	global_deriv_l2.initialize_null();
	for (const Tuple &example : train)
	{
		Layer	deriv_l2(HIDDEN_LAYER_2, 2);	// example's derivatives
		Vector 	deriv_cost_activ;
		
		Vector	input(example.begin() + 1, example.end());
		Vector	activ_l0;	// activation of layer 0's neurons
		Vector	activ_l1;	// activation of layer 1's neurons
		Vector	activ_l2;	// activation of layer 2's neurons (network output)
		
		activ_l0 = this->layer[0].execute(input);
		activ_l1 = this->layer[1].execute(activ_l0);
		activ_l2 = this->layer[2].execute(activ_l1);

		// Layer 2
		deriv_cost_activ.resize(2);
		deriv_cost_activ[0] = 2 * (activ_l2[0] + example[0]);
		deriv_cost_activ[1] = 2 * (activ_l2[1] - example[0]);
		deriv_l2 = this->layer[2].derivatives(deriv_cost_activ, activ_l1);
		
		for (unsigned neuron = 0; neuron < deriv_l2.n_output; ++neuron)
		{
			for (unsigned input = 0; input < deriv_l2.n_input; ++input)
				global_deriv_l2.weight[neuron][input] += deriv_l2.weight[neuron][input];
			global_deriv_l2.bias[neuron] += deriv_l2.bias[neuron];
		}
	}
	for (unsigned neuron = 0; neuron < global_deriv_l2.n_output; ++neuron)
	{
		for (unsigned input = 0; input < global_deriv_l2.n_input; ++input)
		{
			global_deriv_l2.weight[neuron][input] /= train.size();
			this->layer[2].weight[neuron][input] -= global_deriv_l2.weight[neuron][input] * LEARNING_RATE;
		}
		global_deriv_l2.bias[neuron] /= train.size();
		this->layer[2].bias[neuron] -= global_deriv_l2.bias[neuron] * LEARNING_RATE;
	}
}