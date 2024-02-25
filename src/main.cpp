/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 10:20:00 by rarraji           #+#    #+#             */
/*   Updated: 2024/02/25 15:20:05 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


int main() {
    MyServer server;
    printf("---- SERVER ----\n\n");

    // Create server sockets
    int server_socket_1 = server.create_server_socket(PORT_1);
    int server_socket_2 = server.create_server_socket(PORT_2);
    int server_socket_3 = server.create_server_socket(PORT_3);

    if (server_socket_1 == -1 || server_socket_2 == -1 || server_socket_3 == -1) 
        return 1;
    
    // Main loop
    struct timeval timer;
// server.set_fd_max(server_socket_1, server_socket_2, server_socket_3);
    int fd_max = std::max(server_socket_1, std::max(server_socket_2, server_socket_3));
    
    FD_ZERO(&server.copy_write_fds);
    FD_ZERO(&server.copy_read_fds);
    while (true) 
    {
        // Copy the sets of sockets since select() modifies the monitored set
        // FD_SET(server_socket_1, &server.read_fds);
        // FD_SET(server_socket_2, &server.read_fds);
        // FD_SET(server_socket_3, &server.read_fds);
        server.copy_read_fds = server.get_read_fds();
        server.copy_write_fds = server.get_write_fds();

        // Monitor sockets ready for reading or writing
        timer.tv_sec = 2;
        timer.tv_usec = 0;
        
        int status = select(fd_max + 1, &server.copy_read_fds, &server.copy_write_fds, NULL, &timer);
        if (status == -1) 
        {
            fprintf(stderr, "[Server] Select error: %s\n", strerror(errno));
            exit(1);
        }
        else if (status == 0) 
        {
            // No file descriptors ready for reading or writing
            printf("[Server] Waiting...\n");
            continue;
        }

        // Function to loop over the sockets
        server.loopOverSockets(server_socket_1, server_socket_2, server_socket_3);  
    }
>>>>>>> af9e5fc45a9065a6872d43ba59663dcbccd1633f

int main(void) {
    Server server;
    server.run();
    return  0;
}