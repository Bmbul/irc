#include "Client.hpp"

Client::Client() { }

Client::Client(std::string _name) : name(_name)
{
	
}

Client::~Client()
{

}

std::string	Client::getName() { return (name); }

std::string	Client::getNick() { return (nick); }