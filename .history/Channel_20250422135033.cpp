/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roberto <roberto@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:32:56 by vini              #+#    #+#             */
/*   Updated: 2025/04/22 13:50:33 by roberto          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(){}
Channel::Channel(std::string name) : name(name) {}
Channel::Channel(const Channel& toCopy){*this = toCopy;}
Channel&	Channel::operator=(const Channel& toAssign)
{
	if (this != &toAssign)
	{
		this->members = toAssign.members;
		this->name = toAssign.name;
	}
	return *this;
}
Channel::~Channel(){}

void	Channel::setName(std::string channelName){this->name = channelName;}
void	Channel::addMember(int memberFd) {members.push_back(memberFd);}
bool	Channel::removeMember(int memberFd)
{
	bool result = false;
	for (size_t i = 0; i < members.size(); i++)
		if (members[i] == memberFd)
		{
			members.erase(members.begin() + i);
			result = true;
		}


	// std::vector<int>::iterator it = std::find(members.begin(), members.end(), memberFd);
	// if (it != members.end())
	// 	members.erase(it);
	return (result);
}
std::vector<int>	Channel::getMembers() {return this->members;}
std::string	Channel::getName(){return this->name;}

