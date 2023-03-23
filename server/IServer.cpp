/* #include "IServer.hpp"
#include "ClientManager.hpp"


IServer::IServer(int _port) : port(_port)
{}

IServer::IServer( args )
{
}

IServer::~IServer()
{
}

void	IServer::Setup()
{
	addrlen = sizeof(address);
	CreateServer();
	SetOptions();
	BindSocket();
	StartListening();
}

void	IServer::ResetSockets()
{
	FD_ZERO(&readfds);
	FD_SET(socketfd, &readfds);
	
	int max_fd_in_clients = ClientManager::getManager()->AddClientstToReadFds(&readfds);
	max_sd = std::max(socketfd, max_fd_in_clients);
}

void	IServer::CreateServer()
{
	if( (socketfd = socket(AF_INET , SOCK_STREAM , 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
}

void IServer::SetOptions()
{
	int opt = 1;
	if( setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
		sizeof(opt)) < 0 )
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
}

void	IServer::BindSocket()
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

void	IServer::StartListening()
{
	std::cout << "Listener on port " << port << std::endl;
	// specifying maximum of 3 pending connections for the master socket
	if (listen(socketfd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	std::cout << "Waiting for connections ..." << std::endl;
}

void	IServer::ListenForClientInput()
{
	ClientManager::getManager()->HandleInput(&readfds);
}

void	IServer::SendToClient(int sockfd, const char *message)
{
	if (send(sockfd, message, strlen(message), 0) < 0)
		perror("send");
}


void	IServer::WaitForActivity()
{
	int activity = select(max_sd + 1 , &readfds , NULL , NULL , NULL);
	
	if ((activity < 0) && (errno!=EINTR))
		std::cout << "select error" << std::endl;
}

int	IServer::AcceptNewSocket()
{
	int new_socket;
	if ((new_socket = accept(socketfd,
				(struct sockaddr *)&address, &addrlen))<0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	return (new_socket);
}

void	IServer::HandleIncomingConnections()
{
	int new_socket;
	if (FD_ISSET(socketfd, &readfds))
	{
		new_socket = AcceptNewSocket();
		//inform user of socket number - used in send and receive commands
		std::cout << "New connection , socket fd is " << new_socket
			<< ", ip is : " << getHost() << ", port : "
			<< ntohs(address.sin_port) << std::endl;
		
		ClientManager::getManager()->AddClient(new_socket);
	}
}

int	IServer::getaddrlen() { return (addrlen); }

struct	sockaddr_in	*IServer::GetAddress()
{
	return (&address);
}

std::string	const IServer::getHost() const
{
	return (inet_ntoa(address.sin_addr));
} */