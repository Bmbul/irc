#if !defined(COMMANDS_HPP)
#define COMMANDS_HPP

#include "ICommand.hpp"
#include <iostream>
#include <string>
#include "Exceptions.hpp"
#include "Server.hpp"
#include "MessageController.hpp"
#include "ClientManager.hpp"

struct CommandType
{
	enum Type
	{
		pass,
		nick,
		user,
		ping,
		pong,
		privmsg,
		notice,
		join,
		part,
		kick,
		quit,
		mode
	};
};


// class ICommand;
template <CommandType::Type type>
class Command : public ICommand
{
	virtual void	execute(Client &sender, const std::vector<std::string> &arguments);
};

template <CommandType::Type type>
void	Command<type>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;
}

template<>
void	Command<CommandType::pass>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	std::cout << "pass()" <<std::endl;
	if(sender.getIsPassed())
		throw AlreadyRegistered(sender.getNick());
	if(arguments.size() == 0)
	{
		std::cerr << "need more param()"<<std::endl;
		throw NeedMoreParams(sender.getNick(),"PASS");
	}		
	std::cout << "[" << arguments[0] << "]" << "=" << "[" << Server::getServer()->getPass() << "]" << std::endl;
	if (Server::getServer()->getPass() != arguments[0])
	{
		throw PasswordMissmatch(sender.getNick());
	}
	else
		sender.setIsPassed(true);
}

template<>
void	Command<CommandType::user>::execute(Client &sender,const std::vector<std::string> &arguments)
{
	if(arguments.size() < 4)
		throw NeedMoreParams(sender.getNick(),"USER");
	if (sender.getIsPassed() == false)
		throw NOTAUTHORIZED(sender.getNick(),sender.getName());
	if(sender.isDone())
		throw AlreadyRegistered(sender.getNick());

	sender.setName(arguments[0]);
	sender.setIsUsered(true);
}

template<>
void	Command<CommandType::nick>::execute(Client &sender,const std::vector<std::string> &arguments)
{
	if (arguments.size() == 0)
		throw NoNickNameGiven(sender.getName());
	if (sender.getIsPassed() == false)
		throw NOTAUTHORIZED(sender.getName(),sender.getNick());
	if (ClientManager::getManager()->HasClient(arguments[0]))
		 throw NicknameInUse(arguments[0],sender.getNick());
	
	sender.setNick(arguments[0]);
	sender.setIsNicked(true);
}

template<>
void	Command<CommandType::ping>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	if(arguments.size() == 0)
		throw NeedMoreParams(sender.getNick(),"PING");
	if(sender.isDone() == 0)
		throw NOTAUTHORIZED(sender.getNick(),sender.getName());
	//ClientManager *Client = ClientManager::getManager();
	MessageController *message = MessageController::getController();
	message->SendMessage(sender,sender,"PING","ping!");
	/****************************************************************************************************/
	//need to implement send ping message to send ping message to the client like a SendHelloMessage();
	/****************************************************************************************************/
}


template<>
void	Command<CommandType::pong>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	if(arguments.size() == 0)
		throw NeedMoreParams(sender.getNick(),"PONG");
	if(sender.isDone() == 0)
		throw NOTAUTHORIZED(sender.getNick(),sender.getName());
	MessageController *message = MessageController::getController();
	message->SendMessage(sender,sender,"PONG","pong!");
}


template<>
void	Command<CommandType::privmsg>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	MessageController *message_controller = MessageController::getController();
	ClientManager *client_managar = ClientManager::getManager();
	Server *server = Server::getServer();
	if(arguments.size() <= 1)
		throw NeedMoreParams(sender.getNick(),"PRIVMSG");
	if(sender.isDone() == 0)
		throw NOTAUTHORIZED(sender.getNick(),sender.getName());
	std::vector<std::string> args = message_controller->Split(arguments[0],",");
	for (size_t i = 0; i < args.size(); i++)
	{
		if(message_controller->IsValidChannelName(args[i]))
		{
			if(server->HasChannel(args[i]))
			{
				Channel channel = server->getChannel(args[i]);
				channel.Broadcast(sender, arguments.back(), "PRIVMSG");
			}
		}
		else if(client_managar->HasClient(args[i]))
		{
			message_controller->SendMessage(sender,client_managar->getClient(args[i]),
				"PRIVMSG", arguments.back());
		}
		else
		{
			throw NoSuchChannel(sender.getNick(),args[i]);
		}
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
	{

		throw NoSuchChannel(sender.getNick(),arguments[0]);
	}
	if (server->HasChannel(arguments[0]))
	{
		std::cout <<  "HAS CHANNEL" << std::endl;
		Channel channel = server->getChannel(arguments[0]);
		channel.AddMember(sender.getNick());
	}
	else
	{
		Channel channel = server->getChannel(arguments[0]);
		channel.AddMember(sender.getNick());
	}

}


template<>
void	Command<CommandType::part>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	Server *server = Server::getServer();
	if(server->HasChannel(arguments[0]) == false)
		throw NoSuchChannel(sender.getNick(),arguments[0]);
	Channel channel = server->getChannel(arguments[0]);
	if (channel.IsAdmin(sender.getNick()))
	{
		if(channel.getMemberCount("member") == 1)
		{
			server->removeChannel(arguments[0]);
			return ;
		}
		if(channel.getMemberCount("admin") == 1)
		{
			channel.MakeAdmin(sender.getNick(),channel.getNextMember().getNick());
			channel.RemoveMember(channel.getNextMember().getNick(),sender.getNick());
			return ;
		}
	}
	else
		channel.RemoveMember(sender.getNick(),sender.getNick());
	
}


template<>
void	Command<CommandType::kick>::execute(Client &sender, const std::vector<std::string> &arguments)
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
	if(channel.IsAdmin(sender.getNick()))
		channel.RemoveMember(sender.getNick(),arguments[0]);
	else
	{
			//permition denied exeption !
			return ;
	}
	

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
	if(arguments.size() < 2)
		throw NeedMoreParams(sender.getNick(),"MODE");
	std::string channel_name = arguments[0].at(0) == '#' ? arguments[0].substr(1,arguments[0].length() - 1) : arguments[0];
	Server *server = Server::getServer();
	if(server->HasChannel(channel_name) == false)
		throw NoSuchChannel(sender.getNick(),channel_name);//???????????
	Channel channel = server->getChannel(channel_name);
	if(channel.IsAdmin(sender.getNick()) == false)
		throw UsersDontMatch(sender.getNick());
	//add mode 
	for (size_t i = 1; i < arguments.size(); i++)
	{
		//////////////////////////////////////
	}
	
	//std::cout <<"channel name is ===> " << channel_name << std::endl;
}

#endif // COMMANDS_HPP


