/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 15:53:48 by rarraji           #+#    #+#             */
/*   Updated: 2024/05/07 09:42:17 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <thread>

Server::Server() 
{
    s = 0;
    se = 0;
    server_socket_1 = create_server_socket(PORT_1);
    server_socket_2 = create_server_socket(PORT_2);
    server_socket_3 = create_server_socket(PORT_3);
    // int NumberServerSocker = 3;
    // for (size_t i = 0; i < NumberServerSocker; i++)
    // {
    //     int server_socket_1 = create_server_socket(serverSockets[i].port);
    // }
    
}



void Server::RemplirInfo(int socket_fd)
{
    infoserv info;
    // set root
    // set host
    // set port
    // set location
    // set error page
    // set index
    // set maxBodySize
    mapinfo.insert(std::make_pair(socket_fd, info));
}

// add ip addresse;
int Server::create_server_socket(int port) 
{
    struct sockaddr_in sa;
    int socket_fd;
    int status;

    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET; 
    sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);   //ip server
    sa.sin_addr.s_addr = inet_addr("0.0.0.0"); //change ip 0.0.0.0 to myip
    sa.sin_port = htons(port); // port server

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) 
    {
        std::cerr << "[Server] Socket error: " << strerror(errno) << std::endl;
        return (-1);
    }
    RemplirInfo(socket_fd);
    // mapinfo.insert(std::make_pair(socket_fd, info));
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
        *fd_max = client_fd;
    std::cout << "[Server] Accepted new connection on client socket " << client_fd << ".\n";
}

void Server::read_data_from_socket(int socket, fd_set &read_fds, fd_set &write_fds) 
{
    mapinfo[socket].request.Check_read(socket, read_fds, write_fds);
}


void Server::run() 
{
    fd_set read_fds;
    fd_set write_fds;
    int fd_max =  0;
    

    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);


    // boucler sur les socket
    // for (size_t i = 0; i < Sockets.size(); i++)
    // {
    //     FD_SET(Sockets[i], &read_fds);
    // }
    
    
    FD_SET(server_socket_1, &read_fds);
    FD_SET(server_socket_2, &read_fds);
    FD_SET(server_socket_3, &read_fds);

    //fd_max les socket 
    fd_max = std::max(server_socket_1, std::max(server_socket_2, server_socket_3));
    std::cout << "[Server] Set up select fd sets\n";

    while (true) 
    {
        fd_set copy_read_fds = read_fds;
        fd_set copy_write_fds = write_fds;

        int status = select(fd_max + 1, &copy_read_fds, &copy_write_fds, NULL, NULL);
        // std::cout << "test====>: " << status << std::endl;

        if (status == -1) 
        {
            std::cerr << "[Server] Select error: " << strerror(errno) << std::endl;
            return;
        } 
        // else if (status ==  0) 
        // {
        //     std::cout << "[Server] Waiting...\n";
        //     continue;
        // }

        // boucler number of socket 
        for (int i = 3; i <= fd_max; i++) 
        {
            struct stat sb;
            if (FD_ISSET(i, &copy_read_fds)) 
            {
                if (i == server_socket_1 || i == server_socket_2 || i == server_socket_3) 
                    accept_new_connection(i, read_fds, &fd_max);
                else 
                    read_data_from_socket(i, read_fds, write_fds);
            }
            if (FD_ISSET(i, &copy_write_fds)) 
            {
                std::cout << "\033[32m" << "+++++++++++++++++++++++++++++RESPONSE++++++++++++++++++++++++++++++++++\033[0m" << std::endl;
                for (std::map <int, infoserv>::iterator it = mapinfo.begin() ; it != mapinfo.end(); it++)
                {
                    if(it->first == i)
                    {
                        std::cout << "------------------------------------\n";
                        std::cout << mapinfo[i].request.url << std::endl;
                        std::cout << "------------------------------------\n";
                        break;
                    }
                }
                std::cout << "************************************\n";
                mapinfo[i].request.response.run();
                std::cout << "************************************\n";
                s = send(i,  mapinfo[i].request.response.SendResponse.c_str() + se, mapinfo[i].request.response.SendResponse.size() - se, 0);
                se += s;
                std::cout << "s == " << s << std::endl;
                std::cout << "se == " << se << std::endl;
                std::cout << "size == " << mapinfo[i].request.response.SendResponse.size() << std::endl;
                // std::string newsend = mapinfo[i].request.response.SendResponse.c_str() + s;
                if(se == mapinfo[i].request.response.SendResponse.size())
                {
                    // s = send(i,  mapinfo[i].request.response.SendResponse.c_str() + s, mapinfo[i].request.response.SendResponse.size(), 0);
                    FD_CLR(i,&write_fds);
                    close(i);
                    if (stat("./output.txt", &sb) == 0)
                        std::remove("./output.txt");
                    std::cout << "\033[0;35m" << "---------->>>>>CLOSE-SOCKET<<<<<-------- : " << i << "\033[0m" << std::endl;
                    s = 0;
                    se = 0;
                }
                if(s == -1)
                {
                    // s = send(i,  mapinfo[i].request.response.SendResponse.c_str() + s, mapinfo[i].request.response.SendResponse.size(), 0);
                    FD_CLR(i,&write_fds);
                    close(i);
                    if (stat("./output.txt", &sb) == 0)
                        std::remove("./output.txt");
                    std::cout << "\033[0;35m" << "---------->>>>>CLOSE-SOCKET<<<<<-------- : " << i << "\033[0m" << std::endl;
                    // s = 0;
                    se = 0;
                }
            }
            
        }
    }
}

