/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Network.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/08 11:18:45 by pitriche          #+#    #+#             */
/*   Updated: 2021/06/17 09:58:29 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORK_HPP
# define NETWORK_HPP

# include <array>

# include "Layer.hpp"
# include "Types.hpp"
# include "DataPack.hpp"


struct Network
{
	std::array<Layer, 3> layer;

	Network(void);
	~Network(void);

	void					initialize(void);
	std::array<real_t, 2>	execute(const Tuple &input) const;

	real_t					cost(const DataPack &test) const;
	void					learning_cycle_regular(const DataPack &train);
	void					export_file(const std::string &file) const;
	void					import_file(const std::string &file);

	Network					&operator=(const Network &rhs);
};

#endif
