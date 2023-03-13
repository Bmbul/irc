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
		virtual const char* what() const throw ()
		{
			return errorMessage.c_str();
		}
	virtual ~IRCException() throw() { }
};

struct NeedMoreParams : public IRCException
{
	NeedMoreParams(const std::string& command)
		: IRCException(command + " :Not enough parameters") { }
};

struct AlreadyRegistered : public IRCException
{
	AlreadyRegistered() : IRCException(":You may not reregister") { }
};

struct NoNickNameGiven : public IRCException
{
	NoNickNameGiven() : IRCException(":No nickname given") { }
};

struct ErroneusNickname : public IRCException
{
	ErroneusNickname(const std::string& nickname)
		: IRCException(nickname + " :Erroneus nickname") { }
};

struct NicknameInUse : public IRCException
{
	NicknameInUse(const std::string& nickname)
		: IRCException(nickname + " :Nickname is already in use")  { }
};

struct NickColission : public IRCException
{
	NickColission(const std::string& nickname)
		: IRCException(nickname + " :Nickname collision KILL") { }
};

struct BannedFromChannel : public IRCException
{
	BannedFromChannel(const std::string& channelName)
		: IRCException(channelName + " :Cannot join channel (+b)") { }
};

struct InviteOnlyChannel : public IRCException
{

	InviteOnlyChannel(const std::string& channelName)
		: IRCException(channelName + " :Cannot join channel (+i)") { }
};

struct BadChannelKey : public IRCException
{
	BadChannelKey(const std::string& channelName)
		: IRCException(channelName + " :Cannot join channel (+k)") { }
};

struct ChannelIsFull : public IRCException
{

	ChannelIsFull(const std::string& channelName)
		: IRCException(channelName + " :Cannot join channel (+l)") { }
};

struct BadChannelMask : public IRCException
{
	BadChannelMask(const std::string& channelName)
		: IRCException(channelName + " :Cannot join channel (+k)") { }
};

struct NoSuchChannel : public IRCException
{
	NoSuchChannel(const std::string& channelName)
		: IRCException(channelName + " :No such channel") { }
};

struct TooManyChannels : public IRCException
{
	TooManyChannels(const std::string& channelName)
		: IRCException(channelName + " :You have joined too many channels") { }
};

struct NotOnChannel : public IRCException
{
	NotOnChannel(const std::string& channelName)
		: IRCException(channelName + " :You're not on that channel") { }
};

struct ChannelOpPrivsNeeded : public IRCException
{
	ChannelOpPrivsNeeded(const std::string& channelName)
		: IRCException(channelName + " :You're not channel operator") { }
};

struct NoSuchNick : public IRCException
{
	NoSuchNick(const std::string& channelName)
		: IRCException(channelName + " :No such nick/channel") { }
};

struct KeySet : public IRCException
{
	KeySet(const std::string& channelName)
		: IRCException(channelName + " :Channel key already set") { }
};

struct UnknownMode : public IRCException
{
	UnknownMode(const std::string& channelName)
		: IRCException(channelName + " :is unknown mode char to me") { }
};

struct UsersDontMatch : public IRCException
{
	UsersDontMatch() : IRCException(":Cant change mode for other users") { }
};

struct UModeUnknownFlag : public IRCException
{
	UModeUnknownFlag() : IRCException(":Unknown MODE flag") { }
};

struct NoRecipient : public IRCException
{
	NoRecipient(const std::string  &command)
		: IRCException(":No recipient given " + command) { }
};

struct NoTextToSend : public IRCException
{
	NoTextToSend() : IRCException(":No text to send") { }
};

struct CannotSendToChannel : public IRCException
{
	CannotSendToChannel(const std::string& channelName)
		: IRCException(channelName + " :Cannot send to channel") { }
};

struct NoTopLevel : public IRCException
{
	NoTopLevel(const std::string& mask)
		: IRCException(mask + " :No toplevel domain specified") { }
};

struct WildTopLevel : public IRCException
{
	WildTopLevel(const std::string& mask)
		: IRCException(mask + " :Wildcard in toplevel domain") { }
};

struct TooManyTargets : public IRCException
{
	TooManyTargets(const std::string& target)
		: IRCException(target + " :Duplicate recipients. No message delivered") { }
};

struct NoOrigin : public IRCException
{
	NoOrigin() : IRCException(":No origin specified") { }
};

struct NoSuchServer : public IRCException
{
	NoSuchServer(const std::string& serverName)
		: IRCException(serverName + " :No such server") { }
};

struct UnknownCommand : public IRCException
{
	UnknownCommand(const std::string& command)
		: IRCException(command + " :Unknown command") { }
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

/* 
	General Errors:
		421     ERR_UNKNOWNCOMMAND
					"<command> :Unknown command"

*/




#endif // EXCEPTIONS_HPP
