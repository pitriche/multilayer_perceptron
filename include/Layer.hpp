/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Layer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunomartin <brunomartin@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/08 11:18:45 by pitriche          #+#    #+#             */
/*   Updated: 2021/05/09 10:56:18 by brunomartin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LAYER_HPP
# define LAYER_HPP

# include <vector>
# include "real_t.hpp"

struct Layer
{
	std::vector<std::vector<real_t>>	weight;
	std::vector<real_t>					bias;
	const unsigned						n_input;
	const unsigned						n_output;

	Layer(unsigned input, unsigned output);
	~Layer(void);

	void	initialize(void);

	Layer	&operator=(const Layer &rhs);
};

#endif
