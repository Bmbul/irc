#include "Server.hpp"
#include "MessageController.hpp"

Server *Server::instance = NULL;

Server::Server()
{

}

Server::Server(int _port, std::string _password) 
	: port(_port), password(_password)
{
	if (!instance)
		instance = this;
	else
	{
		std::cout << "Creating second instance of Server!!!" << std::endl 
			<< "Bad idea, try new tricks!!!" << std::endl;
		this->~Server();
	}
}

Server::~Server()
{

}

void	Server::Setup()
{
	addrlen = sizeof(address);
	CreateServer();
	SetOptions();
	BindSocket();
	StartListening();
}

void	Server::ResetSockets()
{
	FD_ZERO(&readfds);
	FD_SET(master_socket, &readfds);
	
	int max_fd_in_clients = ClientManager::getManager()->AddClientstToReadFds(&readfds);
	max_sd = std::max(master_socket, max_fd_in_clients);
}

void	Server::CreateServer()
{
	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
}

void Server::SetOptions()
{
	int opt = 1;
	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
		sizeof(opt)) < 0 )
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
}

void	Server::BindSocket()
{
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	if (bind(master_socket, (struct sockaddr *)&address, addrlen)<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
}

void	Server::StartListening()
{
	std::cout << "Listener on port " << port << std::endl;
	// specifying maximum of 3 pending connections for the master socket
	if (listen(master_socket, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	std::cout << "Waiting for connections ..." << std::endl;
}

void	Server::ListenForClientInput()
{
	ClientManager::getManager()->HandleInput(&readfds);
}

void	Server::SendToClient(int sockfd, const char *message)
{
	if (send(sockfd, message, strlen(message), 0) < 0)
		perror("send");
}


void	Server::WaitForActivity()
{
	int activity = select(max_sd + 1 , &readfds , NULL , NULL , NULL);
	
	if ((activity < 0) && (errno!=EINTR))
		std::cout << "select error" << std::endl;
}

int	Server::AcceptNewSocket()
{
	int new_socket;
	if ((new_socket = accept(master_socket,
				(struct sockaddr *)&address, &addrlen))<0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	return (new_socket);
}

void	Server::HandleIncomingConnections()
{
	int new_socket;
	if (FD_ISSET(master_socket, &readfds))
	{
		new_socket = AcceptNewSocket();
		//inform user of socket number - used in send and receive commands
		std::cout << "New connection , socket fd is " << new_socket
			<< ", ip is : " << inet_ntoa(address.sin_addr) << ", port : "
			<< ntohs(address.sin_port) << std::endl;
		
		MessageController::getController()->SendHelloMessage(new_socket);
		
		ClientManager::getManager()->AddClient(new_socket);
	}
}

Server	*Server::getServer()
{
	if (!instance)
		instance = new Server();
	return (instance);
}


int	Server::getaddrlen() { return (addrlen); }

struct	sockaddr_in	*Server::GetAddress()
{
	return (&address);
}

std::string const &Server::getPass()const
{
	return this->password;
}
