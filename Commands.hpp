#if !defined(COMMANDS_HPP)
#define COMMANDS_HPP

#include "ICommand.hpp"
#include <iostream>

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


template <CommandType::Type type>
class Command : public ICommand
{
	virtual void	execute(Client &sender, std::vector<std::string> arguments);
};

template <CommandType::Type type>
void	Command<type>::execute(Client &sender, std::vector<std::string> arguments)
{
	(void) sender;
	(void) arguments;
}

template<>
void	Command<CommandType::pass>::execute(Client &sender, std::vector<std::string> arguments)
{
	(void) sender;
	(void) arguments;
	// if (arguments.find(ArgumentType::password) != arguments.end())
	// 	std::cout << "Argument PASSWORD given" << std::endl;
	// else
	// 	std::cout << "No argument PASSWORD specified" << std::endl;
}

template<>
void	Command<CommandType::kick>::execute(Client &sender, std::vector<std::string> arguments)
{
	(void) sender;
	(void) arguments;
	// if (arguments.find(ArgumentType::user) != arguments.end())
	// 	std::cout << "User is specified" << std::endl;
	// else
	// 	std::cout << "User not specified" << std::endl;
}

#endif // COMMANDS_HPP
