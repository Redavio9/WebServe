#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>


typedef struct param_req
{
  std::string path;
  std::string version_http;
  std::string ip;
  std::string port;
} param_req;



void parse_req(std::string buffer)
{
    std::stringstream ss(buffer);
    std::string str;
    int i = 0;
    int cnt = 0;
    while(getline(ss, str, '\n'))
    {
      // if(str.length() == 1)
      //   break;
      std::vector<std::map<std::string, std::string> >MyMap;
      int j = str.find(' ');
      std::string key;
      std::string value;
      key = str.substr(0,j);
      value = str.substr(j,str.length());
      MyMap[key] = value;
      if (i == 0)
      {
        std::stringstream ss(value);
        std::string buffer;
        while(getline(ss, buffer, ' '))
        {
          if (cnt == 1)
            param_req.path = buffer;
          if (cnt == 2)
            param_req.version_http = buffer;
          cnt++;
        }
      }
      if (i == 1)
      {
        cnt = 0;
        std::stringstream ss(value);
        std::string buffer;
        getline(ss, buffer, ' ');
        while(getline(ss, buffer, ':'))
        {
          if (cnt == 0)
            param_req.ip = buffer;
          if (cnt == 1)
            param_req.port = buffer;
          cnt++;
        }
      }
      if(i == 7)
        break;
      i++;
    }
}











int main() 
{
    std::vector<std::pair<std::string, int> > socket_list;
    std::vector<std::map<std::string, std::string> > MyMap;

    socket_list.push_back(std::make_pair("127.0.0.1", 8000));
    socket_list.push_back(std::make_pair("127.0.0.1", 8001));
    socket_list.push_back(std::make_pair("127.0.0.1", 8002));

    // Create and bind sockets
    std::vector<int> sockets;
    fd_set read_fds;
    int max_fd = -1;

    for (size_t i = 0; i < socket_list.size(); ++i) 
    {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) 
        {
            std::cerr << "Error: Could not create socket\n";
            return 1;
        }

        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(socket_list[i].second);
        addr.sin_addr.s_addr = inet_addr(socket_list[i].first.c_str());

        if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) 
        {
            std::cerr << "Error: Could not bind socket\n";
            close(sockfd);
            return 1;
        }

        if (listen(sockfd, 5) == -1) 
        {
            std::cerr << "Error: Could not listen on socket\n";
            close(sockfd);
            return 1;
        }

        sockets.push_back(sockfd);

        // Update max_fd
        if (sockfd > max_fd)
            max_fd = sockfd;
    }

    std::cout << "listening...\n";

    // Accept connections
    while (true) 
    {
        char buffer[1024];
        FD_ZERO(&read_fds);
        for (size_t i = 0; i < sockets.size(); ++i) 
        {
            FD_SET(sockets[i], &read_fds);
        }

        // Call select
        int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (activity == -1) 
        {
            std::cerr << "Error: select error\n";
            return 1;
        }

        for (size_t i = 0; i < sockets.size(); ++i) 
        {
            if (FD_ISSET(sockets[i], &read_fds)) 
            {
                struct sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);
                int client_sockfd = accept(sockets[i], (struct sockaddr *)&client_addr, &client_addr_len);
                if (client_sockfd == -1) 
                {
                    std::cerr << "Error: Could not accept connection\n";
                    close(sockets[i]);
                    return 1;
                }
                read(client_sockfd, buffer, sizeof(buffer));
                std::cout << "|-------------------------REQ----------------------------|" << std::endl;
                std::cout << buffer << std::endl;
                std::cout << "|--------------------------------------------------------|" << std::endl;
                parse_req(buffer);
                char client_ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
                std::cout << "Accepted connection from " << client_ip << ":" << ntohs(client_addr.sin_port) << std::endl;
                break;
                // Handle the connection as needed

                close(client_sockfd);
            }
        }
        std::cout << "--------------------------------------------------------\n";
    }

    // Close all sockets (not reached in this code snippet)
    for (size_t i = 0; i < sockets.size(); ++i) {
        close(sockets[i]);
    }

    return 0;
}

