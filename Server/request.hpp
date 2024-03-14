/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 09:55:24 by rarraji           #+#    #+#             */
/*   Updated: 2024/03/13 17:26:15 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <utility>
#include <cstdio>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <vector> // Pour gérer plusieurs sockets
#include <map>   
#include <fstream> 
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <thread>


typedef std::map<std::string, std::string> LittleMap;
typedef std::map <int, std::map<std::string ,std::string> > MyMapy;

class Request{
  private:
    MyMapy my_map;
    std::string request;
    char buffer[2048];
    std::string header;
    std::string body;
    int body_lenght;
    bool chanked;
    bool Get;
    bool check;
    bool ContentLength;
    int compareLenBody;
    int header_len;
  public:
    Request();
    int read_socket(int socket);
    void Check_read(int socket, fd_set &read_fds, fd_set &write_fds);
    void remplirMyMap(int socket);
    void addSocketModeWrite();
    void CheckMethodeReq(int socket);
    void calBodylenght();
    void AddHeaderReq(int valread);
    void AddHeaderBody();
    void CheckChunked();
    MyMapy::iterator beginMyMap();
    MyMapy::iterator endMyMap();
};

#endif