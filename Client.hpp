/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roberto <roberto@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 01:03:48 by vini              #+#    #+#             */
/*   Updated: 2025/04/21 14:11:55 by roberto          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <iomanip>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <vector>

class Client
{
public:
	Client();
	Client(const Client& toCopy);
	Client& operator=(const Client& toAssign);
	~Client();

	void	setPassword(std::string password);
	void	setNickname(std::string nickname);
	void	setUsername(std::string username);
	void	setPrefix(std::string prefix);
	void	setAddress(std::string address);
	void	setSocket(int fd);
	void	addBannedChannels(std::vector<std::string> bannedChannels);
	void	addOpChannels(std::vector<std::string> opChannels);

	std::string	getPassword();
	std::string	getNickname();
	std::string	getUsername();
	std::string	getPrefix();
	std::string	getAddress();
	int			getSocket();
	std::vector<std::string> getBannedChannels();
	//faltaría uno para comprobar que esté baneado en el canal especifico que se le mande
	std::vector<std::string> getopChannels();


private:
	std::string	password;
	std::string	nickname;
	std::string	username;
	std::string	prefix;
	std::string	clientAddress;
	std::vector<std::string> bannedChannels;
	std::vector<std::string> opChannels;
	bool		isOperator;
	int			clientSocket;
};

#endif
