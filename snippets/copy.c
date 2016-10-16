      -------- Copy command (an add-on for Ivan's OLC) v1.2 --------

Here's a nice little add-on to Ivan's OLC.  It has a copy command for the room,
mob, and object editors.  It will copy over all the fields from the vnum you
select except the vnum and area name of course :)  There are only 2 things
that I haven't finished yet in it, will hopefully soon.  That's copying over
the shop data and mobprogs.  Although I'm still not sure if most people would
want the mob prog to copy over anyways.  There's a for loop in the medit_copy
for copying over the mob progs.  I worked it out to that point online with
Ivan himself, but it still wasn't copying it over.  I left both this and
the shop data part in (commented out of course) in case you might decide
to finish them up yourself before I release an updated one.

Example of use:
edit obj create 1253  
copy 1255

Now obj 1253 looks exactly like 1255, you change it as needed from there.
Change the name, description, whatever else you want to change.

The instructions are really simple and complete, if you can copy and paste
the 3 functions this could take you less than 5 minutes to add :)

If anyone wants to post this on a snippets page feel free to do so with this
header in tact :) I just ask that you e-mail me to let me know.

I also ask that if you use this snippet you e-mail me, I like to know that I
helped someone :)

Please keep the comment headers with each of the copy functions and if you make
a help entry for the command please give credit in there :)

E-mail address:  zanthras@columbus.rr.com

Thanks to Ivan Toledo and Dennis Reed for pointing out a few bugs that are
now fixed :)

/* olc.h */--------------------------------------------------------------------


in Room Editor Prototypes add this:
DECLARE_OLC_FUN( redit_copy );

in Mobile Editor Prototypes add this:
DECLARE_OLC_FUN( medit_copy );

in Object Editor Prototypes add this:
DECLARE_OLC_FUN( oedit_copy );


/* olc.c */--------------------------------------------------------------------


in const struct olc_cmd_type redit_table[] add this:
{ "copy", redit_copy },

in const struct olc_cmd_type medit_table[] add this:
{ "copy", medit_copy },

in const struct olc_cmd_type oedit_table[] add this:
{ "copy", oedit_copy },


/* olc_act.c */----------------------------------------------------------------
add these function in after the  edit_show functions for each one:


/*
 * redit_copy function thanks to Zanthras of Mystical Realities MUD.
 */
REDIT( redit_copy )
{
    ROOM_INDEX_DATA	*pRoom;
    ROOM_INDEX_DATA	*pRoom2; /* Room to copy */
    int vnum;

    if ( argument[0] == '\0' )
    {
      send_to_char("Syntax: copy <vnum> \n\r",ch);
      return FALSE;
    }

    if ( !is_number(argument) )
    {
      send_to_char("REdit: You must enter a number (vnum).\n\r",ch);
      return FALSE;
    }
    else /* argument is a number */
    {
      vnum = atoi(argument);
      if( !( pRoom2 = get_room_index(vnum) ) )
      {
	send_to_char("REdit: That room does not exist.\n\r",ch);
	return FALSE;
      }
    }

    EDIT_ROOM(ch, pRoom);

    free_string( pRoom->description );
    pRoom->description = str_dup( pRoom2->description );
    
    free_string( pRoom->name );
    pRoom->name = str_dup( pRoom2->name );

    /* sector flags */
    pRoom->sector_type = pRoom2->sector_type;

    /* room flags */
    pRoom->room_flags = pRoom2->room_flags;

    pRoom->heal_rate = pRoom2->heal_rate;
    pRoom->mana_rate = pRoom2->mana_rate;

    pRoom->clan = pRoom2->clan;

    free_string( pRoom->owner );
    pRoom->owner = str_dup( pRoom2->owner );

    pRoom->extra_descr = pRoom2->extra_descr;

    send_to_char( "Room info copied.", ch );
    return TRUE;
}


/*
 * oedit_copy function thanks to Zanthras of Mystical Realities MUD.
 */
OEDIT( oedit_copy )
{
    OBJ_INDEX_DATA *pObj;
    OBJ_INDEX_DATA *pObj2; /* The object to copy */
    int vnum, i;

    if ( argument[0] == '\0' )
    {
      send_to_char("Syntax: copy <vnum> \n\r",ch);
      return FALSE;
    }

    if ( !is_number(argument) )
    {
      send_to_char("OEdit: You must enter a number (vnum).\n\r",ch);
      return FALSE;
    }
    else /* argument is a number */
    {
      vnum = atoi(argument);
      if( !( pObj2 = get_obj_index(vnum) ) )
      {
	send_to_char("OEdit: That object does not exist.\n\r",ch);
	return FALSE;
      }
    }

    EDIT_OBJ(ch, pObj);

    free_string( pObj->name );
    pObj->name = str_dup( pObj2->name );

    pObj->item_type = pObj2->item_type;

    pObj->level = pObj2->level;

    pObj->wear_flags  = pObj2->wear_flags;
    pObj->extra_flags = pObj2->extra_flags;

    free_string( pObj->material );
    pObj->material = str_dup( pObj2->material );
    
    pObj->condition = pObj2->condition;

    pObj->weight = pObj2->weight;
    pObj->cost   = pObj2->cost;

    pObj->extra_descr = pObj2->extra_descr;

    free_string( pObj->short_descr );
    pObj->short_descr = str_dup( pObj2->short_descr );

    free_string( pObj->description );
    pObj->description = str_dup( pObj2->description );

    pObj->affected = pObj2->affected;

    for (i = 0; i < 5; i++)
    {
      pObj->value[i] = pObj2->value[i];
    }

    send_to_char( "Object info copied.", ch );
    return TRUE;
}


