/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_betta.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 11:35:08 by rarraji           #+#    #+#             */
/*   Updated: 2024/02/23 12:57:05 by rarraji          ###   ########.fr       */
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
#include <vector> // Pour gérer plusieurs sockets
#include <map> 

#define PORT_1 8006  // le port de notre premier serveur
#define PORT_2 8007  // le port de notre deuxième serveur
#define PORT_3 8008  // le port de notre troisième serveur

// parametre req
typedef struct param_req
{
  std::string path;
  std::string version_http;
  std::string ip;
  std::string port;
} param_req;



// Déclaration des fonctions
int create_server_socket(int port);
// void accept_new_connection(int listener_socket, fd_set *all_sockets, int *fd_max);
void accept_new_connection(int listener_socket, fd_set &read_fds, int *fd_max);
// void read_data_from_socket(int socket, fd_set *all_sockets, int *fd_max, int server_socket);
void read_data_from_socket(int socket, fd_set &read_fds, int *fd_max, int server_socket, fd_set &write_fds);
// parse req
void parse_req(std::string buffer);

int main(void)
{
    printf("---- SERVER ----\n\n");

    int status;

    // Pour surveiller les sockets clients :
    // fd_set all_sockets; // Ensemble de toutes les sockets du serveur
    fd_set read_fds;    // Ensemble read pour select()
    fd_set write_fds;    // Ensemble write pour select()
    fd_set copy_read_fds;    // Ensemble copy_write pour select()
    fd_set copy_write_fds;    // Ensemble copy_write pour select()
    int fd_max = 0;     // Descripteur de la plus grande socket
    struct timeval timer;

    // Création des sockets serveurs
    int server_socket_1 = create_server_socket(PORT_1);
    int server_socket_2 = create_server_socket(PORT_2);
    int server_socket_3 = create_server_socket(PORT_3);

    if (server_socket_1 == -1 || server_socket_2 == -1 || server_socket_3 == -1) 
    {
        return 1;
    }

    // Préparation des ensembles de sockets pour select()
    FD_ZERO(&copy_write_fds);
    FD_ZERO(&copy_read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&read_fds);

    FD_SET(server_socket_1, &read_fds); // Ajout de la première socket serveur à l'ensemble
    FD_SET(server_socket_2, &read_fds); // Ajout de la deuxième socket serveur à l'ensemble
    FD_SET(server_socket_3, &read_fds); // Ajout de la troisième socket serveur à l'ensemble

    // Trouver le descripteur de fichier maximum
    fd_max = std::max(server_socket_1, std::max(server_socket_2, server_socket_3));

    printf("[Server] Set up select fd sets\n");

    while (1) 
    { // Boucle principale
        // Copie l'ensemble des sockets puisque select() modifie l'ensemble surveillé
        copy_read_fds = read_fds;
        copy_write_fds = write_fds;
        // Timeout de 2 secondes pour select()
        timer.tv_sec = 0;
        timer.tv_usec = 0;

        // Surveille les sockets prêtes à être lues
        status = select(fd_max + 1, &copy_read_fds, &copy_write_fds, NULL, NULL);
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

        // Boucle sur nos sockets
        for (int i = 0; i <= fd_max; i++) 
        {
            if (FD_ISSET(i, &copy_read_fds)) 
            {
                if (i == server_socket_1 || i == server_socket_2 || i == server_socket_3)
                {
                    accept_new_connection(i, copy_read_fds, &fd_max);
                } 
                else
                {
                    read_data_from_socket(i, copy_read_fds, &fd_max, server_socket_1, copy_write_fds);
                }
            }
            if (FD_ISSET(i, &copy_write_fds)) 
            {
                // Envoyer un message de bienvenue au client
                const char *welcome_message = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, World!</h1></body></html>";
                send(i, welcome_message, strlen(welcome_message), 0);
                close(i);
            }
        }
    }
    return (0);
}



// Renvoie la socket du serveur liée à l'adresse et au port qu'on veut écouter
int create_server_socket(int port) 
{
    struct sockaddr_in sa;
    int socket_fd;
    int status;

    // Préparaton de l'adresse et du port pour la socket de notre serveur
    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET; // IPv4
    sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1, localhost
    sa.sin_port = htons(port);

    // Création de la socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) 
    {
        fprintf(stderr, "[Server] Socket error: %s\n", strerror(errno));
        return (-1);
    }
    printf("[Server] Created server socket fd: %d\n", socket_fd);

    // Liaison de la socket à l'adresse et au port
    status = bind(socket_fd, (struct sockaddr *)&sa, sizeof sa);
    if (status != 0) 
    {
        fprintf(stderr, "[Server] Bind error: %s\n", strerror(errno));
        return (-1);
    }
    printf("[Server] Bound socket to localhost port %d\n", port);
    // Mise en écoute de la socket
    status = listen(socket_fd, 10);
    if (status != 0) 
    {
        fprintf(stderr, "[Server] Listen error: %s\n", strerror(errno));
        return (-1);
    }
    printf("[Server] Listening on port %d\n", port);
    return (socket_fd);
}


// Accepte une nouvelle connexion et ajoute la nouvelle socket à l'ensemble des sockets
void accept_new_connection(int listener_socket, fd_set &read_fds, int *fd_max)
{
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    client_fd = accept(listener_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd == -1) 
    {
        fprintf(stderr, "[Server] Accept error: %s\n", strerror(errno));
        return ;
    }
    FD_SET(client_fd, &read_fds); // Ajoute la socket client à l'ensemble
    if (client_fd > *fd_max) 
    {
        *fd_max = client_fd; // Met à jour la plus grande socket
    }
    printf("[Server] Accepted new connection on client socket %d.\n", client_fd);
    // Envoyer un message de bienvenue au client
    // const char *welcome_message = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, World!</h1></body></html>";
    // send(client_fd, welcome_message, strlen(welcome_message), 0);
}



// Lit le message d'une socket et relaie le message à toutes les autres
void read_data_from_socket(int socket, fd_set &read_fds, int *fd_max, int server_socket, fd_set &write_fds)
{
    char buffer[BUFSIZ];
    int bytes_read;

    memset(&buffer, '\0', sizeof buffer);
    bytes_read = recv(socket, buffer, BUFSIZ, 0);
    std::cout << "---------------------------------------------------------\n" << std::endl;
    std::cout << buffer << std::endl;
    std::cout << "---------------------------------------------------------\n" << std::endl;
    parse_req(buffer);
    if (bytes_read <= 0) 
    {
        if (bytes_read == 0) 
        {
            printf("[%d] Client socket closed connection.\n", socket);
        }
        else 
        {
            fprintf(stderr, "[Server] Recv error: %s\n", strerror(errno));
        }
        close(socket); // Ferme la socket
    }
    FD_CLR(socket, &read_fds); // Enlève la socket de l'ensemble
    FD_SET(socket, &write_fds); // Enlève la socket de l'ensemble
}


void parse_req(std::string buffer)
{
        std::stringstream ss(buffer);
        std::string str;
        int i = 0;
        int cnt = 0;
        param_req param_req;
        
        while(getline(ss, str, '\n'))
        {
            std::map<std::string, std::string> MyMap;
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
        std::cout << "ip  : " << param_req.ip << std::endl << "port  : " << param_req.port << std::endl;
        std::cout << "path  : " << param_req.path << std::endl << "version_http  : " << param_req.version_http << std::endl;
}

