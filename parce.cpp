/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parce.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 18:55:50 by rarraji           #+#    #+#             */
/*   Updated: 2024/01/13 19:40:39 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parce.hpp"

Parse::Parse(){};
Parse::~Parse(){};



std::string trimSpaces(const std::string &str) 
{
   std::string result;
   for (size_t i = 0; i < str.size(); ++i) 
   {
       if (!std::isspace(str[i])) 
       {
           result += str[i];
       }
   }
   return result;
}


int Parse::read_file(char *av)
{
  std::pair<std::string, std::vector<std::string> > firstPair;
  // std::pair<std::string, std::vector<std::string> > firstPair;
  std::ifstream file(av);
  std::string line;
  std::string buffer;
  int cnt = 0;
  int add = 0;
  int i = 0;

  if (!file.is_open())
  {
    std::cout << "Failed to open file." << std::endl;
    return (1);
  }
  while(std::getline(file, line))
  {
    std::stringstream ss(line);
    while (std::getline(ss, buffer, '='))
    {
      buffer = trimSpaces(buffer);
      if(std::strcmp(buffer.c_str(), "[[server]]") != 0 && std::strcmp(buffer.c_str(), "[[server.location]]") != 0)
      {
        firstPair.first = buffer;
        std::getline(ss, buffer, '\n');
        firstPair.second.push_back(buffer);
        data.push_back(firstPair);
        std::cout << data[i].first << std::endl;
        std::cout << data[i].second[i] << std::endl;
        std::cout << "-------------------------------------------\n" << std::endl;
        i++;
      }
      cnt++;
    }  
  }
  std::cout  << "NUMBER SERVER : " << cnt << std::endl;
  return(0);
}

std::vector<std::pair<std::string ,std::vector<std::string> > > data;