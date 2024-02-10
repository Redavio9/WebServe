
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>

int main()
{
  int port = 8080; // Choisissez le port que vous souhaitez utiliser
  int backlog = 5; // Nombre maximal de connexions en attente

  // -family définit la famille du socket. Les valeurs principales sont AF_INET pour un socket IPv4, AF_INET6 pour un support IPv6. 
  // -type spécifie le type de socket. Les valeurs principales utilisées sont SOCK_STREAM pour TCP, SOCK_DGRAM pour UDP. 
  // -protocol définit le protocole à utiliser. Il sera dépendant du type de socket et de sa famille. Les valeurs principales sont IPPROTO_TCP pour un socket TCP, IPPROTO_UDP pour un socket UDP.

  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  
  if (serverSocket == -1)
  {
    std::cerr << "Erreur lors de la création du socket." << std::endl;
    return -1;
  }

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(port);
  serverAddress.sin_addr.s_addr = inet_addr("10.12.6.7");

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
    // int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    int bytesRead = read(clientSocket, buffer, sizeof(buffer));
    if (bytesRead == -1)
    {
      std::cerr << "Erreur lors de la lecture des données." << std::endl;
      close(clientSocket);
      close(serverSocket);
      return -1;
    }
    const char *response =  "HTTP/1.1 200 OK\r\n"
                            "Content-Type: text/html\r\n\r\n"
                            "<html>"
                            "<head>"
                            "<style>"
                            "body { background-color: #f2f2f2; font-family: Arial, sans-serif; }"
                            "h1 { color: #333333; text-align: center; }"
                            "</style>"
                            "</head>"
                            "<body>"
                            "<h1>Bienvenue sur notre site !</h1>"
                            "</body>"
                            "</html>";
    // int bytesSent = send(clientSocket, response, strlen(response), 0);
    int bytesSent = write(clientSocket, response, strlen(response));

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
