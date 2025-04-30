/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vini <vini@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 01:03:51 by vini              #+#    #+#             */
/*   Updated: 2025/04/30 11:36:04 by vini             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
	this->password = "";
	this->nickname = "";
	this->username = "";
	this->prefix = "";
	this->isOperator = false;
	this->isRegistered = false;
	this->clientSocket = -1;
	this->clientAddress = "";
}

Client::Client(Client const &toCopy){*this = toCopy;}

Client &Client::operator=(Client const &toAssign)
{
	if (this != &toAssign)
	{
		this->password = toAssign.password;
		this->nickname = toAssign.nickname;
		this->username = toAssign.username;
		this->isOperator = toAssign.isOperator;
		this->isRegistered = toAssign.isRegistered;
		this->clientSocket = toAssign.clientSocket;
		this->clientAddress = toAssign.clientAddress;
	}
	return *this;
}

Client::~Client(){}

void	Client::setPassword(std::string password){this->password = password;}
void	Client::setNickname(std::string nickname){this->nickname = nickname;}
void	Client::setUsername(std::string username){this->username = username;}
void	Client::setPrefix(std::string prefix){this->prefix = prefix;}
void	Client::setAddress(std::string address){this->clientAddress = address;}
void	Client::setSocket(int fd){this->clientSocket = fd;}
void	Client::setRegistration(bool isRegistered){this->isRegistered = isRegistered;}

std::string	Client::getPassword(){return this->password;}
std::string	Client::getNickname(){return this->nickname;}
std::string	Client::getUsername(){return this->username;}
std::string	Client::getPrefix(){return this->prefix;}
std::string	Client::getAddress(){return this->clientAddress;}
int			Client::getSocket(){return this->clientSocket;}
bool		Client::getRegistration(){return this->isRegistered;}
std::vector<std::string> Client::getBannedChannels(){return this->bannedChannels;}
std::vector<std::string> Client::getopChannels(){return this->opChannels;}
