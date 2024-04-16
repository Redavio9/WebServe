/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 09:55:27 by rarraji           #+#    #+#             */
/*   Updated: 2024/04/16 12:09:10 by rarraji          ###   ########.fr       */
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
  if ((pos = request.find("\r\n\r\n")) != std::string::npos && !check)
  {
    check = true;
    compareLenBody = valread - pos + 4;
    header = request;
  }
  else
    compareLenBody += valread;
}

void Request::CheckChunked()
{
  size_t pos;
  if (header.find("chunked") != std::string::npos)
    chunked = true;
  if ((pos = body.find("\r\n\r\n")) != std::string::npos)
  {
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
  body = body.substr(nb + 2 , body.length());
}

void Request::AddHeaderBody()
{
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
      body = body.substr(pos + 14 , body.length());
      body = body.substr(body.find("\n") + 1, body.length());
      body = body.substr(body.find("\n") + 1, body.length());
      body = body.substr(body.find("\n") + 1, body.length());
    }
  
    int nb = 0;
    while(true && chunked)
    {
      std::stringstream ss(body);
      getline(ss, num_one, '\r');
      if(hexStringToDecimal(num_one) == 0)
        break;
      nb =  hexStringToDecimal(num_one);
      body = body.substr(body.find("\n") + 1, body.length());
      new_body += body.substr(start, nb);
      RegContent(nb);
      start = 0;
    }
  }
  if (!Get)
  {
    new_body = body;
    if(chunked)
    {
      int poss = new_body.find("----------------------------");
        new_body = new_body.substr(0, poss);
    } 
  }
}

void Request::check_req_valid()
{
  size_t pos;
  if ((pos = header.find("\r\n\r\n")) != std::string::npos)
  {
    header = header.substr(0, pos + 2);
  }  
  std::cout << "\033[0;31m" << "*******************************HEADER*************************************" << "\033[0m" << std::endl;
  std::cout << "\033[0;31m" << header << "\033[0m" << std::endl;
  std::cout << "\033[0;31m" << "**************************************************************************" << "\033[0m" << std::endl;
  // read line par line 
  std::stringstream ss(header);
  std::string buf;
  int i = 0;
  while(getline(ss, buf, '\n'))
  {
    std::cout << "--->" << buf << std::endl;
    //check line 1
    if (i == 0)
    {
      std::stringstream ss(buf);
      std::string buff;
      int j = 0;
      while (getline(ss, buff, ' '))
      {
        if (j == 0)
        {
          if(buff.compare("GET") != 0 && buff.compare("POST") != 0 && buff.compare("DELETE") == 0)
          {
            //error    
          }
        }
        if (j == 1)
        {
          //check URL !!!!!!
        }
        if (j == 2)
        {
          if (buff.compare("HTTP/1.1") != 0)
          {
            // error
          }
        }  
        if (j == 3)
        {
          // error;
        }
        j++;
      }
    }
    // check lines 2 ...
    i++;
  }
}

void Request::Check_read(int socket, fd_set &read_fds, fd_set &write_fds)
{
  int valread;
  valread = read_socket(socket);
  size_t pos = 0;
  // headre req
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
      check_req_valid();
  }
  // body req
  if ((request.find("\r\n\r\n") != std::string::npos && Get) || (request.find("\r\n\r\n0") != std::string::npos && !Get) || compareLenBody >= body_lenght)
  {
      std::cout << "\033[0;35m" << "---------->>>>>ALL-REQUSTE<<<<<--------" << "\033[0m" << std::endl;
      if(!Get)
        body = request.substr(header_len, request.length());
      AddHeaderBody();
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