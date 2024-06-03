/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 09:55:27 by rarraji           #+#    #+#             */
/*   Updated: 2024/06/03 15:47:48 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

Request::Request()
{
  // response.AddContentType( );
  compareLenBody = 0;
  check_l3mara = 0;
  Get = true;
  last = true;
  start = 0;
  s = 0;
  se = 0;
}

std::string convertIntToString1(int number) 
{
    std::ostringstream ss;
    ss << number;
    return ss.str();
}

std::string generateErrorPage3(int statusCode, const std::string& errorMessage) 
{
  std::ostringstream ss;
  ss << "<html><head><title>Error " << statusCode << "</title></head>"
     "<body><h1>Error: " << statusCode << "</h1>"
     "<p>" << errorMessage << "</p>"
     "</body></html>";
  return ss.str();
}

std::string Request::get_error_pages(std::string key)
{
    std::map<std::string, std::string>::iterator it = error_pages.find(key);
    if (it == error_pages.end())
        return "";
    return it->second;
}

int Request::read_socket(int socket)
{
    int valread;
    fcntl(socket, F_SETFL, O_NONBLOCK , FD_CLOEXEC);
    valread = read(socket, buffer, sizeof(buffer) - 1);  
    // std::cout << buffer <<std::endl;
    return (valread);
}

void Request::AddHeaderReq(int valread)
{
  size_t pos;
  if ((pos = request.find("\r\n\r\n")) != std::string::npos && !check)
  {
    check = true;
    compareLenBody = valread - pos + 4;
    header = request;
  }
  else
    compareLenBody += valread;
}

void Request::CheckChunked()
{
  size_t pos;
  if (header.find("chunked") != std::string::npos)
  {
    chunked = true;
    ////////std::cout << "chunked" << std::endl; 
  }
  if ((pos = body.find("\r\n\r\n")) != std::string::npos)
  {
    body = body.substr(pos + 4 , body.length());
  }
}

int hexStringToDecimal(const std::string& hexString) 
{
    // ////////std::cout << "hna -- > " << hexString << std::endl;
    std::stringstream ss;
    ss << std::hex << hexString; // Utiliser std::hex pour indiquer que la chaîne est hexadécimale
    int decimalValue;
    ss >> decimalValue; // Convertit la chaîne hexadécimale en décimal
    // ////////std::cout << "convert int : "<< decimalValue << std::endl;
    return decimalValue;
}
int convertStringToInt1(const std::string& str) 
{
  std::stringstream ss(str);
  int result;
  
  if (ss >> result) 
    return result;
  return(0);  
}


void Request::RegContent(int nb)
{
  std::string newb;
  body = body.substr(nb + 2 , body.length());
}

void Request::AddHeaderBody()
{
  size_t pos;
  std::string num_one;
  std::string num_one_2;
  if ((pos = header.find("\r\n\r\n")) != std::string::npos)
  {
    header = header.substr(0, pos + 2);
  }
  if(!Get)
  {
    CheckChunked();
    int nb = 0;
    int i = 0;
    while(true && chunked)
    {
      std::stringstream ss(body);
      // ////////std::cout << "***> " << std::endl;
      // exit(1);
      // ////////std::cout << "***> " << std::endl;
      getline(ss, num_one, '\n');
      // getline(ss, num_one, '\r');
      // ////////std::cout << "----->" << num_one << std::endl;
      if(hexStringToDecimal(num_one) == 0)
        break;
      nb =  hexStringToDecimal(num_one);
      body = body.substr(body.find("\n") + 1, body.length());
      new_body += body.substr(start, nb);
      body = body.substr(nb + 2, body.length());
      // ////////std::cout << "ilkm ghid\n";
      // RegContent(nb);
      start = 0;
      i++;
    }
  }
  // ////////std::cout << Get << std::endl;
  if (!Get && !chunked)
  {
    // if ((pos = body.find("\r\n")) != std::string::npos)
    // {
      // ////////std::cout << "hereeeee\n";
    //   new_body = body.substr(pos + 2 , body.length());;
    // }
    // else
      new_body = body;
    // if(chunked)
    // {
      // int poss = new_body.find("----------------------------");
      //   new_body = new_body.substr(0, poss);
    // } 
  }
}
void Request::CreatFiles(std::string NameFile, std::string buf, bool check)
{
  // try 
  // {
    if (check)
    {
      char s[100]; 
      chdir("/Users/rarraji/Desktop/prj/WebServMerge/pages/web3/upload");
      printf("%s\n", getcwd(s, 100));
      std::ofstream outputFile(NameFile , std::ios::binary);
      if (!outputFile.is_open()) 
      {
        std::cerr << "Error: Unable to open file " << NameFile << std::endl;
        return;
      }
      outputFile << buf << std::endl;
    }
    else
    {
      char s[100]; 
      chdir("/Users/rarraji/Desktop/prj/WebServMerge/pages/web3/upload");
      printf("%s\n", getcwd(s, 100)); 
      std::ofstream outputFile(NameFile);
      if (!outputFile.is_open()) 
      {
        std::cerr << "Error: Unable to open file " << NameFile << std::endl;
        return;
      }
      outputFile << buf << std::endl;
    }
  // } 
  // catch (const std::exception& e) 
  // {
  //   std::cerr << "Exception caught: " << e.what() << std::endl;
  // }
}

int checkImgOrText(std::string buf)
{
  if (int pos = buf.find("image") != std::string::npos)
    return(1);
  return (0);  
}


std::string Request::generateErrorPage(int statusCode, const std::string& errorMessage) 
{
  std::ostringstream ss;
  ss << "<html><head><title>Error " << statusCode << "</title></head>"
     "<body><h1>Error: " << statusCode << "</h1>"
     "<p>" << errorMessage << "</p>"
     "</body></html>";
  return ss.str();
}
void Request::UploadFiles()
{
  // std::cout << "\033[0;33m" << "********************************BODY*************************************" << "\033[0m" << std::endl;
  // std::cout << "\033[0;33m" << body << "\033[0m" << std::endl;
  // std::cout << "\033[0;33m" << "*************************************************************************" << "\033[0m" << std::endl;
  std::stringstream ss(new_body);
  std::string buf;
  bool check = false;
  getline(ss,buf);
  std::string Myboundary = buf.substr(0 , buf.length() - 1);
  std::cout << "=====================================ana hna" << Myboundary << std::endl;
  while (getline(ss,buf))
  {
    size_t pos;
    if ((pos = buf.find("filename=")) != std::string::npos || (pos = buf.find("name=")) != std::string::npos)
    {
      std::string content;
      std::string NameFile;
      if ((pos = buf.find("filename=")) != std::string::npos)
      {
        NameFile += buf.substr(pos + 10 , buf.length());
      }
      else if ((pos = buf.find("name=")) != std::string::npos)
      {
        NameFile += buf.substr(pos + 6 , buf.length()); 
        // ////////std::cout << "-->" << NameFile << std::endl;
      }
      NameFile = NameFile.substr(0 , NameFile.length() - 2);
      NameFile += '\0';
      // ////////std::cout << NameFile << std::endl;
      if ((pos = buf.find("filename=")) != std::string::npos)
        getline(ss,buf);
      if(checkImgOrText(buf))
        check = true;
      getline(ss,buf);
      while (getline(ss,buf))
      {
        if(buf.find(Myboundary) != std::string::npos)
          break;
        content += buf;
        content += "\n";
      }
      CreatFiles(NameFile, content, check);
      // ////////std::cout << content;
    }
  }
  
  // ////////std::cout << Myboundary << std::endl;
}

void Request::checkQuery()
{
  std::string str;
  std::string tmp;
  std::vector<std::string> MyQeury;
  size_t pos;
  ////////std::cout << "**-->URL : " << url << std::endl;
  if((pos = url.find("?")) != std::string::npos)
  {
    str = url.substr(pos+1, url.length());
    querystingcgi = str;    
    url = url.substr(0, pos);
  }
  while ((pos = str.find("&")) != std::string::npos)
  {
    tmp = str.substr(0, pos);
    str = str.substr(pos + 1, str.length());
    MyQeury.push_back(tmp);
  }
  tmp = str.substr(0, str.length());
  MyQeury.push_back(tmp);
  // std::cout << "----------------------QUERY---------------------------" << std::endl;
  // for (size_t i = 0; i < MyQeury.size(); i++)
  // {
  //   std::cout << MyQeury[i] << std::endl;
  // }
  // std::cout << "-------------------------------------------------------" << std::endl; 
}

void Request::SaveHost_Port(std::string tmp_host)
{
  // //std::cout << "+-+-+-+-+-+-+-+-+- new host: |" << tmp_host << "|" << std::endl;
  host = tmp_host.substr(6, tmp_host.length());
  // //std::cout << "+-+-+-+-+-+-+-+-+- new host: |" << host << "|" << std::endl;
}
int Request::check_req_valid()
{
  // read line par line 
  std::stringstream ss(header);
  std::string buf;
  int i = 0;
  while(getline(ss, buf, '\n'))
  {
    // ////////std::cout << "--->" << buf << std::endl;
    // check line 1
    if (i == 0)
    {
      std::stringstream ss(buf);
      std::string buff;
      int j = 0;
      while (getline(ss, buff, ' '))
      {
        if (j == 0)
        {
          methode = buff;
          if(buff.compare("GET") != 0 && buff.compare("POST") != 0 && buff.compare("DELETE") != 0)
          {
            // methode = buff;
            if(get_error_pages(convertIntToString1(405)).empty())
            {
                response.errorpage = 1;
                reqerror = generateErrorPage3(405, "methodNotAllowed");
            }
            else
            reqerror = get_error_pages(convertIntToString1(413)); 
            // std::string reqerror = "/Users/rarraji/Desktop/prj/WebServMerge/pages/ErrorPages/methodNotAllowed.html";
            
            return (0);
            // response error in respnose 405
          }
          // ////////std::cout << "Methode : " << buff << std::endl; 
        }
        if (j == 1)
        {
          url = buff;
          //check URL !!!!!!
          checkQuery();
          // if error is a probleme 414
        }
        if (j == 2)
        {
          buf += '\0';
          if (buff.compare("HTTP/1.1") != 0)
          {
            
            std::cout << "-->"<< buff  << "-->" << std::endl;
            // std::string reqerror = "/Users/rarraji/Desktop/prj/WebServMerge/pages/ErrorPages/HTTP_version.html";
            // return(0);
            // response with 505
            // error
          }
        }  
        if (j == 3)
        {
          // error;
        }
        j++;
      }
    }
    host = header.substr(header.find("Host:") , header.length());
    host = host.substr(0 , host.find("\r\n"));
    SaveHost_Port(host);
    // check Maxbodysize();
    // check is req valide or not if error response 400
    i++;
  }
  return(1);
}


void Request::Check_read(int socket, fd_set &read_fds, fd_set &write_fds)
{
  int valread;
  valread = read_socket(socket);
  size_t pos = 0;
  
  // header req
  if (valread > 0) 
  {
      buffer[valread] = '\0';
      request.append(buffer, valread);
      AddHeaderReq(valread);
      if ((pos = request.find("Content-Length: ")) != std::string::npos)
      {
        // check if allow methode post configfile
        Get = false;
        // ////////std::cout << "\033[0;35m"  << "========CHUNKED========" << "\033[0m" << std::endl;
        body_lenght = std::atoi(request.substr(pos + 16,  request.find("\r\n", pos + 16) - pos + 16).c_str());
        header_len = pos + 16;
      }
  }
    // ////////std::cout << "compareLenBody : " << compareLenBody << std::endl;
    // ////////std::cout << "body_lenght : " << body_lenght << std::endl;
    size_t poss;
    if((poss = request.find("boundary")) != std::string::npos && check_l3mara == 0)
    {
      l3mara = "--";
      l3mara += request.substr(poss + 9, request.length());
      // l3mara += '\0';
      l3mara = l3mara.substr(0, l3mara.find('\r'));
      new_3mara = l3mara + "--";
      check_l3mara = 1;
    }
    // ////////std::cout << "GET : "<< Get << "." << std::endl;
    // ////////std::cout << "l3mara : "<< new_3mara << "." <<std::endl;
    // exit(1);
  // body req && cgi && run response
  if ((request.find("\r\n\r\n") != std::string::npos && Get)  || (request.find("\r\n\r\n0") != std::string::npos && !Get) || (request.find(new_3mara) != std::string::npos && !Get))
  {
      // struct stat sb;
      // std::cout << "1 : " << (request.find("\r\n\r\n") != std::string::npos && Get) << std::endl;
      // std::cout << "2 : " << (request.find("\r\n\r\n0") != std::string::npos && !Get) << std::endl;
      // std::cout << "3 : " << (request.find(new_3mara) != std::string::npos && !Get) << std::endl;
      // std::cout << "4 : " << new_3mara << std::endl;
      // exit(1);
    // std::cout << "\033[0;33m" << "********************************req*************************************" << "\033[0m" << std::endl;
    // std::cout << "\033[0;33m" << request << "\033[0m" << std::endl;
    // std::cout << "\033[0;33m" << "*************************************************************************" << "\033[0m" << std::endl;

    // chech max_size !!!!!!!!!!!!!!!!!!
      int tmp = 0;
      // std::cout << ">>> "<< MaxBodySize << "\n";
      if(convertStringToInt1(MaxBodySize) < body_lenght)
      {
        tmp = 1;
        response.status = 413;
        // std::cout << "!!!!!!!!!!!!!!!!!!>>> MAX ad orizri <<!!!!!!!!!!!!!!!!!\n";
        // response.url = "/Content_Too_Large.html";
        if(get_error_pages(convertIntToString1(413)).empty())
        {
            response.errorpage = 1;
            response.url = generateErrorPage3(413, "Content_Too_Large");
        }
        else
         response.url = get_error_pages(convertIntToString1(413)); 
        
      }
    
      if(check_req_valid() && tmp == 0)
      {
        
        // std::cout << "\033[0;35m" << "---------->>>>>ALL-REQUSTE<<<<<--------" << "\033[0m" << std::endl;
        // std::cout << "Get : "<< Get << std::endl;
        if(!Get)
          body = request.substr(header_len, request.length());
        // exit(1);
        AddHeaderBody();
      // std::cout << "\033[0;33m" << "********************************BODY*************************************" << "\033[0m" << std::endl;
      // std::cout << "\033[0;33m" << body << "\033[0m" << std::endl;
      // std::cout << "\033[0;33m" << "*************************************************************************" << "\033[0m" << std::endl;
        ////std::cout << "\033[0;35m" << "---------->>>>>BODY<<<<--------" << "\033[0m" << std::endl;
        ////std::cout << new_body  << std::endl;
        ////std::cout << "\033[0;35m" << "---------->>>>>><<<<<<--------" << "\033[0m" << std::endl;
        if ((pos = header.find("\r\n\r\n")) != std::string::npos)
          header = header.substr(0, pos + 2);
        ////std::cout << "\033[0;35m" << "---------->>>>>Header<<<<--------" << "\033[0m" << std::endl;
        ////std::cout << header  << std::endl;
        ////std::cout << "\033[0;35m" << "---------->>>>>><<<<<<--------" << "\033[0m" << std::endl;
        // exit(1);
        // if (!cgi)
        UploadFiles();
        // ////////std::cout << "herre\n";
        response.SetHeader(header);
        response.SetBody(body);
        // std::cout << "url_req ----------------------------------> " << url << std::endl;
        response.SetUrl(url);
        response.check_body = false; 
        // if (response.url.find(".py") != std::string::npos)
        // {
        //   struct stat sb;
        //   if (stat("./output.txt", &sb) != 0)
        //   {
        //     response.status = 404;
        //     response.url = "/ErrorPages/notFound.html";
        //   }
        //   else
        //   {
        //     ////////std::cout << "hnaaaaaaaaa\n";
        //     response.check_cgi = true;
        //     Cgi cgi;
        //     cgi.SetHeader(header);
        //     cgi.SetBody(new_body);
        //     cgi.url = root + response.url;
        //     cgi.querystingcgi = querystingcgi;
        //     cgi.methode = methode;
        //     cgi.run();
        //     // cgi.methode = "GET";
        //     // //////std::cout << "stat : " << stat("./output.txt", &sb) << std::endl;
        //     if (cgi.time_out == 1)
        //     {
        //       std::cout << "hna\n";
        //       response.check_cgi = false;
        //       response.status = 504;
                  // if(get_error_pages(convertIntToString1(504)).empty())
                  // {
                  //     response.errorpage = 1;
                  //     response.url = generateErrorPage3(504, "getwayTimeout");
                  // }
                  // else
                  // response.url = get_error_pages(convertIntToString1(413));
        //       response.url = "/ErrorPages/getwayTimeout.html";
        //     }
        //     response.url = "/Users/rarraji/Desktop/prj/WebServMerge/pages/output.txt";
        //   }
        // }
        // else
        //   response.run();
      }
      request = "";
      FD_CLR(socket, &read_fds);
      FD_SET(socket, &write_fds);
      compareLenBody = 0;
      body_lenght = 0;
      Get = true;
      check = false;
  }
}