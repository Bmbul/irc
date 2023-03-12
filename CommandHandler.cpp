#include "CommandHandler.hpp"
#include "Commands.hpp"

CommandHandler	*CommandHandler::instance = NULL;

CommandHandler::CommandHandler() : messageController(MessageController::getController())
{
	if (instance)
		this->~CommandHandler();
	else
	{
		instance = this;
		InitilizeCommands();
	}
}

CommandHandler	*CommandHandler::getHandler()
{
	if (!instance)
		new CommandHandler();
	return (instance);
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
	{
		messageController->SendMessageToClient(sender,
			messageController->GetCommandDoesNotExistMessage(data.command));
	}
}