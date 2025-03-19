/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vini <vini@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 17:42:59 by vini              #+#    #+#             */
/*   Updated: 2025/03/19 14:26:21 by vini             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(std::string port, std::string password)
{
	this->serverPort = std::atoi(port.c_str());
	this->serverPassword = password;
	this->serverSocket = -1;
}

void	Server::receiveData(int fd)
{
	
}

void	Server::acceptClient()
{
	Client				newClient;
	struct sockaddr_in	clientAddr;
	socklen_t			addrLen = sizeof(clientAddr);

	// Accept incomming client connection
	int	newFd = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);
	if (newFd < 0)
	{
		std::cout << "\033[31;1mError: failed to accept client.\033[0m" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "\033[32mClient connected successfully.\033[0m" << std::endl;

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
}

void	Server::pollSockets()
{
	// Main loop, waiting for an event in a socket monitored by poll()
	while (true)
	{
		int	activity = poll(&pollFds[0], pollFds.size(), -1);
		if (activity < 0)
		{
			std::cout << "\033[31;1mError: poll().\033[0m" << std::endl;
			exit(EXIT_FAILURE);
		}
		// Check if there is any data to read in the existing connections
		for (size_t i; i < pollFds.size(); i++)
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
	std::cout << "\033[32mSocket created successfully.\033[0m" << std::endl;

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
	std::cout << "\033[32mServer address\033[0m -> " << serverIP << std::endl;
	std::cout << "\033[32mServer listening on port\033[0m -> " << ntohs(serverAddr.sin_port) << std::endl;
}

void	Server::bootServer()
{
	initSocket();
	pollSockets();
	close(serverSocket);
}