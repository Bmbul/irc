#include "Server.hpp"

Server::Server()
{

}

Server::Server(int _port, std::string _password) 
	: port(_port), password(_password)
{
	
}

Server::~Server()
{

}

void	Server::Setup()
{
	CreateServer();
	SetOptions();
	addrlen = sizeof(address);
	BindSocket();

	std::cout << "Listener on port " << port << std::endl;
	
	StartListening();
	std::cout << "Waiting for connections ..." << std::endl;
}


void	Server::ResetSockets()
{
	//clear the socket set
	FD_ZERO(&readfds);

	//add master socket to set
	FD_SET(master_socket, &readfds);
	max_sd = master_socket;
		
	//add child sockets to set
	for (it = clients_map.begin(); it != clients_map.end(); it++)
		FD_SET(it->first, &readfds);
	if (clients_map.size() > 0)
		max_sd = std::max(clients_map.rbegin()->first, max_sd);
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
	// specifying maximum of 3 pending connections for the master socket
	if (listen(master_socket, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
}

void	Server::AddNewSocket(int sockfd)
{
	std::pair<int, Client> newClient(sockfd, Client("Client Name"));
	clients_map.insert(newClient);
	printf("Adding to list of sockets as %lu\n" , clients_map.size());
}

void	Server::ListenForClientInput()
{
	int sd, valread;
	for (it = clients_map.begin(); it != clients_map.end();)
	{
		std::cout << "iterating!" << std::endl;
		sd = it->first; 
		if (FD_ISSET(sd , &readfds))
		{
			//Check if it was for closing , and also read the
			//incoming message
			if ((valread = read( sd , buffer, 1024)) == 0)
			{
				//Somebody disconnected , get his details and print
				getpeername(sd , (struct sockaddr*)&address , \
					&addrlen);
				printf("Host disconnected , ip %s , port %d \n" ,
					inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
					
				//Close the socket and mark as 0 in list for reuse
				close( sd );
				std::cout << "trying to clear!" << std::endl;
				clients_map.erase(it++);
				std::cout << "cleared" << std::endl;
			}
			//Echo back the message that came in
			else
			{
				//set the string terminating NULL byte on the end
				//of the data read
				buffer[valread] = '\0';
				std::cout << "I got this message: " << buffer;
				++it;
			}
		}
		++it;
	}
}

void	Server::SentToClient(int sockfd, const char *message)
{
	if (send(sockfd, message, strlen(message), 0) < 0)
		perror("send");
}


void	Server::WaitForActivity()
{
	int activity = select(max_sd + 1 , &readfds , NULL , NULL , NULL);
	
	if ((activity < 0) && (errno!=EINTR))
		printf("select error");
}

void	Server::HandleIncomingConnections()
{
	int new_socket;
	if (FD_ISSET(master_socket, &readfds))
	{
		if ((new_socket = accept(master_socket,
				(struct sockaddr *)&address, &addrlen))<0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}
		//inform user of socket number - used in send and receive commands
		printf("New connection , socket fd is %d , ip is : %s , port : %d \
		\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs
			(address.sin_port));
	
		SentToClient(new_socket, "First Login\n");
		
		AddNewSocket(new_socket);
		puts("Welcome message sent successfully");
	}

}


int	Server::getMaster() { return (master_socket); }