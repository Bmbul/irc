#include "ClientManager.hpp"
#include "Server.hpp"
#include "Exceptions.hpp"
#include "MessageController.hpp"

ClientManager	*ClientManager::instance = NULL;


ClientManager	*ClientManager::getManager()
{
	if (!instance)
		new ClientManager();
	return (instance);
}


ClientManager::ClientManager() : messageController(MessageController::getController())
{
	if (!instance)
		instance = this;
	else
	{
		std::cout << "Creating second instance of Server!!!" << std::endl 
			<< "Bad idea, try new tricks!!!" << std::endl;
		delete this;
	}
}

ClientManager::~ClientManager()
{

}

void	ClientManager::AddClient(const int socketFd)
{
	std::pair<int, Client> newClient(socketFd, Client("Client Name", socketFd));
	clientMap.insert(newClient);
	std::cout << "Adding to list of sockets as "
		<< clientMap.size() << std::endl;
}

void	ClientManager::RemoveClient(const int socketFd)
{
	std::map<int, Client>::iterator it = clientMap.find(socketFd);

	if (it != clientMap.end())
		clientMap.erase(it);
	else
		std::cout << "No such Client!!" << std::endl;
}

void	ClientManager::RemoveClient(std::map<int, Client>::iterator &iter)
{
	clientMap.erase(iter);
}



int		ClientManager::AddClientstToReadFds(fd_set *readfds)
{
	for (it = clientMap.begin(); it != clientMap.end(); it++)
		FD_SET(it->first, readfds);
	if (clientMap.size() > 0)
		return (clientMap.rbegin()->first);
	return (0);
}

bool	ClientManager::HasClient(const std::string &clientName) const
{
	for (it = clientMap.begin(); it != clientMap.end(); it++)
	{
		if (it->second.getNick() == clientName)
			return (true);
	}
	return (false);
}

int	ClientManager::GetClientSocket(const std::string &clientName) const
{
	for (it = clientMap.begin(); it != clientMap.end(); it++)
	{
		if (it->second.getNick() == clientName)
			return (it->first);
	}
	return (-1);
}

void	ClientManager::HandleMessage(Client &client)
{
	messageController->AddChunk(client.getSocket(), buffer);
	
	if (!messageController->GotEndOfMessage(buffer))
		return ;
	std::string readyMessage = messageController->ConstructFullMessage(client.getSocket());
	std::vector<CommandData> commands = messageController->Parse(readyMessage);
	messageController->PrintData(commands);

	for(std::vector<CommandData>::iterator data = commands.begin();
		data != commands.end(); data++)
	{
		try
		{
			CommandHandler::getHandler()->ExecuteCommand(client, *data);
		}
		catch(const IRCException& exception)
		{
			Server::getServer()->SendMessageToClient(client,
				exception.what());
		}
	}
	messageController->ClearChunk(client.getSocket());
}

void	ClientManager::CloseClient(int	clientSocket)
{
	Server	*server = Server::getServer();
	struct sockaddr_in *address;
	socklen_t	addrlen;

	address = server->GetAddress();
	addrlen = server->getaddrlen();
	//Somebody disconnected , get his details and print
	getpeername(clientSocket , (sockaddr *)address , &addrlen);
	
	std::cout << "Host disconnected , ip " << inet_ntoa(address->sin_addr)
		<< " , port " << ntohs(address->sin_port) << std::endl;
	//Close the socket and mark as 0 in list for reuse
	server->ClearClientFromChannels(clientMap[clientSocket]);
	close(clientSocket);
}

void	ClientManager::HandleInput(fd_set *readfds)
{
	int sd, valread;


	for (std::map<int, Client>::iterator it = clientMap.begin(); it != clientMap.end();)
	{
		sd = it->first;
		if (FD_ISSET(sd , readfds))
		{
			if ((valread = recv(sd, buffer, 1024, MSG_DONTWAIT)) == 0)
			{
				++it;
				CloseClient(sd);
				RemoveClient(sd);
			}
			else // in case if client inputed message
			{
				buffer[valread] = '\0';
				std::cout << "got some message: " << buffer << std::endl;
				if (buffer[0] && !(buffer[0] == '\n' && !messageController->ContainsChunk(sd)))
					HandleMessage(it->second);

				if (clientMap.size() == 0)
					return ;
				it++;
			}
		}
		else
			++it;
	}
}

const Client &ClientManager::getClient(std::string const &nickName) const
{
	std::map<int, Client>::const_iterator it;
	for (it = clientMap.begin(); it != clientMap.end(); it++)
	{
		if (it->second.getNick() == nickName)
			break;
	}
	return it->second;
}