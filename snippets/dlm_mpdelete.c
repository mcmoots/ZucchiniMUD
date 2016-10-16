
/********************************************************************************
 * MobDeaths and MobKills code                                                  *
 * Markanth : dlmud@dlmud.com                                                   *
 * Devil's Lament : dlmud.com port 3778                                      *
 * Web Page : http://www.dlmud.com                                              *
 *                                                                              *
 * Meagre bit of code by Markanth <dlmud@dlmud.com     >                        *
 * for the Room and Object Programs v 1.3 snippet by Ralgha                     *   
 * <ralgha@gatewaynight.betterbox.net>.                                         *
 * Can easily be modded for just normal Mob Programs.                           *
 *                                                                              *
 * All I ask in return is that you give me credit on your mud somewhere         *
 * or email me if you use it, or if you make any changes.                       *
 ********************************************************************************/

>>> OLC_MPCODE.C <<<

1) Add to mpedit_table[]:

	{"delete", mpedit_delete},

2) Add to opedit_table[]:

	{"delete", opedit_delete},

3) Add to rpedit_table[]:

	{"delete", rpedit_delete},


4) Add functions:

/* I use long int vnums */
void unlink_mprog(long pnum)
{
	PROG_LIST *list, *list_next;
	MOB_INDEX_DATA *pMob;
	long vnum = 0;
	int nMatch = 0, count, pos = 0;

	for(vnum = 0 ; nMatch < top_mob_index; vnum++)
	{
		if((pMob = get_mob_index(vnum)) == NULL)
			continue;

		nMatch++;
		count = -1;
		for ( list = pMob->mprogs; list != NULL; list = list_next )
		{
			list_next = list->next;
			count++;

			/* Important line */
			if(list->vnum != pnum)
				continue;

			/* Next part taken from medit_delprog */
			if(count == 0)
			{
				REMOVE_BIT ( pMob->mprog_flags, pMob->mprogs->trig_type );
				list = pMob->mprogs;
				pMob->mprogs = list->next;
				free_mprog ( list );
			}
			else
			{
				while ( ( list_next = list->next ) && ( ++pos < count ) )
					list = list_next;

				if ( list_next )
				{
					REMOVE_BIT ( pMob->mprog_flags, list_next->trig_type );
					list->next = list_next->next;
					free_mprog ( list_next );
				}
			}
		}
	}
}

void unlink_oprog(long pnum)
{
	PROG_LIST *list, *list_next;
	OBJ_INDEX_DATA *pObj;
	long vnum = 0;
	int nMatch = 0, count, pos = 0;

	for(vnum = 0 ; nMatch < top_obj_index; vnum++)
	{
		if((pObj = get_obj_index(vnum)) == NULL)
			continue;

		nMatch++;
		count = -1;
		for ( list = pObj->oprogs; list != NULL; list = list_next )
		{
			list_next = list->next;
			count++;

			if(list->vnum != pnum)
				continue;

			if(count == 0)
			{
				REMOVE_BIT ( pObj->oprog_flags, pObj->oprogs->trig_type );
				list = pObj->oprogs;
				pObj->oprogs = list->next;
				free_oprog ( list );
			}
			else
			{
				while ( ( list_next = list->next ) && ( ++pos < count ) )
					list = list_next;

				if ( list_next )
				{
					REMOVE_BIT ( pObj->oprog_flags, list_next->trig_type );
					list->next = list_next->next;
					free_oprog ( list_next );
				}
			}
		}
	}
}

void unlink_rprog(long pnum)
{
	PROG_LIST *list, *list_next;
	ROOM_INDEX_DATA *pRoom;
	long vnum = 0;
	int nMatch = 0, count, pos = 0;

	for(vnum = 0 ; nMatch < top_room; vnum++)
	{
		if((pRoom = get_room_index(vnum)) == NULL)
			continue;

		nMatch++;
		count = -1;
		for ( list = pRoom->rprogs; list != NULL; list = list_next )
		{
			list_next = list->next;
			count++;

			if(list->vnum != pnum)
				continue;

			if(count == 0)
			{
				REMOVE_BIT ( pRoom->rprog_flags, pRoom->rprogs->trig_type );
				list = pRoom->rprogs;
				pRoom->rprogs = list->next;
				free_rprog ( list );
			}
			else
			{
				while ( ( list_next = list->next ) && ( ++pos < count ) )
					list = list_next;

				if ( list_next )
				{
					REMOVE_BIT ( pRoom->rprog_flags, list_next->trig_type );
					list->next = list_next->next;
					free_rprog ( list_next );
				}
			}
		}
	}
}

5) Add to do_mpedit():

	if (!str_cmp(command, "delete") )
	{
		if(argument[0] == '\0')
		{
			send_to_char("Syntax: mpedit delete [vnum]\n\r", ch);
			return;
		}
        mpedit_delete ( ch, argument );
        return;
	}

