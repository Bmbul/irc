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
	if(arguments.size() == 0)
		throw NeedMoreParams(sender.getNick(),"PING");
	if(sender.isDone() == 0)
		throw NOTAUTHORIZED(sender.getNick(),sender.getName());
}
 
template<>
void	Command<CommandType::pong>::validate(Client &sender,const std::vector<std::string> &arguments)
{
	if(arguments.size() == 0)
		throw NeedMoreParams(sender.getNick(),"PONG");
	if(sender.isDone() == 0)
		throw NOTAUTHORIZED(sender.getNick(),sender.getName());
}
 
template<>
void	Command<CommandType::privmsg>::validate(Client &sender,const std::vector<std::string> &arguments)
{
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
			if(server->HasChannel(args[i]) == false)
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
	Server *server = Server::getServer();
	if(arguments.size() != 1)
		throw NeedMoreParams(sender.getNick(),"PART");
	std::vector<std::string> channels = MessageController::getController()->Split(arguments[0],",");
	for (size_t i = 0; i < channels.size(); i++)
	{
		if(server->HasChannel(channels[i]) == false)
			throw NoSuchChannel(sender.getNick(),channels[i]);
	}
	for (size_t i = 0; i < channels.size(); i++)
	{
		Channel channel = server->getChannel(channels[i]);
		if(channel.HasMember(sender.getNick()) == false)
			throw NotOnChannel(sender.getNick(),channels[i]);
	}
}
 
template<>
void	Command<CommandType::kick>::validate(Client &sender,const std::vector<std::string> &arguments)
{
	if(arguments.size() != 2)
		throw NeedMoreParams(sender.getNick(),"KICK");
	Server *server = Server::getServer();
	ClientManager *manager = ClientManager::getManager();
	if(server->HasChannel(arguments[1]) == false)
		throw NoSuchChannel(sender.getNick(),arguments[1]);
	if(manager->HasClient(arguments[0]) == false)
		throw NoSuchNick(sender.getNick(),arguments[0]);
	Channel channel = server->getChannel(arguments[1]);
	if(channel.HasMember(arguments[0]) == false)
		throw NotOnChannel(sender.getNick(),arguments[1]);
	if(channel.IsAdmin(sender.getNick()) == false)
		throw ChannelOpPrivsNeeded(sender.getNick(),arguments[1]);
}
 
/* template<>
void	Command<CommandType::quit>::validate(Client &sender,const std::vector<std::string> &arguments)
{
	
} */

template<>
void	Command<CommandType::mode>::validate(Client &sender,const std::vector<std::string> &arguments)
{
	if(arguments.size() < 2)
		throw NeedMoreParams(sender.getNick(),"MODE");
	std::string channel_name = arguments[0].at(0) == '#' ? arguments[0].substr(1,arguments[0].length() - 1) : arguments[0];
	Server *server = Server::getServer();
	if(server->HasChannel(channel_name) == false)
		throw NoSuchChannel(sender.getNick(),channel_name);//???????????
	Channel channel = server->getChannel(channel_name);
	if(channel.IsAdmin(sender.getNick()) == false)
		throw UsersDontMatch(sender.getNick());
}