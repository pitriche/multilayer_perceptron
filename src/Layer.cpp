/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Layer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/08 13:18:45 by pitriche          #+#    #+#             */
/*   Updated: 2021/05/21 18:24:04 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>	// rand
#include <climits>	// INT_MAX
#include <cmath>	// tanh
#include "Layer.hpp"

/* ########################################################################## */

inline static real_t	_rand()
{ return (((real_t)std::rand() / (real_t)INT_MAX) * 2.0f - 1.0f); }

// inline static real_t	_sigmoid(real_t x)
// { return (1 / (1 + std::pow(M_E, -x))); }

inline static real_t	_tanh_deriv(real_t x)
{ return (1 - (real_t)std::pow(std::tanh(x), 2)); }

/* ########################################################################## */

Layer::Layer(void) : n_input(0), n_output(0) { }
Layer::Layer(unsigned input, unsigned output) : n_input(input),
	n_output(output)
{
	if (input == 0 || output == 0)
		throw (std::logic_error("Invalid layer size"));
	this->bias.resize(output, 0.0f);
	this->weight.resize(output, Vector(input, 0.0f));
}

Layer::~Layer(void) { }


void	Layer::initialize(void)
{
	for (real_t &n : this->bias)
		n = _rand();
	for (Vector &vec : this->weight)
		for (real_t &n : vec)
			n = _rand();

}

void	Layer::initialize_null(void)
{
	std::fill(this->bias.begin(), this->bias.end(), 0.0f);
	for (Vector &vec : this->weight)
		std::fill(vec.begin(), vec.end(), 0.0f);
}

Vector	Layer::execute(const Vector &input)
{
	Vector result;

	result.resize(this->n_output, 0.0f);
	for (unsigned neuron_id = 0; neuron_id < this->n_output; ++neuron_id)
	{
		for (unsigned input_id = 0; input_id < this->n_input; ++input_id)
			result[neuron_id] += input[input_id] *
			this->weight[neuron_id][input_id];
		result[neuron_id] += this->bias[neuron_id];
		result[neuron_id] = std::tanh(result[neuron_id]);	// activation function
	}
	return(result);
}

# include <iostream> //////////////////////////////
Layer	Layer::derivatives(const Vector &deriv_cost_activ, const Vector &input)
{
	Layer	deriv(this->n_input, this->n_output);
	real_t	weighted_sum;

	for (unsigned neuron_id = 0; neuron_id < this->n_output; ++neuron_id)
	{
		// compute weighted_sum
		weighted_sum = 0;
		for (unsigned input_id = 0; input_id < this->n_input; ++input_id)
			weighted_sum += input[input_id] * this->weight[neuron_id][input_id];
		weighted_sum += this->bias[neuron_id];

		// compute derivatives
		deriv.bias[neuron_id] = _tanh_deriv(weighted_sum) * deriv_cost_activ[neuron_id];
		for (unsigned input_id = 0; input_id < this->n_input; ++input_id)
			deriv.weight[neuron_id][input_id] = input[input_id] * deriv.bias[neuron_id];
	}
	return (deriv);
}


Layer	&Layer::operator=(const Layer &rhs)
{
	if (this->n_input != rhs.n_input || this->n_output != rhs.n_output)
		throw (std::logic_error("Invalid layer assignation: size differ"));
	this->weight = rhs.weight;
	this->bias = rhs.bias;
	return (*this);
}
