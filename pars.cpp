#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <unistd.h>

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
        std::map<std::string, std::string> cgi;
        location()
        {
            NAME = "";
            root = "";
            autoindex = 0;
            POST = 0;
            GET = 0;
            DELETE = 0;
            index = "";
            _return = "";
            alias = "";
        }
        void    print()
        {
            std::cout << NAME << std::endl;
            std::cout << "root :\n" << root << std::endl;
            std::cout << "autoindex :\n" << autoindex << std::endl;
            std::cout << "POST :\n" << POST << std::endl;
            std::cout << "GET :\n" << GET << std::endl;
            std::cout << "DELETE :\n" << DELETE << std::endl;
            std::cout << "index :\n" << index << std::endl;
            std::cout << "return :\n" << _return << std::endl;
            std::cout << "alias :\n" << alias << std::endl;
        }
};


class Server
{    
    public :
        std::vector<u_int16_t>  listen;
        std::string  host;
        std::vector<std::string>  server_name;
        std::map<int, std::string>error_page;
        int  max_body;
        std::string  root;
        std::string  index;
        std::vector<location>locations;
        Server()
        {
            host = "";
            max_body = -1;
            root = "";
            index = "";
        }
        void print()
        {
            std::cout << "listen :"<< std::endl;
            std::vector<u_int16_t>::iterator iter = listen.begin();
            for(iter; iter < listen.end(); iter++)
                std::cout << *iter << " ";
            std::cout << std::endl;
            std::cout << "host : \n" << host << std::endl;
            std::vector<std::string>::iterator it1 = server_name.begin();
            for(it1; it1 < server_name.end(); it1++)
                std::cout << "server_name :\n" << *it1 << std::endl;
            std::map<int , std::string>::iterator it = error_page.begin();
            std::cout << "error_page :\n";
	        while (it != error_page.end())
	        {
                std::cout << it->first << " :: " << it->second << std::endl;
                it++;
            }
            std::cout << "max_body :\n" << max_body << std::endl;
            std::cout << "root :\n" << root << std::endl;
            std::cout << "index :\n" << index << std::endl;
            std::vector<location>::iterator itr = locations.begin();
            for(itr; itr < locations.end(); itr++)
            {
                std::cout << "loactions**************************** :\n";
                itr->print();
            }
        }
};

std::string check_path(std::string path)
{
    // if(access(path.c_str(), F_OK) != 0)
    //     exit(2);
    return(path);
}

std::string removeSpaces(const std::string &input) 
{
    std::string result = input;
    result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
    return result;
}

void get_location(std::ifstream &Myfile, std::string &line,   std::vector<location> &locations)
{
    if(line.empty())
        exit(101);
    location local ;
    local.NAME = line;
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
        else if(x == 0 && token[0] != '{')
            exit(1);
        else if(x == 1 && token == "root")
            local.root = check_path(line);
        else if(x == 1 && token == "autoindex")
        {
            if(line == "on")
                local.autoindex = 1;
            else if (line == "off")
                local.autoindex = 0;
            else
                exit(1);
        }
        else if(x == 1 && token == "GET")
        {
            if(line == "on")
                local.GET = 1;
            else if (line == "off")
                local.GET = 0;
            else
                exit(1);
        }
        else if(x == 1 && token == "POST")
        {
            if(line == "on")
                local.POST = 1;
            else if (line == "off")
                local.POST = 0;
            else
                exit(1);
        }
        else if(x == 1 && token == "DELETE")
        {
            if(line == "on")
                local.DELETE = 1;
            else if (line == "off")
                local.DELETE = 0;
            else
                exit(1);
        }
        else if(x == 1 && token == "index")
            local.index = check_path(line);
        else if(x == 1 && token == "return")
            local._return = check_path(line);
        else if(x == 1 && token == "alias")
            local.alias = check_path(line);
        // else if(x == 1 && token == "cgi")
        //     local.cgi.insert();
        else if (x == 1 && line.find("}") != std::string::npos) 
            break;
        else
            exit(102);
    }
    locations.push_back(local);
   // return (local);
}

int    check_listen(std::string  port)
{
    int i = 0;
    while(port[i])
    {
        if(!isdigit(port[i++]))
            exit(17);
    }
    if(port.length() > 6)
        exit(19);
    if(atoi(port.c_str()) > 65335 || atoi(port.c_str()) < 0)
        exit(18);
    return atoi(port.c_str());
}

int     check_http_code(std::string code)
{
    int i= -1;
    while(code[++i])
        if(!isdigit(code[i]))
            exit(88);
    int x = atoi(code.c_str());
    if(x < 0)
        exit(99);
    return(x);
}

std::string  check_host(std::string host)
{   
    int i = 0;
    int count = 0;
    int start = 0;
    std::string tmp;
    while(host[i])
    {
        if(!isdigit(host[i]) && host[i] != '.')
            exit(8);
        if(host[i] == '.')
        {
            count++;
            tmp =  host.substr(start,i - start);
            if(tmp.length() > 3)
                exit(9);
            if(atoi(tmp.c_str()) > 255 || atoi(tmp.c_str()) <  0)
                exit(10);
            start = i + 1;
        }
        i++;
    }
    if(count != 3)
        exit(11);
    return(host);
}

int max_body(std::string body)
{
    int i = -1;
    while (body[++i])
        if(!isdigit(body[i]))
            exit(1);
    if(i > 4 || i == 0)
        exit(1);
    return atoi(body.c_str());
}


std::vector<Server> mainf(int ac, char **av)
{
    std::vector<Server> servers;
    if(ac == 2)
    {   
        std::ifstream Myfile(av[1]);
        if (Myfile.is_open()) 
        {
            std::string line;
            while (std::getline(Myfile, line)) 
            {
                line.erase(line.find_last_not_of(" \t") + 1);
                if (line.empty() || line[0] == '#')
                    continue;
                std::string token;
                if (line == "server{") 
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
                            S1.listen.push_back((u_int16_t)check_listen(line));
                        else if(token == "host")
                            S1.host = check_host(line);
                        else if(token == "server_name")
                            S1.server_name.push_back(line);
                        else if(token == "error_page")
                        {
                            if(line[3] == ':')
                                S1.error_page.insert(std::make_pair(check_http_code(line.substr(0,3)), line.substr(4)));
                            else 
                                exit(1);
                        }
                        else if(token == "max_body")
                            S1.max_body = max_body(line);
                        else if(token == "root")
                            S1.root = check_path(line);
                        else if(token == "index")
                            S1.index = check_path(line);
                        else if(token == "location")
                            get_location(Myfile , line, S1.locations);
                        else if (line == "};") 
                            break;
                        else
                            exit(3);
                    }
                    if (line != "};") 
                    {
                        std::cout << "End of file reached.   | "<< line<< std::endl;
                        exit (1);
                    }
                    servers.push_back(S1);
                }
                else
                    exit(5);
            }    
        }
        else 
            exit(6);
    }
        return(servers);
}
int main(int ac, char **av)
{
    std::vector<Server> tab = mainf(ac, av);
    tab[0].print();
}




















