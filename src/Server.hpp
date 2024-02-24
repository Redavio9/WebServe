/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 09:46:46 by rarraji           #+#    #+#             */
/*   Updated: 2024/02/24 12:06:46 by rarraji          ###   ########.fr       */
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
#include <vector>
#include <map>

#define PORT_1 8006
#define PORT_2 8007
#define PORT_3 8008

typedef struct param_req {
    std::string path;
    std::string version_http;
    std::string ip;
    std::string port;
} param_req;

class MyServer {
private:
    std::map<std::string, std::string> MyMap;
    param_req param_req_one;
    std::string buffer;
    int fd_max;

public:
    fd_set read_fds;
    fd_set write_fds;
    fd_set copy_read_fds;
    fd_set copy_write_fds;
    MyServer();
    void SetFdToZero();
    void loopOverSockets(int server_socket_1, int server_socket_2, int server_socket_3);
    int create_server_socket(int port);
    void accept_new_connection(int listener_socket);
    void read_data_from_socket(int socket);
    void parse_req(std::string buffer);
    int &get_fdMax();
    void set_copy_read_fds();
    void set_copy_write_fds();
    void SetFdMax(int server_socket_1, int server_socket_2, int server_socket_3);
    fd_set* get_copy_read_fds();
    fd_set* get_copy_write_fds();
};
