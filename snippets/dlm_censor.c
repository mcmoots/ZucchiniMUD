/****************************************************************************
 * Language censoring code copyright 2001/2002                              *
 * Markanth : dlmud@dlmud.com                                               *
 * Devil's Lament : dlmud.com port 3778                                     *
 * Web Page : http://www.dlmud.com                                          *
 *                                                                          *
 * Replaces curse words with '*', if user is rated PG.                      *
 *                                                                          *
 * All I ask in return is that you give me credit on your mud somewhere     *
 * or email me if you use it.                                               *
 ***************************************************************************/
>>> MERC.H <<<

a) Add to comm flags:

#define COMM_CENSOR     (##)

b) if not a RoT MUD add prototype:

char *str_replace args((char *astr, char *bstr, char *cstr));

>>> TABLES.C <<<

a) Add to comm_flags[]:

{"censor", COMM_CENSOR, TRUE},

>>> INTERP.H <<<

DECLARE_DO_FUN(do_censor);

>>> INTERP.C <<<

{"censor", do_censor, POS_DEAD, 0, LOG_NORMAL, 1},

>>> ACT_COMM.C <<<

a) Add function:

char *const swear[] = {
	"shit", "fuck", "bitch", "bastard",
	"bullshit", "pussy", "dick", "cock",
	"motherfuck", "clit", "damn", "dammit",
	"dumbass", "slut", "whore", "asshole", NULL
};

void do_censor(CHAR_DATA *ch, char *argument)
{
	if (!str_cmp (argument, "status"))
		{
			printf_to_char (ch, "Channels currently rated %s.\n\r",
						  !IS_SET (ch->comm, COMM_CENSOR) ? "{RR{x" : "{GPG{x");
			return;
		}
	if (!str_cmp (argument, "list"))
		{
			int x;

			if (IS_SET (ch->comm, COMM_CENSOR))
				{
					send_to_char("You must be set to rated {RR{x to see the list.\n\r", ch);
					return;
				}
			for (x = 0; swear[x] != NULL; x++)
				{
					printf_to_char (ch, "%15s", swear[x]);
					if (x % 4 == 3)
						send_to_char ("\n\r", ch);
				}
			if (x % 4 != 3)
				send_to_char ("\n\r", ch);
			return;
		}
	if (!IS_SET (ch->comm, COMM_CENSOR))
		{
			send_to_char ("Channels are now rated {GPG{x.\n\r", ch);
			SET_BIT (ch->comm, COMM_CENSOR);
		}
	else
		{
			send_to_char ("Channels are now rated {RR{x.\n\r", ch);
			REMOVE_BIT (ch->comm, COMM_CENSOR);
		}
}

char *swearcheck (char *argument, CHAR_DATA * ch)
{
    static char buf_new[5][MSL];
    static int i;
	int x;

	// rotate buffers
	++i %= 5;
	char *result = buf_new[i];

	if (!ch || !IS_SET (ch->comm, COMM_CENSOR))
		return argument;

    strcpy(result, argument);
	for (x = 0; swear[x] != NULL; x++)
		{
			if (!str_infix (swear[x], result))
				{
                    char buf[strlen(swear[x])];
                    int i;

                    for(i = 0; i < strlen(swear[x]); i++)
                        buf[i] = '*';
					result = str_replace (result, swear[x], buf);
				}
		}
	return (result);
}

b) Add to swearcheck to channels:

Ex:
	sprintf(buf, "You shout '%s'{x\n\r", swearcheck (argument, ch));
    send_to_char(buf, ch);
	for (d = descriptor_list; d != NULL; d = d->next)
		{
			CHAR_DATA *victim;

			if (d->connected == CON_PLAYING && (victim = d->character) != NULL
				&& victim != ch && !is_ignoring (victim, ch)
				&& !IS_SET (victim->comm, COMM_SHOUTSOFF)
				&& !IS_SET (victim->comm, COMM_QUIET))
				act ("$n shouts '$t'{x", ch,
					 swearcheck (argument, victim), victim, TO_VICT);
		}

>>> STRING.C <<<

!!RoT Muds already have this function.!!

/*
 * Replace a substring in a string, case insensitive...Russ Walsh
 * looks for bstr within astr and replaces it with cstr.
 */
char *str_replace (char *astr, char *bstr, char *cstr)
{
	char newstr[MSL];
	char buf[MSL];
	bool found = false;
	int sstr1, sstr2;
	int ichar, jchar;
	char c0, c1, c2;

	if (((c0 = LOWER (astr[0])) == NUL) || ((c1 = LOWER (bstr[0])) == NUL)
		|| ((c2 = LOWER (cstr[0])) == NUL))
		return astr;

	if (str_infix (bstr, astr))
		return astr;

/* make sure we don't start an infinite loop */
	if (!str_infix (bstr, cstr))
		return astr;

	sstr1 = strlen (astr);
	sstr2 = strlen (bstr);
	jchar = 0;

	if (sstr1 < sstr2)
		return astr;

	for (ichar = 0; ichar <= sstr1 - sstr2; ichar++)
		{
			if (c1 == LOWER (astr[ichar]) && !str_prefix (bstr, astr + ichar))
				{
					found = true;
					jchar = ichar;
					ichar = sstr1;
				}
		}
	if (found)
		{
			buf[0] = NUL;
			for (ichar = 0; ichar < jchar; ichar++)
				{
					sprintf (newstr, "%c", astr[ichar]);
					strcat (buf, newstr);
				}
			strcat (buf, cstr);
			for (ichar = jchar + sstr2; ichar < sstr1; ichar++)
				{
					sprintf (newstr, "%c", astr[ichar]);
					strcat (buf, newstr);
				}
			sprintf (astr, "%s", str_replace (buf, bstr, cstr));
			return astr;
		}
	return astr;
}
