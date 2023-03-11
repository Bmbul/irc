#include "MessageController.hpp"
#include <cstring>
#include <sstream>

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
	// std::string mainPart;
	// std::string longArg;

	// size_t pos = input.find(':');
    // mainPart = input.substr(0, pos);
    // longArg = input.substr(pos +1, std::string::npos);


	std::stringstream ss(input);
	if (std::getline(ss, str, ' '))
		data.command = str;
	while (std::getline(ss, str, ' '))
	{
		data.args.push_back(str);
	}
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

bool	MessageController::StringStartsWithFromSet(std::string str, std::string set)
{
	char ch = str[0];
	for(size_t i = 0; i < set.size(); i++)
	{
		if (ch == set[i])
			return (true);
	}
	return (false);
}

bool	MessageController::IsValidChannelName(std::string channelName)
{
	return (StringStartsWithFromSet(channelName, "#&"));
}

MessageController *MessageController::getController()
{
	if (!instance)
		new MessageController();
	return (instance);
}