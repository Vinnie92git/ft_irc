/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vini <vini@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 01:03:51 by vini              #+#    #+#             */
/*   Updated: 2025/03/26 15:00:49 by vini             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

void	Client::setNickname(std::string nickname){this->nickname = nickname;}
void	Client::setUsername(std::string username){this->username = username;}
void	Client::setAddress(std::string address){this->clientAddress = address;}
void	Client::setSocket(int fd){this->clientSocket = fd;}

std::string	Client::getNickname(){return this->nickname;}
std::string	Client::getUsername(){return this->username;}
std::string	Client::getAddress(){return this->clientAddress;}
int	Client::getSocket(){return this->clientSocket;}