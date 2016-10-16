/****************************************************************************
 * Battle Prompt Code copyright 2001/2002                                   *
 * Markanth : dlmud@dlmud.com                                               *
 * Devil's Lament : dlmud.com port 3778                                  *
 * Web Page : http://www.dlmud.com                                          *
 *                                                                          *
 * Provides a different prompt while in combat.                             *
 *                                                                          *
 * All I ask in return is that you give me credit on your mud somewhere     *
 * or email me if you use it.                                               *
 ***************************************************************************/

>>> MERC.H <<<

1) Add to char_data after prompt:

    char *battle_prompt;

>>> ACT_INFO.C <<<

1) Add function:

void do_bprompt ( CHAR_DATA *ch, char *argument )
{
    char buf[MSL];

    if ( argument[0] == '\0' )
    {
        send_to_char ( "Syntax: bprompt none   - turn battle prompts off.\n\r", ch );
        send_to_char ( "        bprompt <format> - format a prompt (HELP PROMPT).\n\r", ch );
        if ( ch->battle_prompt != NULL && ch->battle_prompt[0] != '\0' )
        {
            sprintf ( buf, "Your current battle prompt is: %s\n\r",
                      ch->battle_prompt );
            send_to_char ( buf, ch );
        }
        return;
    }

    if ( !str_cmp ( argument, "none" ) )
    {
        free_string ( ch->battle_prompt );
        ch->battle_prompt = &str_empty[0];
        send_to_char ( "Battle prompt cleared.", ch );
        return;
    }

    if ( strlen_color ( argument ) > 50 )
        argument[50] = '\0';

    strcpy ( buf, argument );
    smash_tilde ( buf );
    if ( str_suffix ( "%c", buf ) )
        strcat ( buf, " " );

    free_string ( ch->battle_prompt );
    ch->battle_prompt = str_dup( buf );
    sprintf ( buf, "Battle Prompt set to %s\n\r", ch->battle_prompt );
    send_to_char ( buf, ch );
    return;
}


2) In do_autolist() or do_channels() add:

    if ( ch->battle_prompt != NULL && ch->battle_prompt[0] != '\0' )
    {
        sprintf ( buf, "Your battle prompt is: %s\n\r", ch->battle_prompt );
        send_to_char( buf, ch );
    }


>>> ACT_WIZ.C <<<

1) In do_return() add:

    if (ch->battle_prompt != NULL)
    {
        free_string(ch->battle_prompt);
        ch->prompt = NULL;
    }

2) In do_switch() add:

    if (ch->battle_prompt != NULL)
        victim->battle_prompt = str_dup(ch->battle_prompt);

>>> SAVE.C <<<

1) In fread_char() case 'B' add:

                KEYS ( "Bprom", ch->battle_prompt, fread_string ( fp ) );

2) In load_char_obj() set a default battle prompt:

    ch->battle_prompt  = str_dup( "(%b{rhp %B{mm {G%X{gxp {x(Enemy: %E{x) " );

3) In fwrite_char() after the normal prompt add:

    if ( ch->battle_prompt != NULL && ch->battle_prompt[0] != '\0' )
        fprintf ( fp, "Bprom %s~\n", ch->battle_prompt );

>>> RECYCLE.C <<<

1) In free_char() add:

    free_string ( ch->battle_prompt );

2) In new_char() add:

    ch->battle_prompt = &str_empty[0];

>>> COMM.C <<<

1) In bust_a_prompt() change:

++    if ( ch->fighting != NULL && ch->battle_prompt != NULL && ch->battle_prompt[0] != '\0' )
++        str = ch->battle_prompt;
++    else
          str = ch->prompt;

>>> DB.C <<<

1) In clear_char() add:

    ch->battle_prompt                  = &str_empty[0];

2) In create_mobile() add:

    mob->battle_prompt = NULL;
