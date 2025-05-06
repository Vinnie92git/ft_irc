/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roberto <roberto@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:53:20 by vini              #+#    #+#             */
/*   Updated: 2025/05/06 14:21:06 by roberto          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::setClientPassword(std::vector<std::string>& params, int fd)
{
	// Check if client already registered
	if (getClient(fd)->getRegistration() == true)
	{
		std::string err = ":server 462 * :You may not reregister\r\n";
		send(getClient(fd)->getSocket(), err.c_str(), err.length(), 0);
		return;
	}

	// Check if PASS command has the required parameter
	if (params.empty())
	{
		std::string errorMsg = ":server 461 * PASS :Not enough parameters\r\n";
		send(getClient(fd)->getSocket(), errorMsg.c_str(), errorMsg.length(), 0);
		return;
	}

	// Set password
	getClient(fd)->setPassword(params[0]);

	// Check password validity
	if (serverPassword != "" && getClient(fd)->getPassword() != serverPassword)
	{
		std::string passMismatchMsg = ":server 464 * :Password incorrect\r\n";
		send(getClient(fd)->getSocket(), passMismatchMsg.c_str(), passMismatchMsg.length(), 0);
		return;
	}

	getClient(fd)->setRegistration(true);
	std::cout << "Client " << fd << " password: " << getClient(fd)->getPassword() << std::endl;
}

void	Server::setClientNickname(std::vector<std::string>& params, int fd)
{
	for (size_t i = 0; i < connectedClients.size(); i++)
	{
		if (params[0] == connectedClients[i].getNickname())
		{
			std::string	nickInUseMsg = ":server 433 " + params[0] + " :Nickname is already in use\r\n";
			send(getClient(fd)->getSocket(), nickInUseMsg.c_str(), nickInUseMsg.length(), 0);
		}
	}
	getClient(fd)->setNickname(params[0]);
	getClient(fd)->setPrefix(":" + getClient(fd)->getNickname() + "!user@localhost");
	std::cout << "Client " << fd << " nickname: " << getClient(fd)->getNickname() << std::endl;
	std::cout << "Client " << fd << " prefix: " << getClient(fd)->getPrefix() << std::endl;
}

void	Server::setClientUsername(std::vector<std::string>& params, int fd)
{
	getClient(fd)->setUsername(params[0]);
	std::cout << "Client " << fd << " username: " << getClient(fd)->getUsername() << std::endl;
}

void	Server::joinCmd(std::vector<std::string>& params, int fd)
{
	if (params.empty())
	{
		std::cout << "JOIN error: No channel provided." << std::endl;
		return ;
	}

	std::vector<std::string> channelNames = splitComma(params[0]);

	for (size_t i = 0; i < channelNames.size(); i++)
	{
		std::string channelName = channelNames[i];
		if (channelName[0] != '#')
		{
			std::cout << "JOIN error: Invalid channel name." << std::endl;
			return ;
		}
		joinChannel(channelName, fd);
	}
}

void Server::partCmd(std::vector<std::string>& params, int fd) // part <channel> <reason>
{
	if (params.empty())
	{
		std::cout << "PART error: No channel provided." << std::endl;
		return;
	}

	std::string channelNames = params[0];
	std::string reason = "";

	if (params.size() > 1)
		reason = params[1];

	std::vector<std::string> channels = splitComma(channelNames);

	for (size_t i = 0; i < channels.size(); i++)
	{
		std::string channelName = channels[i];
		if (channelName[0] == '&')
			channelName[0] = '#';
		if (channelName[0] != '#')
		{
			std::cout << "PART error: Invalid channel name." << std::endl;
			return;
		}
		partChannel(channelName, reason, fd);
	}
}

void	Server::quitCmd(std::vector<std::string>& params, int fd) // /quit <reason>
{
	std::cout << "entrasndo" << std::endl;
	std::string reason = "";
	if (params.size() == 1)
		std::string reason = splitComma(params[0])[0];
	quitServer(reason, fd);
}

void	Server::topicCmd(std::vector<std::string>& params, int fd)
{
	if (params.empty())
	{
		std::cout << "PART error: No channel provided." << std::endl;
		return;
	}
	std::cout << "entrasndo: " << params[0] << " " << params[1] << std::endl;

	std::vector<std::string> channelNames = splitComma(params[0]);
	std::string topic = "not set";



	if (params.size() >= 2)
		std::string topic = splitComma(params[1])[0];


	if (params.size() >= 1)
	{
		int i = 0;
		std::string channelName = channelNames[i];
		if (channelName[0] != '#')
		{
			std::cout << "TOPIC error: Invalid channel name." << std::endl;
			return;
		}
		topicChannel(channelName, topic, fd);
	}
}

void Server::kickCmd(std::vector<std::string>& params, int fd)
{
	//si el usuario no es operador no puedes echarlo
	if (params.size() < 2)
	{
		std::cout << "KICK error: Not enough parameters provided." << std::endl;
		return;
	}
	//std::cout << "entrasndo: " << params[0] << " " << params[1] << " " << params[2]  << std::endl;
	std::string channelNames = params[1];
	std::string user = params[2];
	std::string reason = "";
	//std::cout << "entrasndo 2: " << channelNames << " " << user << " "  << std::endl;

	if (params.size() > 2)
		reason = params[2];
	std::vector<std::string> channels = splitComma(channelNames);

	for (size_t i = 0; i < channels.size(); i++)
	{
		std::string channelName = channels[i];
		if (channelName[0] == '&')
			channelName[0] = '#';
		if (channelName[0] != '#')
		{
			std::cout << "KICK error: Invalid channel name." << std::endl;
			return;
		}
		//getChannel(channelName)->addOpUser(fd); hasta que pueda añadir operadores de usuario tiene que descomentarse para funcionar
		if (getChannel(channelName)->isOpMember(fd) == true)
		{
			kickUserFromChannel(channelName, user, reason, fd);
		}
		else
			std::cout << "KICK error: the user is not in the channel." << std::endl;
	}
}

void	Server::inviteCmd(std::vector<std::string>& params, int fd)
{
	if (params.size() != 2)
	{
		std::cout << "INVITE error: wrong number of parameters provided." << std::endl;
		return;
	}

	std::string channelName = params[1];
	std::string nickname = params[0];
	inviteUserToChannel(channelName, user, fd);

}


/*

Command: MODE	target			+-
  Parameters: <canal/user> [<modestring> [<mode arguments>...]]
	MODE #foobar +mb *@127.0.0.1

*/
/* void Server::modeCmd(std::vector<std::string>& params, int fd)
{
	if (params.empty())
	{
		std::cout << "MODE error: No target provided." << std::endl;
		return ;
	}
	if (params.size() == 1)
	{
		std::cout << "0 entras" << std::endl;
		std::vector<std::string> ChannelNameTofind = splitComma(params[0]);
		std::cout << "ChannelNameTofind: " << ChannelNameTofind[0] << std::endl;
		for (size_t i = 0; i < channels.size(); i++)
		{
			if (channels[i].getName() == ChannelNameTofind[0])
			{
				std::cout << "linea 79 Channel found: " << channels[i].getName() << std::endl;
				std::string channelName = ChannelNameTofind[0];
				if (channelName[0] != '#')
				{
					std::cout << "MODE error: Invalid channel name." << std::endl;
					return;
				}
				modeTarget(channels[i].getName(), params,fd, 1);
			}
		}
	}
	std::cout << "entrando1" << std::endl;
	std::cout << "params: " << params[0] << " " << params[1] << std::endl;


	std::vector<std::string> modestring = splitComma(params[1]);
	std::string mode = modestring[0];
	std::cout << "entrando2 printea modestring y mode"<< modestring[0] << " " << mode << std::endl;

	if (mode[0] != '+' && mode[0] != '-')
	{
		std::cout << "MODE error: Invalid modestring." << std::endl;
		return;
	}


	//std::cout << "entrando3" << std::endl;
	//	si es un canal entra aquí
	//recorrer el vector de canales y si lo encuentra entrar

	std::vector<std::string> ChannelNameTofind = splitComma(params[0]);
	std::cout << "ChannelNameTofind: " << ChannelNameTofind[0] << std::endl;

	std::cout << "entrando4" << std::endl;

	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i].getName() == ChannelNameTofind[0])
		{
			std::cout << "Channel found: " << channels[i].getName() << std::endl;
			std::string channelName = ChannelNameTofind[0];
			if (channelName[0] != '#')
			{
				std::cout << "MODE error: Invalid channel name." << std::endl;
				return;
			}
			modeTarget(channels[i].getName(), params,fd, 1);
		}
	}

	std::cout << "entrando5" << std::endl;
	// si es un usuario entra aquí
	std::vector<std::string> UserNameTofind = splitComma(params[0]);
	std::cout << "UserNameTofind: " << ChannelNameTofind[0] << std::endl;

	for (size_t i = 0; i < connectedClients.size(); i++)
	{
		if (connectedClients[i].getNickname() == UserNameTofind[0])
		{
			std::cout << "User found: " << connectedClients[i].getNickname() << std::endl;
			modeTarget(connectedClients[i].getNickname(), fd, 0);
		}
	}
	std::cout << "entrando6" << std::endl;

	std::cout << "MODE error: Target not found." << std::endl;
}
 */
