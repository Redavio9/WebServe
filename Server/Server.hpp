/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 15:53:14 by rarraji           #+#    #+#             */
/*   Updated: 2024/03/15 02:11:33 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

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
#include "request.hpp"

#define PORT_3   8005  // le port de notre troisième serveur
#define PORT_1   8004  // le port de notre premier serveur
#define PORT_2   8002  // le port de notre deuxième serveur

// typedef std::map<std::string, std::string> LittleMap;
// typedef std::map <int, std::map<std::string ,std::string> > MyMapy;



// paramètre req
typedef struct param_req
{
  std::string path;
  std::string version_http;
  std::string ip;
  std::string port;
  std::string methode;
  
} param_req;
// typedef struct client_fd
// {
//   std::string request;
//   std::string header;
//   std::string body;
//   // std::string port;
//   // std::string methode;
// } param_req;

class Server 
{
  private:
    Request request;
    int server_socket_1;
    int server_socket_2;
    int server_socket_3;
    std::string body;
    param_req param_req_one;
    int create_server_socket(int port);
    void accept_new_connection(int listener_socket, fd_set &read_fds, int *fd_max);
    void read_data_from_socket(int socket, fd_set &read_fds, fd_set &write_fds);
    void parse_req(std::string buffer);
    void readLine(int socket, fd_set &read_fds, fd_set &write_fds);
    void readBody(int socket, std::string& body);
    
  public:
    Server();
    void run();
    
};

#endif // SERVER_HPP
