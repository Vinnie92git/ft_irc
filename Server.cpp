/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vini <vini@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 17:42:59 by vini              #+#    #+#             */
/*   Updated: 2025/03/26 19:32:54 by vini             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(std::string port, std::string password)
{
	this->serverPort = std::atoi(port.c_str());
	this->serverPassword = password;
	this->serverSocket = -1;
}

void	Server::shutdownServer()
{
	std::cout << "\033[31;1mServer shutting down. Disconnecting all clients...\033[0m" << std::endl;

	for (size_t i = 0; i < connectedClients.size(); i++)
		close(connectedClients[i].getSocket());

	if (serverSocket != -1)
		close(serverSocket);
}

void	Server::removeClient(int fd)
{
	// Remove client from the vector of connected clients and close its fd
	for (size_t i = 0; i < connectedClients.size(); i++)
	{
		if (connectedClients[i].getSocket() == fd)
		{
			connectedClients.erase(connectedClients.begin() + i);
			close(fd);
		}
	}

	// Remove client socket from the pool of monitored fds
	for (size_t i = 0; i < pollFds.size(); i++)
		if (pollFds[i].fd == fd)
			pollFds.erase(pollFds.begin() + i);
}

std::vector<std::string>	Server::splitCommand(std::string& command)
{
	std::vector<std::string>	tokens;
	std::istringstream			iss(command);
	std::string					token;

	while(iss >> token)
	{
		tokens.push_back(token);
		token.clear();
	}
	return tokens;
}

void	Server::parseCommand(std::string& command, int fd)
{
	(void)fd;
	if (command.empty())
		return ;

	// Split each command into tokens and execute the command
	std::vector<std::string>	tokens = splitCommand(command);
	if (tokens[0] == "PASS" || tokens[0] == "pass")
		std::cout << "Setting password..." << std::endl;
	else if (tokens[0] == "NICK" || tokens[0] == "nick")
		std::cout << "Assigning nickname..." << std::endl;
	else if (tokens[0] == "USER" || tokens[0] == "user")
		std::cout << "Assigning username..." << std::endl;
	else if (tokens[0] == "EXIT" || tokens[0] == "exit")
		std::cout << "Client disconnecting..." << std::endl;
	else
		std::cout << "Unrecognized command..." << std::endl;
}

std::vector<std::string>	Server::parseBuffer(char* buffer)
{
	std::vector<std::string>	lineVector;
	std::istringstream			iss(buffer);
	std::string					line;

	// Stream buffer into vector of strings for parsing
	while(std::getline(iss, line))
	{
		size_t pos = line.find_first_of("\r\n");
		if(pos != std::string::npos)
			line = line.substr(0, pos);
		lineVector.push_back(line);
	}
	return lineVector;
}

void	Server::receiveData(int fd)
{
	std::vector<std::string>	commands;
	char						buffer[1024];
	memset(buffer, 0, sizeof(buffer));

	// Receive data from connected client
	ssize_t	recvBytes = recv(fd, &buffer, sizeof(buffer) - 1, 0);
	if (recvBytes > 0)
	{
		buffer[recvBytes] = '\0';
		commands = parseBuffer(buffer);
		for (size_t i = 0; i < commands.size(); i++)
		{
			timestamp();
			std::cout << "[" << commands[i] << "]" << std::endl;
			parseCommand(commands[i], fd);
		}
	}
	else if (recvBytes == 0)
	{
		std::cout << "\033[31mClient \033[0m" << fd << "\033[31m disconnected\033[0m" << std::endl;
		removeClient(fd);
	}
	else
		std::cout << "\033[31mError receiving data from client \033[0m" << fd << std::endl;
	
}

void	Server::acceptClient()
{
	Client				newClient;
	struct sockaddr_in	clientAddr;
	socklen_t			addrLen = sizeof(clientAddr);
	std::string			welcomeMsg = "Welcome to ft_irc! This is a debugging message, for now.\r\n";

	// Accept incomming client connection
	int	newFd = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);
	if (newFd < 0)
	{
		std::cout << "\033[31;1mError: failed to accept client.\033[0m" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (fcntl(newFd, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cout << "\033[31;1mError: failed to create non-blocking socket.\033[0m" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Add client socket to the pool of monitored fds
	struct pollfd	clientPoll;
	clientPoll.fd = newFd;
	clientPoll.events = POLLIN;
	clientPoll.revents = 0;
	pollFds.push_back(clientPoll);

	// Add client to the vector of connected clients
	newClient.setSocket(newFd);
	newClient.setAddress(inet_ntoa(clientAddr.sin_addr));
	connectedClients.push_back(newClient);
	
	timestamp();
	std::cout << "\033[32mClient connected successfully.\033[0m" << std::endl;
	std::cout << "\033[32mClient socket\033[0m -> " << newFd << std::endl;

	// Send welcome message to newly connected client
	if (send(newFd, welcomeMsg.c_str(), welcomeMsg.length(), MSG_NOSIGNAL) < 0)
	{
		std::cout << "\033[31;1mError: send().\033[0m" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void	Server::pollSockets()
{
	// Main loop, waiting for an event in a socket monitored by poll()
	while (Server::signal == false)
	{
		int	activity = poll(&pollFds[0], pollFds.size(), -1);
		if (activity < 0 && Server::signal == false)
		{
			std::cout << "\033[31;1mError: poll().\033[0m" << std::endl;
			exit(EXIT_FAILURE);
		}
		// Check if there is any data to read in the existing connections
		for (size_t i = 0; i < pollFds.size(); i++)
		{
			if (pollFds[i].revents & POLLIN)
			{
				if (pollFds[i].fd == serverSocket)
					acceptClient();
				else
					receiveData(pollFds[i].fd);
			}
		}
	}

}

void	Server::initSocket()
{
	// Store server socket address info
	struct sockaddr_in	serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(serverPort);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	// Create server socket
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0)
	{
		std::cout << "\033[31;1mError: failed to create socket.\033[0m" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Enable the socket to be reusable and non-blocking
	int	opt = 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		std::cout << "\033[31;1mError: failed to reuse socket after server shutdown.\033[0m" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cout << "\033[31;1mError: failed to create non-blocking socket.\033[0m" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Bind the socket to IP address and Port
	if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	{
		std::cout << "\033[31;1mError: failed to bind socket.\033[0m" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Enable the socket to listen to incomming connections
	if (listen(serverSocket, BACKLOG) < 0)
	{
		std::cout << "\033[31;1mError: socket not listening.\033[0m" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Add the socket to the pool of monitored fds
	struct pollfd	serverPoll;
	serverPoll.fd = serverSocket;
	serverPoll.events = POLLIN;
	serverPoll.revents = 0;
	pollFds.push_back(serverPoll);

	inet_ntop(AF_INET, &serverAddr.sin_addr, serverIP, INET_ADDRSTRLEN);
	std::cout << "\033[32;1m----------SERVER ONLINE----------\033[0m" << std::endl;
	std::cout << "\033[32mServer socket\033[0m -> " << serverSocket << std::endl;
	std::cout << "\033[32mServer address\033[0m -> " << serverIP << std::endl;
	std::cout << "\033[32mServer listening on port\033[0m -> " << ntohs(serverAddr.sin_port) << std::endl;
}

void	Server::bootServer()
{
	initSocket();
	pollSockets();
	shutdownServer();
}

void	Server::timestamp()
{
	std::time_t	timestamp = std::time(NULL);
	std::tm*	localTime = std::localtime(&timestamp);
	char		buffer[20];

	std::strftime(buffer, sizeof(buffer), "%H:%M:%S", localTime);

	std::cout << "[" << buffer << "]: ";
}

bool	Server::signal = false;
void	Server::signalHandler(int signal)
{
	(void)signal;
	std::cout << std::endl;
	Server::signal = true;
}