/*
 * medit_copy function thanks to Zanthras of Mystical Realities MUD.
 * Thanks to Ivan for what there is of the incomplete mobprog part.
 * Hopefully it will be finished in a later release of this snippet.
 */
MEDIT( medit_copy )
{
    MOB_INDEX_DATA *pMob;
    MOB_INDEX_DATA *pMob2; /* The mob to copy */
    int vnum;

    /* MPROG_LIST *list; */ /* Used for the mob prog for loop */

    if ( argument[0] == '\0' )
    {
      send_to_char("Syntax: copy <vnum> \n\r",ch);
      return FALSE;
    }

    if ( !is_number(argument) )
    {
      send_to_char("MEdit: You must enter a number (vnum).\n\r",ch);
      return FALSE;
    }
    else /* argument is a number */
    {
      vnum = atoi(argument);
      if( !( pMob2 = get_mob_index(vnum) ) )
      {
	send_to_char("MEdit: That mob does not exist.\n\r",ch);
	return FALSE;
      }
    }

    EDIT_MOB(ch, pMob);

    free_string( pMob->player_name );
    pMob->player_name = str_dup( pMob2->player_name );

    pMob->new_format = pMob2->new_format;
    pMob->act = pMob2->act;
    pMob->sex = pMob2->sex;
 
    pMob->race = pMob2->race;

    pMob->level = pMob2->level;
    
    pMob->alignment = pMob2->alignment;
    
    pMob->hitroll = pMob2->hitroll;
    
    pMob->dam_type = pMob2->dam_type;

    pMob->group = pMob2->group;

    pMob->hit[DICE_NUMBER] = pMob2->hit[DICE_NUMBER];
    pMob->hit[DICE_TYPE]   = pMob2->hit[DICE_TYPE];
    pMob->hit[DICE_BONUS]  = pMob2->hit[DICE_BONUS];

    pMob->damage[DICE_NUMBER] = pMob2->damage[DICE_NUMBER];
    pMob->damage[DICE_TYPE]   = pMob2->damage[DICE_TYPE];
    pMob->damage[DICE_BONUS]  = pMob2->damage[DICE_BONUS];
    
    pMob->mana[DICE_NUMBER] = pMob2->mana[DICE_NUMBER];
    pMob->mana[DICE_TYPE]   = pMob2->mana[DICE_TYPE];
    pMob->mana[DICE_BONUS]  = pMob2->mana[DICE_BONUS];

    pMob->affected_by = pMob2->affected_by;
    
    pMob->ac[AC_PIERCE] = pMob2->ac[AC_PIERCE];
    pMob->ac[AC_BASH]   = pMob2->ac[AC_BASH];
    pMob->ac[AC_SLASH]  = pMob2->ac[AC_SLASH];
    pMob->ac[AC_EXOTIC] = pMob2->ac[AC_EXOTIC];
    

    pMob->form  = pMob2->form;
    pMob->parts = pMob2->parts;

    pMob->imm_flags  = pMob2->imm_flags;
    pMob->res_flags  = pMob2->res_flags;
    pMob->vuln_flags = pMob2->vuln_flags;
    pMob->off_flags  = pMob2->off_flags;

    pMob->size     = pMob2->size;

    free_string( pMob->material );
    pMob->material = str_dup( pMob2->material ); 

    pMob->start_pos   = pMob2->start_pos;
    pMob->default_pos = pMob2->default_pos;

    pMob->wealth = pMob2->wealth;

    pMob->spec_fun = pMob2->spec_fun;

    free_string( pMob->short_descr );
    pMob->short_descr = str_dup( pMob2->short_descr );

    free_string( pMob->long_descr );
    pMob->long_descr = str_dup( pMob2->long_descr   );

    free_string( pMob->description );
    pMob->description = str_dup( pMob2->description );

    /* Hopefully get the shop data to copy later
     * This is the fields here if you get it copying send me
     * a copy and I'll add it to the snippet with credit to
     * you of course :) same with the mobprogs for loop :)
     */

/*
    if ( pMob->pShop )
    {
	SHOP_DATA *pShop, *pShop2;

	pShop =  pMob->pShop;
	pShop2 = pMob2->pShop;
 
	pShop->profit_buy = pShop2->profit_buy;
	pShop->profit_sell = pShop2->profit_sell;
	
	pShop->open_hour = pShop2->open_hour;
	pShop->close_hour = pShop2->close_hour;
	
	pShop->buy_type[iTrade] = pShop2->buy_type[iTrade];
    }
*/
/*  for loop thanks to Ivan, still needs work though

    for (list = pMob->mprogs; list; list = list->next )
    {
      MPROG_LIST *newp = new_mprog();
      newp->trig_type = list->trig_type;

      free_string( newp->trig_phrase );
      newp->trig_phrase = str_dup( list->trig_phrase );

      newp->vnum = list->vnum;

      free_string( newp->code )
      newp->code = str_dup( list->code );

      pMob->mprogs = newp;
    }
*/

    send_to_char( "Mob info copied.", ch );
    return FALSE;
}



