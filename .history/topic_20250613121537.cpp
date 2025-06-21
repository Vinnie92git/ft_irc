/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roberto <roberto@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:47:21 by vini              #+#    #+#             */
/*   Updated: 2025/06/13 12:15:37 by roberto          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::topicChannel(std::string channelName, std::string topic, int fd)
{
	if (!getChannel(channelName))
	{
		std::cout << "\033[31mClient \033[0m" << getClient(fd)->getSocket() << " \033[0m" << channelName << "\033[31m not found\033[0m" << std::endl;
		return;
	}
	// si el usuario no está en el canal significa que no puede ver el topic ni cambiarlo
	else if (!getChannel(channelName)->isMember(fd))
	{
		std::cout << "\033[31mClient \033[0m" << getClient(fd)->getSocket() << "\033[31m, you are not in \033[0m" << channelName << std::endl;
		return;
	}
	if (topic.empty() || topic[0] == ':')
	{
		//If <topic> is an empty string, the topic for the channel will be cleared.
		std::string	topicMsg = ":server 332 " + getClient(fd)->getNickname() + " " + channelName + " :No topic is set\r\n";
		getChannel(channelName)->setTopic(topicMsg);
		send(getClient(fd)->getSocket(), topicMsg.c_str(), topicMsg.length(), 0);
	}
	else if (topic == "not set")
	{
		//If <topic> is not given, either RPL_TOPIC or RPL_NOTOPIC is returned specifying the current channel topic or lack of one.
		send(getClient(fd)->getSocket(), getChannel(channelName)->getTopic().c_str(), getChannel(channelName)->getTopic().length(), 0);
	}
	else
	{
		std::string	topicMsg = ":server 332 " + getClient(fd)->getNickname() + " " + channelName + " :" + topic + "\r\n";
		getChannel(channelName)->setTopic(topicMsg);
		send(getClient(fd)->getSocket(), topicMsg.c_str(), topicMsg.length(), 0);
		/*
		si el topic cambia se enviará un mensaje a todos lso usuarios del canal de que el topic ha cambiado
		si el topic es el mismo que el anterior se enviará el mensaje a todos lso usuarios del canal
		*/
	}
}

void	Server::topicCmd(std::vector<std::string>& params, int fd)
{
	if (!getClient(fd)->getAuthentication())
	{
		std::string authMsg = ":server 451 * :You have not registered\r\n";
		send(getClient(fd)->getSocket(), authMsg.c_str(), authMsg.length(), 0);
		return;
	}

	if (params.empty() || params[0] = "IRC")
	{
		std::cout << "TOPIC error: No channel provided." << std::endl;
		return;
	}
	std::cout << "entrasndo: " << params[0] << std::endl;

	std::vector<std::string> channelNames = splitComma(params[0]);
	std::string channelName = channelNames[0];
	std::string topic = "not set";
	if (params.size() > 1)
		topic = splitComma(params[1])[0];

	std::cout << "topic: " << topic  << "|| " << "channelNames: " << channelName << std::endl;


	if (params.size() >= 1)
	{
		int i = 0;
		std::string channelName = channelNames[i];
		if (channelName[0] != '#' && channelName[0] != '&')
		{
			std::cout << "TOPIC error: Invalid channel name." << std::endl;
			return;
		}
		topicChannel(channelName, topic, fd);
	}
}
