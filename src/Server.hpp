/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 09:46:46 by rarraji           #+#    #+#             */
/*   Updated: 2024/02/24 19:53:24 by rarraji          ###   ########.fr       */
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

#define PORT_1  8006  // le port de notre premier serveur
#define PORT_2  8007  // le port de notre deuxième serveur
#define PORT_3  8008  // le port de notre troisième serveur

// paramètre req
typedef struct param_req
{
  std::string path;
  std::string version_http;
  std::string ip;
  std::string port;
} param_req;

class Server {
public:
    Server();
    ~Server();
    void run();

private:
    int create_server_socket(int port);
    void accept_new_connection(int listener_socket, fd_set &read_fds, int &fd_max);
    void read_data_from_socket(int socket, fd_set &read_fds, int &fd_max, int server_socket, fd_set &write_fds);
    void parse_req(std::string buffer);

    int server_socket_1, server_socket_2, server_socket_3;
};

#endif // SERVER_HPP