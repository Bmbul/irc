#include "Client.hpp"

Client::Client()
    : name("default name")
{

}

Client::Client(std::string _name, int _socketfd)
    :  name(_name), fd(_socketfd)
{
	
}

Client::~Client()
{

}

std::string	Client::getName() const { return (name); }

std::string	Client::getNick() const { return (nick); }

void Client::setNick(const std::string &newNick)
{
	nick = newNick;
}

int Client::getSocket() const { return (fd); }