6) Add to do_opedit():

	if (!str_cmp(command, "delete") )
	{
		if(argument[0] == '\0')
		{
			send_to_char("Syntax: opedit delete [vnum]\n\r", ch);
			return;
		}
        sprintf ( log_buf, "$N deleting oprog '%s'.", argument );
        wiznet ( log_buf, ch, NULL, WIZ_OLC, WIZ_SECURE, get_trust ( ch ) );
        opedit_create ( ch, argument );
        return;
	}

7) Add to do_rpedit():

	if (!str_cmp(command, "delete") )
	{
		if(argument[0] == '\0')
		{
			send_to_char("Syntax: rpedit delete [vnum]\n\r", ch);
			return;
		}
        rpedit_delete ( ch, argument );
        return;
	}

8) Add functions:

MPEDIT ( mpedit_delete )
{
    PROG_CODE *curr, *prev;
    long value = atol ( argument );
	AREA_DATA *ad;

    if ( IS_NULLSTR ( argument ) || value < 1 )
    {
        send_to_char ( "Syntax : mpedit create [vnum]\n\r", ch );
        return FALSE;
    }
	
    if ( get_prog_index ( value, PRG_MPROG ) == NULL )
    {
        send_to_char ( "MPEdit : Mob program not found.\n\r", ch );
        return FALSE;
    }

	ad = get_vnum_area(value);
	if(ad == NULL)
	{
		send_to_char("MPEdit : Mob program not assigned to an area.\n\r", ch);
		return FALSE;
	}

    if ( !IS_BUILDER ( ch, ad ) )
    {
        send_to_char ( "MPEdit : Insufficient security to create MobProgs.\n\r",
                 ch );
        return FALSE;
    }

	unlink_mprog(value);
    prev = NULL;
    for ( curr = mprog_list; curr != NULL; prev = curr, curr = curr->next )
    {
		if(curr->vnum != value)
			continue;
		if ( prev == NULL )
			mprog_list = mprog_list->next;
		else
			prev->next = curr->next;

		free_mpcode ( curr );
	}
    save_area(ad);
    send_to_char ( "MobProgram Code Deleted.\n\r", ch );

    return TRUE;
}

OPEDIT ( opedit_delete )
{
    PROG_CODE *curr, *prev;
    long value = atol ( argument );
	AREA_DATA *ad;

    if ( IS_NULLSTR ( argument ) || value < 1 )
    {
        send_to_char ( "Syntax : opedit create [vnum]\n\r", ch );
        return FALSE;
    }

    if ( get_prog_index ( value, PRG_OPROG ) == NULL )
    {
        send_to_char ( "OPEdit : Obj program not found.\n\r", ch );
        return FALSE;
    }

	ad = get_vnum_area(value);
	if(ad == NULL)
	{
		send_to_char("OPEdit : Obj program not assigned to an area.\n\r", ch);
		return FALSE;
	}

    if ( !IS_BUILDER ( ch, ad ) )
    {
        send_to_char ( "OPEdit : Insufficient security to create ObjProgs.\n\r",
                 ch );
        return FALSE;
    }

	unlink_oprog(value);
    prev = NULL;
    for ( curr = oprog_list; curr != NULL; prev = curr, curr = curr->next )
    {
		if(curr->vnum != value)
			continue;

		if ( prev == NULL )
			oprog_list = oprog_list->next;
		else
			prev->next = curr->next;

		free_mpcode ( curr );
	}
    save_area(ad);
    send_to_char ( "Obj Program Code Deleted.\n\r", ch );

    return TRUE;
}

RPEDIT ( rpedit_delete )
{
    PROG_CODE *curr, *prev;
    long value = atol ( argument );
	AREA_DATA *ad;

    if ( IS_NULLSTR ( argument ) || value < 1 )
    {
        send_to_char ( "Syntax : rpedit create [vnum]\n\r", ch );
        return FALSE;
    }

    if ( get_prog_index ( value, PRG_RPROG ) == NULL )
    {
        send_to_char ( "RPEdit : Room program not found.\n\r", ch );
        return FALSE;
    }

	ad = get_vnum_area(value);
	if(ad == NULL)
	{
		send_to_char("RPEdit : Room program not assigned to an area.\n\r", ch);
		return FALSE;
	}

    if ( !IS_BUILDER ( ch, ad ) )
    {
        send_to_char ( "RPEdit : Insufficient security to create RoomProgs.\n\r",
                 ch );
        return FALSE;
    }

	unlink_rprog(value);
    prev = NULL;
    for ( curr = rprog_list; curr != NULL; prev = curr, curr = curr->next )
    {
		if(curr->vnum != value)
			continue;
		if ( prev == NULL )
			rprog_list = rprog_list->next;
		else
			prev->next = curr->next;

		free_mpcode ( curr );
	}
    save_area(ad);
    send_to_char ( "Room Program Code Deleted.\n\r", ch );

    return TRUE;
}


>>> OLC.H <<<

1) Add:

DECLARE_OLC_FUN ( rpedit_delete );
DECLARE_OLC_FUN ( mpedit_delete );
DECLARE_OLC_FUN ( opedit_delete );

void unlink_mprog args((long pnum));
void unlink_oprog args((long pnum));
void unlink_rprog args((long pnum));
