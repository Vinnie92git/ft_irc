/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   registration.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roberto <roberto@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 20:35:29 by vini              #+#    #+#             */
/*   Updated: 2025/06/21 12:41:29 by roberto          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::tryAuthentication(int fd)
{

	if (getClient(fd)->getAuthentication())
		return;
	if (!serverPassword.empty() && getClient(fd)->getPassword() != serverPassword)
		return;
	if (!getClient(fd)->getNickname().empty() && !getClient(fd)->getUsername().empty())
	{
		getClient(fd)->setAuthentication(true);

		std::string welcome = ":server 001 " + getClient(fd)->getNickname() + " :Welcome to the IRC network, " + getClient(fd)->getPrefix() + "\r\n";
		std::string yourHost = ":server 002 " + getClient(fd)->getNickname() + " :Your host is server, running version 1.0\r\n";
		std::string created = ":server 003 " + getClient(fd)->getNickname() + " :This server was created today\r\n";
		std::string myInfo = ":server 004 " + getClient(fd)->getNickname() + " server 1.0 o o\r\n";

		send(getClient(fd)->getSocket(), welcome.c_str(), welcome.length(), 0);
		send(getClient(fd)->getSocket(), yourHost.c_str(), yourHost.length(), 0);
		send(getClient(fd)->getSocket(), created.c_str(), created.length(), 0);
		send(getClient(fd)->getSocket(), myInfo.c_str(), myInfo.length(), 0);

		std::cout << "Client " << fd << " registered successfully\n";
	}
}

void	Server::setClientPassword(std::vector<std::string>& params, int fd)
{
	if (getClient(fd)->getRegistration() == true)
	{
		std::string reregisterMsg = ":server 462 * :You may not reregister\r\n";
		send(getClient(fd)->getSocket(), reregisterMsg.c_str(), reregisterMsg.length(), 0);
		return;
	}

	if (params.empty())
	{
		std::string paramsMsg = ":server 461 * PASS :Not enough parameters\r\n";
		send(getClient(fd)->getSocket(), paramsMsg.c_str(), paramsMsg.length(), 0);
		return;
	}

	getClient(fd)->setPassword(params[0]);

	if (!serverPassword.empty() && getClient(fd)->getPassword() != serverPassword)
	{
		std::string passMismatchMsg = ":server 464 * :Password incorrect\r\n";
		send(getClient(fd)->getSocket(), passMismatchMsg.c_str(), passMismatchMsg.length(), 0);
		return;
	}

	getClient(fd)->setRegistration(true);
	std::cout << "Client " << fd << " password: " << getClient(fd)->getPassword() << std::endl;
	tryAuthentication(fd);
}

void	Server::setClientNickname(std::vector<std::string>& params, int fd)
{
	if (params.empty())
	{
		std::string nickEmptyMsg = ":server 431 * :No nickname given\r\n";
		send(getClient(fd)->getSocket(), nickEmptyMsg.c_str(), nickEmptyMsg.length(), 0);
		return;
	}

	if (params[0].length() > 9)
		params[0] = params[0].substr(0, 9);

	for (size_t i = 0; i < connectedClients.size(); i++)
	{
		if (params[0] == connectedClients[i].getNickname() && connectedClients[i].getSocket() != fd)
		{
			std::string	nickInUseMsg = ":server 433 * " + params[0] + " :Nickname is already in use\r\n";
			send(getClient(fd)->getSocket(), nickInUseMsg.c_str(), nickInUseMsg.length(), 0);
			return;
		}
	}

	// Set nickname and prefix
	getClient(fd)->setNickname(params[0]);
	if (!getClient(fd)->getNickname().empty() && !getClient(fd)->getUsername().empty())
		getClient(fd)->setPrefix(":" + getClient(fd)->getNickname() + "!" + getClient(fd)->getUsername() + "@localhost");
	else
		getClient(fd)->setPrefix(":" + getClient(fd)->getNickname() + "!user@localhost");
	std::cout << "Client " << fd << " nickname: " << getClient(fd)->getNickname() << std::endl;
	std::cout << "Client " << fd << " prefix: " << getClient(fd)->getPrefix() << std::endl;
	tryAuthentication(fd);
}

void	Server::setClientUsername(std::vector<std::string>& params, int fd)
{
	// Check if enough parameters are sent with USER command
	if (params.size() < 4)
	{
		std::string paramsMsg = ":server 461 * USER :Not enough parameters\r\n";
		send(getClient(fd)->getSocket(), paramsMsg.c_str(), paramsMsg.length(), 0);
		return;
	}

	// Check if user already has username
	if (!getClient(fd)->getUsername().empty())
	{
		std::string reregisterMsg = ":server 462 * :You may not reregister\r\n";
		send(getClient(fd)->getSocket(), reregisterMsg.c_str(), reregisterMsg.length(), 0);
		return;
	}

	// Set username and prefix
	getClient(fd)->setUsername(params[0]);
	if (!getClient(fd)->getNickname().empty())
		getClient(fd)->setPrefix(":" + getClient(fd)->getNickname() + "!" + getClient(fd)->getUsername() + "@localhost");
	std::cout << "Client " << fd << " username: " << getClient(fd)->getUsername() << std::endl;
	std::cout << "Client " << fd << " prefix: " << getClient(fd)->getPrefix() << std::endl;
	tryAuthentication(fd);
}
