/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Network.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/08 13:18:45 by pitriche          #+#    #+#             */
/*   Updated: 2021/06/17 10:00:19 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "Network.hpp"
#include "Types.hpp"
#include "Defines.hpp"

#include <cmath>	// pow, M_E
#include <fstream>	// file access


/* ########################################################################## */

static inline real_t	_softmax_1(std::array<real_t, 2> res)
{ return (std::exp(res[1]) / (std::exp(res[0]) + std::exp(res[1]))); }

static inline real_t	_softmax_1_vec(Vector res)
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

std::array<real_t, 2>	Network::execute(const Tuple &input) const
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

real_t					Network::cost(const DataPack &test) const
{
	real_t	result;
	real_t	cost;

	cost = 0.0f;
	for (const Tuple &tup : test)
	{
		result = _softmax_1(this->execute(tup));
		cost -= std::log(tup[0] == 0.0f ? (1 - result) : result);
	}
	return (cost / (real_t)test.size());
}

void					Network::learning_cycle_regular(const DataPack &train)
{
	/* average derivatives */
	Network	deriv;

	/* example's derivatives */
	Layer	deriv_l0(TUPLE_SIZE - 1, HIDDEN_LAYER_1);
	Layer	deriv_l1(HIDDEN_LAYER_1, HIDDEN_LAYER_2);
	Layer	deriv_l2(HIDDEN_LAYER_2, 2);
	
	Vector 	deriv_cost_activ;
	Vector	activ_l0;	/* activation of layer 0's neurons */
	Vector	activ_l1;	/* activation of layer 1's neurons */
	Vector	activ_l2;	/* activation of layer 2's neurons (network output) */
	
	for (const Tuple &example : train)
	{
		Vector	input(example.begin() + 1, example.end());
		
		activ_l0 = this->layer[0].execute(input);
		activ_l1 = this->layer[1].execute(activ_l0);
		// activ_l2 = this->layer[2].execute(activ_l1);

		activ_l2.resize(2);
		activ_l2[1] = _softmax_1_vec(this->layer[2].execute(activ_l1));
		activ_l2[0] = 1.0f - activ_l2[1];


		/* calculate Layer 2 derivatives */
		deriv_cost_activ.resize(2);
		deriv_cost_activ[0] = 2 * (activ_l2[0] - (1 - example[0]));
		deriv_cost_activ[1] = 2 * (activ_l2[1] - example[0]);
		deriv_l2 = this->layer[2].derivatives(deriv_cost_activ, activ_l1);
		deriv_l1 = this->layer[1].derivatives(deriv_l2.derivative_activation,
			activ_l0);
		deriv_l0 = this->layer[0].derivatives(deriv_l1.derivative_activation,
			input);
		
		/* average derivatives layer 2 */
		for (unsigned neuron = 0; neuron < deriv_l2.n_output; ++neuron)
		{
			for (unsigned input = 0; input < deriv_l2.n_input; ++input)
				deriv.layer[2].weight[neuron][input] +=
				deriv_l2.weight[neuron][input];
			deriv.layer[2].bias[neuron] += deriv_l2.bias[neuron];
		}
		for (unsigned input = 0; input < deriv_l2.n_input; ++input)
			deriv.layer[2].derivative_activation[input] +=
			deriv_l2.derivative_activation[input];

		/* average derivatives layer 1 */
		for (unsigned neuron = 0; neuron < deriv_l1.n_output; ++neuron)
		{
			for (unsigned input = 0; input < deriv_l1.n_input; ++input)
				deriv.layer[1].weight[neuron][input] +=
				deriv_l1.weight[neuron][input];
			deriv.layer[1].bias[neuron] += deriv_l1.bias[neuron];
		}
		for (unsigned input = 0; input < deriv_l1.n_input; ++input)
			deriv.layer[1].derivative_activation[input] +=
			deriv_l1.derivative_activation[input];

		/* average derivatives layer 0 */
		for (unsigned neuron = 0; neuron < deriv_l0.n_output; ++neuron)
		{
			for (unsigned input = 0; input < deriv_l0.n_input; ++input)
				deriv.layer[0].weight[neuron][input] +=
				deriv_l0.weight[neuron][input];
			deriv.layer[0].bias[neuron] += deriv_l0.bias[neuron];
		}
		for (unsigned input = 0; input < deriv_l0.n_input; ++input)
			deriv.layer[0].derivative_activation[input] +=
			deriv_l0.derivative_activation[input];
	}


	/* apply derivatives layer 2 */
	for (unsigned neuron = 0; neuron < deriv.layer[2].n_output; ++neuron)
	{
		for (unsigned input = 0; input < deriv.layer[2].n_input; ++input)
		{
			deriv.layer[2].weight[neuron][input] /= (real_t)train.size();
			this->layer[2].weight[neuron][input] -=
			deriv.layer[2].weight[neuron][input] * LEARNING_RATE;
		}
		deriv.layer[2].bias[neuron] /= (real_t)train.size();
		this->layer[2].bias[neuron] -= deriv.layer[2].bias[neuron] *
		LEARNING_RATE;
	}

	/* apply derivatives layer 1 */
	for (unsigned neuron = 0; neuron < deriv.layer[1].n_output; ++neuron)
	{
		for (unsigned input = 0; input < deriv.layer[1].n_input; ++input)
		{
			deriv.layer[1].weight[neuron][input] /= (real_t)train.size();
			this->layer[1].weight[neuron][input] -=
			deriv.layer[1].weight[neuron][input] * LEARNING_RATE;
		}
		deriv.layer[1].bias[neuron] /= (real_t)train.size();
		this->layer[1].bias[neuron] -= deriv.layer[1].bias[neuron] *
		LEARNING_RATE;
	}

	/* apply derivatives layer 0 */
	for (unsigned neuron = 0; neuron < deriv.layer[0].n_output; ++neuron)
	{
		for (unsigned input = 0; input < deriv.layer[0].n_input; ++input)
		{
			deriv.layer[0].weight[neuron][input] /= (real_t)train.size();
			this->layer[0].weight[neuron][input] -=
			deriv.layer[0].weight[neuron][input] * LEARNING_RATE;
		}
		deriv.layer[0].bias[neuron] /= (real_t)train.size();
		this->layer[0].bias[neuron] -= deriv.layer[0].bias[neuron] *
		LEARNING_RATE;
	}
}

