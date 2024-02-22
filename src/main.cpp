/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 10:20:00 by rarraji           #+#    #+#             */
/*   Updated: 2024/02/22 10:42:42 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int main(void)
{
    MyServer MyServer;
    printf("---- SERVER ----\n\n");
    int status;
    int fd_max = 0;     // Descripteur de la plus grande socket

    // Création des sockets serveurs
    int server_socket_1 = MyServer.create_server_socket(PORT_1);
    int server_socket_2 = MyServer.create_server_socket(PORT_2);
    int server_socket_3 = MyServer.create_server_socket(PORT_3);

    if (server_socket_1 == -1 || server_socket_2 == -1 || server_socket_3 == -1) 
        return 1;

    // Préparation des ensembles de sockets pour select()

    //function set_fd_to_sero
    MyServer.SetFdToZero();

    // Trouver le descripteur de fichier maximum
    fd_max = std::max(server_socket_1, std::max(server_socket_2, server_socket_3));
    printf("[Server] Set up select fd sets\n");
    // Boucle principale
    while (1)
    { 
      // Copie l'ensemble des sockets puisque select() modifie l'ensemble surveillé
      MyServer.set_copy_read_fds();
      MyServer.set_copy_write_fds();
      // Surveille les sockets prêtes à être lues
      status = select(fd_max + 1, MyServer.get_copy_read_fds(), MyServer.get_copy_write_fds(), NULL, NULL);
      if (status == -1) 
      {
          fprintf(stderr, "[Server] Select error: %s\n", strerror(errno));
          exit(1);
      }
      else if (status == 0) 
      {
          // Aucun descipteur de fichier de socket n'est prêt pour la lecture
          printf("[Server] Waiting...\n");
          continue;
      }
      // function  Boucle sur nos sockets
      MyServer.loopOverSockets(server_socket_1, server_socket_2, server_socket_3, fd_max);  
    }
    return (0);
}