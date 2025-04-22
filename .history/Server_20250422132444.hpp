/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roberto <roberto@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 17:42:56 by vini              #+#    #+#             */
/*   Updated: 2025/04/22 13:24:43 by roberto          ###   ########.fr       */
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
#include <algorithm>
#include <iterator>
#include "Client.hpp"
#include "Channel.hpp"

#define BACKLOG 10

struct	ircMessage
{
	std::string					prefix;
	std::string					command;
	std::vector<std::string>	params;
};

class Server
{
public:
	Server();
	Server(std::string port, std::string password);
	Server(const Server& toCopy);
	Server& operator=(const Server& toAssign);

	void	removeChannel(std::string channelName);

	void	setClientPassword(std::vector<std::string>& params, int fd);
	void	setClientNickname(std::vector<std::string>& params, int fd);
	void	setClientUsername(std::vector<std::string>& params, int fd);
	void	joinCmd(std::vector<std::string>& params, int fd);
	void	partCmd(std::vector<std::string>& params, int fd);
	void	quitCmd(std::vector<std::string>& params, int fd);
	//void	modeCmd(std::vector<std::string>& params, int fd);
	void	joinChannel(std::string channelName, int fd);
	void	partChannel(std::string channelName, int fd);
	//void	modeTarget(std::string channelName, std::vector<std::string>& params, int fd, int mode);

	std::vector<std::string>	splitComma(std::string param);
	std::vector<std::string>	splitBuffer(char* buffer);
	ircMessage					splitMessage(std::string& message);
	void						parseMessage(std::string& message, int fd);

	void		shutdownServer();
	void		removeClient(int fd);
	void		receiveData(int fd);
	void		acceptClient();
	void		pollSockets();
	void		initSocket();
	void		bootServer();
	void		timestamp();
	static void	signalHandler(int signal);

	Client*		getClient(int fd);
	Channel*	getChannel(std::string name);

private:
	std::vector<struct pollfd>	pollFds;
	std::vector<Client>			connectedClients;
	std::vector<Channel>		channels;
	std::string					serverPassword;
	static bool					signal;
	char						serverIP[INET_ADDRSTRLEN];
	int							serverSocket;
	int							serverPort;
};

#endif