std::vector<std::string>	Server::splitComma(std::string param)
{
	std::vector<std::string>	tokenVector;
	std::istringstream			iss(param);
	std::string					token;

	while(std::getline(iss, token, ','))
	{
		if (!token.empty())
			tokenVector.push_back(token);
	}

	return tokenVector;
}

void	Server::joinChannel(std::string channelName, int fd)
{
	if (getChannel(channelName))
	{
		getChannel(channelName)->addMember(fd);
		std::cout << "\033[32mClient \033[0m" << getClient(fd)->getSocket() << "\033[32m joined the channel \033[0m" << channelName << std::endl;
	}
	else
	{
		Channel	newChannel(channelName);
		newChannel.addMember(fd);
		channels.push_back(newChannel);
		std::cout << "\033[32mClient \033[0m" << getClient(fd)->getSocket() << "\033[32m created the channel \033[0m" << channelName << std::endl;
	}

	std::string joinMsg = getClient(fd)->getPrefix() + " JOIN :" + channelName + "\r\n";
	std::string	topicMsg = ":server 332 " + getClient(fd)->getNickname() + " " + channelName + " :No topic is set\r\n";
	std::string	namesMsg = ":server 353 " + getClient(fd)->getNickname() + " = " + channelName + " :";
	for (size_t i = 0; i < getChannel(channelName)->getMembers().size(); i++)
	{
		int	memberFd = getChannel(channelName)->getMembers()[i];
		namesMsg += getClient(memberFd)->getNickname() + " ";
	}
	namesMsg += "\r\n";
	std::string	endMsg = ":server 366 " + getClient(fd)->getNickname() + " " + channelName + " :End of /NAMES list\r\n";

	send(getClient(fd)->getSocket(), joinMsg.c_str(), joinMsg.length(), 0);
	send(getClient(fd)->getSocket(), topicMsg.c_str(), topicMsg.length(), 0);
	send(getClient(fd)->getSocket(), namesMsg.c_str(), namesMsg.length(), 0);
	send(getClient(fd)->getSocket(), endMsg.c_str(), endMsg.length(), 0);
}

