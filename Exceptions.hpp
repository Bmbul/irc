#if !defined(EXCEPTIONS_HPP)
#define EXCEPTIONS_HPP

#include <exception>
#include <iostream>

class IRCException : public std::exception
{
	protected:
		std::string errorMessage;
		IRCException(const std::string & _message) : errorMessage(_message) { }
	public:
		virtual const char* what() const throw()
		{
			return errorMessage.c_str();
		}
};

class NeedMoreParams : public IRCException
{
	NeedMoreParams(const std::string& command)
		: IRCException(command + " :Not enough parameters") { }
};

class AlreadyRegistered : public IRCException
{
	AlreadyRegistered() : IRCException(":You may not reregister") { }
};

class NoNickNameGiven : public IRCException
{
	NoNickNameGiven() : IRCException(":No nickname given") { }
};

class ErroneusNickname : public IRCException
{
	ErroneusNickname(const std::string& nickname)
		: IRCException(nickname + " :Erroneus nickname") { }
};

class NicknameInUse : public IRCException
{
	NicknameInUse(const std::string& nickname)
		: IRCException(nickname + " :Nickname is already in use")  { }
};

class NickColission : public IRCException
{
	NickColission(const std::string& nickname)
		: IRCException(nickname + " :Nickname collision KILL") { }
};

class BannedFromChannel : public IRCException
{
	BannedFromChannel(const std::string& channelName)
		: IRCException(channelName + " :Cannot join channel (+b)") { }
};

class InviteOnlyChannel : public IRCException
{

	InviteOnlyChannel(const std::string& channelName)
		: IRCException(channelName + " :Cannot join channel (+i)") { }
};

class BadChannelKey : public IRCException
{
	BadChannelKey(const std::string& channelName)
		: IRCException(channelName + " :Cannot join channel (+k)") { }
};

class ChannelIsFull : public IRCException
{

	ChannelIsFull(const std::string& channelName)
		: IRCException(channelName + " :Cannot join channel (+l)") { }
};

class BadChannelMask : public IRCException
{
	BadChannelMask(const std::string& channelName)
		: IRCException(channelName + " :Cannot join channel (+k)") { }
};

class NoSuchChannel : public IRCException
{
	NoSuchChannel(const std::string& channelName)
		: IRCException(channelName + " :No such channel") { }
};

class TooManyChannels : public IRCException
{
	TooManyChannels(const std::string& channelName)
		: IRCException(channelName + " :You have joined too many channels") { }
};

class NotOnChannel : public IRCException
{
	NotOnChannel(const std::string& channelName)
		: IRCException(channelName + " :You're not on that channel") { }
};

class ChannelOpPrivsNeeded : public IRCException
{
	ChannelOpPrivsNeeded(const std::string& channelName)
		: IRCException(channelName + " :You're not channel operator") { }
};

class NoSuchNick : public IRCException
{
	NoSuchNick(const std::string& channelName)
		: IRCException(channelName + " :No such nick/channel") { }
};

class KeySet : public IRCException
{
	KeySet(const std::string& channelName)
		: IRCException(channelName + " :Channel key already set") { }
};

class UnknownMode : public IRCException
{
	UnknownMode(const std::string& channelName)
		: IRCException(channelName + " :is unknown mode char to me") { }
};

class UsersDontMatch : public IRCException
{
	UsersDontMatch() : IRCException(":Cant change mode for other users") { }
};

class UModeUnknownFlag : public IRCException
{
	UModeUnknownFlag() : IRCException(":Unknown MODE flag") { }
};

class NoRecipient : public IRCException
{
	NoRecipient(const std::string  &command)
		: IRCException(":No recipient given " + command) { }
};

class NoTextToSend : public IRCException
{
	NoTextToSend() : IRCException(":No text to send") { }
};

class CannotSendToChannel : public IRCException
{
	CannotSendToChannel(const std::string& channelName)
		: IRCException(channelName + " :Cannot send to channel") { }
};

class NoTopLevel : public IRCException
{
	NoTopLevel(const std::string& mask)
		: IRCException(mask + " :No toplevel domain specified") { }
};

class WildTopLevel : public IRCException
{
	WildTopLevel(const std::string& mask)
		: IRCException(mask + " :Wildcard in toplevel domain") { }
};

class WildTopLevel : public IRCException
{
	WildTopLevel(const std::string& target)
		: IRCException(target + " :Duplicate recipients. No message delivered") { }
};

class NoOrigin : public IRCException
{
	NoOrigin() : IRCException(":No origin specified") { }
};

class NoSuchServer : public IRCException
{
	NoSuchServer(const std::string& serverName)
		: IRCException(serverName + " :No such server") { }
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
	ERR_CHANOPRIVSNEEDED+
	ERR_NOSUCHNICK+
	ERR_NOTONCHANNEL+
	ERR_KEYSET+
	RPL_BANLIST (not error)
	RPL_ENDOFBANLIST (not error)
	ERR_UNKNOWNMODE+
	ERR_NOSUCHCHANNEL+
	ERR_USERSDONTMATCH+
	RPL_UMODEIS (not error)
	ERR_UMODEUNKNOWNFLAG+
KICK
	ERR_NEEDMOREPARAMS+
	ERR_NOSUCHCHANNEL+
	ERR_BADCHANMASK+
	ERR_CHANOPRIVSNEEDED+
	ERR_NOTONCHANNEL+
PRIVMSG
	ERR_NORECIPIENT+
	ERR_NOTEXTTOSEND+
	ERR_CANNOTSENDTOCHAN+
	ERR_NOTOPLEVEL+
	ERR_WILDTOPLEVEL+
	ERR_TOOMANYTARGETS+
	ERR_NOSUCHNICK+
	RPL_AWAY (not error)
PING
	ERR_NOORIGIN+
	ERR_NOSUCHSERVER
PONG
	ERR_NOORIGIN+
	ERR_NOSUCHSERVER
*/


#endif // EXCEPTIONS_HPP
