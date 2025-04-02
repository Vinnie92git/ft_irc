/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vini <vini@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:32:56 by vini              #+#    #+#             */
/*   Updated: 2025/04/02 14:35:55 by vini             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(){}
Channel::Channel(const Channel& toCopy){*this = toCopy;}
Channel& Channel::operator=(const Channel& toAssign)
{
	if (this != &toAssign)
		this->name = toAssign.name;
	return *this;
}
Channel::~Channel(){}

void	Channel::setName(std::string channelName){this->name = channelName;}

std::string	Channel::getName(){return this->name;}