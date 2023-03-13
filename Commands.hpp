#if !defined(COMMANDS_HPP)
#define COMMANDS_HPP

#include "ICommand.hpp"
#include <iostream>
#include "./Exceptions.hpp"
#include "./Server.hpp"

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
	if (Server::getServer()->getPass() != arguments[0].substr(arguments[0][0] == ':' ? 1 : 0) )
	{
		std::cerr << "WRONG PASSWORD" << std::endl;
	}
	else
	{
		sender.setIsPassed(true);
	}
	// if (arguments.find(ArgumentType::password) != arguments.end())
	// 	std::cout << "Argument PASSWORD given" << std::endl;
	// else
	// 	std::cout << "No argument PASSWORD specified" << std::endl;
}

template<>
void	Command<CommandType::user>::execute(Client &sender,const std::vector<std::string> &arguments)
{
	if (sender.get_isPassed() == false)
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
	if (sender.get_isPassed() == false)
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
	// if (arguments.find(ArgumentType::user) != arguments.end())
	// 	std::cout << "User is specified" << std::endl;
	// else
	// 	std::cout << "User not specified" << std::endl;
	
}


template<>
void	Command<CommandType::pong>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;
	// if (arguments.find(ArgumentType::user) != arguments.end())
	// 	std::cout << "User is specified" << std::endl;
	// else
	// 	std::cout << "User not specified" << std::endl;
}


template<>
void	Command<CommandType::privmsg>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;
	// if (arguments.find(ArgumentType::user) != arguments.end())
	// 	std::cout << "User is specified" << std::endl;
	// else
	// 	std::cout << "User not specified" << std::endl;
}


template<>
void	Command<CommandType::notice>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;
	// if (arguments.find(ArgumentType::user) != arguments.end())
	// 	std::cout << "User is specified" << std::endl;
	// else
	// 	std::cout << "User not specified" << std::endl;
}


template<>
void	Command<CommandType::join>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;
	// if (arguments.find(ArgumentType::user) != arguments.end())
	// 	std::cout << "User is specified" << std::endl;
	// else
	// 	std::cout << "User not specified" << std::endl;
}


template<>
void	Command<CommandType::part>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;
	// if (arguments.find(ArgumentType::user) != arguments.end())
	// 	std::cout << "User is specified" << std::endl;
	// else
	// 	std::cout << "User not specified" << std::endl;
}


template<>
void	Command<CommandType::kick>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;
	// if (arguments.find(ArgumentType::user) != arguments.end())
	// 	std::cout << "User is specified" << std::endl;
	// else
	// 	std::cout << "User not specified" << std::endl;
}


template<>
void	Command<CommandType::quit>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;
	// if (arguments.find(ArgumentType::user) != arguments.end())
	// 	std::cout << "User is specified" << std::endl;
	// else
	// 	std::cout << "User not specified" << std::endl;
}


template<>
void	Command<CommandType::mode>::execute(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;
	// if (arguments.find(ArgumentType::user) != arguments.end())
	// 	std::cout << "User is specified" << std::endl;
	// else
	// 	std::cout << "User not specified" << std::endl;
}

#endif // COMMANDS_HPP
