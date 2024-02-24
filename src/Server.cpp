/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 09:46:48 by rarraji           #+#    #+#             */
/*   Updated: 2024/02/24 09:44:45 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

MyServer::MyServer() : fd_max(0) 
{
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&copy_read_fds);
    FD_ZERO(&copy_write_fds);
}

void MyServer::set_copy_read_fds() 
{
    copy_read_fds = read_fds;
}

void MyServer::set_copy_write_fds() 
{
    copy_write_fds = write_fds;
}

fd_set* MyServer::get_copy_read_fds() 
{
    return &copy_read_fds;
}

fd_set* MyServer::get_copy_write_fds() 
{
    return &copy_write_fds;
}

void MyServer::SetFdToZero() 
{
    FD_ZERO(&copy_write_fds);
    FD_ZERO(&copy_read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&read_fds);
}

void MyServer::loopOverSockets(int server_socket_1, int server_socket_2, int server_socket_3) 
{
    fd_max = std::max(server_socket_1, std::max(server_socket_2, server_socket_3));
    for (int i = 0; i <= fd_max; i++) {
        if (FD_ISSET(i, &copy_read_fds)) {
            if (i == server_socket_1 || i == server_socket_2 || i == server_socket_3)
                accept_new_connection(i, &fd_max);
            else
                read_data_from_socket(i);
        }
        if (FD_ISSET(i, &copy_write_fds)) {
            const char *welcome_message = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, World!</h1></body></html>";
            send(i, welcome_message, strlen(welcome_message), 0);
            close(i);
        }
    }
}

int MyServer::create_server_socket(int port) {
    struct sockaddr_in sa;
    int socket_fd;
    int status;

    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    sa.sin_port = htons(port);

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        fprintf(stderr, "[Server] Socket error: %s\n", strerror(errno));
        return -1;
    }
    printf("[Server] Created server socket fd: %d\n", socket_fd);

    status = bind(socket_fd, (struct sockaddr *)&sa, sizeof sa);
    if (status != 0) {
        fprintf(stderr, "[Server] Bind error: %s\n", strerror(errno));
        return -1;
    }
    printf("[Server] Bound socket to localhost port %d\n", port);

    status = listen(socket_fd, 10);
    if (status != 0) {
        fprintf(stderr, "[Server] Listen error: %s\n", strerror(errno));
        return -1;
    }
    printf("[Server] Listening on port %d\n", port);

    FD_SET(socket_fd, &read_fds);
    fd_max = std::max(fd_max, socket_fd);
    return socket_fd;
}

void MyServer::accept_new_connection(int listener_socket, int *fd_max) {
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    client_fd = accept(listener_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd == -1) {
        fprintf(stderr, "[Server] Accept error: %s\n", strerror(errno));
        return;
    }
    FD_SET(client_fd, &read_fds);
    *fd_max = std::max(*fd_max, client_fd);
    printf("[Server] Accepted new connection on client socket %d.\n", client_fd);
}

void MyServer::read_data_from_socket(int socket) {
    char buffer[BUFSIZ];
    int bytes_read;

    memset(&buffer, '\0', sizeof buffer);
    bytes_read = recv(socket, buffer, BUFSIZ, 0);
    std::cout << "---------------------------------------------------------\n" << std::endl;
    std::cout << buffer << std::endl;
    std::cout << "---------------------------------------------------------\n" << std::endl;
    parse_req(buffer);
    if (bytes_read <= 0) {
        if (bytes_read == 0)
            printf("[%d] Client socket closed connection.\n", socket);
        else
            fprintf(stderr, "[Server] Recv error: %s\n", strerror(errno));
        close(socket);
    }
    FD_CLR(socket, &read_fds);
    FD_SET(socket, &write_fds);
}

void MyServer::parse_req(std::string buffer) {
    std::stringstream ss(buffer);
    std::string str;
    int i = 0;
    int cnt = 0;
  
    while(getline(ss, str, '\n')) {
        int j = str.find(' ');
        std::string key = str.substr(0, j);
        std::string value = str.substr(j, str.length());
        MyMap[key] = value;
        if (i == 0) {
            std::stringstream ss(value);
            std::string buffer;
            while(getline(ss, buffer, ' ')) {
                if (cnt == 1)
                    param_req.path = buffer;
                if (cnt == 2)
                    param_req.version_http = buffer;
                cnt++;
            }
        }
        if (i == 1) {
            cnt = 0;
            std::stringstream ss(value);
            std::string buffer;
            getline(ss, buffer, ' ');
            while(getline(ss, buffer, ':')) {
                if (cnt == 0)
                    param_req.ip = buffer;
                if (cnt == 1)
                    param_req.port = buffer;
                cnt++;
            }
        }
        if (i == 7)
            break;
        i++;
    }
    std::cout << "ip  : " << param_req.ip << std::endl << "port  : " << param_req.port << std::endl;
    std::cout << "path  : " << param_req.path << std::endl << "version_http  : " << param_req.version_http << std::endl;
}