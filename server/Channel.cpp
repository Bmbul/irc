#include "Channel.hpp"
#include "Exceptions.hpp"
#include "ClientManager.hpp"
#include <algorithm>
#include "MessageController.hpp"
#include "Server.hpp"
#include "Exceptions.hpp"

Channel::Channel() {}

Channel::~Channel() {}

void Channel::AddMember(const std::string &memberNick)
{
	if (members.size() == 0)
		SetAdmin(memberNick);
	Client addingClient = ClientManager::getManager()->getClient(memberNick);
	members.insert(std::pair<std::string, Client>(memberNick, addingClient));
}

void Channel::KickMember(const std::string &admin, const std::string &memberNick)
{
	//":" + admin_->getPrefix() + " KICK " + name_ + " " + client->getNick() + " :" + reason
	/* const Client &admin_class = ClientManager::getManager()->getClient(admin);
	std::string message = ":" + admin_class.GetFormattedText() + " KICK " + name + " " + memberNick + " : BYE!!!";
	std::map<std::string */
	ValidateCanModifyAdmin(admin, memberNick);
	LeaveMember(memberNick);
}

void	Channel::LeaveMember(const std::string &memberNick)
{
	ValidateAdminIsInChannel(memberNick);
	members.erase(memberNick);
	if (IsAdmin(memberNick))
		DeleteAdmin(memberNick);
}

void	Channel::LeaveIfMember(const std::string &memberNick)
{
	if (members.count(memberNick) != 0)
		LeaveMember(memberNick);
}

void Channel::MakeAdmin(const std::string &admin, const std::string &newAdmin)
{
	ValidateCanModifyAdmin(admin, newAdmin);
	SetAdmin(newAdmin);
}

void Channel::SetAdmin(const std::string &newAdmin)
{
	admins.push_back(newAdmin);
}

std::string	Channel::GetAdmin() { return *(admins.begin()); }

void Channel::RemoveFromAdmins(const std::string &admin, const std::string &removingAdmin)
{
	ValidateCanModifyAdmin(admin, removingAdmin);

	DeleteAdmin(removingAdmin);
}

void	Channel::DeleteAdmin(const std::string &removingAdmin)
{
	std::vector<std::string>::iterator it = std::find(admins.begin(), admins.end(), removingAdmin);
	admins.erase(it);
	if (admins.empty() && !members.empty())
		SetAdmin(members.begin()->first);
}


void Channel::ValidateCanModifyAdmin(const std::string &admin, const std::string &modifyingMember) const
{
	ValidateAdmin(admin);
	ValidateClientIsInServer(modifyingMember);
	ValidateClientIsInChannel(admin, modifyingMember);
}

void Channel::ValidateAdmin(const std::string &admin) const
{
	ValidateClientIsInServer(admin);
	ValidateAdminIsInChannel(admin);
	if (!IsAdmin(admin))
		throw ChannelOpPrivsNeeded(admin, this->name);
}

void Channel::ValidateClientIsInChannel(const std::string &admin, const std::string &client) const
{
	if (!HasMember(client))
		throw UserNotInChannel(admin, client, this->name);
}

void Channel::ValidateAdminIsInChannel(const std::string &admin) const
{
	if (!HasMember(admin))
		throw NotOnChannel(admin, this->name);
}

void Channel::ValidateClientIsInServer(const std::string &client) const
{
	if (!ClientManager::getManager()->HasClient(client))
		throw NoSuchNick(client, this->name);
}

bool Channel::IsAdmin(const std::string &memberNick) const
{
	std::vector<std::string>::iterator it = std::find(admins.begin(), admins.end(), memberNick);
	return (it != admins.end());
}

bool Channel::HasMember(const std::string &memberName) const
{
	return (members.count(memberName));
}

void Channel::Ban(const std::string &admin, const std::string &memberName)
{
	ValidateAdmin(admin);
	ValidateClientIsInServer(memberName);
	std::vector<int>::const_iterator it = std::find(bannedClients.begin(),
													bannedClients.end(), members[memberName].getSocket());
	if (it == bannedClients.end())
		bannedClients.push_back(members[memberName].getSocket());
}

void Channel::Unban(const std::string &admin, const std::string &memberName)
{
	ValidateAdmin(admin);
	ValidateClientIsInServer(memberName);
	ValidateClientIsInChannel(admin, memberName);

	int unbanSock = ClientManager::getManager()->GetClientSocket(memberName);
	std::vector<int>::iterator memberToUnban = std::find(bannedClients.begin(), bannedClients.end(), unbanSock);
	if (memberToUnban != bannedClients.end())
		bannedClients.erase(memberToUnban);
}

void Channel::Broadcast(const Client &sender,
	const std::string &message, const std::string &command) const
{
	for (std::map<std::string, Client>::const_iterator it = members.begin();
		it != members.end(); it++)
	{
		if (!(it->second == sender))
			sender.SendMessageFromChannel(it->second, command, this->name, message);
	}
}

