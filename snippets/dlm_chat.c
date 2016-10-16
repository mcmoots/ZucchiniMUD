/****************************************************************************
 * Chat channels Code                                                       *
 * Markanth : dlmud@dlmud.com                                               *
 * Devil's Lament : dlmud.com port 3778                                     *
 * Web Page : http://www.dlmud.com                                          *
 *                                                                          *
 * Allows for player to create, own, and modify chat channels.              *
 *                                                                          *
 * All I ask in return is that you give me credit on your mud somewhere     *
 * or email me if you use it.                                               *
 ***************************************************************************/

>>> CHAT.C <<<

/*
Syntax: chat <channel number> between 1 and 999
Syntax: chat <message>
Syntax: chat 0             Syntax: chat vanish
Syntax: chwho              Syntax: boot <target>
Syntax: ownchan            Syntax: unownchan
Syntax: block              Syntax: topic <topic>
Syntax: chpass <password>

The chat channel allows you to create, join, or talk on one of 999 chat 
channels.  "Chwho" is similar to who, allowing you to see who is on what
chat channels.  "Chown" and "unchown" allow you to moderate a channel or
relinquish moderation respectively.  You can hide yourself from chwho
using the "chat vanish" option. Owners of channels may "boot" a player
off a channel.  To leave a chat channel use "chat 0".  It is not necessary 
to leave a channel to join another.  The Block command is used to make your
channel private so no one else can use it.  You can allow access to private
channels via the "chpass" command. The "topic" command allows the owner to 
set a topic for the channel which will show on chwho.  
*/
#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "merc.h"
#include "chat.h"

void broadcast_chat_channel args ((CHAR_DATA * ch, const char *messg));
CHAT_DATA chat_table[MAX_CHAT];

void init_chat_channels (void)
{
	int i;

	for (i = 0; i < MAX_CHAT; i++)
	{
		chat_table[i].topic = &str_empty[0];
		chat_table[i].password = &str_empty[0];
		chat_table[i].owner = NULL;
		chat_table[i].Private = FALSE;
		chat_table[i].chatters = 0;
	}
}

void clear_chat_channel (int chan)
{
	chat_table[chan].Private = FALSE;
	chat_table[chan].owner = NULL;
	free_string (chat_table[chan].topic);
	chat_table[chan].topic = &str_empty[0];
	free_string (chat_table[chan].password);
	chat_table[chan].password = &str_empty[0];
}

void enter_chat_channel (CHAR_DATA * ch, int chan)
{
	char buf[MAX_STRING_LENGTH];

	if (!VALID_CHAT_CHAN (chan))
		return;

	ch->chat = chan;
	chat_table[chan].chatters += 1;

	sprintf (buf, "$n has entered chat room %d.", chan);
	broadcast_chat_channel (ch, buf);
}

void leave_chat_channel (CHAR_DATA * ch)
{
	if (VALID_CHAT_CHAN (ch->chat))
	{
		char buf[MAX_STRING_LENGTH];

		if (chat_table[ch->chat].chatters == 1)
			clear_chat_channel (ch->chat);
		else if (chat_table[ch->chat].owner == ch)
			chat_table[ch->chat].owner = NULL;
		chat_table[ch->chat].chatters -= 1;
		sprintf (buf, "$n has left chat room %d.", ch->chat);
		broadcast_chat_channel (ch, buf);
	}
}