/* void	Server::modeTarget(std::string channelName, std::vector<std::string>& params, int fd, int mode)
{
	(void)channelName;
	(void)fd;
	std::cout << "entrando7" << std::endl;
		MODE <target> [<modestring> [<mode arguments>...]]

	//0 es un usuario


	//1 es un canal
	if (mode == 1)
	{
		if(params.size() == 1)
		{
			std::cout << "requesting modes for channel: " << channelName << std::endl;
		}
		else if(params.size() >= 2)
		{
			std::cout << "setting mode for channel: " << channelName << " with mode: " << params[1] << " with args: " << params[2] << std::endl;
		}
	}
}
	*/

void	Server::partChannel(std::string channelName, std::string reason, int fd)
{
	if (getChannel(channelName))
	{
		std::string partMsg;
		if (getChannel(channelName)->removeMember(fd))
		{
			std::cout << "\033[31mClient \033[0m" << getClient(fd)->getSocket() << "\033[31m left the channel \033[0m" << channelName << std::endl;
			if (getChannel(channelName)->getMembers().empty())
			{
				removeChannel(channelName);
				std::cout << "\033[31mChannel \033[0m" << channelName << "\033[31m removed\033[0m" << std::endl;
			}
			if (reason != "")
				partMsg = getClient(fd)->getPrefix() + " PART :" + channelName + " :" + reason + "\r\n";
			else
				partMsg = getClient(fd)->getPrefix() + " PART :" + channelName + "\r\n";

			send(getClient(fd)->getSocket(), partMsg.c_str(), partMsg.length(), 0);
		}
	}
	else
		std::cout << "\033[31mClient \033[0m" << getClient(fd)->getSocket() << "\033[31m, you are not in \033[0m" << channelName << std::endl;

}

