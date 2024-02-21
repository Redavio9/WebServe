
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <map>
#include <vector>


typedef struct param_req
{
  std::string path;
  std::string version_http;
  std::string ip;
  std::string port;
} param_req;



int main()
{
  int port = 8080; // Choisissez le port que vous souhaitez utiliser
  int backlog = 5; // Nombre maximal de connexions en attente
  param_req param_req;
  

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
  serverAddress.sin_addr.s_addr = inet_addr("10.12.5.9");

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
    // memset(buffer, 0, sizeof(buffer));
    // std::cout << " --> "<< buffer << std::endl;
    // int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    // std::cout << "HEREEEE !!" << buffer << std::endl;
    int bytesRead = read(clientSocket, buffer, sizeof(buffer));
    //parse buffer

    std::stringstream ss(buffer);
    std::string str;
    int i = 0;
    int cnt = 0;
    while(getline(ss, str, '\n'))
    {
      // if(str.length() == 1)
      //   break;
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

    // if (bytesRead == -1)
    // {
    //   std::cerr << "Erreur lors de la lecture des données." << std::endl;
    //   close(clientSocket);
    //   close(serverSocket);
    //   return -1;
    // }

  const char *response = "HTTP/1.1 200 OK\r\n"
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