void do_chat(CHAR_DATA *ch, char *argument)
{
	int chan;
	char buf[MAX_STRING_LENGTH];
	char arg[MAX_INPUT_LENGTH];

	if (argument[0] == '\0')
	{
		printf_to_char (ch, "Syntax: chat <channel number> between 1 and %d\n\r"
					  "Syntax: chat <message>\n\r" "Syntax: chat 0\n\r"
					  "Syntax: chat vanish\n\r", MAX_CHAT - 1);
		return;
	}

	argument = one_argument (argument, arg);

	if (is_number (arg))
	{
		if ((chan = atoi (arg)) <= 0)
		{
			if (!VALID_CHAT_CHAN (ch->chat))
			{
				send_to_char ("You haven't joined any chat channel.\n\r", ch);
				return;
			}
			leave_chat_channel (ch);
			send_to_char ("You leave the chat channel.\n\r", ch);
			ch->chat = 0;
			ch->chat_vanish = FALSE;
			return;
		}
		else if (chan >= MAX_CHAT)
		{
			printf_to_char (ch, "Chat channels must be between 1 and %d.",
						  MAX_CHAT - 1);
			return;
		}
		else
		{
			if (chat_table[chan].Private)
			{
                if(chat_table[chan].password[0] == '\0')
                {
                    send_to_char ("That channel is private right now.\n\r", ch);
                    return;
                }
                else if(argument[0] == '\0')
                {
                    send_to_char ("That channel requires a password to join.\n\r", ch);
                    return;
                }
                else if(str_cmp (argument, chat_table[chan].password))
                {
                    send_to_char("Incorrect channel password.\n\r", ch);
                    return;
                }
			}

			if (chan != ch->chat)
				leave_chat_channel (ch);
			enter_chat_channel (ch, chan);
			printf_to_char (ch, "You join chat channel %d.\n\r", ch->chat);
			return;
		}
	}

	if (!VALID_CHAT_CHAN (ch->chat))
	{
		send_to_char ("You must join a chat channel first.\n\r", ch);
		return;
	}

	if (!str_cmp (arg, "vanish"))
	{
		ch->chat_vanish = TRUE;
		send_to_char ("You are now invisible on chat channels.\n\r", ch);
		return;
	}

	if (argument[0] != '\0')
	{
		strcat (arg, " ");
		strcat (arg, argument);
	}
    sprintf(buf, "$n says: %s", arg);
	broadcast_chat_channel(ch, buf);
	printf_to_char(ch, "{Y{{{MChat {R%d{Y}{W You say: %s", ch->chat, arg);
	return;
}

void do_ownchan(CHAR_DATA *ch, char *argument)
{
	if (!VALID_CHAT_CHAN (ch->chat))
	{
		send_to_char ("Join a chat channel first!\n\r", ch);
		return;
	}

	if (chat_table[ch->chat].owner != NULL)
	{
		printf_to_char (ch, "%s is already owner of this channel.\n\r",
					  PERS (chat_table[ch->chat].owner, ch));
		return;
	}

	chat_table[ch->chat].owner = ch;
	printf_to_char (ch, "You are now owner of chat channel %d.\n\r", ch->chat);
	return;
}

void do_unownchan(CHAR_DATA *ch, char *argument)
{
	if (!VALID_CHAT_CHAN (ch->chat))
	{
		send_to_char ("You aren't on a channel.\n\r", ch);
		return;
	}

	if (chat_table[ch->chat].owner != ch)
	{
		send_to_char ("You don't own this channel.\n\r", ch);
		return;
	}

	chat_table[ch->chat].owner = NULL;
	send_to_char ("You no longer own this channel.\n\r", ch);
	return;
}

void do_chwho(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *pch;
	bool found = FALSE;

	for (pch = char_list; pch != NULL; pch = pch->next_player)
	{
		if (IS_NPC(pch) || !VALID_CHAT_CHAN (pch->chat)
			|| (pch->chat_vanish = TRUE && !IS_IMMORTAL (ch)))
			continue;

		if (chat_table[pch->chat].owner == pch)
			printf_to_char (ch, "{%c%26s{w: {rchannel {%c%d. {gTopic: {B%s{x",
						  chat_table[pch->chat].owner == ch ? 'C' : 'W',
						  PERS (pch, ch),
						  chat_table[pch->chat].Private ? 'M' : 'R', pch->chat,
						  IS_STRSET (chat_table[pch->chat].topic));
		else
			printf_to_char (ch, "{%c%26s{w: {rchannel {%c%d.{x",
						  chat_table[pch->chat].owner == ch ? 'C' : 'W',
						  PERS (pch, ch),
						  chat_table[pch->chat].Private ? 'M' : 'R', pch->chat);

		found = TRUE;
	}

	if (!found)
		send_to_char ("No one found.\n\r", ch);
}

