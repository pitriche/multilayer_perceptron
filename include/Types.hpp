/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Types.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/09 10:53:26 by pitriche          #+#    #+#             */
/*   Updated: 2021/05/21 17:08:19 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REAL_T_HPP
# define REAL_T_HPP

# include <array>
# include <vector>

# define TUPLE_SIZE 31

typedef	float	real_t;	// real type to use everywhere

typedef std::array<real_t, TUPLE_SIZE>		Tuple;
typedef std::vector<real_t>					Vector;

#endif
