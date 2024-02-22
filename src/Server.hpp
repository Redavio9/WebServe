/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 09:46:46 by rarraji           #+#    #+#             */
/*   Updated: 2024/02/22 10:41:14 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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


#define PORT_1 8006  // le port de notre premier serveur
#define PORT_2 8007  // le port de notre deuxième serveur
#define PORT_3 8008  // le port de notre troisième serveur

typedef struct param_req
{
  std::string path;
  std::string version_http;
  std::string ip;
  std::string port;
} param_req;

class MyServer
{
  private:
    std::map<std::string, std::string> MyMap;
    param_req param_req;
    std::string buffer;
    fd_set read_fds;    // Ensemble read pour select()
    fd_set write_fds;    // Ensemble write pour select()
    fd_set copy_read_fds;    // Ensemble copy_write pour select()
    fd_set copy_write_fds;    // Ensemble copy_write pour select()
    int fd_max;
  public:

    void SetFdToZero();
    void loopOverSockets(int server_socket_1, int server_socket_2, int server_socket_3, int fd_max);
    // Déclaration des fonctions
    int create_server_socket(int port);
    // void accept_new_connection(int listener_socket, fd_set *all_sockets, int *fd_max);
    void accept_new_connection(int listener_socket, fd_set &read_fds, int *fd_max);
    // void read_data_from_socket(int socket, fd_set *all_sockets, int *fd_max, int server_socket);
    void read_data_from_socket(int socket, fd_set &read_fds, fd_set &write_fds);
    // parse req
    void parse_req(std::string buffer);

    //set_ copy_read_fds
    void set_copy_read_fds();
    void set_copy_write_fds();

    // geters
    std::string get_ip();
    std::string get_port();
    std::string get_version_http();
    std::string get_path();
    fd_set get_copy_read_fds();
    fd_set get_copy_write_fds();
};