/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vini <vini@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 17:35:56 by vini              #+#    #+#             */
/*   Updated: 2025/03/23 22:48:21 by vini             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <iomanip>
#include "Server.hpp"
#include "Client.hpp"

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "\033[31mUsage: ./ircserv <port> <password>\033[0m" << std::endl;
		return 1;
	}
	Server	server(argv[1], argv[2]);
	signal(SIGINT, Server::signalHandler);
	signal(SIGQUIT, Server::signalHandler);
	server.bootServer();
	return 0;
}