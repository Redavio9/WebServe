/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 10:20:00 by rarraji           #+#    #+#             */
/*   Updated: 2024/02/23 13:00:39 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include <stdio.h>
#include <sys/select.h>

void print_fd_set(fd_set& fds) {
    for (int i = 0; i < FD_SETSIZE; ++i) {
        if (FD_ISSET(i, &fds)) {
            printf("%d ", i);
        }
    }
}
// int main(void)
// {
//     MyServer MyServer;
//     printf("---- SERVER ----\n\n");
//     int status;
//     int fd_max = 0;     // Descripteur de la plus grande socket

//     // Création des sockets serveurs
//     int server_socket_1 = MyServer.create_server_socket(PORT_1);
//     int server_socket_2 = MyServer.create_server_socket(PORT_2);
//     int server_socket_3 = MyServer.create_server_socket(PORT_3);

//     if (server_socket_1 == -1 || server_socket_2 == -1 || server_socket_3 == -1) 
//         return 1;

//     // Préparation des ensembles de sockets pour select()

//     //function set_fd_to_sero
//     MyServer.SetFdToZero();

//     // Trouver le descripteur de fichier maximum
//     fd_max = std::max(server_socket_1, std::max(server_socket_2, server_socket_3));
//     printf("[Server] Set up select fd sets\n");
//     // Boucle principale
//     while (1)
//     { 
//       // Copie l'ensemble des sockets puisque select() modifie l'ensemble surveillé
//       MyServer.set_copy_read_fds();
//       MyServer.set_copy_write_fds();
//       // Surveille les sockets prêtes à être lues   
//       printf("[Server] Select returned\n");
//       status = select(fd_max + 1, &MyServer.get_copy_read_fds(), &MyServer.get_copy_write_fds(), NULL, NULL);
//       printf("[Server] Select returned status: %d\n", status);
//       if (status == -1) 
//       {
//           fprintf(stderr, "[Server] Select error: %s\n", strerror(errno));
//           exit(1);
//       }
//       else if (status == 0) 
//       {
//           // Aucun descipteur de fichier de socket n'est prêt pour la lecture
//           printf("[Server] Waiting...\n");
//           continue;
//       }
//       // function  Boucle sur nos sockets
//       MyServer.loopOverSockets(server_socket_1, server_socket_2, server_socket_3, fd_max);  
//     }
//     return (0);
// }


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

        // server.set_copy_read_fds();
        // server.set_copy_write_fds();

        // Monitor sockets ready for reading or writing
        // timer.tv_sec = 2;
        // timer.tv_usec = 0;
        // int status = 0;
        int status = select(10000, &server.copy_read_fds, &server.copy_write_fds, NULL, &timer);
        // std::cout << "HEEER: " << strerror(errno) << std::endl;

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