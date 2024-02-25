/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 09:46:46 by rarraji           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/02/24 19:53:24 by rarraji          ###   ########.fr       */
=======
/*   Updated: 2024/02/24 12:06:46 by rarraji          ###   ########.fr       */
>>>>>>> af9e5fc45a9065a6872d43ba59663dcbccd1633f
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

<<<<<<< HEAD
class Server {
=======
class MyServer {
private:
    std::map<std::string, std::string> MyMap;
    param_req param_req_one;
    std::string buffer;
    int fd_max;

>>>>>>> af9e5fc45a9065a6872d43ba59663dcbccd1633f
public:
    Server();
    ~Server();
    void run();

private:
    int create_server_socket(int port);
<<<<<<< HEAD
    void accept_new_connection(int listener_socket, fd_set &read_fds, int &fd_max);
    void read_data_from_socket(int socket, fd_set &read_fds, int &fd_max, int server_socket, fd_set &write_fds);
    void parse_req(std::string buffer);

    int server_socket_1, server_socket_2, server_socket_3;
=======
    void accept_new_connection(int listener_socket);
    void read_data_from_socket(int socket);
    void parse_req(std::string buffer);
    int &get_fdMax();
    void set_copy_read_fds();
    void set_copy_write_fds();
    void SetFdMax(int server_socket_1, int server_socket_2, int server_socket_3);
    fd_set* get_copy_read_fds();
    fd_set* get_copy_write_fds();
>>>>>>> af9e5fc45a9065a6872d43ba59663dcbccd1633f
};

#endif // SERVER_HPP