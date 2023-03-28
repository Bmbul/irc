#include "CommandResponse.hpp"
#include "Server.hpp"

void	CommandResponse::SendMessageToClient(const Client &client,
	const std::string &message) const
{
	SendMessageWithSocket(client.getSocket(), message);
}

void CommandResponse::SendJoinMessage(const Client &client, std::string const &channelName) const
{
	Channel &channel = Server::getServer()->getChannel(channelName);
	std::string message = ":" + client.getNick() + " JOIN " + channelName;
	std::string admin_reply = ":" + channel.GetAdmin() + " is a " + channelName + " ADMIN ";
	channel.Broadcast(client, message, "");
	SendMessageToClient(client, message);
	SendMessageToClient(client, admin_reply);
    channel.SendJoinReply(client);
	std::string end = ":366" + client.getNick() + " " + channelName + " :End of /NAMES list";
	SendMessageToClient(client, end);
}

void	CommandResponse::PartMessage(const Client &client,std::string const &channelName) const
{
    (void) client;
	Channel &channel = Server::getServer()->getChannel(channelName);
	std::string part_reply = " PART " + channelName;
    channel.SendChannelReply(part_reply);
}

void	CommandResponse::KickMessage(const Client &client, std::string const &channelName, std::string const &admin) const
{
    (void) client;
	Channel &channel = Server::getServer()->getChannel(channelName);
	std::string kick_reply = " KICK FROM " + channelName + " BY " + admin;
    channel.SendChannelReply(kick_reply);
}

void	CommandResponse::WhoMessage(const Client &client, const std::string &channelName) const
{
	Channel &channel = Server::getServer()->getChannel(channelName);
	channel.SendWhoReply(client);
	std::string endingString = client.GetFormattedText() + " 315 " + channelName + " :End of /WHO list";
	SendMessageToClient(client, endingString);
}