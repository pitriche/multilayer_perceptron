/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Layer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/08 11:18:45 by pitriche          #+#    #+#             */
/*   Updated: 2021/05/12 14:03:53 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LAYER_HPP
# define LAYER_HPP

# include <vector>
# include "Types.hpp"

struct Layer
{
	std::vector<std::vector<real_t>>	weight;
	std::vector<real_t>					bias;
	const unsigned						n_input;
	const unsigned						n_output;

	Layer(void);
	Layer(unsigned input, unsigned output);
	~Layer(void);

	void				initialize(void);
	std::vector<real_t>	execute(std::vector<real_t> input);

	Layer	&operator=(const Layer &rhs);
};

#endif
