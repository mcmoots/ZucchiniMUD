Room renting for Rom2.4

Please accredit me with this and any code you recieve from me.  Here's how
you do that:

Remember to change <whatever you want to charge> to the amount you want to
charge for rental.

replace the RENT help file in help.are with:

-1 RENT RENTAL~
Rental of rooms is possible via hotels.  Look for these rooms throughout the
world.  By paying 10 gold pieces, you make the room 'PRIVATE' and double
your regeneration.  This also costs you <whatever you want to charge>
silver each additional tick, Until you use the command 'CHECKOUT' in the 
rented room.

Rent code by Ryouga (ryouga@jessi.indstate.edu)
~


Okay...here's what you need to do:

in merc.h:

Add a new ROOM flag:
#define       	ROOM_RENTAL (##) 
#define		ROOM_RENTED (##)

# is any free value.

in struct pc_data:
add
int	rentroom;
to the end of the list.

in save.c:
Add to fwrite_char

   if (ch->pcdata->rentroom != 0)
       fprintf( fp, "Rroom %d\n",  ch->pcdata->rentroom  );
Add to fread_char under case 'R'

KEY( "Rroom",    ch->pcdata->rentroom,  fread_number( fp ) );

in interp.h add:
DECLARE_DO_FUN(do_rental   );

in interp.c add:

("rental",	do_rental,	POS_RESTING,	0,	LOG_NORMAL, 1);

in update.c add:
in hit_gain after the check for COND_THRIST

 if ( ch->in_room->vnum == ch->pcdata->rentroom  && ch->silver > 0)
   gain *= 2;
 if ( IS_SET(ch->act, PLR_TENNANT) && ch->silver > 0)
   ch->silver -= <whatever you want to charge>;
           
in act_move.c:
in move_char add:

     if ( ch->pcdata->rentroom != to_room->vnum  && IS_SET(to_room->room_flags, ROOM_RENTED))
       {
        if ( !IS_IMMORTAL(ch))
        {
         send_to_char( "You can't go in there.\n\r", ch );
         return;
        }
       }
       
Delete everything above the line of aterisks here and put
rent.o into your Makefile
/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Thanks to abaddon for proof-reading our comm.c and pointing out bugs.  *
 *  Any remaining bugs are, of course, our work, not his.  :)              *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
*	ROM 2.4 is copyright 1993-1996 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@efn.org)				   *
*	    Gabrielle Taylor						   *
*	    Brian Moore (zump@rom.org)					   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Rom24/doc/rom.license			   *
***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "merc.h"
#include "recycle.h"




void do_rental( CHAR_DATA *ch )
{
   

    if ( IS_SET(ch->in_room->room_flags, ROOM_RENTAL) )
    {

     if ( IS_NPC(ch))
       return;
    
     if ( IS_SET(ch->act, PLR_TENNANT))
      {
       send_to_char("You haven't checked out from last time, yet!\n\r", ch);
       return;
      }
    
     send_to_char("You rent the room.\n\r", ch);
     REMOVE_BIT(ch->in_room->room_flags, ROOM_RENTAL);
     SET_BIT(ch->in_room->room_flags, ROOM_PRIVATE);
     SET_BIT(ch->in_room->room_flags, ROOM_RENTED);
     SET_BIT(ch->act, PLR_TENNANT);
     ch->silver -= <whatever you want to charge>;
     return;
    }
    else
    {
     send_to_char("You can't rent this room.\n\r", ch);
     return;
    }
}

void do_checkout( CHAR_DATA *ch )
{
    
    
    if ( IS_SET(ch->in_room->room_flags, ROOM_RENTED) )
    {
	
     if ( IS_NPC(ch))
       return;
    
     if ( IS_SET(ch->act, PLR_TENNANT) && ch->pcdata->rentroom == ch->in_room->vnum)
      {
       send_to_char("You check out of the room.\n\r", ch);
       REMOVE_BIT(ch->act, PLR_TENNANT);
       REMOVE_BIT(ch->in_room->room_flags, ROOM_RENTED);
       SET_BIT(ch->in_room->room_flags, ROOM_RENTAL);
       ch->pcdata->rentroom = 0;
       return;
      }
     else
     {
       send_to_char("This is not your room.\n\r", ch);
       return;
     }
    }
}