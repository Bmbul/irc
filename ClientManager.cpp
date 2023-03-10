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

void	ClientManager::RemoveClient(std::map<int, Client>::iterator iter)
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
		if (it->second.getName() == clientName)
			return (true);
	}
	return (false);
}

int	ClientManager::GetClientSocket(const std::string &clientName) const
{
	for (it = clientMap.begin(); it != clientMap.end(); it++)
	{
		if (it->second.getName() == clientName)
			return (true);
	}
	return (-1);
}

void	ClientManager::HandleMessage(Client &client)
{
	messageController->AddChunk(client.getSocket(), buffer);
	std::cout << "ADDING: " << buffer << std::endl;
	if (!messageController->GotEndOfMessage(buffer))
		return ;
		
	std::cout << "end of message" << std::endl;
	std::string readyMessage = MessageController::getController()->ConstructFullMessage(client.getSocket());
	std::vector<CommandData> commands = MessageController::getController()->Parse(readyMessage);
	MessageController::getController()->PrintData(commands);

	for(std::vector<CommandData>::iterator data = commands.begin();
		data != commands.end(); data++)
	{
		try
		{
			CommandHandler::getHandler()->ExecuteCommand(client, *data);
		}
		catch(const IRCException& exception)
		{
			messageController->SendMessageToClient(client,
				exception.what());
		}
	}
	messageController->ClearChunk(client.getSocket());
}

void	ClientManager::HandleInput(fd_set *readfds)
{
	int sd, valread;
	struct sockaddr_in *address;
	socklen_t	addrlen;

	address = Server::getServer()->GetAddress();
	addrlen = Server::getServer()->getaddrlen();
	for (std::map<int, Client>::iterator it = clientMap.begin(); it != clientMap.end();)
	{
		sd = it->first; 
		if (FD_ISSET(sd , readfds))
		{
			if ((valread = recv(sd, buffer, 1024, MSG_DONTWAIT)) == 0)
			{
				//Somebody disconnected , get his details and print
				getpeername(sd , (sockaddr *)address , &addrlen);
				
				std::cout << "Host disconnected , ip " << inet_ntoa(address->sin_addr)
					<< " , port " << ntohs(address->sin_port) << std::endl;
					
				//Close the socket and mark as 0 in list for reuse
				close( sd );
				RemoveClient(it++);
				messageController->ClearChunk(sd);
				continue;
			}
			else // in case if client inputed message
			{
				buffer[valread] = '\0';
				if (buffer[0] && !(buffer[0] == '\n' && !messageController->ContainsChunk(sd)))
					HandleMessage(it->second);
			}
		}
		++it;
	}
}