template <CommandType::Type type>
void	Command<type>::validate(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;

	throw std::exception();
}

template<>
void	Command<CommandType::pass>::validate(Client &sender, const std::vector<std::string> &arguments)
{
	if(sender.getIsPassed())
	{
		std::cout << "is passed!!!!!!" <<std::endl;
		throw AlreadyRegistered(sender.getNick());
	}
	if(arguments.size() == 0)
	{
		std::cout << "is sizeeee!!!!!!" <<std::endl;

		throw NeedMoreParams(sender.getNick(),"PASS");
	}
	if (Server::getServer()->getPass() != arguments[0])
	{
		std::cout << "PASSS ["<< Server::getServer()->getPass() << "]" << "arguments[0] = ["<< arguments[0] << "]" << "is true == " << (Server::getServer()->getPass() != arguments[0]) << std::endl;
		std::cout << "is misssmachhhhhhh!!!!!!" <<std::endl;
		throw PasswordMissmatch(sender.getNick());
	}
	
}

template<>
void	Command<CommandType::user>::validate(Client &sender,const std::vector<std::string> &arguments)
{
	if(sender.getIsPassed() == false)
		throw NotRegistered(sender.getNick());
	if(arguments.size() < 4)
		throw NeedMoreParams(sender.getNick(),"USER");
	if(sender.getIsUsered())
		throw AlreadyRegistered(sender.getNick());
}

template<>
void	Command<CommandType::nick>::validate(Client &sender,const std::vector<std::string> &arguments)
{
	if(sender.getIsPassed() == false)
		throw NotRegistered(sender.getNick());
	if (arguments.size() == 0)
		throw NoNickNameGiven(sender.getName());
	if (sender.getIsPassed() == false)
		throw NOTAUTHORIZED(sender.getName(),sender.getNick());
	if (ClientManager::getManager()->HasClient(arguments[0]))
		 throw NicknameInUse(arguments[0],sender.getNick());
}

template<>
void	Command<CommandType::ping>::validate(Client &sender,const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw NotRegistered(sender.getNick());
	if(arguments.size() == 0)
		throw NeedMoreParams(sender.getNick(),"PING");
}
 
template<>
void	Command<CommandType::pong>::validate(Client &sender,const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw NotRegistered(sender.getNick());
	if(arguments.size() == 0)
		throw NeedMoreParams(sender.getNick(),"PONG");
}
 
template<>
void	Command<CommandType::privmsg>::validate(Client &sender,const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw NotRegistered(sender.getNick());
	MessageController *messageController = MessageController::getController();
	ClientManager *client_managar = ClientManager::getManager();
	Server *server = Server::getServer();
	std::vector<std::string> args = messageController->Split(arguments[0],",");
	if(arguments.size() <= 1)
		throw NeedMoreParams(sender.getNick(),"PRIVMSG");
	if(sender.isDone() == 0)
		throw NOTAUTHORIZED(sender.getNick(),sender.getName());
	for (size_t i = 0; i < args.size(); i++)
	{
		if(messageController->IsValidChannelName(args[i]))
		{
			std::string channelName = messageController->GetChannelName(args[i]);
			if(server->HasChannel(channelName) == false)
				throw NoSuchChannel(sender.getNick(),args[i]);
			if(!(server->getChannel(channelName).GetMode() & ModeType::write_))
				throw CannotSendToChannel(sender.getNick(),channelName);
			if(!(server->getChannel(channelName).HasMember(sender.getNick())))
				throw NoSuchNick(sender.getNick(),"PRIVMSG");
		}
	 	else if(client_managar->HasClient(args[i]) == false)
			throw NoSuchNick(sender.getNick(),args[i]);
	}
}
 
template<>
void	Command<CommandType::join>::validate(Client &sender,const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw NotRegistered(sender.getNick());
	if(arguments.size() == 0)
		throw NeedMoreParams(sender.getNick(),"JOIN");
	Server *server =  Server::getServer();
	MessageController *messageController = MessageController::getController();
	std::vector<std::string> args = messageController->Split(arguments[0],",");
	for (size_t i = 0; i < args.size(); i++)
	{
		if (!messageController->IsValidChannelName(args[i]))
			throw BadChannelMask(sender.getNick(), args[i]);
		std::string channelName = messageController->GetChannelName(args[i]);
		if (server->HasChannel(channelName))
		{
			if (server->getChannel(channelName).GetMode() & ModeType::invite)
				throw InviteOnlyChannel(sender.getNick(),channelName);
		}
	}
}
 
