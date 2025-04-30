/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vini <vini@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 17:35:56 by vini              #+#    #+#             */
/*   Updated: 2025/04/30 12:25:52 by vini             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <iomanip>
#include "Server.hpp"
#include "Client.hpp"

bool validPort(const std::string& port)
{
	if (port.empty())
	{
		std::cout << "\033[31mPort required\033[0m" << std::endl;
		return false;
	}
	for (std::string::const_iterator it = port.begin(); it != port.end(); ++it)
	{
		if (!std::isdigit(*it))
		{
			std::cout << "\033[31mInvalid port number\033[0m" << std::endl;
			return false;
		}
	}
	std::istringstream iss(port);
	int portNumber = 0;
	iss >> portNumber;
	if (iss.fail() || !iss.eof())
	{
		std::cout << "\033[31mError in stream\033[0m" << std::endl;
		return false;
	}
	if (portNumber < 1024 || portNumber > 65535)
	{
		std::cout << "\033[31mPort number must not be reserved\033[0m" << std::endl;
		return false;
	}
	return true;
}

bool validPassword(const std::string& password)
{
	if (password.empty() || password.length() < 8 || password.length() > 50)
	{
		std::cout << "\033[31mPassword must have between 8 and 50 characters\033[0m" << std::endl;
		return false;
	}
	for (std::string::const_iterator it = password.begin(); it != password.end(); ++it)
	{
		char c = *it;
		if (c < 33 || c > 126)
		{
			std::cout << "\033[31mPassword must have valid characters\033[0m" << std::endl;
			return false;
		}
	}
	return true;
}

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "\033[31mUsage: ./ircserv <port> <password>\033[0m" << std::endl;
		return 1;
	}
	if (!validPort(argv[1]) || !validPassword(argv[2]))
		return 1;
	Server	server(argv[1], argv[2]);
	try
	{
		signal(SIGINT, Server::signalHandler);
		signal(SIGQUIT, Server::signalHandler);
		server.bootServer();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		server.shutdownServer();
	}
	return 0;
}