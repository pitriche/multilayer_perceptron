/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DataPack.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 16:04:13 by pitriche          #+#    #+#             */
/*   Updated: 2021/06/17 10:53:56 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATAPACK_HPP
# define DATAPACK_HPP

# include <iostream>
# include <sstream>
# include <vector>
# include <array>
# include <string>

# include "Types.hpp"

struct DataPack : std::vector<Tuple>
{
	public:
		/* parse input stream to gather the data */
		void					parse(std::istream &is);

		/* normalize the data */
		void					normalize(void);

		/* split the data in 2 sets randomly */
		DataPack				split(double ratio);

		/* split the data in n sets randomly */
		std::vector<DataPack>	batch(unsigned n);
};

#endif
