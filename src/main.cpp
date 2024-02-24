/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 10:20:00 by rarraji           #+#    #+#             */
/*   Updated: 2024/02/24 09:45:31 by rarraji          ###   ########.fr       */
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
    while (true) 
    {
        // Copy the sets of sockets since select() modifies the monitored set
        server.copy_read_fds = server.read_fds;
        server.copy_write_fds = server.write_fds;

        // Monitor sockets ready for reading or writing
        timer.tv_sec = 2;
        timer.tv_usec = 0;
        
        int status = select(7, &server.copy_read_fds, &server.copy_write_fds, NULL, &timer);

        if (status == -1) {
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

    return 0;
}