/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Network.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/08 13:18:45 by pitriche          #+#    #+#             */
/*   Updated: 2021/05/26 13:21:27 by pitriche         ###   ########.fr       */
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
		cost -= std::log(tup[0] == 0.0f ? (1 - result) : result);
	}
	return (cost / test.size());
}

void					Network::learning_cycle(const DataPack &train)
{
	/* average derivatives */
	Network	deriv;

	Layer	deriv_l0(TUPLE_SIZE - 1, HIDDEN_LAYER_1);	/* layer 1 activation to cost derivatives */
	Layer	deriv_l1(HIDDEN_LAYER_1, HIDDEN_LAYER_2);	/* layer 2 activation to cost derivatives */
	Layer	deriv_l2(HIDDEN_LAYER_2, 2);				/* example's derivatives */
	
	Vector 	deriv_cost_activ;
	Vector	activ_l0;	/* activation of layer 0's neurons */
	Vector	activ_l1;	/* activation of layer 1's neurons */
	Vector	activ_l2;	/* activation of layer 2's neurons (network output) */
	
	for (const Tuple &example : train)
	{
		Vector	input(example.begin() + 1, example.end());
		
		activ_l0 = this->layer[0].execute(input);
		activ_l1 = this->layer[1].execute(activ_l0);
		activ_l2 = this->layer[2].execute(activ_l1);

		/* calculate Layer 2 derivatives */
		deriv_cost_activ.resize(2);
		deriv_cost_activ[0] = 2 * (activ_l2[0] - (1 - example[0]));
		deriv_cost_activ[1] = 2 * (activ_l2[1] - example[0]);
		deriv_l2 = this->layer[2].derivatives(deriv_cost_activ, activ_l1);
		deriv_l1 = this->layer[1].derivatives(deriv_l2.derivative_activation, activ_l0);
		deriv_l0 = this->layer[0].derivatives(deriv_l1.derivative_activation, input);
		
		/* average derivatives layer 2 */
		for (unsigned neuron = 0; neuron < deriv_l2.n_output; ++neuron)
		{
			for (unsigned input = 0; input < deriv_l2.n_input; ++input)
				deriv.layer[2].weight[neuron][input] += deriv_l2.weight[neuron][input];
			deriv.layer[2].bias[neuron] += deriv_l2.bias[neuron];
		}
		for (unsigned input = 0; input < deriv_l2.n_input; ++input)
			deriv.layer[2].derivative_activation[input] += deriv_l2.derivative_activation[input];

		/* average derivatives layer 1 */
		for (unsigned neuron = 0; neuron < deriv_l1.n_output; ++neuron)
		{
			for (unsigned input = 0; input < deriv_l1.n_input; ++input)
				deriv.layer[1].weight[neuron][input] += deriv_l1.weight[neuron][input];
			deriv.layer[1].bias[neuron] += deriv_l1.bias[neuron];
		}
		for (unsigned input = 0; input < deriv_l1.n_input; ++input)
			deriv.layer[1].derivative_activation[input] += deriv_l1.derivative_activation[input];

		/* average derivatives layer 0 */
		for (unsigned neuron = 0; neuron < deriv_l0.n_output; ++neuron)
		{
			for (unsigned input = 0; input < deriv_l0.n_input; ++input)
				deriv.layer[0].weight[neuron][input] += deriv_l0.weight[neuron][input];
			deriv.layer[0].bias[neuron] += deriv_l0.bias[neuron];
		}
		for (unsigned input = 0; input < deriv_l0.n_input; ++input)
			deriv.layer[0].derivative_activation[input] += deriv_l0.derivative_activation[input];
	}


	/* apply derivatives layer 2 */
	for (unsigned neuron = 0; neuron < deriv.layer[2].n_output; ++neuron)
	{
		for (unsigned input = 0; input < deriv.layer[2].n_input; ++input)
		{
			deriv.layer[2].weight[neuron][input] /= train.size();
			this->layer[2].weight[neuron][input] -= deriv.layer[2].weight[neuron][input] * LEARNING_RATE;
		}
		deriv.layer[2].bias[neuron] /= train.size();
		this->layer[2].bias[neuron] -= deriv.layer[2].bias[neuron] * LEARNING_RATE;
	}

	/* apply derivatives layer 1 */
	for (unsigned neuron = 0; neuron < deriv.layer[1].n_output; ++neuron)
	{
		for (unsigned input = 0; input < deriv.layer[1].n_input; ++input)
		{
			deriv.layer[1].weight[neuron][input] /= train.size();
			this->layer[1].weight[neuron][input] -= deriv.layer[1].weight[neuron][input] * LEARNING_RATE;
		}
		deriv.layer[1].bias[neuron] /= train.size();
		this->layer[1].bias[neuron] -= deriv.layer[1].bias[neuron] * LEARNING_RATE;
	}

	/* apply derivatives layer 0 */
	for (unsigned neuron = 0; neuron < deriv.layer[0].n_output; ++neuron)
	{
		for (unsigned input = 0; input < deriv.layer[0].n_input; ++input)
		{
			deriv.layer[0].weight[neuron][input] /= train.size();
			this->layer[0].weight[neuron][input] -= deriv.layer[0].weight[neuron][input] * LEARNING_RATE;
		}
		deriv.layer[0].bias[neuron] /= train.size();
		this->layer[0].bias[neuron] -= deriv.layer[0].bias[neuron] * LEARNING_RATE;
	}
}