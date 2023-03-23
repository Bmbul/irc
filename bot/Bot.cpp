#include "Bot.hpp"
#include <iostream>

Bot::Bot(const std::string &_host, int _port, const std::string &_pass,
	const std::string &_user, const std::string &_nick) :
	port(_port), host(_host), pass(_pass), user(_user), nick(_nick) { }

Bot::~Bot()
{

}

void	Bot::SetUser(const std::string &_user) { user = _user; }

void	Bot::SetNick(const std::string &_nick) { nick = _nick; }

void	Bot::Setup()
{
	addrlen = sizeof(address);
	CreateServer();
	SetOptions();
	BindSocket();
	ConnectToServer();
}

void	Bot::CreateServer()
{
	if( (socketfd = socket(AF_INET , SOCK_STREAM , 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
}

void Bot::SetOptions()
{
	int opt = 1;
	if( setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
		sizeof(opt)) < 0 )
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
}

void	Bot::BindSocket()
{
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	if (bind(socketfd, (struct sockaddr *)&address, addrlen)<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
}

void	Bot::ConnectToServer()
{
	std::cout << "The bot is running " << port << std::endl;
	std::cout << nick << std::endl;
	std::cout << "Waiting for connections ..." << std::endl;
}