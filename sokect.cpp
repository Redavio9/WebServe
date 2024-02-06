
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

int main()
{
  int port = 8080; // Choisissez le port que vous souhaitez utiliser
  int backlog = 5; // Nombre maximal de connexions en attente

  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1)
  {
    std::cerr << "Erreur lors de la création du socket." << std::endl;
    return -1;
  }

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(port);
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  memset(serverAddress.sin_zero, '\0', sizeof(serverAddress.sin_zero));

  if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
  {
    std::cerr << "Erreur lors de la liaison du socket." << std::endl;
    close(serverSocket);
    return -1;
  }

  if (listen(serverSocket, backlog) == -1)
  {
    std::cerr << "Erreur lors du passage en mode écoute." << std::endl;
    close(serverSocket);
    return -1;
  }

  while (true)
  {
    sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
    if (clientSocket == -1)
    {
      std::cerr << "Erreur lors de l'acceptation de la connexion." << std::endl;
      close(serverSocket);
      return -1;
    }
    char buffer[1024];
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead == -1)
    {
      std::cerr << "Erreur lors de la lecture des données." << std::endl;
      close(clientSocket);
      close(serverSocket);
      return -1;
    }

    const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Bienvenue Solix !</h1></body></html>";
    int bytesSent = send(clientSocket, response, strlen(response), 0);
    if (bytesSent == -1)
    {
      std::cerr << "Erreur lors de l'envoi de la réponse." << std::endl;
      close(clientSocket);
      close(serverSocket);
      return -1;
    }

    close(clientSocket);
  }

  close(serverSocket);

  return 0;
}