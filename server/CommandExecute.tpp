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
	sender.setIsNicked(true);
	if(sender.isDone())
		server->SendHelloMessage(sender);

}

template<>
void	Command<CommandType::ping>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	validate(sender,arguments);
	sender.SendMessage(sender,"PING","ping!");
}


template<>
void	Command<CommandType::pong>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	validate(sender,arguments);
	sender.SendMessage(sender,"PONG","pong!");
}


template<>
void	Command<CommandType::privmsg>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	validate(sender,arguments);
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
		if(messageController->IsValidChannelName(args[i]))
		{
			if(server->HasChannel(args[i]))
			{
				std::string channelName = messageController->GetChannelName(args[i]);
				Channel channel = server->getChannel(channelName);
				channel.Broadcast(sender, MessageBody, "NOTICE");
			}
		}
		else if(client_managar->HasClient(args[i]))
		{
			sender.SendMessage(client_managar->getClient(args[i]),
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
	validate(sender,arguments);
	Server *server = Server::getServer();
	MessageController *message = MessageController::getController();
	std::vector<std::string> args = message->Split(arguments[0],",");
	for (size_t i = 0; i < args.size(); i++)
	{
		if (server->HasChannel(args[i]))
		{
		
			Channel &channel = server->getChannel(args[i]);
			channel.AddMember(sender.getNick());
		}
		else
		{
			Channel &channel = server->getChannel(args[i]);
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
	for (size_t i = 0; i < args.size(); i++)
	{
		Server *server = Server::getServer();
		if(!server->HasChannel(args[i]))
			throw NoSuchChannel(sender.getNick(),args[i]);
			
		Channel &channel = server->getChannel(args[i]);

    	channel.LeaveMember(sender.getNick());
    	if(channel.getMemberCount() == 0)
        	server->removeChannel(args[i]);
		
	}
	
}

template<>
void	Command<CommandType::kick>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	validate(sender,arguments);
	Server *server = Server::getServer();
	std::string channelName = MessageController::getController()->GetChannelName(arguments[1]);
	Channel &channel = server->getChannel(channelName);
	channel.KickMember(sender.getNick(),arguments[0]);
}


template<>
void	Command<CommandType::quit>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void)arguments;
	if(sender.isDone() == false)
		throw NotRegistered(sender.getNick());
	//validation !!!
	ClientManager *manager = ClientManager::getManager();
	int socket = sender.getSocket();
	manager->CloseClient(socket);
	manager->RemoveClient(socket);
}


template<>
void	Command<CommandType::mode>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;
	std::string channelName = MessageController::getController()->GetChannelName(arguments[0]);
	Server *server =  Server::getServer();
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
		}
		else if(arguments[i].at(0) == '-')
		{
			if(arguments[i].at(1) == 'W')
				channel.RemoveMode(ModeType::write_);
			else if(arguments[i].at(1) == 'R')
				channel.RemoveMode(ModeType::read);
			else if(arguments[i].at(1) == 'I')
				channel.RemoveMode(ModeType::invite);
		}
		
	}
	
	//std::cout <<"channel name is ===> " << channelName << std::endl;
}

#include <fstream>

template<>
void	Command<CommandType::ftp>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	std::string defaultFileName = "Makefile";
	if(sender.isDone() == false)
		throw NotRegistered(sender.getNick());
	//validation !!!
	std::ifstream input(arguments.size() >= 1 ? arguments[0] : defaultFileName);
	
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
