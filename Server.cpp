/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vini <vini@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 17:42:59 by vini              #+#    #+#             */
/*   Updated: 2025/03/13 01:05:39 by vini             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(std::string port, std::string password)
{
	this->serverPort = std::atoi(port.c_str());
	this->serverPassword = password;
}

void	Server::acceptClients()
{
	while (true)
	{
		
	}
}

void	Server::initSocket()
{
	// Store socket address info
	struct sockaddr_in	server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(serverPort);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	// Create socket
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
	if (bind(serverSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		std::cout << "\033[31;1mError: failed to bind socket.\033[0m" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Enable the socket to listen to incomming connections
	if (listen(serverSocket, 10) < 0)
	{
		std::cout << "\033[31;1mError: socket not listening.\033[0m" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "\033[32mServer listening on port\033[0m -> " << serverPort << std::endl;
}

void	Server::bootServer()
{
	initSocket();
	acceptClients();
}