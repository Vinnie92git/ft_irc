/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vini <vini@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 01:03:48 by vini              #+#    #+#             */
/*   Updated: 2025/04/02 14:06:52 by vini             ###   ########.fr       */
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
	void	setAddress(std::string address);
	void	setSocket(int fd);

	std::string	getPassword();
	std::string	getNickname();
	std::string	getUsername();
	std::string	getAddress();
	int			getSocket();

private:
	std::string	password;
	std::string	nickname;
	std::string	username;
	std::string	clientAddress;
	int			clientSocket;
};

#endif