void	Channel::SendChannelReply(const std::string &message) const
{
	Server *server = Server::getServer();
	for (std::map<std::string, Client>::const_iterator it = members.begin();
		it != members.end(); it++)
	{
		server->SendMessageToClient(it->second, message);
		std::cout << message <<std::endl;
	}
}

void	Channel::SendJoinReply(const Client &client) const
{
	Server *server = Server::getServer();
	std::string message_body;
	for (std::map<std::string, Client>::const_iterator it = members.begin(); it != members.end();it++)
	{
		std::string sign = " :+";
		if(IsAdmin(it->first))
			sign = " :@";
		message_body = "353 " + client.getNick() + " = " + "#" + name + sign + it->second.getNick() ;
		server->SendMessageToClient(client, message_body);
		std::cout << message_body <<std::endl;
	}
	//366 a2 #a :End of /NAMES list
	message_body = "366 " +  client.getNick() + " " + "#"+ name + " :End of /NAMES list";
	server->SendMessageToClient(client, message_body);
	std::cout << message_body <<std::endl;
}

void	Channel::SendWhoReply(const Client &client) const
{
	Server	*server = Server::getServer();
	std::string host = server->getHost();
	for (std::map<std::string, Client>::const_iterator it = members.begin(); it != members.end();it++)
	{
		std::string sign = " +";
		if(IsAdmin(it->first))
			sign = " @";
		std::string message_body = client.GetFormattedText() + " 352 " + name 
			+ " " + it->second.getName() + " " + host + " IRC Server "
				 + it->second.getNick() + " " + sign + " :0 " + it->second.getName();
		server->SendMessageToClient(client, message_body);
	}
}

void Channel::PrintData()
{
	std::cout << "CHANNEL: " << name << std::endl;
	std::cout << "PASSWORD: " << password << std::endl;
	std::cout << "MEMBERS: " << std::endl;
	for (std::map<std::string, Client>::iterator it = members.begin();
		it != members.end(); it++)
	{
		std::cout << "Nick: " << it->first << ", fd: " << it->second.getSocket() << std::endl;
	}
	std::cout << std::endl;
	std::cout << "ADMINS: " << std::endl;
	for (std::vector<std::string>::iterator it = admins.begin();
		it != admins.end(); it++)
	{
		std::cout << "Socket: " << *it << std::endl;
	}
	std::cout << "MODES: " << std::endl;
	std::cout << "WRITEONLY: " << (mode & ModeType::write_) << std::endl
		<< "READONLY: " << (mode & ModeType::read) << std::endl
		<< "INVITEONLY: " << (mode & ModeType::invite) << std::endl
		<< "PRIVATE: " << (mode & ModeType::private_) << std::endl;
 	std::cout << std::endl;
}

void	Channel::SetPassword(const std::string &_password)
{
	password = _password;
}

bool	Channel::CheckPassword(const std::string &_checkingPass) const
{
	return (password == _checkingPass);
}


int Channel::getMemberCount()
{
	return members.size();
}

Channel::Channel(std::string const &_name) : name(_name), password(""), mode(ModeType::none) { }

int Channel::HasMode(ModeType::Mode modeToCheck) const
{
	return (mode & modeToCheck);
}

void Channel::AddMode(ModeType::Mode newMode)
{
	mode |= newMode;
}

void Channel::RemoveMode(ModeType::Mode mode)
{
	this->mode ^= mode;
}

std::string	Channel::ModeInfo() const
{
	std::string modesString = "";

	std::cout <<"Password: " <<  password << std::endl;
	if (mode & ModeType::read)
		modesString += 'r';
	if (mode & ModeType::write_)
		modesString += 'w';
	if (mode & ModeType::invite)
		modesString += 'i';
	if (mode & ModeType::private_)
		modesString += 'k';
	if (mode & ModeType::private_)
		modesString += (" " + password);
	return (modesString);
}

void Channel::ChannelWhoResponse(Client const &client)
{

	/*"352 " + client->getNick() + " " + name_ + " " + (*it)->getUser() + " " + 
	(*it)->getHost() + " ft_irc " + (*it)->getNick() 
	+ " " + "H" + " :1 " + (*it)->getReal()*/
	std::string message;
	std::map<std::string,Client>::iterator it = members.begin();
	for (; it != members.end(); ++it)
	{
		message = "352 " + client.getNick() + " " + name + " " + it->second.getName() +
		" 127.0.0.1 irc " + it->second.getNick() + " H :1 " + it->second.getName();
		SendMessageToClient(client,message);
	}
	message = "315 " + client.getNick() + " " + name + " :End of WHO list";
	SendMessageToClient(client,message);
}

void Channel::ChannelJoinResponse(Client const &client)
{
	
	std::string message = client.GetFormattedText() + " JOIN " + "#" + name;
	std::map<std::string,Client>::iterator it = members.begin();
	if(it == members.end())
		std::cout << "errrorrr" << std::endl;
	for (; it != members.end() ; it++)
	{
		SendMessageWithSocket(it->second.getSocket(),message);
		std::cout << "sockett = "<<it->second.getSocket() << std::endl;
	}
	SendJoinReply(client);
}