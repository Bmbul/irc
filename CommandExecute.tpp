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
	sender.setName(arguments[0]);
	sender.setIsUsered(true);
}

template<>
void	Command<CommandType::nick>::execute(Client &sender,const std::vector<std::string> &arguments)
{
	
	validate(sender,arguments);
	sender.setNick(arguments[0]);
	sender.setIsNicked(true);
}

template<>
void	Command<CommandType::ping>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	validate(sender,arguments);
	MessageController *message = MessageController::getController();
	message->SendMessage(sender,sender,"PING","ping!");
}


template<>
void	Command<CommandType::pong>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	validate(sender,arguments);
	MessageController *message = MessageController::getController();
	message->SendMessage(sender,sender,"PONG","pong!");
}


template<>
void	Command<CommandType::privmsg>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	validate(sender,arguments);
	MessageController *message_controller = MessageController::getController();
	ClientManager *client_managar = ClientManager::getManager();
	Server *server = Server::getServer();
	std::string MessageBody = "";
	for (size_t i = 1; i < arguments.size(); i++)
		MessageBody = MessageBody + arguments[i] + " ";
	
	std::vector<std::string> args = message_controller->Split(arguments[0],",");
	for (size_t i = 0; i < args.size(); i++)
	{
		if(message_controller->IsValidChannelName(args[i]))
				server->getChannel(args[i]).Broadcast(sender, MessageBody, "PRIVMSG");
		else
			message_controller->SendMessage(sender,client_managar->getClient(args[i]),
				"PRIVMSG", MessageBody);
	}

}


template<>
void	Command<CommandType::notice>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	MessageController *message_controller = MessageController::getController();
	ClientManager *client_managar = ClientManager::getManager();
	Server *server = Server::getServer();
	if(arguments.size() <= 1)
		throw NeedMoreParams(sender.getNick(),"NOTICE");
	std::vector<std::string> args = message_controller->Split(arguments[0],",");
	for (size_t i = 0; i < args.size(); i++)
	{
		if(message_controller->IsValidChannelName(args[i]))
		{
			if(server->HasChannel(args[i]))
			{
				Channel channel = server->getChannel(args[i]);
				channel.Broadcast(sender, arguments.back(), "NOTICE");
			}
		}
		else if(client_managar->HasClient(args[i]))
		{
			message_controller->SendMessage(sender,client_managar->getClient(args[i]),
				"NOTICE", arguments.back());
		}
		else
		{
			//throw NoSuchChannel(sender.getNick(),args[i]);
			return ;
		}
	}
}


template<>
void	Command<CommandType::join>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	Server *server = Server::getServer();
	MessageController *message = MessageController::getController();

	if(arguments.size() == 0)
		throw NeedMoreParams(sender.getNick(),"JOIN");
	if(message->IsValidChannelName(arguments[0]))
		throw NoSuchChannel(sender.getNick(),arguments[0]);
        
	if (server->HasChannel(arguments[0]))
	{
		Channel &channel = server->getChannel(arguments[0]);
		channel.AddMember(sender.getNick());
	}
	else
	{
		Channel &channel = server->getChannel(arguments[0]);
		channel.AddMember(sender.getNick());
	}

}


template<>
void	Command<CommandType::part>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	validate(sender,arguments);
	Server *server = Server::getServer();
	if(server->HasChannel(arguments[0]) == false)
	{
		throw NoSuchChannel(sender.getNick(),arguments[0]);
	}
	Channel &channel = server->getChannel(arguments[0]);

    channel.LeaveMember(sender.getNick());
    if(channel.getMemberCount() == 0)
        server->removeChannel(arguments[0]);
}


template<>
void	Command<CommandType::kick>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	validate(sender,arguments);
	Server *server = Server::getServer();
	Channel channel = server->getChannel(arguments[1]);
	channel.KickMember(sender.getNick(),arguments[0]);
}


template<>
void	Command<CommandType::quit>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void)arguments;
	//validation !!!
	ClientManager *manager = ClientManager::getManager();
	int socket = sender.getSocket();
	std::cout << socket << std::endl;
	manager->CloseClient(socket);
	manager->RemoveClient(socket);
}


template<>
void	Command<CommandType::mode>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;
	
	//add mode 
	for (size_t i = 1; i < arguments.size(); i++)
	{
		//////////////////////////////////////
	}
	
	//std::cout <<"channel name is ===> " << channel_name << std::endl;
}
