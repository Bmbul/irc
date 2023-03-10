#if !defined(EXCEPTIONS_HPP)
#define EXCEPTIONS_HPP

#include <exception>
#include <iostream>

class IRCException : public std::exception
{
	public:
		virtual const char* what() const throw()
		{
			return "IRC Exception!!";
		}
};

class NeedMoreParams : public IRCException
{
	std::string errorMessage;

	NeedMoreParams(const std::string& command)
		: errorMessage(command + " :Not enough parameters") { }

	virtual const char* what() const throw()
	{
		return errorMessage.c_str();
	}
};

class AlreadyRegistered : public IRCException
{
	virtual const char* what() const throw()
	{
		return ":You may not reregister";
	}
};

class NoNickNameGiven : public IRCException
{
	virtual const char* what() const throw()
	{
		return ":No nickname given";
	}
};

class ErroneusNickname : public IRCException
{
	std::string errorMessage;
	
	ErroneusNickname(const std::string& nickname)
		: errorMessage(nickname + " :Erroneus nickname") { }

	virtual const char* what() const throw()
	{
		return errorMessage.c_str();
	}
};

class NicknameInUse : public IRCException
{
	std::string errorMessage;

	NicknameInUse(const std::string& nickname)
		: errorMessage(nickname + " :Nickname is already in use")  { }
	virtual const char* what() const throw()
	{
		return errorMessage.c_str();
	}
};

class NickColission : public IRCException
{
	std::string errorMessage;
	NickColission(const std::string& nickname)
		: errorMessage(nickname + " :Nickname collision KILL") { }
	virtual const char* what() const throw()
	{
		return errorMessage.c_str();
	}
};

class BannedFromChannel : public IRCException
{
	std::string errorMessage;
	BannedFromChannel(const std::string& channelName)
		: errorMessage(channelName + " :Cannot join channel (+b)") { }
	virtual const char* what() const throw()
	{
		return errorMessage.c_str();
	}
};

class InviteOnlyChannel : public IRCException
{
	std::string errorMessage;
	InviteOnlyChannel(const std::string& channelName)
		: errorMessage(channelName + " :Cannot join channel (+i)") { }
	virtual const char* what() const throw()
	{
		return errorMessage.c_str();
	}
};

class BadChannelKey : public IRCException
{
	std::string errorMessage;
	BadChannelKey(const std::string& channelName)
		: errorMessage(channelName + " :Cannot join channel (+k)") { }
	virtual const char* what() const throw()
	{
		return errorMessage.c_str();
	}
};

class ChannelIsFull : public IRCException
{
	std::string errorMessage;
	ChannelIsFull(const std::string& channelName)
		: errorMessage(channelName + " :Cannot join channel (+l)") { }
	virtual const char* what() const throw()
	{
		return errorMessage.c_str();
	}
};

class BadChannelMask : public IRCException
{
	std::string errorMessage;
	BadChannelMask(const std::string& channelName)
		: errorMessage(channelName + " :Cannot join channel (+k)") { }
	virtual const char* what() const throw()
	{
		return errorMessage.c_str();
	}
};

class NoSuchChannel : public IRCException
{
	std::string errorMessage;
	NoSuchChannel(const std::string& channelName)
		: errorMessage(channelName + " :No such channel") { }
	virtual const char* what() const throw()
	{
		return errorMessage.c_str();
	}
};

class TooManyChannels : public IRCException
{
	std::string errorMessage;
	TooManyChannels(const std::string& channelName)
		: errorMessage(channelName + " :You have joined too many channels") { }
	virtual const char* what() const throw()
	{
		return errorMessage.c_str();
	}
};

class NotOnChannel : public IRCException
{
	std::string errorMessage;
	NotOnChannel(const std::string& channelName)
		: errorMessage(channelName + " :You're not on that channel") { }
	virtual const char* what() const throw()
	{
		return errorMessage.c_str();
	}
};

// All Possible Exceptions
/*
PASS
	ERR_NEEDMOREPARAMS +
	ERR_ALREADYREGISTRED +
NICK
	ERR_NONICKNAMEGIVEN +
	ERR_ERRONEUSNICKNAME +
	ERR_NICKNAMEINUSE+
	ERR_NICKCOLLISION+
USER
	ERR_NEEDMOREPARAMS+
	ERR_ALREADYREGISTRED+
QUIT
	...
JOIN
	ERR_NEEDMOREPARAMS+
	ERR_BANNEDFROMCHAN+
	ERR_INVITEONLYCHAN+
	ERR_BADCHANNELKEY+
	ERR_CHANNELISFULL+
	ERR_BADCHANMASK+
	ERR_NOSUCHCHANNEL+
	ERR_TOOMANYCHANNELS+
	RPL_TOPIC (not error)
PART
	ERR_NEEDMOREPARAMS+
	ERR_NOSUCHCHANNEL+
	ERR_NOTONCHANNEL+
MODE
	ERR_NEEDMOREPARAMS+
	RPL_CHANNELMODEIS (not error)
	ERR_CHANOPRIVSNEEDED
	ERR_NOSUCHNICK
	ERR_NOTONCHANNEL+
	ERR_KEYSET
	RPL_BANLIST (not error)
	RPL_ENDOFBANLIST (not error)
	ERR_UNKNOWNMODE
	ERR_NOSUCHCHANNEL+
	ERR_USERSDONTMATCH
	RPL_UMODEIS (not error)
	ERR_UMODEUNKNOWNFLAG
KICK
	ERR_NEEDMOREPARAMS+
	ERR_NOSUCHCHANNEL+
	ERR_BADCHANMASK+
	ERR_CHANOPRIVSNEEDED
	ERR_NOTONCHANNEL+
PRIVMSG
	ERR_NORECIPIENT
	ERR_NOTEXTTOSEND
	ERR_CANNOTSENDTOCHAN
	ERR_NOTOPLEVEL
	ERR_WILDTOPLEVEL
	ERR_TOOMANYTARGETS
	ERR_NOSUCHNICK
	RPL_AWAY
PING
	ERR_NOORIGIN
	ERR_NOSUCHSERVER
PONG
	ERR_NOORIGIN
	ERR_NOSUCHSERVER
*/


#endif // EXCEPTIONS_HPP
