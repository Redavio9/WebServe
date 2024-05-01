/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 10:17:38 by rarraji           #+#    #+#             */
/*   Updated: 2024/05/01 15:43:37 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

Response::Response()
{
  
}
Response::~Response()
{
  
}
void Response::SetHeader(std::string header_tmp)
{
  this->header = header_tmp;
}
void Response::SetBody(std::string body_tmp)
{
  this->body = body_tmp;
}
void Response::SetUrl(std::string url_tmp)
{
  this->url = url_tmp;
}
void Response::SetSocket(int tmp_SetSocket)
{
  this->url = tmp_SetSocket;
}

std::string Response::GetHeader()
{
  std::cout << "here\n";
  return(this->header);
}
std::string Response::GetBody()
{
  return(this->body);
}

void Response::run()
{
  std::cout << "\033[0;31m" << "*******************************HEADER*************************************" << "\033[0m" << std::endl;
  std::cout << "\033[0;31m" << header << "\033[0m" << std::endl;
  std::cout << "\033[0;31m" << "**************************************************************************" << "\033[0m" << std::endl;
  std::cout << "\033[0;33m" << "********************************BODY*************************************" << "\033[0m" << std::endl;
  std::cout << "\033[0;33m" << body << "\033[0m" << std::endl;
  std::cout << "\033[0;33m" << "*************************************************************************" << "\033[0m" << std::endl;
  
                std::string test = "/";
                std::string test1 = "/favicon.ico";
                std::string path = "./pages";
                std::string new_path = "./pages";
                // std::cout << "----> " << url << std::endl;
                if (url.compare("/home") == 0)
                    url = "/home";
                if (url.compare("/favicon.ico") == 0)
                    url = "./images/rarraji.jpg";
                if (url.compare("/") == 0)
                    url = "/index";
                if (url.compare("/upload") == 0)
                    url = "/upload";
                if (url.compare("/images/rarraji.jpg") == 0)
                    url = "./images/rarraji.jpg";
                if (url.compare("/images/bel-kdio.jpg") == 0)
                    url = "./images/bel-kdio.jpg";
                if (url.compare("/images/maxresdefault.jpg") == 0)
                    url = "./images/maxresdefault.jpg";
                if (url.compare("/images/vedeo.mp4") == 0)
                    url = "./images/vedeo.mp4";
                // if(mapinfo[i].request.methode.compare("POST") == 0)
                //     url = "../fichier.txt";          
                if (url.compare("./images/rarraji.jpg") != 0 && url.compare("./images/bel-kdio.jpg") != 0 && url.compare("./images/maxresdefault.jpg") && url.compare("./images/vedeo.mp4") && url.compare("../fichier.txt") != 0)
                {
                    new_path += url + ".html";
                    std::cout << new_path << std::endl;
                    std::cout << "HERE2\n";
                }   
                else
                {
                    new_path = url;
                    // std::cout << "HERE1\n";
                }
                    
                // std::cout << new_path << std::endl;
        
                // std::string response;
                std::stringstream buffer;
                SendResponse = "HTTP/1.1 200 OK\r\n";

                // if(param_req_one.)


                
                if (new_path.compare("./images/rarraji.jpg") == 0 || new_path.compare("./images/bel-kdio.jpg") == 0 || new_path.compare("/images/maxresdefault.jpg") == 0)
                {
                    std::ifstream file(new_path.c_str(), std::ios::binary);
                    if (!file.is_open()) 
                    {
                        std::cerr << "[Server] Impossible d'ouvrir image " << "\n";
                    }
                    std::cout << "here" << std::endl;
                    buffer << file.rdbuf();
                    file.close();
                    SendResponse += "Content-Type: image/jpg\r\n";
                }
                else if (new_path.compare("./images/vedeo.mp4") == 0)
                {
                    std::ifstream file(new_path.c_str(), std::ios::binary);
                    if (!file.is_open()) 
                    {
                        std::cerr << "[Server] Impossible d'ouvrir le vedeo " << "\n";
                    }
                    buffer << file.rdbuf();
                    file.close();
                    // std::stringstream fi;
                    std::ofstream fi("vedeo.mp4", std::ios::binary);
                    if (!fi.is_open()) 
                    {
                        std::cerr << "[Server] Impossible d'ouvrir le vedeo " << "\n";
                    }
                    fi << buffer.rdbuf();
                    SendResponse += "Content-Type: video/mp4\r\n";
                }
                else if (new_path.compare("../fichier.txt") == 0)
                {
                    std::ofstream file_1("../image.txt");
                    // std::ofstream file_1(new_path.c_str());
                    if (!file_1.is_open()) 
                    {
                        std::cerr << "[Server] Impossible d'ouvrir le fichier " << "\n";
                    }
                    file_1 << body;
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
                    }
                    buffer << file.rdbuf();
                    file.close();
                    SendResponse += "Content-Type: text/html\r\n";                    
                }
                // std::string SendResponse = "hello";
                SendResponse += "\r\n";
                // SendResponse += SendResponse.append(buffer.str(), sizeof(buffer));
                SendResponse += buffer.str();
                // send(i, SendResponse.c_str(), SendResponse.size(), 0);
}