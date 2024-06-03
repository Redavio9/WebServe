/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 15:53:48 by rarraji           #+#    #+#             */
/*   Updated: 2024/06/03 19:40:25 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() 
{
    
    // server_socket_1 = create_server_socket(PORT_1);
    // server_socket_2 = create_server_socket(PORT_2);
    // server_socket_3 = create_server_socket(PORT_3);
    // int NumberServerSocker = 3;
    // for (size_t i = 0; i < NumberServerSocker; i++)
    // {
    //     int server_socket_1 = create_server_socket(serverSockets[i].port);
    // }
    // RemplirContentType();
    // exit(1);
    
}

// void Server::RemplirContentType()
// {
//     std::ifstream ss("./Content.type");
//     std::string buffer;
//     std::string data;
//     while (getline(ss, buffer))
//     {
//         data += buffer;
//         data += "\n";
//     }
    
//     std::stringstream sss(data);
//     int i = 0;
//     std::string first;
//     std::string second;
//     while (sss >> buffer)
//     { 
//         if (i % 2 == 0)
//             first = buffer;
//         else
//         {
//             second = buffer;
//             ContentType.insert(std::make_pair(first, second));
//         }
//         i++;    
//     }
//     for (std::map<std::string, std::string>::iterator it = ContentType.begin(); it != ContentType.end(); ++it);
//         // ////////std::cout << ContentType[it->first] << std::endl; 
// }


// int Server::checkAlias(int sock)
// {
//     int valide = 0;
//     int lenght = 0;
    
//     for (std::map <std::string, location_param>::iterator it_bg = mapinfo[sock].location.begin(); it_bg != mapinfo[sock].location.end(); it_bg++)
//     {
        
//     }
    
// }



int convertStringToInt(const std::string& str) 
{
  std::stringstream ss(str);
  int result;
  
  if (ss >> result) 
    return result;
  return(0);  
}

void Server::CreatServers() 
{
    // this->servers.size() = 3;
    for (size_t i = 0; i < this->servers.size(); i++)
    {
        // std::cout << "here\n";
        // int server_socket_1 = create_server_socket(convertStringToInt(this->servers[i].get_port()), this->servers[i].get_host(), this->servers[i].get_root(), "this->servers[i].get_server_name()");
        int server_socket_1 = create_server_socket1(i);
        if(server_socket_1 != -2)
            Sockets.push_back(server_socket_1);
    }
}




// void Server::RemplirInfo(int socket_fd, std::string root, std::string host)
// {
//     infoserv info;
    // ////////std::cout << "root: " << root << std::endl;
    // ////////std::cout << "host: " << host << std::endl;
    // info.root = root;
    // info.host = host;
    // info.errorPage = this->servers[socket_fd].get_error_pages()
    // info.MaxBodySize = this->servers[socket_fd].get_max_body_size();
    // set host
    // set port
    // set location
    // set error page
    // set index
    // set maxBodySize
//     mapinfo.insert(std::make_pair(socket_fd, info));
// }

void Server::RemplirInfo1(int socket_fd, int sock)
{
    infoserv info;
    info.servers = this->servers;
    info.port = this->servers[sock].get_port();
    info.root = this->servers[sock].get_root();
    info.host = this->servers[sock].get_host();
    info.error_pages = this->servers[sock].error_pages;
    info.location = this->servers[sock].location;
    info.MaxBodySize = this->servers[sock].get_max_body_size();
    info.index = this->servers[sock].get_index();
    info.server_name = this->servers[sock].server_name;
    // mapinfo[socket_fd] = info;
    // std::cout << "socket_fd : " << socket_fd << std::endl;
    mapinfo.insert(std::make_pair(socket_fd, info));
}

// Integer::
// add ip addresse;
// int Server::create_server_socket(int port, long ip, std::string root, std::string host) 
// {
//     struct sockaddr_in sa;
//     int socket_fd;
//     int status;
//     (void) ip;

//     ////std::cout << "ip : " << ip << "   port :" << port << std::endl;

//     memset(&sa, 0, sizeof sa);
//     // sa.sin_family = AF_INET; 
//     //sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);   //ip server
//     // sa.sin_addr.s_addr = inet_addr(ip.c_str()); //change ip 0.0.0.0 to myip
//     // sa.sin_port = htons(port); // port server
//     sa.sin_family = AF_INET;
//     sa.sin_addr.s_addr = htonl(ip);
//     //std::cout << "ip : " << sa.sin_addr.s_addr << std::endl;
//     //std::cout << "port : " << port << std::endl;
    
//     // if (inet_pton(AF_INET, ip.c_str(), &sa.sin_addr) <= 0) 
//     // {
//     //     // Handle error
//     //     return -1;
//     // }
//     sa.sin_port = htons(port);

//     socket_fd = socket(AF_INET, SOCK_STREAM, 0);
//     if (socket_fd == -1)
//     {
//         std::cerr << "[Server] Socket error: " << strerror(errno) << std::endl;
//         return (-1);
//     }
//     RemplirInfo(socket_fd, root, host);
//     // mapinfo.insert(std::make_pair(socket_fd, info));
//     ////////std::cout << "\033[0;31m" " [Server] " << "\033[0m" << "Created server socket fd: " << socket_fd << std::endl;
//     int on = 1;
//     setsockopt(socket_fd,SOL_SOCKET, SO_REUSEADDR,(const char *)&on, sizeof(int));
//     status = bind(socket_fd, (struct sockaddr *)&sa, sizeof sa);
//     if (status !=   0) 
//     {
//         std::cerr << "[Server] Bind error: " << strerror(errno) << std::endl;
//         // return (-1);
//     }
//     ////////std::cout << "\033[0;31m" " [Server] " << "\033[0m" << "Bound socket to port " << "\033[0;34m" << port << "\033[0m" << std::endl;

//     status = listen(socket_fd,   10);
//     if (status !=   0) 
//     {
//         std::cerr << "[Server] Listen error: " << strerror(errno) << std::endl;
//         return (-1);
//     }
//     ////////std::cout << "\033[0;31m" " [Server] " << "\033[0m" << "Listening on port "  << "\033[0;34m" << port << "\033[0m" << std::endl;
//     return (socket_fd);
// }
int Server::create_server_socket1(int sock) 
{
    struct sockaddr_in sa;
    int socket_fd;
    int status;

    ////std::cout << "ip : " << ip << "   port :" << port << std::endl;

    std::memset(&sa, 0, sizeof sa);
    // sa.sin_family = AF_INET; 
    //sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);   //ip server
    // sa.sin_addr.s_addr = inet_addr(ip.c_str()); //change ip 0.0.0.0 to myip
    // sa.sin_port = htons(port); // port server
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(this->servers[sock].get_host());
    
    // if (inet_pton(AF_INET, ip.c_str(), &sa.sin_addr) <= 0) 
    // {
    //     // Handle error
    //     return -1;
    // }

    for (std::map <int, infoserv>::iterator it = mapinfo.begin() ; it != mapinfo.end(); ++it)
    {
        if(it->second.host == servers[sock].get_host() && it->second.port == servers[sock].get_port())
            return(-2);
            //skip;
    }
    

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        std::cerr << "[Server] Socket error: " << strerror(errno) << std::endl;
        return (-1);
    }
    // RemplirInfo(socket_fd, this->servers[sock].get_root(), this->servers[sock].get_root());
    RemplirInfo1(socket_fd, sock);
    sa.sin_port = htons(convertStringToInt(this->servers[sock].get_port()));
    // std::cout << "ip : " << sa.sin_addr.s_addr << std::endl;
    // std::cout << "port : " << this->servers[sock].get_port() << std::endl;
    // mapinfo.insert(std::make_pair(socket_fd, info));
    ////////std::cout << "\033[0;31m" " [Server] " << "\033[0m" << "Created server socket fd: " << socket_fd << std::endl;
    int on = 1;
    setsockopt(socket_fd,SOL_SOCKET, SO_REUSEADDR,(const char *)&on, sizeof(int));
    status = bind(socket_fd, (struct sockaddr *)&sa, sizeof sa);
    if (status !=   0) 
    {
        std::cerr << "[Server] Bind error: " << strerror(errno) << std::endl;
        // return (-1);
    }
    ////////std::cout << "\033[0;31m" " [Server] " << "\033[0m" << "Bound socket to port " << "\033[0;34m" << port << "\033[0m" << std::endl;

    status = listen(socket_fd, 100000);
    if (status !=   0) 
    {
        std::cerr << "[Server] Listen error: " << strerror(errno) << std::endl;
        return (-1);
    }
    std::cout << "\033[0;31m" " [Server] " << "\033[0m" << "Listening on port "  << "\033[0;34m" << this->servers[sock].get_port() << "\033[0m" << std::endl;
    return (socket_fd);
}

void Server::accept_new_connection(int listener_socket, fd_set &read_fds, int *fd_max)  
{
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);


    
    client_fd = accept(listener_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    // std::cout << "listener_socket : " << listener_socket << std::endl;
    std::map <int, infoserv> ::iterator it_bg = mapinfo.find(listener_socket);
    mapinfo[client_fd] = it_bg->second;
    // std::cout << "root : " << mapinfo[client_fd].root << std::endl;
    // std::cout << "port : " << mapinfo[client_fd].port << std::endl;
    // mapinfo.insert(std::make_pair(client_fd, it_bg->second));
    if (client_fd == -1) 
    {
        std::cerr << "[Server] Accept error: " << strerror(errno) << std::endl;
        return;
    }
    // //std::cout << << ""
    FD_SET(client_fd, &read_fds);
    if (client_fd > *fd_max) 
        *fd_max = client_fd;
    // std::cout <<"\033[0;31m" " [Server] " << "\033[0m" << "Accepted new connection on client socket " << client_fd << ".\n";
}

void Server::read_data_from_socket(int socket, fd_set &read_fds, fd_set &write_fds) 
{
    mapinfo[socket].request.root =  mapinfo[socket].root;
    mapinfo[socket].request.location = mapinfo[socket].location;
    // std::cout << "mapinfo[socket].request.root" << mapinfo[socket].request.root << std::endl;
    mapinfo[socket].request.MaxBodySize =  mapinfo[socket].MaxBodySize;
    mapinfo[socket].request.Check_read(socket, read_fds, write_fds);
}

int Server::CheckIsMyServer(int nb)
{
  for (size_t i = 0; i < Sockets.size(); i++)
  {
    // ////////std::cout << Sockets[i] << std::endl;
    if (nb == Sockets[i])
        return(1); 
  }
  return(0);
//   exit(1);
}


int Server::CheckServerNmae(int i, std::string servername)
{
    for (size_t j = 0; j < this->servers[i].get_server_name().size(); j++)
    {
        if(this->servers[i].get_server_name()[j].compare(servername) == 0)
            return(1);
    }
    return(0);
}

std::string long_to_string(long nb) 
{
    std::stringstream ss(nb);
    std::string buf;
    if(getline(ss, buf))
        //std::cout << buf << std::endl;
    buf += '\0';
    return(buf);
}

//void Server::checkResponse(int socket, std::string host , std::string port)
// {
//     bool check_red = false;
//     std::string ServerName = "\0";
//     for (size_t i = 0; i < this->servers.size(); i++)
//     {
//         // check man server 
//         // if(this->servers[i].get_host().compare(host) == 0 && this->servers[i].get_port().compare(port) != 0)
//         //     continue;
//         //     if(this->servers[i].get_serverName().length() > 1 && this->servers[i].get_port().compare(ServerName) == 0)
//             // if(CheckServerNmae(int i, std::string servername) || long_to_string(this->servers[i].get_host()).compare(host) == 0 && this->servers[i].get_port().compare(port) == 0)
//             if(long_to_string(this->servers[i].get_host()).compare(host) == 0 && this->servers[i].get_port().compare(port) == 0)
//             {
//                 struct stat fileStat;
//                 std::string tmp1;
//                 // //std::cout << "url : " << mapinfo[socket].request.response.url << std::endl;
//                 // //std::cout << "root + url : " <<  this->servers[i].get_root() + mapinfo[socket].request.response.url << std::endl;
//                 if (mapinfo[socket].request.response.url.find(this->servers[i].get_root()) == std::string::npos)// cheak url isgisila root
//                 {
//                     tmp1 = this->servers[i].get_root();
//                     tmp1.append(mapinfo[socket].request.response.url);
//                 }
//                 else
//                     tmp1 = mapinfo[socket].request.response.url;
//                 ////////std::cout << "check_red : " << check_red << std::endl;
//                 if (mapinfo[socket].request.response.url.compare("/favicon.ico") == 0)
//                 {
//                     ////////std::cout << "ila b3da lpath ad: " << stat(tmp1.c_str(), &fileStat) << " | " << tmp1 << std::endl;
//                     mapinfo[socket].request.response.url = "/Users/rarraji/Desktop/prj/WebServMerge/pages/images/me.png";
//                     break;
//                 }  
//                 else if (stat(tmp1.c_str(), &fileStat) == 0) // checkdir
//                 {
//                     int j = 0;

//                     if (S_ISDIR(fileStat.st_mode))
//                     {
//                         //////std::cout << "dossier\n";
//                         std::string tmp  = tmp1;

//                         //check_if_location_end "/"
//                         if(tmp1.find(".py") != std::string::npos)
//                             j = 1;
//                         // if (j == 0 && tmp[tmp.length() - 1] == '/' && check_red == true)
//                         // {
//                         //     ////////std::cout << "ikan\n";
//                         //     ////////std::cout << "redirect_URL : " << this->servers[i].get_location(mapinfo[socket].request.response.url).get_redirect_url() << std::endl;
//                         // }
//                         if(tmp[tmp.length() - 1] != '/')
//                         {
//                             mapinfo[socket].request.url += '/';
//                             mapinfo[socket].request.response.url += '/';  
//                         }
//                         if(this->servers[i].get_location_first(mapinfo[socket].request.url).compare(mapinfo[socket].request.url) == 0) 
//                         {
//                             check_red = true;
//                             //std::cout << "DONE IS LOCATION" << std::endl;
//                         }
//                         else
//                         {
//                             //std::cout << "ISN'T LOCATION" << std::endl;
//                             j = 1;
//                         }
//                         if (j == 0 && tmp[tmp.length() - 1] != '/')
//                         {
//                             // //////std::cout << "ikan1\n";
//                             // if(check_red == true)
//                             //std::cout << "1-redirect_URL : " << this->servers[i].get_location(mapinfo[socket].request.url).get_redirect_url() << std::endl;
//                             mapinfo[socket].request.response.redur = true;
//                             mapinfo[socket].request.response.new_redur = mapinfo[socket].request.response.url;
//                             ////////std::cout << "new_url_to_red : " << mapinfo[socket].request.response.new_redur << std::endl;
//                             j = 1;
//                             break;
//                         }
//                         //check red url 
//                         else if (j == 0 && this->servers[i].get_location(mapinfo[socket].request.url).get_redirect_url().length() >= 1)
//                         {
//                             //std::cout << "2-redirect_URL : " << this->servers[i].get_location(mapinfo[socket].request.url).get_redirect_url() << std::endl;
//                             mapinfo[socket].request.response.redur = true;
//                             mapinfo[socket].request.response.new_redur = this->servers[i].get_location(mapinfo[socket].request.url).get_redirect_url();
//                             //std::cout << "2-new_url_to_red : " << mapinfo[socket].request.response.new_redur << std::endl;
//                             j = 1;
//                             break;
//                             //check_redirect_URL
//                         }
//                         else
//                             mapinfo[i].request.response.redur = false;
//                         // for (std::map<std::string, bool>::iterator it = this->servers[i].get_location(mapinfo[socket].request.url).MapMethods.begin(); it != this->servers[i].get_location(mapinfo[socket].request.url).MapMethods.end(); ++it)
//                         // {
//                         //     //check methodes
//                         // }
//                         // if(j == 0)
//                             ////////std::cout << "index To serve : " << this->servers[i].get_location(mapinfo[socket].request.url).get_index().length() << std::endl;
//                         // //std::cout << this->servers[i].get_location(mapinfo[socket].request.url).get_methods("GET") << std::endl;   
//                         // //std::cout << "'" << mapinfo[socket].request.url << "'" << std::endl;    
//                         if (j == 0 && this->servers[i].get_location(mapinfo[socket].request.url).get_index().length() > 1)
//                         {
//                         //  mapinfo[socket].request.response.root = this->servers[i].get_root() + mapinfo[socket].request.url;
//                             if(this->servers[i].get_location(mapinfo[socket].request.url).get_methods("GET") != true)
//                                 //std::cout << "orghin tli adada\n";
//                             //std::cout << "hna "<< this->servers[i].get_location(mapinfo[socket].request.url).get_index() << std::endl;
//                             mapinfo[socket].request.response.new_redur = this->servers[i].get_location(mapinfo[socket].request.url).get_index();
//                             mapinfo[socket].request.response.url = this->servers[i].get_root() + '\0' + this->servers[i].get_location_first(mapinfo[socket].request.url) + this->servers[i].get_location(mapinfo[socket].request.url).get_index();
//                             mapinfo[socket].request.response.redur = true;
//                             // this->servers[i].get_location(mapinfo[socket].request.url)
//                             //std::cout << "index To serve : " << mapinfo[socket].request.response.new_redur << std::endl;
//                             j = 1;
//                             break;
//                         }
//                         // //////std::cout << "directory_listing :" << this->servers[i].get_location(mapinfo[socket].request.response.url).get_directory_listing() << std::endl;
//                         if (j == 0 && this->servers[i].get_location(mapinfo[socket].request.url).get_directory_listing().compare("true") == 0)
//                         {
//                             if(this->servers[i].get_location(mapinfo[socket].request.url).get_methods("GET") != true)
//                                 //std::cout << "orghin tli adada\n";
//                             //std::cout << "DONE directory_listing" << std::endl;  
//                             mapinfo[socket].request.response.url = this->servers[i].get_root() + mapinfo[socket].request.response.url;
//                             mapinfo[socket].request.response.directory_listing = true;
//                             break;
//                         }
//                         else
//                         {
//                             // if(this->servers[i].get_location(mapinfo[socket].request.url).get_methods("GET") != true)
//                             //     //std::cout << "orghin tli adada\n";
//                             if (mapinfo[socket].request.response.url.find(this->servers[i].get_root()) == std::string::npos)// cheak url isgisila root
//                                 mapinfo[socket].request.response.url = this->servers[i].get_root() + mapinfo[socket].request.response.url;
//                             mapinfo[socket].request.response.redur = true;
//                             mapinfo[socket].request.response.new_redur = mapinfo[socket].request.response.url + "index.html";
                            
//                             break;
//                         }
//                         break;
//                     }
//                     else
//                     {
//                         // if(this->servers[i].get_location(mapinfo[socket].request.url).get_methods("GET") != true)
//                         //         //std::cout << "orghin tli adada\n";
//                         if (mapinfo[socket].request.response.url.find(this->servers[i].get_root()) == std::string::npos)// cheak url isgisila root
//                             mapinfo[socket].request.response.url = this->servers[i].get_root() + mapinfo[socket].request.response.url;
//                         // mapinfo[socket].request.response.url = this->servers[i].get_root() + mapinfo[socket].request.response.url;
//                         //////std::cout << "FILE : " << mapinfo[socket].request.response.url << std::endl;
//                         //////std::cout << "File Done" << std::endl;
//                         break;    
//                     }
//                 }
//                 else
//                 {
//                     mapinfo[socket].request.response.url = "/Users/rarraji/Desktop/prj/WebServMerge/pages/ErrorPages/notFound.html";
//                     break;
//                 }
//         }
//     }
//     // exit(1);
// }
std::string generateErrorPage2(int statusCode, const std::string& errorMessage) 
{
  std::ostringstream ss;
  ss << "<html><head><title>Error " << statusCode << "</title></head>"
     "<body><h1>Error: " << statusCode << "</h1>"
     "<p>" << errorMessage << "</p>"
     "</body></html>";
  return ss.str();
}

std::string convertIntToString2(int number) 
{
    std::ostringstream ss;
    ss << number;
    return ss.str();
}




void Server::checkResponse1(int sock, std::string host)
{
    (void) host;
    // bool check_red = false;
    // std::string ServerName = "\0";
    
    for (size_t i = 0; i < this->servers.size(); i++)
    {
        if(this->servers[i].get_host() == mapinfo[sock].host && this->servers[i].get_port() == mapinfo[sock].port)
        {
            std::vector<std::string> server_names = this->servers[i].get_server_name();
            
            for(std::vector<std::string>::iterator it = server_names.begin(); it != server_names.end(); it++)
            {
                // std::cout << "++++++++++++ ANA D5ALT" << std::endl;
                // std::cout << "++++++++++++ host: |" << host << "|" << std::endl;
                // std::cout << "++++++++++++ server_name: |" << (*it) << "|" << std::endl;
                if((*it) == host)
                {
                    // std::cout << "ghid irbi" << std::endl;
                    mapinfo[sock].error_pages = this->servers[i].error_pages;
                    mapinfo[sock].location = this->servers[i].location;
                    mapinfo[sock].MaxBodySize = this->servers[i].get_max_body_size();
                    mapinfo[sock].index = this->servers[i].get_index();
                    mapinfo[sock].server_name = this->servers[i].server_name;
                    mapinfo[sock].root = this->servers[i].get_root();       //!!!!!
                }
            }
        }
    }
    mapinfo[sock].request.response.errorpage = 0;

    // {
        // check man server 
        // if(this->servers[i].get_host().compare(host) == 0 && this->servers[i].get_port().compare(port) != 0)
        //     continue;
        //     if(this->servers[i].get_serverName().length() > 1 && this->servers[i].get_port().compare(ServerName) == 0)
            // if(CheckServerNmae(int i, std::string servername) || long_to_string(this->servers[i].get_host()).compare(host) == 0 && this->servers[i].get_port().compare(port) == 0)
            // if(long_to_string(this->servers[i].get_host()).compare(host) == 0 && this->servers[i].get_port().compare(port) == 0)
            // {
                
                struct stat fileStat;
                std::string tmp1;
                mapinfo[sock].request.response.redur = false;
                std::cout << "url : " << mapinfo[sock].request.response.url << std::endl;
                std::cout << "url req: " << mapinfo[sock].request.url << std::endl;
                std::cout << "root + url : " <<  mapinfo[sock].root +  mapinfo[sock].request.response.url << std::endl;
                if (mapinfo[sock].request.response.url.find(mapinfo[sock].root) == std::string::npos)// cheak url isgisila root      //!!!!!
                {
                    tmp1 = this->mapinfo[sock].root;    //!!!!!
                    tmp1.append(mapinfo[sock].request.response.url);
                }
                else
                    tmp1 = mapinfo[sock].request.response.url;
                ////////std::cout << "check_red : " << check_red << std::endl;
                if (mapinfo[sock].request.response.url.compare("/favicon.ico") == 0)
                {
                    ////////std::cout << "ila b3da lpath ad: " << stat(tmp1.c_str(), &fileStat) << " | " << tmp1 << std::endl;
                    mapinfo[sock].request.response.url = "/Users/rarraji/Desktop/prj/WebServMerge/pages/images/me.png";
                    return;
                }
                else if (stat(tmp1.c_str(), &fileStat) == 0) // checkdir
                {
                    //std::cout << "tmp1 : " << tmp1 << std::endl;
                    int j = 0;

                    if (S_ISDIR(fileStat.st_mode))
                    {
                        std::cout << "dossier\n";
                        std::string tmp  = tmp1;

                        //check_if_location_end "/"
                        if(tmp1.find(".py") != std::string::npos)
                            j = 1;
                        // if (j == 0 && tmp[tmp.length() - 1] == '/' && check_red == true)
                        // {
                        //     ////////std::cout << "ikan\n";
                        //     ////////std::cout << "redirect_URL : " << this->servers[i].get_location(mapinfo[socket].request.response.url).get_redirect_url() << std::endl;
                        // }
                        if(tmp[tmp.length() - 1] != '/' && mapinfo[sock].request.methode != "DELETE")
                        {
                            mapinfo[sock].request.url += '/';
                            mapinfo[sock].request.response.url += '/'; 
                            mapinfo[sock].request.response.redur = true;
                            mapinfo[sock].request.response.new_redur = mapinfo[sock].request.response.url;
                            std::cout << "-->*"<< mapinfo[sock].request.response.new_redur << std::endl;
                            std::cout << "hnaaaa a3chir \n";
                            return;  
                        }
                        std::cout << "methode : "<< mapinfo[sock].request.methode << std::endl;
                        std::cout << "errorpage : "<< mapinfo[sock].request.response.errorpage << std::endl;
                        if(mapinfo[sock].request.methode != "DELETE")
                        {
                            
                            //std::cout << "============================================" << std::endl;
                            
                            // for (std::map<std::string, location_param>::iterator it = mapinfo[sock].location.begin(); it !=  mapinfo[sock].location.end(); ++it)
                            // {
                            //     //std::cout << it->first << std::endl;
                            // }
                            //std::cout << "============================================" << std::endl;
                        
                            //std::cout << "this->mapinfo[sock].get_location_first(mapinfo[sock].request.url)" << this->mapinfo[sock].get_location_first(mapinfo[sock].request.url) << std::endl;
                            if(this->mapinfo[sock].get_location_first(mapinfo[sock].request.url ).compare((mapinfo[sock].request.url)) == 0) 
                            {
                                // check_red = true;
                                //std::cout << "DONE IS LOCATION" << std::endl;
                            }
                            else
                            {
                                //std::cout << "ISN'T LOCATION" << std::endl;
                                j = 1;
                            }
                            
                            // if (j == 0 && tmp[tmp.length() - 1] != '/')
                            // {
                            //     // //////std::cout << "ikan1\n";
                            //     // if(check_red == true)
                            //     //std::cout << "1-redirect_URL : " << mapinfo[sock].get_location(mapinfo[sock].request.url).get_redirect_url() << std::endl;
                            //     mapinfo[sock].request.response.redur = true;
                            //     mapinfo[sock].request.response.new_redur = mapinfo[sock].request.response.url;
                            //     ////////std::cout << "new_url_to_red : " << mapinfo[socket].request.response.new_redur << std::endl;
                            //     j = 1;
                            //     return;
                            // }
                            //check red url 
                            if (j == 0 && mapinfo[sock].get_location(mapinfo[sock].request.url).get_redirect_url().length() >= 1)
                            {
                                // std::cout << "2-redirect_URL : " << mapinfo[sock].get_location(mapinfo[sock].request.url).get_redirect_url() << std::endl;
                                mapinfo[sock].request.response.redur = true;
                                mapinfo[sock].request.response.new_redur = mapinfo[sock].get_location(mapinfo[sock].request.url).get_redirect_url();
                                //std::cout << "2-new_url_to_red : " << mapinfo[sock].request.response.new_redur << std::endl;
                                j = 1;
                                return;
                                //check_redirect_URL
                            }
                            else
                                mapinfo[sock].request.response.redur = false;
                            // for (std::map<std::string, bool>::iterator it = mapinfo[sock].get_location(mapinfo[sock].request.url).MapMethods.begin(); it != mapinfo[sock].get_location(mapinfo[sock].request.url).MapMethods.end(); ++it)
                            // {
                            //     //check methodes
                            // }
                            // if(j == 0)
                                // std::cout << "index To serve : " << mapinfo[sock].get_location(mapinfo[sock].request.url).get_index().length() << std::endl;
                            // //std::cout << mapinfo[sock].get_location(mapinfo[sock].request.url).get_methods("GET") << std::endl;   
                            // //std::cout << "'" << mapinfo[sock].request.url << "'" << std::endl;    
                            if (j == 0 && mapinfo[sock].get_location(mapinfo[sock].request.url).get_index().length() > 1)
                            {
                            //  mapinfo[sock].request.response.root = mapinfo[sock].get_root() + mapinfo[sock].request.url;
                                if(mapinfo[sock].get_location(mapinfo[sock].request.url).get_methods("GET") != true)
                                {
                                    // std::cout << "orghin tli adada\n";
                                    mapinfo[sock].request.response.status = 501;
                                    if(mapinfo[sock].get_error_pages(convertIntToString2(mapinfo[sock].request.response.status)).empty())
                                    {
                                        mapinfo[sock].request.response.errorpage = 1;
                                        mapinfo[sock].request.response.url = generateErrorPage2(mapinfo[sock].request.response.status, "notImplemented");
                                    }
                                    else
                                        mapinfo[sock].request.response.url = mapinfo[sock].root + mapinfo[sock].get_error_pages(convertIntToString2(mapinfo[sock].request.response.status));
                                    // mapinfo[sock].request.response.url = "/Users/rarraji/Desktop/prj/WebServMerge/pages/ErrorPages/notImplemented.html";
                                    return;
                                }
                                //std::cout << "hna "<< mapinfo[sock].get_location(mapinfo[sock].request.url).get_index() << std::endl;
                                // mapinfo[sock].request.response.new_redur = mapinfo[sock].get_location(mapinfo[sock].request.url).get_index(); //!!!!
                                mapinfo[sock].request.response.url = mapinfo[sock].root;    //!!!!!
                                mapinfo[sock].request.response.url = mapinfo[sock].request.response.url + mapinfo[sock].get_location_first(mapinfo[sock].request.url);
                                mapinfo[sock].request.response.url = mapinfo[sock].request.response.url + mapinfo[sock].get_location(mapinfo[sock].request.url).get_index();
                                
                                // mapinfo[sock].request.response.redur = true;  //!!!!!!
                                // mapinfo[sock].get_location(mapinfo[sock].request.url)
                                
                                // std::cout << "root : " << mapinfo[sock].root << std::endl;
                                // std::cout << "location : " << mapinfo[sock].get_location_first(mapinfo[sock].request.url) << std::endl;
                                // std::cout << "index To serve : " << mapinfo[sock].request.response.url << std::endl;
                                j = 1;
                                return;
                            }
                            // //////std::cout << "directory_listing :" << mapinfo[sock].get_location(mapinfo[sock].request.response.url).get_directory_listing() << std::endl;
                            if (j == 0 && mapinfo[sock].get_location(mapinfo[sock].request.url).get_directory_listing().compare("true") == 0)
                            {
                                if(mapinfo[sock].get_location(mapinfo[sock].request.url).get_methods("GET") != true)
                                {
                                    // std::cout << "orghin tli adada\n";
                                    mapinfo[sock].request.response.status = 501;
                                    if(mapinfo[sock].get_error_pages(convertIntToString2(mapinfo[sock].request.response.status)).empty())
                                    {
                                        mapinfo[sock].request.response.errorpage = 1;
                                        mapinfo[sock].request.response.url = generateErrorPage2(mapinfo[sock].request.response.status, "notImplemented");
                                    }
                                    else
                                        mapinfo[sock].request.response.url = mapinfo[sock].root + mapinfo[sock].get_error_pages(convertIntToString2(mapinfo[sock].request.response.status));
                                    // mapinfo[sock].request.response.url = "/Users/rarraji/Desktop/prj/WebServMerge/pages/ErrorPages/notImplemented.html";
                                    return;
                                }
                                // std::cout << "DONE directory_listing" << std::endl;  
                                mapinfo[sock].request.response.url = mapinfo[sock].root + mapinfo[sock].request.response.url;
                                mapinfo[sock].request.response.directory_listing = true;
                                return;
                            }
                            else
                            {
                                if(mapinfo[sock].get_location(mapinfo[sock].request.url).get_methods("GET") != true)
                                {
                                    // std::cout << "orghin tli adada\n";
                                    mapinfo[sock].request.response.status = 501;
                                    if(mapinfo[sock].get_error_pages(convertIntToString2(mapinfo[sock].request.response.status)).empty())
                                    {
                                        mapinfo[sock].request.response.errorpage = 1;
                                        mapinfo[sock].request.response.url = generateErrorPage2(mapinfo[sock].request.response.status, "notImplemented");
                                    }
                                    else
                                        mapinfo[sock].request.response.url = mapinfo[sock].root + mapinfo[sock].get_error_pages(convertIntToString2(mapinfo[sock].request.response.status));
                                    // mapinfo[sock].request.response.url = "/Users/rarraji/Desktop/prj/WebServMerge/pages/ErrorPages/notImplemented.html";
                                    return;
                                }
                                if (mapinfo[sock].request.response.url.find(mapinfo[sock].root) == std::string::npos)// cheak url isgisila root
                                    mapinfo[sock].request.response.url = mapinfo[sock].root + mapinfo[sock].request.response.url;     //!!!!!
                                // mapinfo[sock].request.response.redur = true;
                                mapinfo[sock].request.response.new_redur = mapinfo[sock].request.response.url + "index.html";
                                
                                return;
                            }
                            return;
                        }
                        else
                        {
                            if(mapinfo[sock].get_location(mapinfo[sock].request.url).get_methods("GET") != true)
                            {
                                    // std::cout << "orghin tli adada\n";
                                    mapinfo[sock].request.response.status = 501;
                                    if(mapinfo[sock].get_error_pages(convertIntToString2(mapinfo[sock].request.response.status)).empty())
                                    {
                                        mapinfo[sock].request.response.errorpage = 1;
                                        mapinfo[sock].request.response.url = generateErrorPage2(mapinfo[sock].request.response.status, "notImplemented");
                                    }
                                    else
                                        mapinfo[sock].request.response.url = mapinfo[sock].root + mapinfo[sock].get_error_pages(convertIntToString2(mapinfo[sock].request.response.status));
                                    // mapinfo[sock].request.response.url = "/Users/rarraji/Desktop/prj/WebServMerge/pages/ErrorPages/notImplemented.html";
                                    return;
                            }
                            if (mapinfo[sock].request.response.url.find(mapinfo[sock].root) == std::string::npos)// cheak url isgisila root      //!!!!!
                                mapinfo[sock].request.response.url = mapinfo[sock].root + mapinfo[sock].request.response.url;
                            // mapinfo[sock].request.response.url = mapinfo[sock].get_root() + mapinfo[sock].request.response.url;
                            //////std::cout << "FILE : " << mapinfo[sock].request.response.url << std::endl;
                            //////std::cout << "File Done" << std::endl;
                            return;    
                        }
                    }
                    else
                    {
                        std::cout << "tmp1 : " << tmp1 << std::endl;
                        mapinfo[sock].request.response.status = 404;
                        if(mapinfo[sock].get_error_pages(convertIntToString2(mapinfo[sock].request.response.status)).empty())
                        {
                            mapinfo[sock].request.response.errorpage = 1;
                            mapinfo[sock].request.response.url = generateErrorPage2(mapinfo[sock].request.response.status, "NOT-FOUND");
                        }
                        else
                            mapinfo[sock].request.response.url = mapinfo[sock].root + mapinfo[sock].get_error_pages(convertIntToString2(mapinfo[sock].request.response.status));
                        return;
                    }
                    }
                    std::cout << "ghid adlkm";
    // exit(1);
}



void Server::run() 
{
    fd_set read_fds;
    fd_set write_fds;
    int fd_max =  0;
    

    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);


    // boucler sur les socket
    for (size_t i = 0; i < Sockets.size(); i++)
    {
        // ////////std::cout << Sockets[i] << std::endl;
        FD_SET(Sockets[i], &read_fds);
    }
    
    
    // FD_SET(server_socket_1, &read_fds);
    // FD_SET(server_socket_2, &read_fds);
    // FD_SET(server_socket_3, &read_fds);

    // fd_max = std::max(server_socket_1, std::max(server_socket_2, server_socket_3));

    //fd_max les socket 
    for (size_t i = 0; i < Sockets.size(); ++i)
    {
        fd_max = std::max(fd_max, Sockets[i]);
        // ////////std::cout << "fdmax : "<< fd_max << std::endl;
    }
    // ////////std::cout << "Sockets.size =  : "<< Sockets.size() << std::endl;   
    // ////////std::cout << "fdmax : "<< fd_max << std::endl;   
    ////////std::cout << "\033[0;31m" " [Server] " << "\033[0m" << "Set up select fd sets\n";

    while (true) 
    {
        fd_set copy_read_fds = read_fds;
        fd_set copy_write_fds = write_fds;

        int status = select(fd_max + 1, &copy_read_fds, &copy_write_fds, NULL, NULL);
        // ////////std::cout << "test====>: " << status << std::endl;

        if (status == -1) 
        {
            std::cerr << "[Server] Select error: " << strerror(errno) << std::endl;
            return;
        } 
        // else if (status ==  0) 
        // {
        //     ////////std::cout << "[Server] Waiting...\n";
        //     continue;
        // }

        // boucler number of socket 
        for (int i = 3; i <= fd_max; i++) 
        {
            struct stat sb;
            if (FD_ISSET(i, &copy_read_fds)) 
            {
                if (CheckIsMyServer(i) == 1)
                {
                    // ////////std::cout << "i = "<<i<<std::endl;
                    accept_new_connection(i, read_fds, &fd_max);
                } 
                else 
                    read_data_from_socket(i, read_fds, write_fds);
            }
            if (FD_ISSET(i, &copy_write_fds)) 
            {
                ////////std::cout << "\033[32m" << "+++++++++++++++++++++++++++++RESPONSE++++++++++++++++++++++++++++++++++\033[0m" << std::endl;
                for (std::map <int, infoserv>::iterator it = mapinfo.begin() ; it != mapinfo.end(); it++)
                {
                    if(it->first == i)
                    {
                        // std::cout << "------------------------------------------------------------------------\n";
                        // std::cout << mapinfo[i].root << std::endl;
                        // std::cout << "------------------------------------\n";
                        break;
                    }
                }
                ////////std::cout << "************************************";
                // long nb = string_to_long(mapinfo[i].request.host);
                // checkResponse(i, mapinfo[i].request.host, mapinfo[i].request.port);
                //std::cout << "----------------------------------------------------->" << mapinfo[i].root << std::endl;
                // if(mapinfo[i].request.methode != "DELETE")
                // {
                    
                    checkResponse1(i, mapinfo[i].request.host);
                    // exit(1);
                    mapinfo[i].request.response.run();
                    // std::cout << mapinfo[i].request.response.SendResponse << std::endl;
                    // ////////std::cout << "************************************\n";
                    // ////////std::cout << "i == " << i << std::endl;
                    // ////////std::cout << "hna fach taykhrj " << std::endl;
                    // //////std::cout << "--> mapinfo[i].request.response.SendResponse.c_str() + mapinfo[i].request.se :" << mapinfo[i].request.response.SendResponse.c_str() + mapinfo[i].request.se << std::endl;
                    // //////std::cout << "--> mapinfo[i].request.response.SendResponse.size() - mapinfo[i].request.se :" << mapinfo[i].request.response.SendResponse.size() - mapinfo[i].request.se << std::endl;

                    
                    mapinfo[i].request.s = send(i,  mapinfo[i].request.response.SendResponse.c_str() + mapinfo[i].request.se, mapinfo[i].request.response.SendResponse.size() - mapinfo[i].request.se, 0);
                    // if (mapinfo[i].request.s == -1) 
                    // {
                    //     perror("send failed");
                    //     exit(1);
                    // }
                    mapinfo[i].request.se += mapinfo[i].request.s;
                    // //////std::cout << "size == " << mapinfo[i].request.response.SendResponse.size() << std::endl;
                    // //////std::cout << "s == " << mapinfo[i].request.s << std::endl;
                    // //////std::cout << "se == " << mapinfo[i].request.se << std::endl;
                    // std::string newsend = mapinfo[i].request.response.SendResponse.c_str() + s;
                    // if ()
                    if (mapinfo[i].request.se == mapinfo[i].request.response.SendResponse.size())
                    {
                        // s = send(i,  mapinfo[i].request.response.SendResponse.c_str() + s, mapinfo[i].request.response.SendResponse.size(), 0);
                        FD_CLR(i,&write_fds);
                        mapinfo[i].request.s = 0;
                        mapinfo[i].request.se = 0;
                        if (stat("./output.txt", &sb) == 0 && stat("./input.txt", &sb) == 0)
                        {
                            std::remove("./output.txt");
                            std::remove("./input.txt");
                        }
                        mapinfo[i].request.response.SendResponse = "";
                        mapinfo[i].request.header = "";
                        mapinfo[i].request.body = "";
                        // FD_CLR(i, &read_fds);
                        close(i);

                        // mapinfo[i] = infoserv();
                        // std::cout << "\033[0;35m" << "---------->>>>>CLOSE-SOCKET<<<<<-------- : " << i << "\033[0m" << std::endl;
                        // mapinfo[i].request.s = 0;
                        // mapinfo[i].request.se = 0;
                    }
                    if (mapinfo[i].request.s == -1)
                    {
                        // s = send(i,  mapinfo[i].request.response.SendResponse.c_str() + s, mapinfo[i].request.response.SendResponse.size(), 0);
                        FD_CLR(i,&write_fds);
                        mapinfo[i].request.s = 0;
                        mapinfo[i].request.se = 0;
                        mapinfo[i].request.response.SendResponse = "";
                        mapinfo[i].request.header = "";
                        mapinfo[i].request.body = "";
                        // FD_CLR(i, &read_fds);
                        close(i);
                        if (stat("./output.txt", &sb) == 0 && stat("./input.txt", &sb) == 0)
                        {
                            std::remove("./output.txt");
                            std::remove("./input.txt");
                        }
                        // mapinfo[i].request = Request();
                        // mapinfo[i] = infoserv();
                        // std::cout << "\033[0;35m" << "---------->>>>>CLOSE-SOCKET<<<<<-------- : " << i << "\033[0m" << std::endl;
                        // s = 0;
                        // mapinfo[i].request.se = 0;
                        // mapinfo[i].request.s = 0;
                    }
                // }
                // else
                // {
                //     send(i, mapinfo[i].request.response.SendResponse.c_str(), mapinfo[i].request.response.SendResponse.size() , 0);
                //     std::cout << "\033[0;35m" << "---------->>>>>CLOSE-SOCKET<<<<<-------- : " << i << "\033[0m" << std::endl;
                //     close(i);
                // }
            }
            
        }
    }
}


