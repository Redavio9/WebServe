/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 15:53:14 by rarraji           #+#    #+#             */
/*   Updated: 2024/02/26 09:59:21 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdlib>
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

#define PORT_1   8006  // le port de notre premier serveur
#define PORT_2   8007  // le port de notre deuxième serveur
#define PORT_3   8008  // le port de notre troisième serveur

// paramètre req
typedef struct param_req
{
  std::string path;
  std::string version_http;
  std::string ip;
  std::string port;
} param_req;

class Server 
{
  private:
    int server_socket_1;
    int server_socket_2;
    int server_socket_3;
    param_req param_req;
    int create_server_socket(int port);
    void accept_new_connection(int listener_socket, fd_set &read_fds, int *fd_max);
    void read_data_from_socket(int socket, fd_set &read_fds, fd_set &write_fds);
    void parse_req(std::string buffer);
    
  public:
    Server();
    void run();
};

#endif // SERVER_HPP
