#if !defined(COMMAND_HANDLER_HPP)
#define COMMAND_HANDLER_HPP

#include "Server.hpp"
#include "Commands.hpp"
#include <vector>

class CommandHandler
{
	private:
		std::map<std::string, ICommand *> commands;
		std::map<std::string, ICommand *>::iterator it;

		void	InitilizeCommands();
		void	ClearCommands();

	public:
		void	ExecuteCommand(Client &client, const CommandData &data);
		CommandHandler();
		~CommandHandler();
};

CommandHandler::CommandHandler()
{
	InitilizeCommands();
}

CommandHandler::~CommandHandler()
{
	ClearCommands();
}

void	CommandHandler::InitilizeCommands()
{
	commands["PASS"] = new Command<CommandType::pass>();
	commands["NICK"] = new Command<CommandType::nick>();
	commands["USER"] = new Command<CommandType::user>();
	commands["PING"] = new Command<CommandType::ping>();
	commands["PONG"] = new Command<CommandType::pong>();
	commands["PRIVMSG"] = new Command<CommandType::privmsg>();
	commands["NOTICE"] = new Command<CommandType::notice>();
	commands["JOIN"] = new Command<CommandType::join>();
	commands["PART"] = new Command<CommandType::part>();
	commands["KICK"] = new Command<CommandType::kick>();
	commands["QUIT"] = new Command<CommandType::quit>();
	commands["MODE"] = new Command<CommandType::mode>();
}

void	CommandHandler::ClearCommands()
{
	for (it = commands.begin(); it != commands.end(); it++)
		delete it->second;
}


void	CommandHandler::ExecuteCommand(Client &sender, const CommandData &data)
{
	it = commands.find(data.command);
	if (it != commands.end())
		it->second->execute(sender, data.args);
	else
		std::cout << "Command does not exist" << std::endl;
}
#endif // COMMAND_HANDLER_HPP
