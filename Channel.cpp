#include "Channel.hpp"
#include "Exceptions.hpp"

Channel::Channel() { }

Channel::~Channel() { }

void	Channel::AddMember(std::string memberNick)
{
	(void) memberNick;
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
}

void	RemoveMember(const std::string &memberNick, const std::string &reason = NULL)
{
	(void) memberNick;
	(void) reason;
}

bool	Channel::IsAdmin(const std::string &memberNick)
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