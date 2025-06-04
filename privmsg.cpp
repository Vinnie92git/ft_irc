/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vini <vini@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:47:09 by vini              #+#    #+#             */
/*   Updated: 2025/06/04 18:01:18 by vini             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::privMsgChannel(std::string target, std::string message, int fd)
{
	std::string msg = getClient(fd)->getPrefix() + " PRIVMSG " + target + " :" + message + "\r\n";
	std::vector<int> fdsChannel = getChannel(target)->getMembers();

	for (size_t i = 0; i < fdsChannel.size(); i++)
	{
		if (fdsChannel[i] != fd)
			send(fdsChannel[i], msg.c_str(), msg.length(), 0);
	}
	std::cout << "El canal no existe" << std::endl;
}

void	Server::privMsgUser(std::string target, std::string message, int fd)
{
	std::string msg = getClient(fd)->getPrefix() + " PRIVMSG " + target + " :" + message + "\r\n";

	for (size_t i = 0; i < connectedClients.size(); i++)
	{
		if (connectedClients[i].getNickname() == target)
		{
			std::cout << "El usuario existe" << std::endl;
			send(fd, msg.c_str(), msg.length(), 0); // esto ha que checkearlo
			send(connectedClients[i].getSocket(), msg.c_str(), msg.length(), 0);
		}
	}
}

// Parameters: <target>{,<target>} <text to be sent>
void	Server::privmsgCmd(std::vector<std::string>& params, int fd)
{
	if (!getClient(fd)->getAuthentication())
	{
		std::string authMsg = ":server 451 * :You have not registered\r\n";
		send(getClient(fd)->getSocket(), authMsg.c_str(), authMsg.length(), 0);
		return;
	}

	if (params.size() < 2)
	{
		std::cout << "PRIVMSG error: Not enough parameters provided." << std::endl;
		return;
	}
	std::string	target = params[0];
	std::vector<std::string> targets = splitComma(target);
	std::string	message = "";
	for (size_t i = 1; i < params.size(); i++)
	{
		message += params[i] + " ";
	}

	std::cout << "target: " << target << " " << "message: " << message <<  std::endl;

	for (size_t i = 0; i < targets.size(); i++)
	{
		std::string trg = targets[i];
		if (trg[0] == '#' || trg[0] == '&')
		{
			std::cout << "entrando en el canal" << std::endl;
			privMsgChannel(target, message, fd);
		}
		else
		{
			std::cout << "entrando en el usuario" << std::endl;
			privMsgUser(target, message, fd);
		}
	}
	std::cout << "PRIVMSG error: Invalid target." << std::endl;
	return;
}