void do_chboot(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	char buf[MAX_STRING_LENGTH];

	if (!VALID_CHAT_CHAN (ch->chat))
	{
		send_to_char ("You aren't on a chat channel!\n\r", ch);
		return;
	}

	if (chat_table[ch->chat].owner != ch)
	{
		send_to_char ("You don't own this chat channel!\n\r", ch);
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char ("Boot who off the channel?\n\r", ch);
		return;
	}

	if ((victim = get_char_world (NULL, argument)) == NULL || IS_NPC(victim)
		|| victim->chat != ch->chat)
	{
		send_to_char ("They aren't on your channel.\n\r", ch);
		return;
	}

    chat_table[victim->chat].chatters--;
	victim->chat = 0;
	victim->chat_vanish = FALSE;
	printf_to_char (victim, "%s has kicked you off the channel.\n\r",
					  PERS (ch, victim));
	printf_to_char (ch, "You kick %s off the channel.\n\r", PERS (victim, ch));
	sprintf (buf, "$n has kicked %s of the channel.", victim->name);
	broadcast_chat_channel (ch, buf);
	return;
}

void do_block(CHAR_DATA *ch, char *argument)
{
	if (!VALID_CHAT_CHAN (ch->chat))
	{
		send_to_char ("Your not on a chat channel!\n\r", ch);
		return;
	}

	if (chat_table[ch->chat].owner != ch)
	{
		send_to_char ("You're not an owner of this channel!\n\r", ch);
		return;
	}

    if(!chat_table[ch->chat].Private)
    {
	chat_table[ch->chat].Private = TRUE;
	send_to_char ("OK, channel now private.\n\r", ch);
	send_to_char ("Use the 'chpass' command to allow password access.\n\r", ch);
	broadcast_chat_channel (ch, "$n has made the channel private.");
    }
    else
    {
        chat_table[ch->chat].Private = FALSE;
        send_to_char("OK, channel is no longer private.\n\r", ch);
        if(chat_table[ch->chat].password[0] != '\0')
        {
            free_string(chat_table[ch->chat].password);
            chat_table[ch->chat].password = &str_empty[0];
        }
        broadcast_chat_channel (ch, "$n has made the channel public.");
    }	return;
}

void broadcast_chat_channel (CHAR_DATA * ch, const char *messg)
{
	CHAR_DATA *pch;
	char buf[MAX_STRING_LENGTH];

	if (!VALID_CHAT_CHAN (ch->chat))
		return;

	for (pch = char_list; pch != NULL; pch = pch->next_player)
	{
		if (IS_NPC(pch) || !VALID_CHAT_CHAN (pch->chat) || pch->chat != ch->chat)
			continue;

		sprintf (buf, "{Y{{{MChat {R%d{Y}{W %s", ch->chat, messg);
		act (buf, ch, NULL, pch, TO_VICT);
	}
}

void do_topic(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];

	if (!VALID_CHAT_CHAN (ch->chat))
	{
		send_to_char ("Your not on a chat channel!.\n\r", ch);
		return;
	}

	if (chat_table[ch->chat].owner != ch)
	{
		send_to_char ("Your not an owner of this channel.\n\r", ch);
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char ("Change topic to what?\n\r", ch);
		return;
	}

	free_string(chat_table[ch->chat].topic);
    chat_table[ch->chat].topic = str_dup(argument);

	printf_to_char (ch, "Topic Changed to: %s\n\r", argument);
	sprintf (buf, "$n has changed the topic to %s.", argument);
	broadcast_chat_channel (ch, argument);
}

