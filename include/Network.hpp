/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Network.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/08 11:18:45 by pitriche          #+#    #+#             */
/*   Updated: 2021/05/20 15:02:23 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORK_HPP
# define NETWORK_HPP

# include <array>

# include "Layer.hpp"
# include "Types.hpp"
# include "DataPack.hpp"

# define HIDDEN_LAYER_1 21
# define HIDDEN_LAYER_2 16

struct Network
{
	std::array<Layer, 3> layer;

	Network(void);
	~Network(void);

	void					initialize(void);
	std::array<real_t, 2>	execute(const Tuple &input);

	void					learning_cycle(const DataPack &train);
};

#endif
