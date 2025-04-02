/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vini <vini@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 17:42:56 by vini              #+#    #+#             */
/*   Updated: 2025/04/02 14:15:45 by vini             ###   ########.fr       */
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
#include <csignal>
#include <ctime>
#include <sstream>
#include <string>
#include "Client.hpp"
#include "Channel.hpp"

#define BACKLOG 10

class Server
{
public:
	Server();
	Server(std::string port, std::string password);
	Server(const Server& toCopy);
	Server& operator=(const Server& toAssign);

	void	setClientPassword(std::string& command, int fd);
	void	setClientNickname(std::string& command, int fd);
	void	setClientUsername(std::string& command, int fd);
	void	joinCmd(std::string& command, int fd);

	std::vector<std::string>	splitCommand(std::string& command);
	std::vector<std::string>	splitBuffer(char* buffer);
	void						parseCommand(std::string& command, int fd);

	void		shutdownServer();
	void		removeClient(int fd);
	void		receiveData(int fd);
	void		acceptClient();
	void		pollSockets();
	void		initSocket();
	void		bootServer();
	void		timestamp();
	static void	signalHandler(int signal);

private:
	std::vector<struct pollfd>	pollFds;
	std::vector<Client>			connectedClients;
	std::string					serverPassword;
	static bool					signal;
	char						serverIP[INET_ADDRSTRLEN];
	int							serverSocket;
	int							serverPort;
};

#endif