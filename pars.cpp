#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

class location
{
    public : 
        std::string NAME;
        std::string  root;
        bool autoindex;
        bool POST;
        bool GET;
        bool DELETE;
        std::string index;
        std::string _return;
        std::string alias;
        std::string cgi_path;
};


class Server
{    
    public :
        std::string  listen;
        std::string  host;
        std::string  server_name;
        std::string  error_page_404;
        std::string  max_body;
        std::string  root;
        std::string  index;
        std::vector<location>locations;
};

std::string removeSpaces(const std::string &input) 
{
    std::string result = input;
    result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
    return result;
}
location* get_location(std::ifstream &Myfile, std::string &line)
{
    location *local = new location;
    local->NAME = line;
    bool x = 0;
    std::string token;
    while(getline(Myfile, line))
    {
        std::istringstream iss(line);
        iss >> token;
        std::getline(iss, line);
        line = removeSpaces(line);
        if(x == 0 && token[0] == '{')
            x = 1;
        else if(x == 1 && token == "root")
            local->root = line;
        else if(x == 1 && token == "autoindex")
        {
            if(line == "on")
                local->autoindex = 1;
            else if (line == "off")
                local->autoindex = 0;
            else
                exit(1);
        }
        else if(x == 1 && token == "GET")
        {
            if(line == "on")
                local->GET = 1;
            else if (line == "off")
                local->GET = 0;
            else
                exit(1);
        }
        else if(x == 1 && token == "POST")
        {
            if(line == "on")
                local->POST = 1;
            else if (line == "off")
                local->POST = 0;
            else
                exit(1);
        }
        else if(x == 1 && token == "DELETE")
        {
            if(line == "on")
                local->DELETE = 1;
            else if (line == "off")
                local->DELETE = 0;
            else
                exit(1);
        }
        else if(x == 1 && token == "index")
            local->index = line;
        else if(x == 1 && token == "return")
            local->_return = line;
        else if(x == 1 && token == "alias")
            local->alias = line;
        else if(x == 1 && token == "cgi_path")
            local->cgi_path = line;
        else if (x == 1 && line.find("}") != std::string::npos) 
            break;
    }
    return (local);
}

// void    check_listen(std::string  port)
// {
//     int i = 0;
//     while(port[i])
//     {
//         if(!isdigit(port[i]))
//             exit(1);
//     }
//     if(port.length() > 6)
//         exit(1);
// }

// void    check_arguments(Server S1)
// {
//     check_listen(S1.listen);
// }




int main(int ac, char **av)
{
    if(ac == 2)
    {   
        std::vector<Server> servers;
        std::ifstream Myfile(av[1]);
        if (Myfile.is_open()) 
        {
            std::string line;
            while (std::getline(Myfile, line)) 
            {
                line.erase(line.find_last_not_of(" \t") + 1);

                if (line.empty() || line[0] == '#') 
                {
                    continue;
                }

                std::istringstream iss(line);
                std::string token;
                iss >> token;
                std::getline(iss, line);
                if (token == "server{") 
                {
                    Server S1;
                    while (std::getline(Myfile, line)) 
                    {
                        std::istringstream iss(line);
                        token.clear();
                        iss >> token;
                        std::getline(iss, line);
                        line = removeSpaces(line);
                        if(token == "listen")
                            S1.listen = line;
                        else if(token == "host")
                            S1.host = line;
                        else if(token == "server_name")
                            S1.server_name = line;
                        else if(token == "error_page_404")
                            S1.error_page_404 = line;
                        else if(token == "max_body")
                            S1.max_body = line;
                        else if(token == "root")
                            S1.root = line;
                        else if(token == "index")
                            S1.index = line;
                        else if(token == "location")
                        {
                            S1.locations.push_back(*get_location(Myfile , line));
                        }
                        else if (line == "};") 
                        {
                            break;
                        }
                        else
                            exit(1);
                    }
                    if (line != "};") 
                    {
                        std::cout << "End of file reached.   | "<< line<< std::endl;
                        exit (1);
                    }
                    // check_arguments(S1);
                    // std::cout <<  S1.listen << std::endl;
                    // std::cout <<  S1.host << std::endl;
                    // std::cout <<  S1.server_name << std::endl;
                    // std::cout <<  S1.error_page_404 << std::endl;
                    // std::cout <<  S1.max_body << std::endl;
                    // std::cout <<  S1.root << std::endl;
                    // std::cout <<  S1.index << std::endl;
                    // std::cout <<  S1.locations[0].root << std::endl;
                    // std::cout <<  S1.locations[0].autoindex << std::endl;
                    // std::cout <<  S1.locations[0].alias << std::endl;
                    // std::cout <<  S1.locations[0].GET << std::endl;
                    // std::cout <<  S1.locations[0].POST << std::endl;
                    // std::cout <<  S1.locations[0].DELETE << std::endl;
                    // std::cout <<  S1.locations[0].index << std::endl;
                    // std::cout <<  S1.locations[0]._return << std::endl;
                    // std::cout <<  S1.locations[0].cgi_path << std::endl;
                    // std::cout <<  S1.locations[1].root << std::endl;
                    // std::cout <<  S1.locations[1].autoindex << std::endl;
                    // std::cout <<  S1.locations[1].alias << std::endl;
                    // std::cout <<  S1.locations[1].GET << std::endl;
                    // std::cout <<  S1.locations[1].POST << std::endl;
                    // std::cout <<  S1.locations[1].DELETE << std::endl;
                    // std::cout <<  S1.locations[1].index << std::endl;
                    // std::cout <<  S1.locations[1]._return << std::endl;
                    // std::cout <<  S1.locations[1].cgi_path << std::endl;
                    // std::cout << "-************************-"<< std::endl;
                    servers.push_back(S1);
                }
                else
                    exit(1);
            }    
        }
    }
}