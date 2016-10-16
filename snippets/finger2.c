From: jwlia19@idt.net

This is the do_finger command I wrote for my mud.  Some things may be
specific to my mud and need to be changed, these should be fairly obvious,
I would think.  I'd appreciate a mention somewhere, or at least an email,
if you use this mostly as is.  (Formatting of the output may not be
exactly perfect, that's something I never bothered to change.)  Hope this
helps, lemme know if you're still having problems.

=============================================================================

/* Finger command, by Wench. */

void do_finger(CHAR_DATA *ch, char *argument)
{
    FILE *fp;
    char strsave[MAX_INPUT_LENGTH];
    bool email_found = FALSE;
    bool real_found = FALSE;
    bool laston_found = FALSE;
    bool host_found = FALSE;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    char *real = "";
    char *email = "";
    char *host = "";
    long laston;
    bool finished = FALSE;
    int d, h, m, s = 0;

    if(argument[0] == '\0' || argument[0] == '/' || argument[0] == '\\')
    {
	send_to_char("Finger who?\n\r", ch);
	return;
    }

    smash_tilde(argument);
    argument = one_argument(argument, arg);
    strcpy(arg2, argument);
    if(!str_cmp(arg, "email"))
    {
	if(arg2[0] == '\0')
	{
	  sprintf(buf, "Your email address is currently listed as %s.\n\r",
			ch->pcdata->email);
	  send_to_char(buf, ch);
	  return;
	}
	ch->pcdata->email = strdup(arg2);
	sprintf(buf, "Your email address will now be displayed as %s.\n\r",
			 ch->pcdata->email);
	send_to_char(buf, ch);
	return;
    }

    if(!str_cmp(arg, "real"))
    {
	if(arg2[0] == '\0')
	{
	  sprintf(buf, "Your real name is currently listed as %s.\n\r",
		ch->pcdata->real_name);
	  send_to_char(buf, ch);
	  return;
	}
	ch->pcdata->real_name = strdup(arg2);
	sprintf(buf, "Your real name will now appear as %s.\n\r",
		ch->pcdata->real_name);
	send_to_char(buf, ch);
	return;
    }

    if(((victim = get_char_world(ch, arg)) != NULL) && (!IS_NPC(victim)))
    {
	int seconds = 0;
	sprintf(buf,"Name: %16s   Real Name: %16s\n\r", victim->name,
		victim->pcdata->real_name);
	send_to_char(buf, ch);
	sprintf(buf, "E-mail address: %35s\n\r", victim->pcdata->email);
	send_to_char(buf, ch);
	if(victim->desc != NULL)
	sprintf(buf, "%s is currently playing Mo' Mud.\n\r", victim->name);
	else
	sprintf(buf, "%s is currently linkdead.\n\r", victim->name);
	send_to_char(buf, ch);
	seconds = (int) (current_time - victim->pcdata->idle_time);
	if(seconds > 10)
	{
	  int hours = 0;
	  int minutes = 0;
	  hours = seconds/3600;
	  seconds -= hours*3600;
	  minutes = seconds/60;
	  seconds -= minutes*60;
	  sprintf(buf, 
"Idle:                                       %02d:%02d:%02d\n\r", hours,
		minutes, seconds);
	  send_to_char(buf, ch);
	}
	return;
    }

    sprintf(strsave, "%s%s", PLAYER_DIR, capitalize(arg));
    if((fp = fopen(strsave, "r")) == NULL)
    {
	sprintf(buf, "No such player: %s.\n\r", capitalize(arg));
	send_to_char(buf, ch);
	return;
    }

    for(;;)
    {
	char *word;
	fread_to_eol(fp);
	word = feof(fp) ? "End" : fread_word(fp);
	switch(UPPER(word[0]))
	{
	  case 'E':
	    if(!str_cmp(word, "Email"))
	    {
	      email = fread_string(fp);
	      email_found = TRUE;
	    }
	    if(!str_cmp(word, "End"))
	    {
	      finished = TRUE;
	      break;
	    }
	    break;
	  case 'H':
	    if(!str_cmp(word, "Host"))
	    {
		host = fread_string(fp);
		host_found = TRUE;
	    }
	    break;
	  case 'R':
	    if(!str_cmp(word, "Real"))
	    {
	      real = fread_string(fp);
	      real_found = TRUE;
	    }
	    break;
	  case 'L':
	    if(!str_cmp(word, "LogO"))
	    {
	      laston = fread_number(fp);
	      laston_found = TRUE;
	    }
	    break;
	  case '#':
	    finished = TRUE;
	    break;
	  default:
	    fread_to_eol(fp);
	    break;
	}
	if((finished)||(real_found&&laston_found&&email_found))
	  break;	
    }

    s = current_time - laston;
    d = s/86400;
    s-=d*86400;
    h = s/3600;
    s -= h*3600;
    m = s/60;
    s -= m*60;

    sprintf(buf, "Name: %16s   Real Name: %16s\n\r", capitalize(arg), 
		real_found ? real: "Not specified.");
    send_to_char(buf, ch);
    sprintf(buf, "E-mail address: %20s\n\r", 
		email_found ? email : "Not specified.");
    send_to_char(buf, ch);
    sprintf(buf, "%s last logged on at %s\n\r", capitalize(arg),
		laston_found ? ctime(&laston) : "Not found.");
    send_to_char(buf, ch);
    printf_to_char(ch, 
	"That was %d days, %d hours, %d minutes and %d seconds ago.\n\r",
	d, h, m, s);
#ifdef BUILDER_PORT
    if(ch->level < MAX_LEVEL)
    {
	fclose(fp);
	return;
    }
#endif
    if(IS_IMMORTAL(ch))
    {
	if(host_found)
	    sprintf(buf, "%s last logged on from %s\n\r", capitalize(arg),
		host);
	else
	    sprintf(buf, "No logon site found for %s.\n\r", capitalize(arg));
	send_to_char(buf, ch);
    }
    fclose(fp);
    return;
}
