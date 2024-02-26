/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 15:53:48 by rarraji           #+#    #+#             */
/*   Updated: 2024/02/26 10:10:28 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() 
{
    server_socket_1 = create_server_socket(PORT_1);
    server_socket_2 = create_server_socket(PORT_2);
    server_socket_3 = create_server_socket(PORT_3);
}

int Server::create_server_socket(int port) 
{
    struct sockaddr_in sa;
    int socket_fd;
    int status;

    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    sa.sin_port = htons(port);

    socket_fd = socket(AF_INET, SOCK_STREAM,   0);
    if (socket_fd == -1) 
    {
        std::cerr << "[Server] Socket error: " << strerror(errno) << std::endl;
        return (-1);
    }
    std::cout << "[Server] Created server socket fd: " << socket_fd << std::endl;

    status = bind(socket_fd, (struct sockaddr *)&sa, sizeof sa);
    if (status !=   0) 
    {
        std::cerr << "[Server] Bind error: " << strerror(errno) << std::endl;
        return (-1);
    }
    std::cout << "[Server] Bound socket to localhost port " << port << std::endl;

    status = listen(socket_fd,   10);
    if (status !=   0) 
    {
        std::cerr << "[Server] Listen error: " << strerror(errno) << std::endl;
        return (-1);
    }
    std::cout << "[Server] Listening on port " << port << std::endl;

    return (socket_fd);
}

void Server::accept_new_connection(int listener_socket, fd_set &read_fds, int *fd_max) 
{
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    client_fd = accept(listener_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd == -1) 
    {
        std::cerr << "[Server] Accept error: " << strerror(errno) << std::endl;
        return;
    }
    FD_SET(client_fd, &read_fds);
    if (client_fd > *fd_max) 
    {
        *fd_max = client_fd;
    }
    std::cout << "[Server] Accepted new connection on client socket " << client_fd << ".\n";
}

void Server::read_data_from_socket(int socket, fd_set &read_fds, fd_set &write_fds) 
{
    char buffer[BUFSIZ];
    int bytes_read;

    memset(&buffer, '\0', sizeof buffer);
    bytes_read = recv(socket, buffer, BUFSIZ,   0);
    std::cout << "---------------------------------------------------------\n" << std::endl;
    std::cout << buffer << std::endl;
    std::cout << "---------------------------------------------------------\n" << std::endl;
    parse_req(buffer);
    if (bytes_read <=   0) 
    {
        if (bytes_read ==   0) 
        {
            std::cout << "[Server] Client socket closed connection.\n";
        } else 
        {
            std::cerr << "[Server] Recv error: " << strerror(errno) << std::endl;
        }
        close(socket);
    }
    FD_CLR(socket, &read_fds);
    FD_SET(socket, &write_fds);
}

void Server::parse_req(std::string buffer) 
{
    std::stringstream ss(buffer);
    std::string str;
    int i =   0;
    int cnt =   0;
    // param_req param_req;

    while(getline(ss, str, '\n')) 
    {
        std::map<std::string, std::string> MyMap;
        int j = str.find(' ');
        std::string key;
        std::string value;
        key = str.substr(0,j);
        value = str.substr(j,str.length());
        MyMap[key] = value;
        if (i ==   0) 
        {
            std::stringstream ss(value);
            std::string buffer;
            while(getline(ss, buffer, ' ')) 
            {
                if (cnt ==   1)
                    param_req.path = buffer;
                if (cnt ==   2)
                    param_req.version_http = buffer;
                cnt++;
            }
        }
        if (i ==   1) 
        {
            cnt =   0;
            std::stringstream ss(value);
            std::string buffer;
            getline(ss, buffer, ' ');
            while(getline(ss, buffer, ':')) 
            {
                if (cnt ==   0)
                    param_req.ip = buffer;
                if (cnt ==   1)
                    param_req.port = buffer;
                cnt++;
            }
        }
        if(i ==   7)
            break;
        i++;
    }
    std::cout << "ip  : " << param_req.ip << std::endl << "port  : " << param_req.port << std::endl;
    std::cout << "path  : " << param_req.path << std::endl << "version_http  : " << param_req.version_http << std::endl;
}

void Server::run() 
{
    fd_set read_fds;
    fd_set write_fds;
    int fd_max =  0;
    struct timeval timeout;

    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);

    FD_SET(server_socket_1, &read_fds);
    FD_SET(server_socket_2, &read_fds);
    FD_SET(server_socket_3, &read_fds);

    // fd_max = std::max({server_socket_1, server_socket_2, server_socket_3});
    fd_max = std::max(server_socket_1, std::max(server_socket_2, server_socket_3));
    std::cout << "[Server] Set up select fd sets\n";

    while (true) 
    {
        fd_set copy_read_fds = read_fds;
        fd_set copy_write_fds = write_fds;

        timeout.tv_sec =  2;
        timeout.tv_usec =  0;

        int status = select(fd_max +  1, &copy_read_fds, &copy_write_fds, NULL, &timeout);

        if (status == -1) 
        {
            std::cerr << "[Server] Select error: " << strerror(errno) << std::endl;
            return;
        } else if (status ==  0) {
            std::cout << "[Server] Waiting...\n";
            continue;
        }

        for (int i =  0; i <= fd_max; i++) {
            if (FD_ISSET(i, &copy_read_fds)) {
                if (i == server_socket_1 || i == server_socket_2 || i == server_socket_3) {
                    accept_new_connection(i, copy_read_fds, &fd_max);
                } else {
                    read_data_from_socket(i, copy_read_fds, copy_write_fds);
                }
            }
            if (FD_ISSET(i, &copy_write_fds)) 
            {
                // std::cout << "---->" << " ip  : " << param_req.ip << std::endl << "port  : " << param_req.port << std::endl;
                std::string test = "/";
                std::string test1 = "/favicon.ico";
                std::string path = "./pages";
                std::string new_path = "./pages";
                
                if (param_req.path.compare(test) == 0 || param_req.path.compare(test1) == 0)
                    param_req.path = "/index";
                new_path = path + param_req.path + ".html";
                std::cout << new_path << std::endl;
                // // Envoi d'un message de bienvenue au client
                // const char *welcome_message = "HTTP/1.1  200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, World!</h1></body></html>";
                // send(i, welcome_message, strlen(welcome_message),  0);
                std::ifstream file(new_path);
                if (!file.is_open()) {
                    std::cerr << "[Server] Impossible d'ouvrir le fichier '" << "\n";
                    return;
                }

                // // Envoi du contenu du fichier via la socket
                std::stringstream buffer;
                buffer << file.rdbuf();
                file.close();

                // Envoi du contenu du fichier via la socket
                std::string response = buffer.str();
                send(i, response.c_str(), response.size(),   0);
                close(i);
            }
        }
    }
}
