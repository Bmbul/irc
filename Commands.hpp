#if !defined(COMMANDS_HPP)
#define COMMANDS_HPP

#include "ICommand.hpp"
#include <iostream>
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
	(void) sender;
	(void) arguments;
	std::cout << "pass()" <<std::endl;
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
	{
		sender.setIsPassed(true);
	}
}

template<>
void	Command<CommandType::user>::execute(Client &sender,const std::vector<std::string> &arguments)
{
	if (sender.getIsPassed() == false)
	{
		throw NOTAUTHORIZED(sender.getNick(),sender.getName());
	}
	else if(sender.isDone())
	{
		throw AlreadyRegistered(sender.getNick());
	}
	else
	{
		sender.setName(arguments[0]);
		sender.setIsUsered(true);
	}
}
template<>
void	Command<CommandType::nick>::execute(Client &sender,const std::vector<std::string> &arguments)
{
	if (sender.getIsPassed() == false)
	{
		throw NOTAUTHORIZED(sender.getName(),sender.getNick());
	}
	else if (ClientManager::getManager()->HasClient(arguments[0]))
	{
		 throw NicknameInUse(arguments[0],sender.getNick());
	}
	else
	{
		sender.setNick(arguments[0]);
		sender.setIsNicked(true);
	}
}

template<>
void	Command<CommandType::ping>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;
	if(arguments.size() == 0)
		throw NeedMoreParams(sender.getNick(),"PING");
	//ClientManager *Client = ClientManager::getManager();
	MessageController *message = MessageController::getController();
	message->SendHelloMessage(sender);
	/****************************************************************************************************/
	//need to implement send ping message to send ping message to the client like a SendHelloMessage();
	/****************************************************************************************************/
}


template<>
void	Command<CommandType::pong>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;
	if(arguments.size() == 0)
		throw NeedMoreParams(sender.getNick(),"PONG");
	//ClientManager *Client = ClientManager::getManager();
	MessageController *message = MessageController::getController();
	message->SendHelloMessage(sender);
	/****************************************************************************************************/
	//need to implement send ping message to send ping message to the client like a SendHelloMessage();
	/****************************************************************************************************/
}


template<>
void	Command<CommandType::privmsg>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;
	MessageController *message_controller = MessageController::getController();
	ClientManager *client_managar = ClientManager::getManager();
	Server *server = Server::getServer();
	if(arguments.size() <= 1)
		throw NeedMoreParams(sender.getNick(),"PRIVMSG");
	for (size_t i = 0; i < arguments.size() - 1; i++)
	{
		if(message_controller->IsValidChannelName(arguments[i]))
		{
			if(server->HasChannel(arguments[i]))
			{
				Channel channel = server->getChannel(arguments[i]);
				channel.Broadcast(sender,arguments.back());
			}
		}
		else if(client_managar->HasClient(arguments[i]))
		{
			//message_controller->SendMessage(sender,client_managar->getClient(arguments[i]),arguments.back());
		}
		else
		{
					throw NoSuchChannel(sender.getNick(),arguments[i]);
		}
	}

}


template<>
void	Command<CommandType::notice>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;
	MessageController *message_controller = MessageController::getController();
	ClientManager *client_managar = ClientManager::getManager();
	Server *server = Server::getServer();
	if(arguments.size() <= 1)
		throw NeedMoreParams(sender.getNick(),"PRIVMSG");
	for (size_t i = 0; i < arguments.size() - 1; i++)
	{
		if(message_controller->IsValidChannelName(arguments[i]))
		{
			if(server->HasChannel(arguments[i]))
			{
				Channel channel = server->getChannel(arguments[i]);
				channel.Broadcast(sender,arguments.back());
			}
		}
		else if(client_managar->HasClient(arguments[i]))
		{
			//message_controller->SendMessage(sender,client_managar->getClient(arguments[i]),arguments.back(),"COMMAND",arguments.);
		}
		else
			return ;
	}
}


template<>
void	Command<CommandType::join>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;
	Server *server = Server::getServer();
	if(arguments.size() == 0)
		throw NeedMoreParams(sender.getNick(),"JOIN");
	if (server->HasChannel(arguments[0]))
	{
		Channel channel = server->getChannel(arguments[0]);
		channel.AddMember("",sender.getNick());
	}
	else
	{
		Channel Channel = server->getChannel(arguments[0]);
		Channel.MakeAdmin("ADMIN",sender.getNick());//what should i pass in 1st arguments?
	}

}


template<>
void	Command<CommandType::part>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;
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
			channel.MakeAdmin("ADMIN",channel.getNextMember().getNick());
			channel.RemoveMember("ADMIN",sender.getNick());
			return ;
		}
	}
	else
		channel.RemoveMember("ADMIN",sender.getNick());
	
}


template<>
void	Command<CommandType::kick>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;
	if(arguments.size() != 2)
		throw NeedMoreParams(sender.getNick(),"KICK");
	Server *server = Server::getServer();
	ClientManager *manager = ClientManager::getManager();
	if(server->HasChannel(arguments[1]) == false)
		throw NoSuchChannel(sender.getNick(),arguments[1]);
	if(manager->HasClient(arguments[0]) == false)
		throw NoSuchNick(sender.getNick(),arguments[0]);
	Channel channel = server->getChannel(arguments[1]);
	if(channel.IsAdmin(sender.getNick()))
		channel.RemoveMember("ADMIN",arguments[0]);
	else
	{
			//permition denied exeption !
			return ;
	}
	

}


template<>
void	Command<CommandType::quit>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;
	//validation !!!
	ClientManager *managar = ClientManager::getManager();
	managar->RemoveClient(managar->GetClientSocket(sender.getName()));
}


template<>
void	Command<CommandType::mode>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;
}

#endif // COMMANDS_HPP


