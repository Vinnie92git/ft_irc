/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roberto <roberto@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 17:42:59 by vini              #+#    #+#             */
/*   Updated: 2025/04/17 19:37:23 by roberto          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{
	this->serverPort = 6667;
	this->serverPassword = "";
	this->serverSocket = -1;

}

Server::Server(std::string port, std::string password)
{
	this->serverPort = std::atoi(port.c_str());
	this->serverPassword = password;
	this->serverSocket = -1;
}

Server::Server(const Server& toCopy){*this = toCopy;}

Server& Server::operator=(const Server& toAssign)
{
	if (this != &toAssign)
	{
		this->pollFds = toAssign.pollFds;
		this->connectedClients = toAssign.connectedClients;
		this->channels = toAssign.channels;
		this->signal = toAssign.signal;
		this->serverPort = toAssign.serverPort;
		this->serverPassword = toAssign.serverPassword;
		this->serverSocket = toAssign.serverSocket;
	}
	return *this;
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

ircMessage	Server::splitMessage(std::string& message)
{
	ircMessage					parsedMessage;
	std::istringstream			iss(message);
	std::string					token;

	// Handle optional prefix
	if(!message.empty() && message[0] == ':')
	{
		iss >> token;
		parsedMessage.prefix = token.substr(1);
	}

	// Get the command
	if (iss >> token)
		parsedMessage.command = token;

	// Store parameters
	while(iss >> token)
	{
		if (token[0] == ':')
		{
			std::string trailing = token.substr(1);
			std::string rest;
			std::getline(iss, rest);
			parsedMessage.params.push_back(trailing + rest);
			break;
		}
		parsedMessage.params.push_back(token);
	}
	return parsedMessage;
}

void	Server::parseMessage(std::string& message, int fd)
{
	if (message.empty())
		return ;

	// Parse each message into a struct to facilitate execution
	ircMessage	parsedMessage = splitMessage(message);
	if (parsedMessage.command == "CAP" || parsedMessage.command == "cap")
		std::cout << "Server has no capability negotiation." << std::endl;
	else if (parsedMessage.command == "PASS" || parsedMessage.command == "pass")
		setClientPassword(parsedMessage.params, fd);
	else if (parsedMessage.command == "NICK" || parsedMessage.command == "nick")
		setClientNickname(parsedMessage.params, fd);
	else if (parsedMessage.command == "USER" || parsedMessage.command == "user")
		setClientUsername(parsedMessage.params, fd);
	else if (parsedMessage.command == "JOIN" || parsedMessage.command == "join")
		joinCmd(parsedMessage.params, fd);
	else if (parsedMessage.command == "MODE" || parsedMessage.command == "mode")
	{
		if (!parsedMessage.params.empty())
			modeCmd(parsedMessage.params, fd);
		else
			std::cout << "MODE error: No parameters provided." << std::endl;
	}
	else if (parsedMessage.command == "EXIT" || parsedMessage.command == "exit"
		|| parsedMessage.command == "QUIT" || parsedMessage.command == "quit")
		std::cout << "Client disconnecting..." << std::endl;
	else
		std::cout << "Unrecognized command..." << std::endl;
}

std::vector<std::string>	Server::splitBuffer(char* buffer)
{
	std::vector<std::string>	splitLines;
	std::istringstream			iss(buffer);
	std::string					line;

	// Stream buffer into vector of strings for parsing
	while(std::getline(iss, line))
	{
		size_t pos = line.find_first_of("\r\n");
		if(pos != std::string::npos)
			line = line.substr(0, pos);
		splitLines.push_back(line);
	}
	return splitLines;
}

void	Server::receiveData(int fd)
{
	std::vector<std::string>	messages;
	char						buffer[512];
	memset(buffer, 0, sizeof(buffer));

	// Receive data from connected client
	ssize_t	recvBytes = recv(fd, &buffer, sizeof(buffer) - 1, 0);
	if (recvBytes > 0)
	{
		buffer[recvBytes] = '\0';
		messages = splitBuffer(buffer);
		for (size_t i = 0; i < messages.size(); i++)
		{
			timestamp();
			std::cout << "[" << messages[i] << "]" << std::endl;
			parseMessage(messages[i], fd);
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
		return ;
	}
	if (fcntl(newFd, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cout << "\033[31;1mError: failed to create non-blocking socket.\033[0m" << std::endl;
		return ;
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
		return ;
	}
}

void	Server::pollSockets()
{
	// Main loop, waiting for an event in a socket monitored by poll()
	while (Server::signal == false)
	{
		int	activity = poll(&pollFds[0], pollFds.size(), -1);
		if (activity < 0 && Server::signal == false)
			throw std::runtime_error("\033[31;1mError: poll().\033[0m");

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
		throw std::runtime_error("\033[31;1mError: failed to create server socket.\033[0m");

	// Enable the socket to be reusable and non-blocking
	int	opt = 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw std::runtime_error("\033[31;1mError: failed to reuse socket after server shutdown.\033[0m");
	if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("\033[31;1mError: failed to create non-blocking socket.\033[0m");

	// Bind the socket to IP address and Port
	if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
		throw std::runtime_error("\033[31;1mError: failed to bind server socket.\033[0m");

	// Enable the socket to listen to incomming connections
	if (listen(serverSocket, BACKLOG) < 0)
		throw std::runtime_error("\033[31;1mError: server socket not listening.\033[0m");

	// Add the socket to the pool of monitored fds
	struct pollfd	serverPoll;
	serverPoll.fd = serverSocket;
	serverPoll.events = POLLIN;
	serverPoll.revents = 0;
	pollFds.push_back(serverPoll);

	// Display server address info
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

Client*	Server::getClient(int fd)
{
	for (size_t i = 0; i < this->connectedClients.size(); i++)
	{
		if (this->connectedClients[i].getSocket() == fd)
			return &this->connectedClients[i];
	}
	return NULL;
}

Channel*	Server::getChannel(std::string name)
{
	for(size_t i = 0; i < this->channels.size(); i++)
	{
		if (this->channels[i].getName() == name)
			return &this->channels[i];
	}
	return NULL;
}

bool	Server::signal = false;
void	Server::signalHandler(int signal)
{
	(void)signal;
	std::cout << std::endl;
	Server::signal = true;
}