/* ########################################################################## */

void	Network::export_file(const std::string &file) const
{
	std::ofstream fs;

	fs.open(file);
	if (!fs.is_open())
	{
		std::cout << "You dumb fucking cretin, you fucking fool, absolute fucki"
		"ng buffoon, you bumbling idiot. Fuck you." << std::endl;
		exit(0);
	}

	for (const Layer &layers : this->layer)
	{
		for (const Vector &weights_neuron : layers.weight)
			for (const real_t &weight : weights_neuron)
				fs << weight << ' ';
		for (const real_t &bias : layers.bias)
			fs << bias << ' ';
	}
	fs.close();
}

void	Network::import_file(const std::string &file)
{
		std::ifstream fs;

	fs.open(file);
	if (!fs.is_open())
	{
		std::cout << "Impossible to open [" << file << ']' << std::endl;
		exit(0);
	}

	for (Layer &layers : this->layer)
	{
		for (Vector &weights_neuron : layers.weight)
			for (real_t &weight : weights_neuron)
				fs >> weight;
		for (real_t &bias : layers.bias)
			fs >> bias;
	}
	fs.close();
}

/* ########################################################################## */

Network	&Network::operator=(const Network &rhs)
{
	this->layer[0] = rhs.layer[0];
	this->layer[1] = rhs.layer[1];
	this->layer[2] = rhs.layer[2];
	return (*this);
}
