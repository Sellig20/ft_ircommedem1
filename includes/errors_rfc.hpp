#ifndef IRC_ERROR_CODES_H
#define IRC_ERROR_CODES_H

// Codes d'erreur RFC 1459
#define ERR_NOSUCHNICK     "401"
#define ERR_NOSUCHSERVER   "402"
#define ERR_NOSUCHCHANNEL  "403"
#define ERR_CANNOTSENDTOCHAN "404"
#define ERR_TOOMANYCHANNELS "405"
#define ERR_WASNOSUCHNICK   "406"
#define ERR_TOOMANYTARGETS  "407"
#define ERR_NOORIGIN       "409"
#define ERR_NORECIPIENT    "411"
#define ERR_NOTEXTTOSEND   "412"
#define ERR_NOTOPLEVEL     "413"
#define ERR_WILDTOPLEVEL   "414"
#define ERR_UNKNOWNCOMMAND "421"
#define ERR_NOMOTD         "422"
#define ERR_NOADMININFO    "423"
#define ERR_FILEERROR      "424"
#define ERR_NONICKNAMEGIVEN "431"
#define ERR_ERRONEUSNICKNAME "432"
#define ERR_NICKNAMEINUSE  "433"
#define ERR_NICKCOLLISION  "436"
#define ERR_USERNOTINCHANNEL "441"
#define ERR_NOTONCHANNEL   "442"
#define ERR_USERONCHANNEL  "443"
#define ERR_NOLOGIN        "444"
#define ERR_SUMMONDISABLED "445"
#define ERR_USERSDISABLED  "446"
#define ERR_NOTREGISTERED  "451"
#define ERR_NEEDMOREPARAMS "461"
#define ERR_ALREADYREGISTRED "462"
#define ERR_NOPERMFORHOST  "463"
#define ERR_PASSWDMISMATCH "464"
#define ERR_YOUREBANNEDCREEP "465"
#define ERR_KEYSET         "467"
#define ERR_CHANNELISFULL  "471"
#define ERR_UNKNOWNMODE    "472"
#define ERR_INVITEONLYCHAN "473"
#define ERR_BANNEDFROMCHAN "474"
#define ERR_BADCHANNELKEY  "475"
#define ERR_NOPRIVILEGES   "481"
#define ERR_CHANOPRIVSNEEDED "482"
#define ERR_CANTKILLSERVER "483"
#define ERR_NOOPERHOST     "491"
#define ERR_UMODEUNKNOWNFLAG "501"
#define ERR_USERSDONTMATCH "502"
#define ERR_GHOSTEDCLIENT  "503"
#define ERR_USERNOTONSERV  "504"
#define ERR_SILELISTFULL   "511"
#define ERR_TOOMANYWATCH   "512"
#define ERR_BADPING        "513"
#define ERR_TOOMANYDCC     "514"
#define ERR_BADEXPIRE      "515"
#define ERR_DONTCHEAT      "516"
#define ERR_DISABLED       "517"
#define ERR_WHOSYNTAX      "522"
#define ERR_WHOLIMEXCEED   "523"
#define ERR_WHOSYNTAXSEC   "526"
#define ERR_WHOLIMEXCEEDSEC "526"
#define ERR_REMOTEPFX      "550"
#define ERR_PFXUNROUTABLE  "551"
#define ERR_BADHOSTMASK    "553"
#define ERR_HOSTUNAVAIL    "554"
#define ERR_USINGSLINE     "555"
#define ERR_STATSSLINE     "556"
#define ERR_SERVICECONFUSED "557"
#define ERR_NOPROTORCOL    "560"
#define ERR_CHANOPEN       "713"
#define ERR_KICKEDFROMCHAN "708"
#define ERR_BADCHANPASS    "715"
#define ERR_BANLISTFULL "716"
#define ERR_LINKSET        "717"
#define ERR_LINKCHANNEL    "718"
#define ERR_BADCHANMASK    "718"
#define ERR_WHOTRUNC       "520"
#define ERR_HELPNOTFOUND   "524"
#define ERR_BANEXPIRED     "525"
#define ERR_RESTRICTED     "484"
#define ERR_UNIQOPRIVSNEEDED "485"
#define ERR_NOFEATURE      "1001"
#define ERR_BADFEATURE     "1002"
#define ERR_BADLOGTYPE     "1003"
#define ERR_BADLOGSYS      "1004"
#define ERR_BADLOGVALUE    "1005"
#define ERR_ISOPERLCHAN    "1006"
#define ERR_CHANOWNPRIVNEEDED "1007"
#define ERR_BADCHANNAME    "1009"

