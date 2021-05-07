/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DataPack.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunomartin <brunomartin@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 16:04:13 by pitriche          #+#    #+#             */
/*   Updated: 2021/05/07 15:03:34 by brunomartin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <vector>
#include <array>
#include <string>

// #include <cmath>
// #include <cstdlib>

typedef	float real_t;	// real type to use everywhere

#define TUPLE_SIZE 31

typedef std::array<real_t, TUPLE_SIZE>	Tuple;

struct DataPack : std::vector<Tuple>
{
	public:
		// parse input stream to gather the data
		void		parse(std::istream &is);

		// normalize the data
		void		normalize(void);

		// split the data in 2 sets randomly
		DataPack	split(double ratio);
};
