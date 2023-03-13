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



int Client::getSocket() const { return (fd); }


bool Client::get_isPassed()const 
{
	return this->isPassed;
}

bool Client::get_isNicked()const 
{
	return this-> isNicked;
}

bool Client::get_isUsered()const 
{
	return this->isUsered;
}

void Client::setIsPassed(bool isPassed)
{
	this->isPassed = isPassed;
}
void Client::setIsUsered(bool is_usered)
{
	 this->isUsered = is_usered;
}
void Client::setIsNicked(bool is_Nicked)
{
	 this->isNicked = is_Nicked;
}
void Client::setName(std::string const &name)
{
	 this->name = name;
}
void Client::setNick(std::string const &nick)
{
	 this->nick = nick;
}

bool Client::isDone()
{
	if(isPassed && isUsered && isNicked)
		return true;
	return false;
}
