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

CommandData	MessageController::ParseSingleCommand(const std::string &commandLine) const
{
	CommandData data;
	std::string mainPart;
	std::string longArg;
	std::string str;

	size_t found = commandLine.find(':');
	if (found != std::string::npos)
	{
		mainPart = commandLine.substr(0, found);
  		longArg = commandLine.substr(found +1, std::string::npos);
	} else mainPart = commandLine;

	// trimming the beginning and the end of the message
	int  actualStart = mainPart.find_first_not_of(" ");
	int actualEnd = mainPart.find_last_not_of("\n\r ");
	mainPart = mainPart.substr(actualStart, actualEnd + 1);

	std::stringstream ss(mainPart);
	if (std::getline(ss, str, ' '))
		data.command = str;
	while (std::getline(ss, str, ' '))
	{
		if (str[0])
			data.args.push_back(str);
	}
	if (found != std::string::npos)
		data.args.push_back(commandLine);
	
	return data;
}

std::vector<CommandData>	MessageController::Parse(std::string &input) const
{
	std::vector<CommandData>	commandDatas;

	std::string singleCommand;

	for(int end = input.find("\n"); end != -1; end = input.find("\n"))
	{
		singleCommand = input.substr(0, end);
		input.erase(input.begin(), input.begin() + end + 1);
		commandDatas.push_back(ParseSingleCommand(singleCommand));
	}

	return (commandDatas);
}

void	MessageController::PrintData(std::vector<CommandData> &dataVector) const
{
	std::vector<CommandData>::iterator	data;
	for(data = dataVector.begin(); data != dataVector.end(); data++)
	{
		if (!data->command.empty())
			std::cout << "MY: COMMAND: "<< data->command << std::endl;
		for (size_t i = 0; i < data->args.size(); i++)
		{
			std::cout << "MY: ARG[" << i << "]: " << data->args[i] << std::endl;
		}
		std::cout << std::endl;
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

bool	MessageController::GotEndOfMessage(const char *messageChunk) const
{
	int i = -1;
	while (messageChunk[++i])
	{
		if (!strncmp(messageChunk + i, "\n", 1))
		{
			return (true);
		}
	}
	return (false);
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


bool	MessageController::ContainsChunk(int clientSocket) const
{
	return (chunksMap.count(clientSocket));
}

void	MessageController::AddChunk(int clientSocket, const std::string &messageChunk)
{
	if (!ContainsChunk(clientSocket))
		chunksMap[clientSocket] = std::vector<std::string>();
	chunksMap[clientSocket].push_back(messageChunk);
}

void	MessageController::ClearChunk(int clientSocket)
{
	if (ContainsChunk(clientSocket))
		chunksMap.erase(clientSocket);
}

std::string	MessageController::ConstructFullMessage(int clientSocket)
{
	std::string joinedString;
	
	for (std::vector<std::string>::iterator iter = chunksMap[clientSocket].begin();
		iter != chunksMap[clientSocket].end(); iter++)
	{
		joinedString += *iter;
	}
	return (joinedString);
}