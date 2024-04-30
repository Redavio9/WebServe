/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 15:53:48 by rarraji           #+#    #+#             */
/*   Updated: 2024/04/29 12:19:56 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
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
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <thread>


// Fonction pour lire le corps de la requête
// void Server::readBody(int socket, std::string& body) 
// {
//     char buffer[1024];
//     ssize_t bytesRead;
//     bytesRead = recv(socket, buffer, sizeof(buffer) - 1, 0);
//     if (bytesRead > 0) 
//     {
//         buffer[bytesRead] = '\0';
//         body += buffer;
//     }
//     while (bytesRead > 0);
// }


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

    infoserv info;
    //3mer zwar info 3ad pushit s map
    mapinfo.insert(std::make_pair(socket_fd, info));
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


// void Server::parse_req(std::string buffer) 
// {
//     std::stringstream ss(buffer);
//     std::string str;
//     int i =   0;
//     int cnt =   0;

//     while(getline(ss, str, '\n')) 
//     {
//         std::map<std::string, std::string> MyMap;
//         int j = str.find(' ');
//         std::string key;
//         std::string value;
//         key = str.substr(0,j);
//         value = str.substr(j,str.length());
//         MyMap[key] = value;
//         if (i == 0) 
//         {
//             std::stringstream ss(value);
//             std::string buffer;
//             while(getline(ss, buffer, ' ')) 
//             {
//                 if (cnt == 0 && i == 0)
//                     param_req_one.methode = key;
//                 if (cnt ==   1)
//                     param_req_one.path = buffer;
//                 if (cnt ==   2)
//                     param_req_one.version_http = buffer;
//                 cnt++;
//             }
//         }
//         if (i ==   1) 
//         {
//             cnt =   0;
//             std::stringstream ss(value);
//             std::string buffer;
//             getline(ss, buffer, ' ');
//             while(getline(ss, buffer, ':')) 
//             {
//                 if (cnt ==   0)
//                     param_req_one.ip = buffer;
//                 if (cnt ==   1)
//                     param_req_one.port = buffer;
//                 cnt++;
//             }
//         }
//         if(i ==   7)
//             break;
//         i++;
//     }
//     std::cout << "\033[32m" << "methode  : " << param_req_one.methode << "\033[0m" << std::endl;
//     std::cout << "\033[32m" << "ip  : " << param_req_one.ip << std::endl << "port  : " << param_req_one.port << "\033[0m" << std::endl;
//     std::cout << "\033[32m" << "path  : " << param_req_one.path << std::endl << "version_http  : " << param_req_one.version_http << "\033[0m" << std::endl;
// }

void Server::run() 
{
    fd_set read_fds;
    fd_set write_fds;
    int fd_max =  0;
    

    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);

    FD_SET(server_socket_1, &read_fds);
    FD_SET(server_socket_2, &read_fds);
    FD_SET(server_socket_3, &read_fds);

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

        for (int i =  3; i <= fd_max; i++) 
        {
            if (FD_ISSET(i, &copy_read_fds)) 
            {
                if (i == server_socket_1 || i == server_socket_2 || i == server_socket_3) 
                    accept_new_connection(i, read_fds, &fd_max);
                else 
                    read_data_from_socket(i, read_fds, write_fds);
            }
            if (FD_ISSET(i, &copy_write_fds)) 
            {
                for (std::map <int, infoserv>::iterator it = mapinfo.begin() ; it != mapinfo.end(); it++)
                {
                    if(it->first == i)
                    {
                        std::cout << "------------------------------------\n";
                        std::cout << mapinfo[i].request.url << std::endl;
                        std::cout << "------------------------------------\n";
                        break;
                    }
                    // for (std::map<int, std::string>::const_iterator it1 = it->second.begin() ; it1 != it->second.end(); it1++)
                    // {
                    //     if(it1->first == i)
                    //     {
                    //         std::cout << "------------------------------------\n";
                    //         std::cout << it1->second << std::endl;
                    //         std::cout << "------------------------------------\n";
                    //     }     
                    // }        
                    // if (it->first. == i)
                }
                
                // std::cout << "---->" << " ip  : " << param_req.ip << std::endl << "port  : " << param_req.port << std::endl;
                std::string test = "/";
                std::string test1 = "/favicon.ico";
                std::string path = "./pages";
                std::string new_path = "./pages";
                // std::cout << "----> " << param_req_one.path << std::endl;
                if (mapinfo[i].request.url.compare("/home") == 0)
                    mapinfo[i].request.url = "/home";
                if (mapinfo[i].request.url.compare("/favicon.ico") == 0)
                    mapinfo[i].request.url = "./images/rarraji.jpg";
                if (mapinfo[i].request.url.compare("/") == 0)
                    mapinfo[i].request.url = "/index";
                if (mapinfo[i].request.url.compare("/upload") == 0)
                    mapinfo[i].request.url = "/upload";
                if (mapinfo[i].request.url.compare("/images/rarraji.jpg") == 0)
                    mapinfo[i].request.url = "./images/rarraji.jpg";
                if (mapinfo[i].request.url.compare("/images/bel-kdio.jpg") == 0)
                    mapinfo[i].request.url = "./images/bel-kdio.jpg";
                if (mapinfo[i].request.url.compare("/images/maxresdefault.jpg") == 0)
                    mapinfo[i].request.url = "./images/maxresdefault.jpg";
                if (mapinfo[i].request.url.compare("/images/vedeo.mp4") == 0)
                    mapinfo[i].request.url = "./images/vedeo.mp4";
                // if(mapinfo[i].request.methode.compare("POST") == 0)
                //     mapinfo[i].request.url = "../fichier.txt";          
                if (mapinfo[i].request.url.compare("./images/rarraji.jpg") != 0 && mapinfo[i].request.url.compare("./images/bel-kdio.jpg") != 0 && mapinfo[i].request.url.compare("./images/maxresdefault.jpg") && mapinfo[i].request.url.compare("./images/vedeo.mp4") && mapinfo[i].request.url.compare("../fichier.txt") != 0)
                {
                    new_path += mapinfo[i].request.url + ".html";
                    std::cout << new_path << std::endl;
                    std::cout << "HERE2\n";
                }   
                else
                {
                    new_path = mapinfo[i].request.url;
                    // std::cout << "HERE1\n";
                }
                    
                // std::cout << new_path << std::endl;
        
                std::string response;
                std::stringstream buffer;
                response = "HTTP/1.1 200 OK\r\n";

                // if(param_req_one.)


                
                if (new_path.compare("./images/rarraji.jpg") == 0 || new_path.compare("./images/bel-kdio.jpg") == 0 || new_path.compare("/images/maxresdefault.jpg") == 0)
                {
                    std::ifstream file(new_path.c_str(), std::ios::binary);
                    if (!file.is_open()) 
                    {
                        std::cerr << "[Server] Impossible d'ouvrir image " << "\n";
                        continue;
                    }
                    std::cout << "here" << std::endl;
                    buffer << file.rdbuf();
                    file.close();
                    response += "Content-Type: image/jpg\r\n";
                }
                else if (new_path.compare("./images/vedeo.mp4") == 0)
                {
                    std::ifstream file(new_path.c_str(), std::ios::binary);
                    if (!file.is_open()) 
                    {
                        std::cerr << "[Server] Impossible d'ouvrir le vedeo " << "\n";
                        continue;
                    }
                    buffer << file.rdbuf();
                    file.close();
                    // std::stringstream fi;
                    std::ofstream fi("vedeo.mp4", std::ios::binary);
                    if (!fi.is_open()) 
                    {
                        std::cerr << "[Server] Impossible d'ouvrir le vedeo " << "\n";
                        continue;
                    }
                    fi << buffer.rdbuf();
                    response += "Content-Type: video/mp4\r\n";
                }
                else if (new_path.compare("../fichier.txt") == 0)
                {
                    std::ofstream file_1("../image.txt");
                    // std::ofstream file_1(new_path.c_str());
                    if (!file_1.is_open()) 
                    {
                        std::cerr << "[Server] Impossible d'ouvrir le fichier " << "\n";
                        continue;
                    }
                    file_1 << mapinfo[i].request.body;
                    // this->body = '\0';
                    buffer << file_1.rdbuf();
                    file_1.close();
                }
                
                else
                {
                    std::ifstream file(new_path.c_str());
                    if (!file.is_open()) 
                    {
                        std::cerr << "[Server] Impossible d'ouvrir le fichier " << "\n";
                        exit(1);
                        continue;
                    }
                    buffer << file.rdbuf();
                    file.close();
                    response += "Content-Type: text/html\r\n";                    
                }
                // std::string response = "hello";
                response += "\r\n";
                FD_CLR(i,&write_fds);
                response += response.append(buffer.str(), sizeof(buffer));
                // response += buffer.str();
                send(i, response.c_str(), response.size(), 0);
                close(i);
                std::cout << "\033[0;35m" << "---------->>>>>CLOSE-SOCKET<<<<<-------- : " << i << "\033[0m" << std::endl;
            }
            
        }
    }
}