void do_chpass(CHAR_DATA *ch, char *argument)
{
	if (!VALID_CHAT_CHAN (ch->chat))
	{
		send_to_char ("Your not on a chat channel!\n\r", ch);
		return;
	}

	if (chat_table[ch->chat].owner != ch)
	{
		send_to_char ("Only the owner of a channel can set the password.\n\r", ch);
		return;
	}

    if (!chat_table[ch->chat].Private)
    {
        send_to_char ("The channel must be private first to set password access.\n\r", ch);
        return;
    }

	if (argument[0] == '\0')
	{
		send_to_char ("Change the password to what?\n\r", ch);
		return;
	}

	free_string(chat_table[ch->chat].password);
    chat_table[ch->chat].password = str_dup(argument);
	printf_to_char (ch, "Channel password changed to %s.\n\r", argument);
}


>>> END CHAT.C <<<

>>> CHAT.H <<<

#if !defined(CHAT_H)
#define CHAT_H

#define MAX_CHAT 1000

typedef struct chat_data CHAT_DATA;
struct chat_data
{
	const char *topic;
	CHAR_DATA *owner;
	bool Private;
	const char *password;
	int chatters;
};
extern CHAT_DATA chat_table[MAX_CHAT];

#define VALID_CHAT_CHAN(chan)   (chan > 0 && chan < MAX_CHAT)

void leave_chat_channel args ((CHAR_DATA * ch));
void enter_chat_channel args ((CHAR_DATA * ch, int chan));
void init_chat_channels args ((void));

#endif


>>> END CHAT.H <<<


>>> DB.C <<<

1) At top add:

#include "chat.h"

2) In boot_db() after gsn inits add:

init_chat_channels ();

>>> SAVE.C <<<

1) At top add:

#include "chat.h"

2) In fwrite_char() Add:

	if (VALID_CHAT_CHAN (ch->chat))
		fprintf (fp, "Chatnum %d\n", ch->chat);
	if (ch->chat_vanish == true)
		fprintf (fp, "Chatvanish %d\n", ch->chat_vanish);

3) In fread_char() Add under case 'C':

            if (!str_cmp (word, "Chatnum"))
			{
				int num = fread_number (fp);

				if (VALID_CHAT_CHAN (num) && chat_table[num].Private == false)
					enter_chat_channel (ch, num);
				fMatch = true;
				break;
			}
			KEY ("Chatvanish", ch->chat_vanish, fread_number (fp));

>>> COMM.C <<<

1) At top add:

#include "chat.h"

2) In close_socket() Add:

    ..
    act ("$n has lost $s link.", ch, NULL, NULL, TO_ROOM);
+    leave_chat_channel (ch);
    ...

>>> ACT_COMM.C <<<

1) At top add:

#include "chat.h"

2) In do_quit() add:

leave_chat_channel (ch);

>>> MERC.H <<<

1) In char_data struct add:

    int chat;
    bool chat_vanish;

>>> INTERP.H <<<

DECLARE_DO_FUN (do_topic);
DECLARE_DO_FUN (do_block);
DECLARE_DO_FUN (do_chboot);
DECLARE_DO_FUN (do_chwho);
DECLARE_DO_FUN (do_unownchan);
DECLARE_DO_FUN (do_ownchan);
DECLARE_DO_FUN (do_chat);
DECLARE_DO_FUN (do_chpass);


>>> INTERP.C <<<

	{"block", do_block, POS_DEAD, 0, LOG_NORMAL, 1},
	{"boot", do_chboot, POS_DEAD, 0, LOG_NORMAL, 1},
	{"chwho", do_chwho, POS_DEAD, 0, LOG_NORMAL, 1},
	{"chpass", do_chpass, POS_DEAD, 0, LOG_NORMAL, 1},
	{"unownchan", do_unownchan, POS_DEAD, 0, LOG_NORMAL, 1},
	{"ownchan", do_ownchan, POS_DEAD, 0, LOG_NORMAL, 1},
	{"chat", do_chat, POS_DEAD, 0, LOG_NORMAL, 1},
    {"topic", do_topic, POS_DEAD, 0, LOG_NORMAL, 1},

>>> END <<<