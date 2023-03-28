#include <fstream>
#include "CommandResponse.hpp"

template <CommandType::Type type>
void	Command<type>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;
}



template<>
void	Command<CommandType::pass>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) arguments;
	sender.setIsPassed(true);
}

template<>
void	Command<CommandType::user>::execute(Client &sender,const std::vector<std::string> &arguments)
{
	Server *server = Server::getServer();
	sender.setName(arguments[0]);
	sender.setIsUsered(true);
	if(sender.isDone())
		server->SendHelloMessage(sender);
}

template<>
void	Command<CommandType::nick>::execute(Client &sender,const std::vector<std::string> &arguments)
{
	Server *server = Server::getServer();
	sender.setNick(arguments[0]);
	if(sender.isDone())
		return ;
	sender.setIsNicked(true);
	if(sender.isDone())
		server->SendHelloMessage(sender);

}

template<>
void	Command<CommandType::ping>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	sender.SendPongMessage(arguments[0]);
}


template<>
void	Command<CommandType::pong>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	sender.SendPongMessage(arguments[0]);
}


template<>
void	Command<CommandType::privmsg>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	MessageController *messageController = MessageController::getController();
	ClientManager *clientManager = ClientManager::getManager();
	Server *server = Server::getServer();
	std::string MessageBody = "";
	for (size_t i = 1; i < arguments.size(); i++)
		MessageBody = MessageBody + arguments[i] + " ";
	
	std::vector<std::string> args = messageController->Split(arguments[0],",");
	for (size_t i = 0; i < args.size(); i++)
	{

		std::string channelName = messageController->GetChannelName(args[i]);
		if (server->HasChannel(channelName))
			server->getChannel(channelName).Broadcast(sender, MessageBody, "PRIVMSG");
		else
			sender.SendMessage(clientManager->getClient(args[i]),
				"PRIVMSG", MessageBody);
	}

}


template<>
void	Command<CommandType::notice>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		return ;
	MessageController *messageController = MessageController::getController();
	ClientManager *client_managar = ClientManager::getManager();
	Server *server = Server::getServer();
	
	if(arguments.size() <= 1)
		return ;
	std::vector<std::string> args = messageController->Split(arguments[0],",");
	std::string MessageBody = "";
	
	for (size_t i = 1; i < arguments.size(); i++)
		MessageBody = MessageBody + arguments[i] + " ";
	
	for (size_t i = 0; i < args.size(); i++)
	{
		bool isValidChannel = messageController->IsValidChannelName(args[i]);
		std::string channelName = messageController->GetChannelName(args[i]);
		if(isValidChannel && server->HasChannel(channelName))
		{
			Channel channel = server->getChannel(channelName);
			if (channel.HasMember(sender.getNick()))
				channel.Broadcast(sender, MessageBody, "NOTICE");
		}
		else if(client_managar->HasClient(args[i]))
		{
			sender.SendMessage(client_managar->getClient(args[i]),
				"NOTICE", arguments.back());
		}
		else return;
	}
}


template<>
void	Command<CommandType::join>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	Server *server = Server::getServer();
	MessageController *message = MessageController::getController();
	std::vector<std::string> args = message->Split(arguments[0],",");
	for (size_t i = 0; i < args.size(); i++)
	{
		std::string channelName = message->GetChannelName(args[i]);
		if (server->HasChannel(args[i]))
		{
			Channel &channel = server->getChannel(channelName);
			channel.AddMember(sender.getNick());
			channel.PrintData();
		}
		else
		{
			Channel &channel = server->getChannel(channelName);
			channel.AddMember(sender.getNick());
			channel.AddMode(ModeType::write_);
			if (arguments.size() > 1)
			{
				channel.SetPassword(arguments[1]);
				channel.AddMode(ModeType::private_);
			}
			channel.PrintData();
		}
		server->SendJoinMessage(sender ,channelName);
	}
}

template<>
void	Command<CommandType::part>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	validate(sender,arguments);
	MessageController *message = MessageController::getController();
	std::vector<std::string> args = message->Split(arguments[0],",");
	std::string channelName;
	for (size_t i = 0; i < args.size(); i++)
	{
		Server *server = Server::getServer();
		channelName = message->GetChannelName(args[i]);

		Channel &channel = server->getChannel(channelName);
		Server::getServer()->PartMessage(sender,channelName);
    	channel.LeaveMember(sender.getNick());
    	/* if(channel.getMemberCount() == 0)
        	server->removeChannel(channelName); */
	}
	
}

template<>
void	Command<CommandType::kick>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	validate(sender,arguments);
	Server *server = Server::getServer();
	std::string channelName = MessageController::getController()->GetChannelName(arguments[0]);
	Channel &channel = server->getChannel(channelName);
	server->KickMessage(ClientManager::getManager()->getClient(arguments[1]),channelName,sender.getNick());
	channel.KickMember(sender.getNick(),arguments[1]);
}


template<>
void	Command<CommandType::quit>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	ClientManager *manager = ClientManager::getManager();
	int socket = sender.getSocket();
	manager->CloseClient(socket, arguments[0]);
	manager->RemoveClient(socket);
}


template<>
void	Command<CommandType::mode>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	std::string channelName = MessageController::getController()->GetChannelName(arguments[0]);
	Server *server =  Server::getServer();
	Channel &channel = server->getChannel(channelName);

	for (size_t i = 1; i < arguments.size(); i++)
	{
		int sign = 0;
		char mode = arguments[i][1];
		ModeType::Mode type = ModeType::none;
		if(arguments[i][0] == '+')
			sign = 1;

		if(mode == 'W')
			type = ModeType::write_;
		else if(mode == 'R')
			type = ModeType::read;
		else if(mode == 'I')
			type = ModeType::invite;
		else if (mode == 'K')
		{
			type = ModeType::private_;
			channel.SetPassword(arguments[i + 1]);
		}
		else if(mode == 'O')
		{
			if(sign == 1) 
				channel.MakeAdmin(sender.getNick(),arguments[i + 1]);
			else
				channel.RemoveFromAdmins(sender.getNick(),arguments[i + 1]);
		}

		if (sign == 0)
			channel.RemoveMode(type);
		else
			channel.AddMode(type);
	}
}

template<>
void	Command<CommandType::who>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void)sender;
	(void)arguments;
	
}

template<>
void	Command<CommandType::bot>::execute(Client &sender, const std::vector<std::string> &arguments)
{

	std::string receiver;
	if (arguments.size() == 1)
		receiver = sender.getNick();
	else 
		receiver = arguments[1];
	Server::getServer()->SendMessageToBot(arguments[0] + " " + receiver);
}

template<>
void	Command<CommandType::botme>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) arguments;
	Server::getServer()->setBotDescriptor(sender.getSocket());
}