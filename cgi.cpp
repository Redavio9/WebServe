 /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 15:35:12 by rarraji           #+#    #+#             */
/*   Updated: 2024/05/02 09:13:07 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "cgi.hpp"
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>


Cgi::Cgi()
{
   ////std::cout << "\033[32m" << "+++++++++++++++++++++++++++++CGI++++++++++++++++++++++++++++++++++\033[0m" << std::endl;
}
Cgi::~Cgi()
{
  
}
void Cgi::SetHeader(std::string header_tmp)
{
  this->header = header_tmp;
}
void Cgi::SetBody(std::string body_tmp)
{
  this->body = body_tmp;
}

std::string Cgi::GetHeader()
{
  return(this->header);
}
std::string Cgi::GetBody()
{
  return(this->body);
}

std::string itoa(int number) 
{
    std::ostringstream ss;
    ss << number;
    return ss.str();
}
void Cgi::run()
{
  // ////////////std::cout << "\033[0;31m" << "*******************************HEADER*************************************" << "\033[0m" << std::endl;
  // ////////////std::cout << "\033[0;31m" << header << "\033[0m" << std::endl;
  // ////////////std::cout << "\033[0;31m" << "**************************************************************************" << "\033[0m" << std::endl;
  // ////////////std::cout << "\033[0;33m" << "********************************BODY*************************************" << "\033[0m" << std::endl;
  // ////////////std::cout << "\033[0;33m" << body << "\033[0m" << std::endl;
  // ////////////std::cout << "\033[0;33m" << "*************************************************************************" << "\033[0m" << std::endl;
    size_t pos = header.find("Content-Type: ");
    std::string content_type = header.substr(pos + 14, header.find("\r\n", pos) - (pos + 14));
    std::string cc = std::string("CONTENT_TYPE=") + content_type;
    std::string cc1 = std::string("QUERY_STRING=") + querystingcgi;
    methode = "REQUEST_METHOD=" + methode;
    std::string url_tmp =  url;
    url = "PATH_INFO=" + url;
    //std::cout << "methode" << methode << std::endl;
    // std::string cc1 = "QUERY_STRING=fname_reda&anickname_solix";
    // std::string cc2 = "REQUEST_METHOD=GET";

    ////////std::cout << "++++++++++++++++++" << cc << "++++++++++++++++++"<< std::endl;
  //  std::string env[11] = 
  //   {
  //     ("HOME=/home/user\0"),
  //     ("REQUEST_METHOD=GET\0"),
  //     ("QUERY_STRING=\0"),
  //     ("CONTENT_LENGTH=1000000000000\0"),
  //     ("PATH_INFO=/Users/rarraji/Desktop/prj/WebServMerge/pages/web3/Up_Solix.py\0"),
  //     ("SCRIPT_NAME=Up_Solix.py\0"),
  //     ("HTTP_ACCEPT=text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\0"),
  //     ("HTTP_USER_AGENT=Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:123.0) Gecko/20100101 Firefox/123.0\0"),
  //     ("HTTP_CONNECTION=keep-alive"),
  //   };
    std::string envp[1] = 
    {
      ("CONTENT_LENGTH=1000000000000\0"),
    };
    // char *envp[14];
    // int i = 0;
    // for (i = 0; i < 12; i++)
    //   envp[i] = &env[i][0];

    // envp[11] = (char *)cc.c_str();
    // envp[12] = (char *)cc1.c_str();
    // envp[13] = NULL;
    // ////std::cout << "+-+-+-+-+-+-+-+-" << cc1 << std::endl;
    // ////std::cout << "+-+-+-+-+-+-+-+-" << envp[12] << std::endl;

    char *en[6];
    for (int i = 0; i < 1; i++)
      en[i] = &envp[i][0];
    en[1] = (char *)cc.c_str();
    en[2] = (char *)cc1.c_str();
    en[3] = (char *)methode.c_str();
    en[4] = (char *)url.c_str();
    en[5] = NULL;
    

    // Open a temporary file to store the output
    char s[100];
    chdir(root.c_str());
    printf("%s\n", getcwd(s, 100));
    int outputFile = open("output.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    std::ofstream ss("./input.txt");
    if(ss.is_open())
      ss << body;
    ss.close();
    int inputFile = open("./input.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    ////////std::cout << "\033[0;31m" << "**************" << "\033[0m" << std::endl;
    // // size_t size = 0;
    // std::string buffer;
    // while (body.size() > size)
    // {
    //   char buff[1024];
    //   int s = read(inputFile, buff, 1024);
    //   size += s;
    //   std::string bb(buff,s);
    //   buffer.append(bb);

    // }
    ////////std::cout << "\033[0;31m" << "**************" << "\033[0m" << std::endl;
    // exit(1);
    // int fd = fileno(ss);  //
    if (outputFile == -1) 
    {
        std::cerr << "Failed to open output file 1\n";
        return;
    }
    
    // int fds[2];
    // pipe(fds);

    // write(fds[1], body.c_str(), 147824122);
    // close(fds[1]);

    pid_t pid = fork();
    if (pid < 0) 
    {
      // Fork failed
      std::cerr << "Fork failed\n";
      return ;
    } 
    else if (pid == 0) 
    {
      // Child process
      // const char *argvv[] = 
      // {
      //   "/usr/bin/python3",
      //   "/Users/rarraji/Desktop/prj/WebServMerge/pages/web3/Get_Solix.py",     
      //   NULL
      // };


      std::string arg[1] = 
      {
        ("/usr/bin/python3\0"),
      };

      char *argg[3];
      int i = 0;
      for (i = 0; i < 1; i++)
        argg[i] = &arg[i][0];
      argg[1] = (char *)url_tmp.c_str();
      argg[2] = NULL;
      // envp[12] = (char *)cc1.c_str();
      // envp[13] = NULL;
      // ////std::cout << "+-+-+-+-+-+-+-+-" << cc1 << std::endl;
      // ////std::cout << "+-+-+-+-+-+-+-+-" << envp[12] << std::endl;
      // Redirect stdout to the temporary file
      

      dup2(inputFile, STDIN_FILENO);
      dup2(outputFile, STDOUT_FILENO);

      close(outputFile);
      close(inputFile);
      
      // int numEnvVars;
      // for (numEnvVars = 0; env[numEnvVars] != NULL; ++numEnvVars); // Use NULL instead of nullptr

      // const char* newEnv[numEnvVars + 2]; // +2 for the new variable and null terminator

      // Copy existing environment variables
      // for (int i = 0; i < numEnvVars; ++i) 
      // {
      //   newEnv[i] = env[i];
      // }

      // std::string newbody = "BODY=";
      // newbody += body;
      // newEnv[numEnvVars] = newbody.c_str();
      // newEnv[numEnvVars + 1] = NULL;
      execve("/usr/bin/python3", const_cast<char* const*>(argg), en);
      std::cerr << "execv() failed\n";
      return ;
    } 
    else 
    {
      // dup2(fds[0], STDOUT_FILENO);
      // close(fds[0]);/

        // struct stat sb;
        int status;
        time_t start = time(NULL);
        time_out = 0;
        // waitpid(pid, &status, 0);
        while (waitpid(pid, &status, WNOHANG) != -1)
        {
          time_t end = time(NULL);
          if(end - start > 5)
          {
            std::cerr << "TIME OUT \n";
            time_out = 1;
            // close(outputFile);
            kill(pid, 15);
            return ;
          }
        }
        status_code_error = 0;
        status = status >> 8;
        if(status != 0)
        {
          status_code_error = 1;
          return;
        }
        // Close the file
        close(outputFile);
        close(inputFile);

        // Read the output from the file
        std::ostringstream output;
        int outputFile = open("output.txt", O_RDONLY);
        
        // {
        //   std::cerr << "Failed to open output file\n";
        // }

        size_t size = 0;
        std::string buffer;
        char buff[1024];
        while (int s = read(outputFile, buff, 1024))
        {
          size += s;
          std::string bb(buff,s);
          buffer.append(bb);
        }

        if(buffer.find("HTTP/1.1") == std::string::npos)
        {
          status_code_error = 1;
        }
        else if(buffer.find("HTTP/1.1") != std::string::npos && buffer.find("Content-Length:") == std::string::npos)
        {
          size_t pos = buffer.find("\r\n") + 2;
          std::string content_L = "Content-Length: " + itoa(buffer.substr(buffer.find("\r\n\r\n") + 4).size()) + "\r\n";
          buffer.insert(pos, content_L);
          // std::cout << "sllslsl " <<buffer << std::endl;
        }
        close(outputFile);
        std::ofstream file("output.txt", std::ios::in);
        file << buffer;
        file.close();
        // if (outputFile) 
        // {
        //   while (outputFile.good()) 
        //   {
        //     std::string line;
        //     std::getline(outputFile, line);
        //     output << line;
        //   }

        //   outputFile.close();
        //   close(inputFile);
        //   Optional: //////////std::cout << "Output from Python script:\n" << output.str() << std::endl;
        // } 
        // else 
        // std::cout << "Output from Python script:\n" << buffer << std::endl;
        // remove("output.txt");
    }
}