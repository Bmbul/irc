#include "Client.hpp"
#include <sys/socket.h>
#include "MessageController.hpp"

Client::Client()
    : name("default name")
{

}

Client::Client(std::string _name, int _socketfd)
    : isPassed(false), isUsered(false), isNicked(false), name(_name), fd(_socketfd)
{ }

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
	std::cout << "setting nick to " << nick << std::endl;
	this->nick = nick;
}

bool Client::isDone()
{
	if(isPassed && isUsered && isNicked)
		return true;
	return false;
}

void	Client::SendMessage(const Client &reciever,
	const std::string &commmand, const std::string message) const
{
	//:senderNickname!name@host COMMAND recieverNickname:message
	std::string finalizedMessage = ":" + MessageController::getController()->GetClientFormatedName(*this) + " "
		+ commmand + " " + reciever.getNick() + ":" + message;
	SendMessageToClient(reciever, finalizedMessage);
}

void	Client::SendMessageFromChannel(const Client &reciever, const std::string &command,
	const std::string &channel, const std::string message) const 
{
	//:senderNickname!name@host COMMAND recieverNickname:message
	std::string finalizedMessage = ":" + MessageController::getController()->GetClientFormatedName(*this) + " "
		+ command + " #" + channel  + ":" + message;
	SendMessageToClient(reciever, finalizedMessage);
}

bool	Client::operator==(const Client &rhs) const
{
	return (this->fd == rhs.fd);
}