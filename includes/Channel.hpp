#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include "Server.hpp"
#include "Client.hpp"
// class Server;

class Client;

class Channel : public Server
{
	public :
		Channel();
		~Channel();
		Channel(Client *my_client);
		Channel(std::string num, std::string name);
		std::vector<std::string>				getMemberOfActiveChan(Client *my_client, std::map<Channel *, bool> chanList, std::string chan);
		std::vector<std::string> 				getMemberOfThisChan();
		std::vector<std::string> 				getInvitedMembers();
		std::string 							getNameChannel();
		const std::vector<std::string>& 		getOperatorsMembers() const;
		const std::string 						getTopicChannel();
		const std::string 						getNumChannel();
		const std::string& 						getNewKeyMode() const;
		const std::string& 						getPassWord() const;
		unsigned int 							getLimitUserMode() const;
		bool 									isMember(std::string memberName);
		bool 									is_member_invited(std::string nick);
		bool 									is_member_operator(std::string nick);
		bool 									getHasPassword() const;
		bool 									getTopicRestrictionMode() const;
		bool 									getIsSuperOperMode() const;
		bool 									getCanalInviteOnlyMode() const;
		bool 									getIsOperatorMode() const;
		void 									setNameChannel(std::string nameToBe);
		void 									setTopicChannel(std::string topicToBe);
		void 									addMember(const std::string& member);
		void 									setMemberOfthisChan(std::vector<std::string> new_members);
		void 									addInviteMembers(std::string name);
		void 									setIsOperatorMode(bool isOperatorMode);
		void 									setHasPassword(bool isOperatorMode);
		void 									setIsSuperOperMode(bool isSuperOperMode);
		void 									setCanalInviteOnlyMode(bool canalInviteOnlyMode);
		void 									setTopicRestrictionMode(bool topicRestrictionMode);
		void 									setLimitUserMode(unsigned int limitUserMode);
		void 									addOperatorsMembers(std::string name);
		void 									setOperatorsMembers(std::vector<std::string>& operatorsMembers);
		void 									setPassWord(std::string& passWord);
		void 									setNewKeyMode(const std::string& newKeyMode);

	private :
		std::map<Client*, std::string> 			_clientListForChan;
		std::vector<std::string> 				memberOfThisChan;
		std::vector<std::string>				_operators_menbers;
		std::vector<std::string>				_invited_members;
		std::string 							nameChannel;
		std::string 							numChannel;
		std::string 							topicChannel;
		std::string								_newKeyMode;
		std::string								pass_word;
		bool									_isOperatorMode;
		bool									_isSuperOperMode;
		bool									_canalInviteOnlyMode;
		bool									_topicRestrictionMode;
		bool									_hasPassChannel;
		unsigned int							_limitUserMode;
};

std::ostream& operator<<(std::ostream& o, const std::vector<std::string>& memberVector);
std::ostream& operator<<(std::ostream& o, const Channel *memberVector);

#endif