// Messages d'erreur correspondants
// const char* ERR_NOSUCHNICK_MSG = "No such nick/channel";
// const char* ERR_NOSUCHSERVER_MSG = "No such server";
// const char* ERR_NOSUCHCHANNEL_MSG = "No such channel";
// const char* ERR_CANNOTSENDTOCHAN_MSG = "Cannot send to channel";
// const char* ERR_TOOMANYCHANNELS_MSG = "You have joined too many channels";
// const char* ERR_WASNOSUCHNICK_MSG = "Nickname given was not found";
// const char* ERR_TOOMANYTARGETS_MSG = "Too many recipients";
// const char* ERR_NOORIGIN_MSG = "No origin specified";
// const char* ERR_NORECIPIENT_MSG = "No recipient given";
// const char* ERR_NOTEXTTOSEND_MSG = "No text to send";
// const char* ERR_NOTOPLEVEL_MSG = "No toplevel domain specified";
// const char* ERR_WILDTOPLEVEL_MSG = "Wildcard in toplevel domain";
// const char* ERR_UNKNOWNCOMMAND_MSG = "Unknown command";
// const char* ERR_NOMOTD_MSG = "MOTD File is missing";
// const char* ERR_NOADMININFO_MSG = "No administrative info available";
// const char* ERR_FILEERROR_MSG = "File error doing requested action";
// const char* ERR_NONICKNAMEGIVEN_MSG = "No nickname given";
// const char* ERR_ERRONEUSNICKNAME_MSG = "Erroneous nickname";
// const char* ERR_NICKNAMEINUSE_MSG = "Nickname is already in use";
// const char* ERR_NICKCOLLISION_MSG = "Nickname collision KILL";
// const char* ERR_USERNOTINCHANNEL_MSG = "They aren't on that channel";
// const char* ERR_NOTONCHANNEL_MSG = "You're not on that channel";
// const char* ERR_USERONCHANNEL_MSG = "is already on channel";
// const char* ERR_NOLOGIN_MSG = "User not logged in";
// const char* ERR_SUMMONDISABLED_MSG = "SUMMON has been disabled";
// const char* ERR_USERSDISABLED_MSG = "USERS has been disabled";
// const char* ERR_NOTREGISTERED_MSG = "You have not registered";
// const char* ERR_NEEDMOREPARAMS_MSG = "Not enough parameters";
// const char* ERR_ALREADYREGISTRED_MSG = "You may not reregister";
// const char* ERR_NOPERMFORHOST_MSG = "Your host isn't among the privileged";
// const char* ERR_PASSWDMISMATCH_MSG = "Password incorrect";
// const char* ERR_YOUREBANNEDCREEP_MSG = "You are banned from this server";
// const char* ERR_KEYSET_MSG = "Channel key already set";
// const char* ERR_CHANNELISFULL_MSG = "Cannot join channel (+l)";
// const char* ERR_UNKNOWNMODE_MSG = "is unknown mode char to me";
// const char* ERR_INVITEONLYCHAN_MSG = "Cannot join channel (+i)";
// const char* ERR_BANNEDFROMCHAN_MSG = "Cannot join channel (+b)";
// const char* ERR_BADCHANNELKEY_MSG = "Cannot join channel (+k)";
// const char* ERR_NOPRIVILEGES_MSG = "Permission Denied- You're not an IRC operator";
// const char* ERR_CHANOPRIVSNEEDED_MSG = "You're not channel operator";
// const char* ERR_CANTKILLSERVER_MSG = "You cant kill a server!";
// const char* ERR_NOOPERHOST_MSG = "No O-lines for your host";
// const char* ERR_UMODEUNKNOWNFLAG_MSG = "Unknown mode flag";
// const char* ERR_USERSDONTMATCH_MSG = "Cannot change mode for other users";
// const char* ERR_GHOSTEDCLIENT_MSG = "Ghosted client";
// const char* ERR_USERNOTONSERV_MSG = "User not on server";
// const char* ERR_SILELISTFULL_MSG = "SILENCE list is full";
// const char* ERR_TOOMANYWATCH_MSG = "You have too many WATCH entries";
// const char* ERR_BADPING_MSG = "Bad PING command";
// const char* ERR_TOOMANYDCC_MSG = "Too many DCC connections";
// const char* ERR_BADEXPIRE_MSG = "BAD expire time";
// const char* ERR_DONTCHEAT_MSG = "You're not allowed to cheat!";
// const char* ERR_DISABLED_MSG = "Service unavailable - try again later";
// const char* ERR_WHOSYNTAX_MSG = "Cannot WHO to that extent";
// const char* ERR_WHOLIMEXCEED_MSG = "WHO is limited";
// const char* ERR_WHOSYNTAXSEC_MSG = "Cannot WHO to that extent on this connection";
// const char* ERR_WHOLIMEXCEEDSEC_MSG = "WHO is limited on this connection";
// const char* ERR_REMOTEPFX_MSG = "Remote server has no prefix";
// const char* ERR_PFXUNROUTABLE_MSG = "Prefix from remote server is unrouteable";
// const char* ERR_BADHOSTMASK_MSG = "Bad hostmask";
// const char* ERR_HOSTUNAVAIL_MSG = "Host unavailable";
// const char* ERR_USINGSLINE_MSG = "You are already using that server";
// const char* ERR_STATSSLINE_MSG = "Stats are already set for that server";
// const char* ERR_SERVICECONFUSED_MSG = "Services are confused";
// const char* ERR_NOPROTORCOL_MSG = "Unable to join channel (address is banned)";
// const char* ERR_CHANOPEN_MSG = "Channel parameter doesn't match for ";
// const char* ERR_KICKEDFROMCHAN_MSG = "You have been kicked from ";
// const char* ERR_BADCHANPASS_MSG = "Cannot join channel (need correct key)";
// const char* ERR_BANLISTFULL_ = "Channel list is full";
// const char* ERR_LINKSET_MSG = "You are already linked";
// const char* ERR_LINKCHANNEL_MSG = "Channel is linked";
// const char* ERR_BADCHANMASK_MSG = "Cannot join channel (invalid characters in channel name)";
// const char* ERR_WHOTRUNC_MSG = "WHO list is too long, some names have been truncated";
// const char* ERR_HELPNOTFOUND_MSG = "Help not found";
// const char* ERR_BANEXPIRED_MSG = "Ban has expired";
// const char* ERR_RESTRICTED_MSG = "Your connection is restricted!";
// const char* ERR_UNIQOPRIVSNEEDED_MSG = "You're not the original channel operator";
// const char* ERR_NOFEATURE_MSG = "Requested feature not available";
// const char* ERR_BADFEATURE_MSG = "Bad feature parameter given";
// const char* ERR_BADLOGTYPE_MSG = "Bad log type parameter given";
// const char* ERR_BADLOGSYS_MSG = "Bad log system parameter given";
// const char* ERR_BADLOGVALUE_MSG = "Bad log value parameter given";
// const char* ERR_ISOPERLCHAN_MSG = "Cannot join channel (+L)";
// const char* ERR_CHANOWNPRIVNEEDED_MSG = "You're not the channel owner";
// const char* ERR_BADCHANNAME_MSG = "Bad Channel Name";


#endif // IRC_ERROR_CODES_H