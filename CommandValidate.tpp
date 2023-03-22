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
		throw AlreadyRegistered(sender.getNick());
	if(arguments.size() == 0)
		throw NeedMoreParams(sender.getNick(),"PASS");
	if (Server::getServer()->getPass() != arguments[0])
		throw PasswordMissmatch(sender.getNick());
}

template<>
void	Command<CommandType::user>::validate(Client &sender,const std::vector<std::string> &arguments)
{
	if(sender.getIsPassed() == false)
		throw NotRegistered(sender.getNick());
	if(arguments.size() < 4)
		throw NeedMoreParams(sender.getNick(),"USER");
	if (sender.getIsPassed() == false)
		throw NOTAUTHORIZED(sender.getNick(),sender.getName());
	if(sender.isDone())
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
	if(sender.isDone() == 0)
		throw NOTAUTHORIZED(sender.getNick(),sender.getName());
}
 
template<>
void	Command<CommandType::pong>::validate(Client &sender,const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw NotRegistered(sender.getNick());
	if(arguments.size() == 0)
		throw NeedMoreParams(sender.getNick(),"PONG");
	if(sender.isDone() == 0)
		throw NOTAUTHORIZED(sender.getNick(),sender.getName());
}
 
template<>
void	Command<CommandType::privmsg>::validate(Client &sender,const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw NotRegistered(sender.getNick());
	MessageController *message_controller = MessageController::getController();
	ClientManager *client_managar = ClientManager::getManager();
	Server *server = Server::getServer();
	std::vector<std::string> args = message_controller->Split(arguments[0],",");
	if(arguments.size() <= 1)
		throw NeedMoreParams(sender.getNick(),"PRIVMSG");
	if(sender.isDone() == 0)
		throw NOTAUTHORIZED(sender.getNick(),sender.getName());
	for (size_t i = 0; i < args.size(); i++)
	{
		if(message_controller->IsValidChannelName(args[i]))
		{
			std::string channelName = args[i].at(0) == '#' ? args[i].substr(1,args[i].size() - 1) : args[i]; 
			if(server->HasChannel(channelName) == false)
				throw NoSuchChannel(sender.getNick(),args[i]);
		}
		else if(client_managar->HasClient(args[i]) == false)
			throw NoSuchNick(sender.getNick(),args[i]);
	}
}
 
/* template<>
void	Command<CommandType::notice>::validate(Client &sender,const std::vector<std::string> &arguments)
{
	
}*/
 
/* template<>
void	Command<CommandType::join>::validate(Client &sender,const std::vector<std::string> &arguments)
{
	
} */
 
template<>
void	Command<CommandType::part>::validate(Client &sender,const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw NotRegistered(sender.getNick());
	Server *server = Server::getServer();
	if(arguments.size() != 1)
		throw NeedMoreParams(sender.getNick(),"PART");
	std::vector<std::string> channels = MessageController::getController()->Split(arguments[0],",");
	for (size_t i = 0; i < channels.size(); i++)
	{
		std::string channelName = channels[i].at(0) == '#' ? channels[i].substr(1,channels[i].size() - 1) : channels[i];
		if(server->HasChannel(channelName) == false)
			throw NoSuchChannel(sender.getNick(),channels[i]);
	}
	for (size_t i = 0; i < channels.size(); i++)
	{
		std::string channelName = channels[i].at(0) == '#' ? channels[i].substr(1,channels[i].size() - 1) : channels[i];
		Channel channel = server->getChannel(channelName);
		if(channel.HasMember(sender.getNick()) == false)
			throw NotOnChannel(sender.getNick(),channelName);
	}
}
 
template<>
void	Command<CommandType::kick>::validate(Client &sender,const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw NotRegistered(sender.getNick());
	if(arguments.size() != 2)
		throw NeedMoreParams(sender.getNick(),"KICK");
	std::string ChannelName = arguments[1].at(0) == '#' ? arguments[1].substr(1,arguments[1].size() - 1) : arguments[1];
	Server *server = Server::getServer();
	if(server->HasChannel(ChannelName) == false)
		throw NoSuchChannel(sender.getNick(),ChannelName);
}
 
/* template<>
void	Command<CommandType::quit>::validate(Client &sender,const std::vector<std::string> &arguments)
{
	
} */

template<>
void	Command<CommandType::mode>::validate(Client &sender,const std::vector<std::string> &arguments)
{
	if(sender.isDone() == false)
		throw NotRegistered(sender.getNick());
	if(arguments.size() < 2)
		throw NeedMoreParams(sender.getNick(),"MODE");
	std::string channel_name = arguments[0].at(0) == '#' ? arguments[0].substr(1,arguments[0].length() - 1) : arguments[0];
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
		if(set != 'W' && set != 'R' && set != 'I')
			throw UnknownMode(sender.getNick(),channel.name);
	}
	
}