/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 10:17:38 by rarraji           #+#    #+#             */
/*   Updated: 2024/06/04 10:18:48 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"
#include<dirent.h>


Response::Response()
{
    status = 0;
//   check_cgi = false;
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
  return(this->header);
}
std::string Response::GetBody()
{
  return(this->body);
}




std::string Response::generateHTML(const char* path) {
  DIR *dir = opendir(path);
  if (!dir) {
    return "";
  }

  std::ostringstream ss;
  ss << "<html><head><title>Directory Listing</title>"
     "<style>"
     "body {"
     "  font-family: 'Arial', sans-serif;  /* Set font family */"
     "  background-color: #f5f5f5;  /* Set background color */"
     "  margin: 30px;  /* Adjust margins */"
     "  padding: 20px;  /* Add padding */"
     "}"
     "h1 {"
     "  text-align: center;  /* Center header */"
     "  color: #333;  /* Set header color */"
     "  font-size: 24px;  /* Adjust font size */"
     "  margin-bottom: 20px;  /* Add space below header */"
     "}"
     "ul {"
     "  list-style: none;  /* Remove default bullets */"
     "  padding: 0;  /* Reset padding */"
     "}"
     "li {"
     "  margin-bottom: 15px;  /* Space between list items */"
     "}"
     "a {"
     "  text-decoration: none;  /* Remove underline */"
     "  color: #007bff;  /* Link color */"
     "  font-weight: bold;  /* Make links bolder */"
     "}"
     "a:hover {"
     "  color: #0056b3;  /* Change link color on hover */"
     "}"
     "</style>"
     "</head><body><h1>Directory Listing</h1><ul><br>";

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_name[0] != '.') {
      ss << "<li><a href=\"" << entry->d_name << "\">" << entry->d_name << "</a></li>";
    }
  }
  ss << "</ul></body></html>";
  closedir(dir);
  return ss.str();
}


std::string Response::AddContentType()
{
    std::cout << "an3mr contant type\n";
    std::string part = "Content-Type: ";
    std::string check;
    size_t pos = url.find('.');
    if (pos != std::string::npos)
    {
        check = url.substr(pos + 1 , url.length());
        std::cout << "****" << check << std::endl;
        for (std::map<std::string, std::string>::iterator it = ContentType.begin(); it != ContentType.end(); ++it)
        {
            if(check.compare(it->first) == 0)
            {
                
                std::cout << "HNA : ---> "<< it->second << std::endl;
                part += it->second;
                part += "\r\n";
                break;
            }
        }
    }
    if(directory_listing == 1)
    {
        part += "text/html";
        part += "\r\n";
    }
    if(part.compare("Content-Type: ") == 0)
    {
        part += "text/plain";
        part += "\r\n";
    }
    return(part);   
}







std::string Response::get_error_pages(std::string key)
{
    std::map<std::string, std::string>::iterator it = error_pages.find(key);
    if (it == error_pages.end())
        return "";
    return it->second;
}

std::string generateErrorPage1(int statusCode, const std::string& errorMessage) 
{
  std::ostringstream ss;
  ss << "<html><head><title>Error " << statusCode << "</title></head>"
     "<body><h1>Error: " << statusCode << "</h1>"
     "<p>" << errorMessage << "</p>"
     "</body></html>";
  return ss.str();
}

std::string convertIntToString(int number) 
{
    std::ostringstream ss;
    ss << number;
    return ss.str();
}

std::string response_generate_error_page(std::string resp, std::string new_url)
{
    resp += "Content-Type: text/html\r\n";
    resp += "\r\n";
    resp += new_url;
    return(resp);
}

void Response::run()
{

                if (redur == true)
                {
                    SendResponse = "HTTP/1.1 301 Moved Permanently\r\n";
                    SendResponse += "Content-Type: text/html\r\n";
                    SendResponse += "Location: " + new_redur;
                }
                else
                {
                    
                    if (check_cgi == false)
                    {
                    
                    }
                    if(methode == "POST")
                       status = 201;
                    else if(status == 0)                        
                        status = 200;
                    std::stringstream buffer;
                    std::string tmp_status = "HTTP/1.1 " + convertIntToString(status);
                    SendResponse = tmp_status + " OK\r\n";
                    std::string new_url;
                    bool dir = false;
                    new_url = url;
                    std::cout << "new_url -- > " << new_url << std::endl;
                    std::cout << "errorpage -- > " << errorpage << std::endl;
                    if(errorpage == 1 || methode == "DELETE")
                    {

                        SendResponse = response_generate_error_page(SendResponse, new_url);

                    }
                    else
                    {

                            if (new_url.find(".txt") != std::string::npos)
                            {
                                std::ifstream file(new_url.c_str());
                                if (!file.is_open()) 
                                {
                                    std::cerr << "[Server] Impossible d'ouvrir file " << "\n";
                                }
                                buffer << file.rdbuf();
                                file.close();
                                SendResponse = buffer.str();
                            } 
                            if (directory_listing == 1)
                            {

                                std::cout << "-->new_path directory_listing : "<< url << std::endl;
                                SendResponse += AddContentType();
                                SendResponse += "\r\n";
                                SendResponse += generateHTML(url.c_str());
                                dir = true;
                            }
                            
                            else
                            {
                                std::cout << "1---->new_url : "<< new_url << std::endl;
                                if(check_cgi == true)
                                    new_url = url; 
                                else if (check_cgi == false)
                                {
                                    std::ifstream file(new_url.c_str(), std::ios::in);
                                    if (!file.is_open()) 
                                    {
                                        file.close();
                                        std::string tmp;
                                        if(get_error_pages(convertIntToString(404)).empty())
                                        {
                                            std::cout << "lslllslls: |" << new_url.c_str() << "|" << std::endl;
                                            errorpage = 1;
                                            SendResponse = response_generate_error_page(SendResponse, generateErrorPage1(404, "notfound"));
                                            std::cout << SendResponse << std::endl;
                                        }
                                        else
                                        {
                                            tmp = root + get_error_pages(convertIntToString(404));
                                        }
                                    }
                                    else
                                    {
                                        std::cout << "00---->new_url : "<< new_url << std::endl;
                                        std::cout << "---->char * : "<< new_url.c_str() << std::endl;
                                        std::string buf;
                                        buffer << file.rdbuf();
                                        file.close();
                                        SendResponse += AddContentType();
                                        SendResponse += "Content-Lenght: " + convertIntToString(buffer.str().size()) + "\r\n";
                                    }
                                    
                                }  
                            }
                            if(directory_listing == false && errorpage == 0)
                            {
                                std::cout << "file 3adi \n";
                                SendResponse += "\r\n";
                                SendResponse += buffer.str();
                            }
                        }
                    }
                check_cgi = false;
                redur = false;
                directory_listing = false;
                
}