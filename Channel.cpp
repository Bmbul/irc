#include "Channel.hpp"
#include "Exceptions.hpp"
#include "ClientManager.hpp"
#include <algorithm>
#include "MessageController.hpp"
#include "Server.hpp"

Channel::Channel() { }

Channel::~Channel() { }


// Change this to exception
// ERR_NEEDMOREPARAMS+
// ERR_BANNEDFROMCHAN+
// ERR_INVITEONLYCHAN+
// ERR_BADCHANNELKEY+
// ERR_CHANNELISFULL+
// ERR_BADCHANMASK+
// ERR_NOSUCHCHANNEL+
// ERR_TOOMANYCHANNELS+
// RPL_TOPIC (not error)

void	Channel::AddMember(const std::string &admin, const std::string &memberNick)
{
	ValidateCanAdd(admin, memberNick);
	Client	addingClient =  ClientManager::getManager()->getClient(memberNick);
	members.insert(std::pair<std::string, Client>(memberNick, addingClient));
}

void	Channel::RemoveMember(const std::string &admin, const std::string &memberNick)
{
	ValidateCanRemove(admin, memberNick);
	members.erase(memberNick);
}

void	Channel::ValidateCanAdd(const std::string &admin, const std::string &newMember) const
{
	ValidateAdmin(admin);
	ValidateClientIsInServer(newMember);

	if (HasMember(newMember))
	{
		std::cout << "Some (other x 3) kind of error!!" << std::endl;
	}
}


void	Channel::ValidateCanRemove(const std::string &admin, const std::string &removingMember) const
{
	ValidateAdmin(admin);
	ValidateClientIsInServer(removingMember);
	if (!HasMember(removingMember))
	{
		std::cout << "Some (other x 3) kind of error!!" << std::endl;
	}
}

void	Channel::ValidateAdmin(const std::string &admin) const
{
	if (!HasMember(admin))
	{
		std::cout << "Some other kind of error!!" << std::endl;
	}
	if (!IsAdmin(admin))
	{
		std::cout << "Some kind of error!!" << std::endl;
	}
}

void	Channel::ValidateClientIsInServer(const std::string &client) const
{
	if (!ClientManager::getManager()->HasClient(client))
	{
		std::cout << "Some other other kind of error!!" << std::endl;
	}
}


bool	Channel::IsAdmin(const std::string &memberNick) const
{
	return (adminsMap[memberNick]);
}

bool Channel::HasMember(const std::string &memberName) const
{
	return (members.count(memberName));
}

void	Channel::Ban(const std::string &memberName)
{
	if (members.count(memberName) > 0)
	{
		bannedClients.push_back(members[memberName].getSocket());
	}
	else 
	{
		std::cout << "No such member in the Channel" << std::endl;
	}
}

void	Channel::Unban(const std::string &memberName)
{
	// should be changed to exceptions
	if (!ClientManager::getManager()->HasClient(memberName))
	{
		std::cout << "No Such Client in the Server" << std::endl;
		return ;
	}
	if (!HasMember(memberName))
	{
		std::cout << "No Such Client in the Channel" << std::endl;
		return ;
	}
	int unbanSock = ClientManager::getManager()->GetClientSocket(memberName);
	std::vector<int>::iterator	memberToUnban = std::find(bannedClients.begin(), bannedClients.end(), unbanSock);
	if (memberToUnban != bannedClients.end())
		bannedClients.erase(memberToUnban);
	else
		std::cout << "The member was not banned at all" << std::endl;
}

void	Channel::Broadcast(const Client &sender, const std::string &message)
{
	for(std::map<std::string, Client>::iterator it; it != members.end(); it++)
	{
		MessageController::getController()->SendMessage(sender, it->second, message);
	}
}