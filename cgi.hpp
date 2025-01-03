/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 15:35:09 by rarraji           #+#    #+#             */
/*   Updated: 2024/06/02 11:17:57 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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
#include <sys/stat.h>

class Cgi
{
  private:
    std::string header;
    std::string body;
  public:
    Cgi();
    ~Cgi();
    std::string url;
    std::string root;
    std::string querystingcgi;
    std::string methode;
    int time_out;
    int status_code_error;
    // Request request;
    void SetHeader(std::string header);
    void SetBody(std::string body);
    std::string GetBody();
    std::string GetHeader();
    void run();
};