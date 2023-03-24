#include <fstream>

template <CommandType::Type type>
void	Command<type>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;

	throw std::exception();
}



template<>
void	Command<CommandType::pass>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	validate(sender, arguments);
	sender.setIsPassed(true);
}

template<>
void	Command<CommandType::user>::execute(Client &sender,const std::vector<std::string> &arguments)
{
	validate(sender, arguments);
	Server *server = Server::getServer();
	sender.setName(arguments[0]);
	sender.setIsUsered(true);
	if(sender.isDone())
		server->SendHelloMessage(sender);
}

template<>
void	Command<CommandType::nick>::execute(Client &sender,const std::vector<std::string> &arguments)
{
	validate(sender,arguments);
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
	validate(sender,arguments);
	sender.SendMessage(sender,"PING","pong!");
}


template<>
void	Command<CommandType::pong>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	validate(sender, arguments);
	sender.SendMessage(sender,"PONG","pong!");
}


template<>
void	Command<CommandType::privmsg>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	validate(sender, arguments);
	MessageController *messageController = MessageController::getController();
	ClientManager *clientManager = ClientManager::getManager();
	Server *server = Server::getServer();
	std::string MessageBody = "";
	for (size_t i = 1; i < arguments.size(); i++)
		MessageBody = MessageBody + arguments[i] + " ";
	
	std::vector<std::string> args = messageController->Split(arguments[0],",");
	for (size_t i = 0; i < args.size(); i++)
	{
		if(messageController->IsValidChannelName(args[i]))
		{
			std::string channelName = messageController->GetChannelName(args[i]);
			server->getChannel(channelName).Broadcast(sender, MessageBody, "PRIVMSG");
		}
		else
			sender.SendMessage(clientManager->getClient(args[i]),
				"PRIVMSG", MessageBody);
	}

}


template<>
void	Command<CommandType::notice>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw NotRegistered(sender.getNick());
	MessageController *messageController = MessageController::getController();
	ClientManager *client_managar = ClientManager::getManager();
	Server *server = Server::getServer();
	
	if(arguments.size() <= 1)
		throw NeedMoreParams(sender.getNick(),"NOTICE");
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
	validate(sender,arguments);
	Server *server = Server::getServer();
	MessageController *message = MessageController::getController();
	std::vector<std::string> args = message->Split(arguments[0],",");
	for (size_t i = 0; i < args.size(); i++)
	{
		if (server->HasChannel(args[i]))
		{
			std::string channelName = message->GetChannelName(args[i]);
			Channel &channel = server->getChannel(channelName);
			channel.AddMember(sender.getNick());
		}
		else
		{
			std::string channelName = message->GetChannelName(args[i]);
			Channel &channel = server->getChannel(channelName);
			channel.AddMember(sender.getNick());
			channel.AddMode(ModeType::write_);
		}
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
    	channel.LeaveMember(sender.getNick());
    	if(channel.getMemberCount() == 0)
        	server->removeChannel(channelName);
	}
	
}

template<>
void	Command<CommandType::kick>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	validate(sender,arguments);
	Server *server = Server::getServer();
	std::string channelName = MessageController::getController()->GetChannelName(arguments[0]);
	Channel &channel = server->getChannel(channelName);
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
	(void) sender;
	(void) arguments;
	std::string channelName = MessageController::getController()->GetChannelName(arguments[0]);
	Server *server =  Server::getServer();
	std::cout << "CHANNEL NAME: " << channelName << std::endl;
	Channel &channel = server->getChannel(channelName);
	//add mode 
	for (size_t i = 1; i < arguments.size(); i++)
	{
		if(arguments[i].at(0) == '+')
		{
			if(arguments[i].at(1) == 'W')
				channel.AddMode(ModeType::write_);
			else if(arguments[i].at(1) == 'R')
				channel.AddMode(ModeType::read);
			else if(arguments[i].at(1) == 'I')
				channel.AddMode(ModeType::invite);
			else if(arguments[i].at(1) == 'O')
			{
				channel.MakeAdmin(sender.getNick(),arguments[i + 1]);
			}
		}
		else if(arguments[i].at(0) == '-')
		{
			if(arguments[i].at(1) == 'W')
				channel.RemoveMode(ModeType::write_);
			else if(arguments[i].at(1) == 'R')
				channel.RemoveMode(ModeType::read);
			else if(arguments[i].at(1) == 'I')
				channel.RemoveMode(ModeType::invite);
			else if(arguments[i].at(1) == 'O')
				channel.RemoveFromAdmins(sender.getNick(),arguments[i + 1]);
		}
	}
}

template<>
void	Command<CommandType::ftp>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	std::string defaultFileName = "Makefile";
	if(sender.isDone() == false)
		throw NotRegistered(sender.getNick());
		
	std::ifstream input(arguments.size() >= 1 ? 
		arguments[0].c_str() : defaultFileName.c_str());
	
    if (!input) 
	{
		Server::getServer()->SendMessageToClient(sender, "Cannot Open the given File");
        return ;
    }

    std::string line;
    while (std::getline(input, line)) 
	{
		Server::getServer()->SendMessageToClient(sender, line);
    }
}

template<>
void	Command<CommandType::cap>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void)sender;
	(void)arguments;
}

template<>
void	Command<CommandType::bot>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	validate(sender, arguments);

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