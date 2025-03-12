/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vini <vini@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 17:35:56 by vini              #+#    #+#             */
/*   Updated: 2025/03/12 18:01:15 by vini             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <iomanip>

int	main(int argc, char **argv)
{
	(void)argv;
	if (argc != 3)
	{
		std::cout << "\033[31m Usage: ./ircserv <port> <password> \033[0m" << std::endl;
		return 1;
	}
	return 0;
}