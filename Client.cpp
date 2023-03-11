#include "Client.hpp"
#include "Server.hpp"

Client::Client()
    : name("default name")
{

}

Client::Client(std::string _name)
    :  name(_name)
{
	
}

Client::~Client()
{

}

std::string	Client::getName() { return (name); }

std::string	Client::getNick() { return (nick); }

// void    Client::SentToServer(Server *server, std::string request)
// {
//     if (isAuthenticated)
//     {
//         std::cout << "authenticated: " << request << std::endl;
//         if (server->TryToAuthenticate(request))
//             isAuthenticated = true;
//     }
//     else
//     {
//         std::cout << "not authenticated: " << request << std::endl;
//     }
// }