template<>
void	Command<CommandType::part>::validate(Client &sender,const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw NotRegistered(sender.getNick());
	Server *server = Server::getServer();
	if(arguments.size() == 0)
		throw NeedMoreParams(sender.getNick(),"PART");
	MessageController *messageController = MessageController::getController();
	std::vector<std::string> channels = messageController->Split(arguments[0],",");
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (!messageController->IsValidChannelName(channels[i]))
			throw BadChannelMask(sender.getNick(), channels[i]);
		std::string channelName = messageController->GetChannelName(channels[i]);
		if(!server->HasChannel(channelName))
			throw NoSuchChannel(sender.getNick(),channels[i]);
		Channel channel = server->getChannel(channelName);
		if(channel.HasMember(sender.getNick()) == false)
			throw NotOnChannel(sender.getNick(),channelName);
	}
}
 
template<>
void	Command<CommandType::kick>::validate(Client &sender,const std::vector<std::string> &arguments)
{
	std::string chName = arguments[0];
	if(sender.isDone() == false)
		throw NotRegistered(sender.getNick());
	if(arguments.size() != 2)
		throw NeedMoreParams(sender.getNick(),"KICK");
	if (!MessageController::getController()->IsValidChannelName(chName))
		throw BadChannelMask(sender.getNick(), chName);
	std::string channelName = MessageController::getController()->GetChannelName(chName);
	Server *server = Server::getServer();
	if(server->HasChannel(channelName) == false)
		throw NoSuchChannel(sender.getNick(),channelName);
}
 
template<>
void	Command<CommandType::mode>::validate(Client &sender,const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw NotRegistered(sender.getNick());
	if(arguments.size() < 2)
		throw NeedMoreParams(sender.getNick(),"MODE");

	std::string channel_name = MessageController::getController()->GetChannelName(arguments[0]);
	Server *server = Server::getServer();
	if(server->HasChannel(channel_name) == false)
		throw NoSuchChannel(sender.getNick(),channel_name);//???????????
	Channel channel = server->getChannel(channel_name);
	if(channel.IsAdmin(sender.getNick()) == false)
		throw UsersDontMatch(sender.getNick());
	for (size_t i = 1; i < arguments.size(); i++)
	{
		if(arguments[i].size() != 2)
			throw UnknownMode(sender.getNick(),channel.name);
		char set =arguments[i].at(1);
		 if(arguments[i].at(0) != '+' || arguments[i].at(0) != '-')
			throw UnknownMode(sender.getNick(),channel.name);
		if(set != 'W' && set != 'R' && set != 'I' && set != 'O')
			throw UnknownMode(sender.getNick(),channel.name);
		if(set == 'O')
		{
			if(arguments[i + 1].size() == 0)
				throw NeedMoreParams(sender.getNick(),"MODE");
			if(channel.HasMember(arguments[i + 1]) == false)
				throw UserNotInChannel(sender.getName(),sender.getNick(),channel_name);
		}

	}
	
}

template<>
void	Command<CommandType::bot>::validate(Client &sender, const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw NotRegistered(sender.getNick());
	Server *server = Server::getServer();
	MessageController *controller = MessageController::getController();
	ClientManager *manager = ClientManager::getManager();
	if (arguments.size() == 0)
		throw NeedMoreParams(sender.getNick(), "/bot");
	if (!server->IsBotConnected())
		throw NoBotConnected(sender.getNick());
	if (arguments.size() == 1)
		return ;
	std::string channelName = controller->GetChannelName(arguments[1]);
	if (controller->IsValidChannelName(arguments[1]))
	{
		if (!server->HasChannel(channelName))
			throw NoSuchChannel(sender.getNick(), channelName);
		Channel &channel = server->getChannel(channelName);
		if (!channel.HasMember(sender.getNick()))
			throw UserNotInChannel(sender.getNick(), sender.getNick(), channelName);
	}
	else if (!manager->HasClient(arguments[1]))
		throw NoSuchNick(sender.getNick(), channelName);
}