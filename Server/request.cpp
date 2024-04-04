/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 09:55:27 by rarraji           #+#    #+#             */
/*   Updated: 2024/03/23 17:55:37 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

Request::Request()
{
  compareLenBody = 0;
  Get = false;
  last = true;
  start = 0;
}


int Request::read_socket(int socket)
{
    int valread;
    fcntl(socket, F_SETFL, O_NONBLOCK , FD_CLOEXEC);
    valread = read(socket, buffer, sizeof(buffer) - 1);  
    return (valread);
}

void Request::remplirMyMap(int socket)
{
  LittleMap new_map;
  if (request.find("GET") != std::string::npos)
      new_map[request] = "GET";
  else
      new_map[request] = "POST";    
  my_map[socket] =  new_map;
}


void Request::CheckMethodeReq(int socket)
{
  for (MyMapy::iterator it = my_map.begin(); it != my_map.end(); it++)
  {
    if(it->first == socket)
    {
      LittleMap::iterator it1 = it->second.begin();
      if (it1->second == "GET")
      {
        Get = true;
        // std::cout << "GET" << std::endl;
      }
      else;
        // std::cout << "POST" << std::endl;  
    }
  }
}

void Request::AddHeaderReq(int valread)
{
  size_t pos;
  // std::cout << check << std::endl;
  if ((pos = request.find("\r\n\r\n")) != std::string::npos && !check)
  {
    check = true;
    compareLenBody = valread - pos + 4;
    // std::cout << "--------------------->" << "compareLenBody : " << compareLenBody << std::endl;
    header = request;
  }
  else
    compareLenBody += valread;
}

void Request::CheckChunked()
{
  // bool chunked = false;
  size_t pos;
  if (header.find("chunked") != std::string::npos)
    chunked = true;
  if ((pos = body.find("\r\n\r\n")) != std::string::npos)
  {
    // std::cout << "->>>>>>>>>>hereeeeeeeeeeee\n";
    body = body.substr(pos + 4 , body.length());
  }
}

int hexStringToDecimal(const std::string& hexString) 
{
    std::stringstream ss;
    ss << std::hex << hexString; // Utiliser std::hex pour indiquer que la chaîne est hexadécimale
    int decimalValue;
    ss >> decimalValue; // Convertit la chaîne hexadécimale en décimal
    return decimalValue;
}


void Request::RegContent(int nb)
{
  std::string newb;

  // std::stringstream ss(body);
  // getline(ss, newb , '\n');
  body = body.substr(nb + 2 , body.length());
  // std::cout << body;
  // start = nb + 2;
    // const char* filename = body.c_str();
    // int fd = open(filename, O_RDONLY);
    // char buffer[nb + 1]; // Buffer pour stocker les données lues
    // ssize_t bytesRead;
    // bytesRead = read(fd, buffer, sizeof(buffer) - 1);
    //     buffer[bytesRead] = '\0';
    // std::cout << buffer << std::endl;    
    // close(fd);
    // return(buffer);
}

void Request::AddHeaderBody()
{
  // std::cout << "----->hereeeeee !!\n";
  size_t pos;
  std::string num_one;
  std::string num_one_2;
  if ((pos = header.find("\r\n\r\n")) != std::string::npos)
  {
    header = header.substr(0, pos + 2);
  }
  if(!Get)
  {
    CheckChunked();
    std::cout << body << std::endl;
    if ((pos = body.find("Content-Type: ")) != std::string::npos)
    {
      std::cout << "hereeeeeeeeeeeeeeeeeeeeeeeeeeee\n";
      body = body.substr(pos + 14 , body.length());
      body = body.substr(body.find("\n") + 1, body.length());
      body = body.substr(body.find("\n") + 1, body.length());
      body = body.substr(body.find("\n") + 1, body.length());
      // body = body.substr(body.find("\n"), body.length());
      // int poss = body.find("----------------------------");
      // body = body.substr(0, poss);
    }
    // num_one_2 = body.substr(num_one_2.length() + 2, body.find("\n") - 2);
  
    int nb = 0;
    while(true && chunked)
    {
      std::stringstream ss(body);
      getline(ss, num_one, '\r');
      // std::cout << num_one << std::endl;
      if(hexStringToDecimal(num_one) == 0)
        break;
      nb =  hexStringToDecimal(num_one);
      // std::cout << "nb :" << nb << std::endl;
      // std::cout << "body_lenght :" << body.length() << std::endl;
      body = body.substr(body.find("\n") + 1, body.length());
      // std::cout << "start :" << start << std::endl;
      // std::cout << "-------------------\n";
      // std::cout << body.substr(start, nb);
      // std::cout << "-------------------\n";
      new_body += body.substr(start, nb);
      RegContent(nb);
      // body = body.substr(nb + 1, body.length());
      // std::cout << "\033[0;36m" << "----->"  << nb << "\033[0m" << std::endl;
      start = 0;
    }
  }
  if (Get)
  {
    new_body = body;
    // std::cout << "\033[0;32m" << "-------------------------------------------------------" << "\033[0m" << std::endl ;
    // std::cout  << new_body << std::endl;
    // std::cout << "\033[0;32m" << "-------------------------------------------------------" << "\033[0m" << std::endl;
    int poss = new_body.find("----------------------------");
      new_body = new_body.substr(0, poss);
  }
}

void Request::Check_read(int socket, fd_set &read_fds, fd_set &write_fds)
{
  int valread;
  valread = read_socket(socket);
  size_t pos = 0;
  if (valread > 0) 
  {
      buffer[valread] = '\0';
      request.append(buffer, valread);
      remplirMyMap(socket);
      CheckMethodeReq(socket);
      AddHeaderReq(valread);
      if ((pos = request.find("Content-Length: ")) != std::string::npos)
      {
        std::cout << "\033[0;35m"  << "========CHUNKED========" << "\033[0m" << std::endl;
        body_lenght = std::atoi(request.substr(pos + 16,  request.find("\r\n", pos + 16) - pos + 16).c_str());
        header_len = pos + 16;
      }
  }
  // std::cout << "body_lenght :" << body_lenght << std::endl;
  // std::cout << "GET :" << Get << std::endl;
  // std::cout << "compareLenBod : " << compareLenBody << std::endl;
  if ((request.find("\r\n\r\n") != std::string::npos && Get) || (request.find("\r\n\r\n0") != std::string::npos && !Get) || compareLenBody >= body_lenght)
  {
      std::cout << "\033[0;35m" << "---------->>>>>ALL-REQUSTE<<<<<--------" << "\033[0m" << std::endl;
      // std::cout << "\033[0;35m"  <<  request.length() << "\033[0m" << std::endl;
      if(!Get)
        body = request.substr(header_len, request.length());
      AddHeaderBody();
      if ((pos = header.find("\r\n\r\n")) != std::string::npos)
      {
        header = header.substr(0, pos + 2);
      }  
      std::cout << "\033[0;31m" << "*******************************HEADER*************************************" << "\033[0m" << std::endl;
      std::cout << "\033[0;31m" << header << "\033[0m" << std::endl;
      std::cout << "\033[0;31m" << "**************************************************************************" << "\033[0m" << std::endl;
      std::cout << "\033[0;33m" << "********************************BODY*************************************" << "\033[0m" << std::endl;
      std::cout << "\033[0;33m" << new_body << "\033[0m" << std::endl;
      std::cout << "\033[0;33m" << "**************************************************************************" << "\033[0m" << std::endl;
      request = "";
      FD_CLR(socket, &read_fds);
      FD_SET(socket, &write_fds);
      compareLenBody = 0;
      body_lenght = 0;
      Get = false;
      check = false;
  }
}




MyMapy::iterator Request::beginMyMap() 
{
  return my_map.begin();
}

MyMapy::iterator Request::endMyMap() {
    return my_map.end();
}



// Transfer-Encoding