// cuando un usuario se desconecta no muestra que el usuario se ha ido del canal
void	Server::quitServer(std::string reason, int fd)
{
	for (size_t i = 0; i < channels.size(); i++)
	{
		std::cout << "entrasndo" << std::endl;
		std::string channelName = channels[i].getName();
		partChannel(channelName, "", fd);
	}
	std::string quitServerMsg = getClient(fd)->getPrefix() + " QUIT :" + reason + "\r\n";
	send(getClient(fd)->getSocket(), quitServerMsg.c_str(), quitServerMsg.length(), 0);

	// en caso de que se puedan enviar mensajes privados a otros usuarios tambien habría que borrar el chat
}

void	Server::topicChannel(std::string channelName, std::string topic, int fd)
{
	/*
	si el topic está protegido tendría que tener permisos para modificarlo

	*/
	// si el canal no existe no puedes hacer nada
	if (!getChannel(channelName))
	{
		std::cout << "_____________-1_______________" << std::endl;

		std::cout << "\033[31mClient \033[0m" << getClient(fd)->getSocket() << " \033[0m" << channelName << "\033[31m not found\033[0m" << std::endl;
		return;
	}
	// si el usuario no está en el canal significa que no puede ver el topic ni cambiarlo
	else if (!getChannel(channelName)->isMember(fd))
	{
		std::cout << "_____________-2_______________" << std::endl;
		std::cout << "\033[31mClient \033[0m" << getClient(fd)->getSocket() << "\033[31m, you are not in \033[0m" << channelName << std::endl;
		return;
	}
	std::cout << "entrando, channelname:" << channelName << " topic:" << topic << std::endl;
	if (topic.empty() || topic[0] == ':')
	{
		std::cout << "_____________1_______________" << std::endl;
		//If <topic> is an empty string, the topic for the channel will be cleared.
		std::string	topicMsg = ":server 332 " + getClient(fd)->getNickname() + " " + channelName + " :No topic is set\r\n";
		getChannel(channelName)->setTopic(topicMsg);
		send(getClient(fd)->getSocket(), topicMsg.c_str(), topicMsg.length(), 0);
	}
	else if (topic == "not set")
	{
		std::cout << "_____________2_______________" << std::endl;
		//If <topic> is not given, either RPL_TOPIC or RPL_NOTOPIC is returned specifying the current channel topic or lack of one.
		send(getClient(fd)->getSocket(), getChannel(channelName)->getTopic().c_str(), getChannel(channelName)->getTopic().length(), 0);
	}
	else
	{
		std::cout << "_____________3_______________" << std::endl;
		std::string	topicMsg = ":server 332 " + getClient(fd)->getNickname() + " " + channelName + " :" + topic + "\r\n";
		getChannel(channelName)->setTopic(topicMsg);
		send(getClient(fd)->getSocket(), topicMsg.c_str(), topicMsg.length(), 0);
		/*
		si el topic cambia se enviará un mensaje a todos lso usuarios del canal de que el topic ha cambiado
		si el topic es el mismo que el anterior se enviará el mensaje a todos lso usuarios del canal
		*/
	}
}

void	Server::kickUserFromChannel(std::string channelName, std::string user, std::string reason, int fd)
{
	int userSocket = -1;


	for (size_t i = 0; i < connectedClients.size(); i++)
	{
		if (connectedClients[i].getNickname() == user)
			userSocket = connectedClients[i].getSocket();
	}

	if (userSocket != -1)
		std::cout << "El socket para el nickname " << user << " es: " << userSocket << std::endl;
	else
		std::cout << "No se encontró el socket para el nickname " << user << std::endl;


	if (getChannel(channelName)->isMember(fd) && getChannel(channelName)->isMember(userSocket))
	{
		std::cout << "entrando" << std::endl;
		std::string kickMsg = getClient(userSocket)->getPrefix() + " KICK " + channelName + " " + user + " :" + reason + "\r\n";
		send(getClient(userSocket)->getSocket(), kickMsg.c_str(), kickMsg.length(), 0);
	}
	else
		std::cout << "\033[31mClient \033[0m" << getClient(fd)->getSocket() << "\033[31m, you are not in \033[0m" << channelName << std::endl;
}

