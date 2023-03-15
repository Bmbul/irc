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
		throw NeedMoreParams("PASS");
	}
	std::cout << "[" << arguments[0] << "]" << "=" << "[" << Server::getServer()->getPass() << "]" << std::endl;
	if (Server::getServer()->getPass() != arguments[0])
	{
		std::cerr << "WRONG PASSWORD" << std::endl;
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
		throw NOTAUTHORIZED(sender.getName());
	}
	else if(sender.isDone())
	{
		throw AlreadyRegistered();
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
		//throw NOTAUTHORIZED(sender.getName());
		std::cerr << "NOT AUTHORIZED" << std::endl;
		return;
	}
	else if (ClientManager::getManager()->HasClient(arguments[0]))
	{
		std::cerr << "nick name already exist" << std::endl;
		return;
		/* throw NicknameInUse(arguments[0]); */
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
		throw NeedMoreParams("too few params");
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
		throw NeedMoreParams("too few params");
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
		throw NeedMoreParams("PRIVMSG");
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
			message_controller->SendMessage(sender,client_managar->getClient(arguments[i]),arguments.back());
		}
		else
		{
					std::cerr << "no such a channel/client" <<std::endl;
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
		throw NeedMoreParams("PRIVMSG");
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
			message_controller->SendMessage(sender,client_managar->getClient(arguments[i]),arguments.back());
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
		throw NeedMoreParams("JOIN");
	if (server->HasChannel(arguments[0]))
	{
		Channel channel = server->getChannel(arguments[0]);
		channel.AddMember("",sender.getNick());
	}
	else
	{
		return ;
	}

}


template<>
void	Command<CommandType::part>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;
}


template<>
void	Command<CommandType::kick>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;
}


template<>
void	Command<CommandType::quit>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;
}


template<>
void	Command<CommandType::mode>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;
}

#endif // COMMANDS_HPP
