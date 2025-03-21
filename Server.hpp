/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vini <vini@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 17:42:56 by vini              #+#    #+#             */
/*   Updated: 2025/03/19 14:26:28 by vini             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <iomanip>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include "Client.hpp"

#define BACKLOG 10

class Server
{
public:
	Server(std::string port, std::string password);

	void	receiveData(int fd);
	void	acceptClient();
	void	pollSockets();
	void	initSocket();
	void	bootServer();

private:
	std::vector<struct pollfd>	pollFds;
	std::vector<Client>			connectedClients;
	std::string					serverPassword;
	char						serverIP[INET_ADDRSTRLEN];
	int							serverSocket;
	int							serverPort;
};

#endif