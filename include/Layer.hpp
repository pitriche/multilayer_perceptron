/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Layer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/08 11:18:45 by pitriche          #+#    #+#             */
/*   Updated: 2021/06/17 10:54:33 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LAYER_HPP
# define LAYER_HPP

# include <vector>
# include "Types.hpp"

struct Layer
{
	std::vector<Vector>	weight;
	Vector				bias;
	const unsigned		n_input;
	const unsigned		n_output;

	/* for learning, derivative of the cost from this layer's activations */
	Vector				derivative_activation;

	Layer(void);
	Layer(unsigned input, unsigned output);
	~Layer(void);

	void	initialize(void);
	Vector	execute(const Vector &input) const;
	Layer	derivatives(const Vector &deriv_cost_activ, const Vector &input) const;

	Layer	&operator=(const Layer &rhs);
};

#endif
