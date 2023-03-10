#include "Channel.hpp"

Channel::Channel() { }

Channel::~Channel() { }

void	Channel::AddMember(std::string memberNick)
{
	// Change this to exception
	if (!Server::getServer()->clientManager->HasClient(memberNick))
	{
		std::cout << "No such client" << std::endl;
	}
}
