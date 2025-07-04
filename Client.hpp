/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roberto <roberto@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 01:03:48 by vini              #+#    #+#             */
/*   Updated: 2025/06/30 01:50:49 by roberto          ###   ########.fr       */
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
	void	setRegistration(bool isRegistered);
	void	setAuthentication(bool isAuthenticated);
	void	addBannedChannels(std::vector<std::string> bannedChannels);
	void	addOpChannels(std::vector<std::string> opChannels);

	std::string	getPassword();
	std::string	getNickname();
	std::string	getUsername();
	std::string	getPrefix();
	std::string	getAddress();
	int			getSocket();
	bool		getRegistration();
	bool		getAuthentication();
	std::vector<std::string> getBannedChannels();
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
	bool		isRegistered;
	bool		isAuthenticated;
	int			clientSocket;
};

#endif
