#include "MessageController.hpp"
#include <sys/socket.h>
#include <stdio.h>

MessageController	*MessageController::instance = NULL;

MessageController::MessageController()
{
	if (!instance)
		instance = this;
	else
	{
		std::cout << "Creating second instance of MessageController!!!" << std::endl 
			<< "Bad idea, try new tricks!!!" << std::endl;
		delete this;
	}
}

MessageController::~MessageController()
{

}

CommandData	MessageController::Parse(const std::string &input) const
{
	CommandData	data;
	std::string str;
	std::string mainPart;
	std::string longArg;

	size_t found = input.find(':');
	if (found != std::string::npos)
	{
		mainPart = input.substr(0, found);
  		longArg = input.substr(found +1, std::string::npos);
	} else mainPart = input;

	std::stringstream ss(mainPart);

	if (std::getline(ss, str, ' '))
		data.command = str;
	while (std::getline(ss, str, ' '))
	{
		data.args.push_back(str);
	}
	
	if (found != std::string::npos)
		data.args.push_back(longArg);
	
	return (data);
}

void	MessageController::PrintData(const CommandData &data) const
{
	if (!data.command.empty())
		std::cout << "MY: COMMAND: "<< data.command << std::endl;
	for (size_t i = 0; i < data.args.size(); i++)
	{
		std::cout << "MY: ARG[" << i << "]: " << data.args[i] << std::endl;
	}
}

bool	MessageController::StringStartsWithFromSet
	(const std::string &str, const std::string &set) const
{
	char ch = str[0];
	for(size_t i = 0; i < set.size(); i++)
	{
		if (ch == set[i])
			return (true);
	}
	return (false);
}

bool	MessageController::IsValidChannelName(std::string channelName) const
{
	return (StringStartsWithFromSet(channelName, "#&"));
}

MessageController *MessageController::getController()
{
	if (!instance)
		new MessageController();
	return (instance);
}


void	MessageController::SendMessageWithSocket(int clientSocket,
	const std::string &message) const
{
	if (send(clientSocket, (message + "\n").c_str(), message.length() + 1, 0) < 0)
		perror("send");
}

void	MessageController::SendMessageToClient(const Client &client,
	const std::string &message) const
{
	SendMessageWithSocket(client.getSocket(), message);
}


void	MessageController::SendHelloMessage(const Client &client) const
{
	SendHelloMessage(client.getSocket());
}

void	MessageController::SendHelloMessage(int clientSocket) const
{
	SendMessageWithSocket(clientSocket, "Hello to irc world");
}