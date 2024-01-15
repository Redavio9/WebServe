/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parce.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 18:55:48 by rarraji           #+#    #+#             */
/*   Updated: 2024/01/13 18:55:21 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_HPP
#define PARSE_HPP


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>


class Parse
{
	private:
		std::vector<std::pair<std::string ,std::vector<std::string> > > data;
	public:
		Parse();	
		~Parse();	
		int read_file(char *av);	
	
};



#endif