#include "../includes/Command.hpp"
#include "../includes/Channel.hpp"
#include <string>
#include <algorithm>

std::vector<std::string> Command::eraseUserFromChan(std::vector<std::string> memberOfThisChan, std::string userNickname)
{
	// size_t i = 0;
	if (!memberOfThisChan.empty())
	{
		for (std::vector<std::string>::iterator it = memberOfThisChan.begin(); it != memberOfThisChan.end(); it++)
		{
			if (*it == userNickname)
			{
				// std::cout << " >>>>>>>>>>>>> it : " << *it << std::endl;
				memberOfThisChan.erase(it);
				break;
			}
			// std::cout << "clientNick name dans erase : " << userNickname;
			std::cout << std::endl;
		}
	}
	return memberOfThisChan;
}

std::string Command::builtReasonWhy(std::vector<std::string> reason)
{
	std::string sample;
	std::cout << ">>>>>>>>>> REASON >>>>>>> " << reason << std::endl;
	for(size_t j = 0; j < reason.size(); j ++)
	{
		sample.append(reason[j]);
		sample.append(" ");
	}
	_flagPart = 1;
	std::cout << ">>>>>>>>>>>>>>> SAMPLE >>>>>>>>>> " << sample << std::endl;
	return (sample);
}

void Command::part()
{
	Server *server = my_client->getMyServer();

	std::map<Channel *, bool> chanList = server->GetChannelList();
	std::vector<std::string> memberOfThisChan;
	std::vector<std::string> tabSeg;

	//ON VA RECUP COMMAND_LEFTOVERS POUR PARSER CHANNEL VS REASON
	std::istringstream ss(command_leftovers);
	std::string seg;
	std::string ind;
	std::vector<std::string> toReason;
	std::cout << "+++++++++++++++++++++++++++++++++++++ " << command_leftovers << std::endl;
	
	if (command_leftovers.find(',') != std::string::npos) //SI Y A QUE UNE CHANNEL A FERMER
		std::cout << "YESSAI" << std::endl;
	else //SI Y A PAS PLUSIEURS CHANNEL A FERMER
	{
		std::cout << "NOSSAI" << std::endl;
		while (std::getline(ss, seg, ' '))
		{
			tabSeg.push_back(seg);
		}
		if (!tabSeg.empty() && tabSeg[0].find('#') != std::string::npos)
		{
			std::cout << "daaaaaaaaaaaaaaaaaaaaans tabSeg pour to Reason" << std::endl;
			for (size_t i = 1; i < tabSeg.size(); i++)
			{
				std::string test;
				test = tabSeg[i];
				if (test.find(':'))
					test.erase(0, 1);
				std::cout << "---------------------------------------------------------------> " << test << std::endl;
				toReason.push_back(tabSeg[i]); 
			}
		}
	}
	
	//JE METS LE DERNIER DANS IND = LA REASON 
	ind = tabSeg.back();
	

	std::cout << ">>>>>>> TAB SEG >>>>>>> " << tabSeg << std::endl;
	std::cout << ">>>>>>>>>> IND >>>>>>>>> " << ind << std::endl;

	std::istringstream ss1(ind);
	std::string r;
	std::vector<std::string> reason;
	while (std::getline(ss1, r, ' '))
	{
		//PUSHBACK EN FONCTION DES ESPACES : LE PREMIER EST UNE CHANNEL JE DOIS L'EXTRAIRE
		reason.push_back(r);
	}

	tabSeg.pop_back();

	// exit(1);

	tabSeg.push_back(reason.front());
	//LA JE VIENS DE REMPLACER [CHANNEL + REASON] PAS [REASON]

	reason.erase(reason.begin());
	//LA JE VIENS DE SUPPRIMER LA CHANNEL DE MON VECTOR DE REASON
	// for(size_t i = 0; i < reason.size(); i++)
	// {
		std::cout << ">>>>>> REASON_VECTOR = " << reason << std::endl;
	// }

	for(size_t j = 0; j < tabSeg.size(); j++)
	{
		// std::cout << "Channel to quit -----------> " << tabSeg[j] << std::endl;
	}
	
	std::string reasonWhy = builtReasonWhy(reason);

	exit(1);
	// std::cout << "Reason why -----------> " << reasonWhy << std::endl;
	for (std::map<Channel *, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
	{//LOOP DANS MES CHANNELS 
		std::string channelToFind = it->first->getNameChannel();
		// std::string clientName = my_client->getname();
		std::string clientNickname = my_client->getNickname();
		// std::cout << "clientName = " << clientNickname ;


		memberOfThisChan = it->first->getMemberOfThisChan();
		std::vector<std::string>::iterator itTS;

		for (itTS = tabSeg.begin(); itTS != tabSeg.end(); itTS++)
		{//LOOP DANS LE NOM DE CHANNEL ENVOYE EN ARGV
			std::vector<std::string>::iterator ita = std::find(memberOfThisChan.begin(), memberOfThisChan.end(), clientNickname);
			//FIND LOOP DANS LES MEMBRES ACTIFS DE LA CHANNEL ACTIVE

			// std::cout << "channelToFind = " << channelToFind << " | *itTs = " << *itTS <<std::endl;
			// std::cout << " | *ita = " << *ita << std::endl;
			if (channelToFind == *itTS && ita != memberOfThisChan.end() && it->second == true)
			{

				// error_code = "!";
				std::string ch;
				if (!channelToFind.empty())
					ch = channelToFind.erase(0, 1);
				std::string channelToFind = it->first->getNameChannel();
			
				// if (!reasonWhy.empty())
					// response_buffer.append(" because : " + reasonWhy);
				response_buffer.append(":" + clientNickname + "!" + my_client->getUsername() + "@localhost PART " + channelToFind + " " + ch + "\r\n");
				is_ready = true;
				is_not_accepted = false;
				setConcernedClients(clientNickname);
				status = SINGLE_SEND;
				memberOfThisChan = eraseUserFromChan(memberOfThisChan, clientNickname);
				break;
			}
			else if (channelToFind == *itTS && *ita != clientNickname && it->second == true)
			{
				error_code = "442";
				response_buffer.append(" You're not on that chaaaaaaaaaaaaannel");
				is_ready = true;
				is_not_accepted = true;
				setConcernedClients(clientNickname);
			}
			else
			{
				error_code = "403";
				response_buffer.append(" No such chaaaaaaaaaaaaaaaannel\n");
				is_ready = true;
				is_not_accepted = true;
				setConcernedClients(clientNickname);
			}
		}
	}
	std::cout << std::endl;

}