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
	virtual void	validate(Client &sender,const std::vector<std::string> &arguments);

};

template class Command<CommandType::pass>;
template class Command<CommandType::nick>;
template class Command<CommandType::user>;
template class Command<CommandType::ping>;
template class Command<CommandType::pong>;
template class Command<CommandType::privmsg>;
template class Command<CommandType::notice>;
template class Command<CommandType::join>;
template class Command<CommandType::part>;
template class Command<CommandType::kick>;
template class Command<CommandType::quit>;
template class Command<CommandType::mode>;


#include "CommandValidate.tpp"
#include "CommandExecute.tpp"

#endif // COMMANDS_HPP


