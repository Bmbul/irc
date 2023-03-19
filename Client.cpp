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


bool Client::getIsPassed()const 
{
	return this->isPassed;
}

bool Client::getIsNicked()const 
{
	return this-> isNicked;
}

bool Client::getIsUsered()const 
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

std::vector<std::string> Client::split(std::string str, std::string delimiter)
{

 std::vector<std::string> values = std::vector<std::string>();

 size_t position;
 while ((position = str.find(delimiter)) != std::string::npos)
 {
  values.push_back(str.substr(0, position));
  str.erase(0, position + delimiter.length());
 }
 values.push_back(str);

 return values;
}