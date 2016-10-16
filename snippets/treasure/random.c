/***************************************************************************

                      _____  __      __  __      __
                     /  _  \/  \    /  \/  \    /  \
                    /  /_\  \   \/\/   /\   \/\/   /
                   /    |    \        /  \        /
                   \____|__  /\__/\  /    \__/\  /
                           \/      \/          \/

    As the Wheel Weaves based on ROM 2.4. Original code by Dalsor.
    See changes.log for a list of changes from the original ROM code.
    Credits for code created by other authors have been left
 	intact at the head of each function.

 ***************************************************************************/

/* Random items - not really - code by Dalsor for AWW. */

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aww.h"
#include "recycle.h"
#include "interp.h"
#include "tables.h"
/*
	else if ( chance chance <= 95 ) {
		obj->item_type = ITEM_NOTEPAPER;
	}

	else if ( chance <= 99 ) {
		obj->item_type = ITEM_DRINK_CON;
	}

}
*/
OBJ_DATA *rand_item( OBJ_DATA *obj, int level )
{
	char item_name[MSL];
	char mat_short[MSL];
	char mat_long[MSL];
	char buf_name[MSL];
	char buf_short[MSL];
	char buf_long[MSL];
	int chance = number_percent();
	int wear_chance = number_range(1,10);
	int item_chance = number_percent();
	int weap_chance = number_percent();
	int wname_chance = number_percent();
	int weight = level * 5;
	item_name[0] = '\0';
	mat_short[0] = '\0';
	mat_long[0] = '\0';
	buf_name[0] = '\0';
	buf_short[0] = '\0';
	buf_long[0] = '\0';

	if ( weight > 50 ) weight = 50;

	/* determine the item type and include the name if not a weapon */
	if ( item_chance <= 10 ) {
		obj->item_type = ITEM_LOCKPICK;
		free_string(item_name);
		strcat(item_name,"lockpick");
		obj->wear_flags   = ITEM_TAKE;
		obj->wear_flags   += ITEM_HOLD;
	}
	else if ( item_chance <= 20 ) {
		obj->item_type = ITEM_PILL;
		free_string(item_name);
		strcat( item_name,"pill");
		obj->wear_flags   = ITEM_TAKE;
	}
	else if ( item_chance <= 30 ) {
		obj->item_type = ITEM_SCROLL;
		free_string(item_name);
		strcat( item_name,"scroll");
		obj->wear_flags   = ITEM_TAKE;
		obj->wear_flags   += ITEM_HOLD;
	}
	else if ( item_chance <= 40 ) {
		obj->item_type = ITEM_POTION;
		free_string(item_name);
		strcat( item_name,"potion");
	}
	else if ( item_chance <= 70 ) {
		obj->item_type = ITEM_ARMOR;
	}
	else if ( item_chance <= 90 ) {
		obj->item_type = ITEM_WEAPON;
		obj->wear_flags   = ITEM_TAKE;
		obj->wear_flags   += ITEM_WIELD;
	}
	else if ( item_chance <= 91 ) {
		obj->item_type = ITEM_ANGREAL_FEMALE;
		free_string(item_name);
		strcat( item_name,"angreal");
		obj->value[1] = wear_chance;
		obj->wear_flags   = ITEM_TAKE;
		obj->wear_flags   += ITEM_WEAR_ANGREAL;
	}
	else if ( item_chance <= 92 ) {
		obj->item_type = ITEM_ANGREAL_MALE;
		free_string(item_name);
		strcat( item_name,"angreal");
		obj->value[1] = wear_chance;
		obj->wear_flags   = ITEM_TAKE;
		obj->wear_flags   += ITEM_WEAR_ANGREAL;
	}
	else if ( item_chance <= 93 ) {
		obj->item_type = ITEM_LIGHT;
		free_string(item_name);
		strcat( item_name,"lamp");
		obj->value[2] = number_range(1,999);
	}
	else if ( item_chance <= 94 ) {
		obj->item_type = ITEM_FOOD;
		free_string(item_name);
		strcat( item_name,"meal");
		obj->value[0] = number_range(1,99);
		obj->value[1] = number_range(1,99);
		obj->value[2] = 0;
	}
	else if ( item_chance <= 95 ) {
		obj->item_type = ITEM_MIRROR;
		free_string(item_name);
		strcat( item_name,"mirror");
	}
	else if ( item_chance <= 96 ) {
		obj->item_type = ITEM_PIPE_TABAC;
		free_string(item_name);
		strcat( item_name,"pipe");
		obj->value[0] = number_range(1,20);
		obj->value[1] = number_range(1,20);
	}
	else if ( item_chance <= 97 ) {
		obj->item_type = ITEM_COMM_DEV;
		free_string(item_name);
		strcat( item_name,"device");
		obj->wear_flags   = ITEM_TAKE;
		switch( wear_chance )
		{
			case 1:
			case 2:
			case 3:
				obj->wear_flags   += ITEM_WEAR_FINGER;
				break;
			case 4:
			case 5:
			case 6:
				obj->wear_flags   += ITEM_WEAR_NECK;
				break;
			case 7:
			case 8:
			case 9:
			case 10:
				obj->wear_flags   += ITEM_WEAR_FLOAT;
				break;
		}
		obj->value[0] = number_range(10,100);
	}
	else {
		obj->item_type = ITEM_WEAPON;
		obj->wear_flags   = ITEM_TAKE;
		obj->wear_flags   += ITEM_WIELD;
	}

	/* if the type is a weapon, give it a name */
	if ( obj->item_type == ITEM_WEAPON )
	{
		if ( weap_chance <= 10 ) {
			obj->value[0] = WEAPON_FLAIL;
			if ( wname_chance <= 25 )
			{
				free_string(item_name);
				strcat( item_name,"horseman's flail");
			}
			else if ( wname_chance <= 50 )
			{
				free_string(item_name);
				strcat( item_name,"footman's flail");
			}
			else if ( wname_chance <= 75 )
			{
				free_string(item_name);
				strcat( item_name,"morning-star");
			}
			else
			{
				free_string(item_name);
				strcat( item_name,"flail");
			}
		}
		else if ( weap_chance <= 15 ) {
			obj->value[0] = WEAPON_STAFF;
			if ( wname_chance <= 15 )
			{
				free_string(item_name);
				strcat( item_name,"staff");
			}
			else if ( wname_chance <= 25 )
			{
				free_string(item_name);
				strcat( item_name,"bo-staff");
			}
			else if ( wname_chance <= 35 )
			{
				free_string(item_name);
				strcat( item_name,"quarter-staff");
			}
			else if ( wname_chance <= 50 )
			{
				free_string(item_name);
				strcat( item_name,"bludgeon");
			}
			else if ( wname_chance <= 60 )
			{
				free_string(item_name);
				strcat( item_name,"cane");
			}
			else if ( wname_chance <= 75 )
			{
				free_string(item_name);
				strcat( item_name,"rod");
			}
			else if ( wname_chance <= 85 )
			{
				free_string(item_name);
				strcat( item_name,"cudgel");
			}
			else
			{
				free_string(item_name);
				strcat( item_name,"stick");
			}
		}
		else if ( weap_chance <= 20 ) {
			obj->value[0] = WEAPON_WHIP;
			if ( wname_chance <= 15 )
			{
				free_string(item_name);
				strcat( item_name,"whip");
			}
			else if ( wname_chance <= 25 )
			{
				free_string(item_name);
				strcat( item_name,"scourge");
			}
			else if ( wname_chance <= 35 )
			{
				free_string(item_name);
				strcat( item_name,"blackjack");
			}
			else if ( wname_chance <= 50 )
			{
				free_string(item_name);
				strcat( item_name,"cat-o' nine tails");
			}
			else if ( wname_chance <= 60 )
			{
				free_string(item_name);
				strcat( item_name,"lash");
			}
			else if ( wname_chance <= 75 )
			{
				free_string(item_name);
				strcat( item_name,"flog");
			}
			else if ( wname_chance <= 85 )
			{
				free_string(item_name);
				strcat( item_name,"quirt");
			}
			else
			{
				free_string(item_name);
				strcat( item_name,"whip");
			}
		}
		else if ( weap_chance <= 25 ) {
			obj->value[0] = WEAPON_MACE;
			if ( wname_chance <= 15 )
			{
				free_string(item_name);
				strcat( item_name,"mace");
			}
			else if ( wname_chance <= 25 )
			{
				free_string(item_name);
				strcat( item_name,"club");
			}
			else if ( wname_chance <= 35 )
			{
				free_string(item_name);
				strcat( item_name,"hammer");
			}
			else if ( wname_chance <= 50 )
			{
				free_string(item_name);
				strcat( item_name,"warhammer");
			}
			else if ( wname_chance <= 60 )
			{
				free_string(item_name);
				strcat( item_name,"horseman's mace");
			}
			else if ( wname_chance <= 75 )
			{
				free_string(item_name);
				strcat( item_name,"footman's mace");
			}
			else if ( wname_chance <= 85 )
			{
				free_string(item_name);
				strcat( item_name,"beating rod");
			}
			else
			{
				free_string(item_name);
				strcat( item_name,"bludgeon");
			}
		}
		else if ( weap_chance <= 40 ) {
			obj->value[0] = WEAPON_SPEAR;
			if ( wname_chance <= 15 )
			{
				free_string(item_name);
				strcat( item_name,"spear");
			}
			else if ( wname_chance <= 25 )
			{
				free_string(item_name);
				strcat( item_name,"pike");
			}
			else if ( wname_chance <= 35 )
			{
				free_string(item_name);
				strcat( item_name,"lance");
			}
			else if ( wname_chance <= 50 )
			{
				free_string(item_name);
				strcat( item_name,"javelin");
			}
			else if ( wname_chance <= 70 )
			{
				free_string(item_name);
				strcat( item_name,"spear");
			}
			else if ( wname_chance <= 75 )
			{
				free_string(item_name);
				strcat( item_name,"spontoon");
			}
			else
			{
				free_string(item_name);
				strcat( item_name,"spear");
			}
		}
		else if ( weap_chance <= 80 ) {
			obj->value[0] = WEAPON_DAGGER;
			if ( wname_chance <= 15 )
			{
				free_string(item_name);
				strcat( item_name,"dagger");
			}
			else if ( wname_chance <= 25 )
			{
				free_string(item_name);
				strcat( item_name,"knife");
			}
			else if ( wname_chance <= 35 )
			{
				strcat( item_name,"dirk");
				free_string(item_name);
			}
			else
			{
				free_string(item_name);
				strcat( item_name,"dagger");
			}
		}
		else if ( weap_chance <= 90 ) {
			obj->value[0] = WEAPON_SWORD;
			if ( wname_chance <= 15 )
			{
				free_string(item_name);
				strcat( item_name,"sword");
			}
			else if ( wname_chance <= 25 )
			{
				free_string(item_name);
				strcat( item_name,"cutlass");
			}
			else if ( wname_chance <= 35 )
			{
				free_string(item_name);
				strcat( item_name,"sabre");
			}
			else if ( wname_chance <= 50 )
			{
				free_string(item_name);
				strcat( item_name,"sword");
			}
			else if ( wname_chance <= 70 )
			{
				free_string(item_name);
				strcat( item_name,"blade");
			}
			else if ( wname_chance <= 75 )
			{
				free_string(item_name);
				strcat( item_name,"falchion");
			}
			else if ( wname_chance <= 85 )
			{
				free_string(item_name);
				strcat( item_name,"broadsword");
			}
			else if ( wname_chance <= 95 )
			{
				free_string(item_name);
				strcat( item_name,"shortsword");
			}
			else
			{
				free_string(item_name);
				strcat( item_name,"sword");
			}
		}
		else if ( weap_chance >= 80 ) {
			obj->value[0] = WEAPON_AXE;
			if ( wname_chance <= 15 )
			{
				free_string(item_name);
				strcat( item_name,"axe");
			}
			else if ( wname_chance <= 25 )
			{
				free_string(item_name);
				strcat( item_name,"hand-axe");
			}
			else if ( wname_chance <= 35 )
			{
				free_string(item_name);
				strcat( item_name,"battle-axe");
			}
			else if ( wname_chance <= 50 )
			{
				free_string(item_name);
				strcat( item_name,"war-axe");
			}
			else if ( wname_chance <= 70 )
			{
				free_string(item_name);
				strcat( item_name,"axe");
			}
			else if ( wname_chance <= 75 )
			{
				free_string(item_name);
				strcat( item_name,"woodsman's axe");
			}
			else if ( wname_chance <= 85 )
			{
				free_string(item_name);
				strcat( item_name,"broad-axe");
			}
			else
			{
				free_string(item_name);
				strcat( item_name,"axe");
			}
		}
		/* now we assign an attack noun to the weapon */
		switch ( obj->value[0] )
		{
        default : obj->value[3] = attack_lookup("pound"); break;
        case(WEAPON_SWORD):
	        if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("slice"); break; }
	        else if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("slash"); break; }
	        else if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("stab"); break; }
	        else if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("pierce"); break; }
	        else { obj->value[3] = attack_lookup("slash");	break; }
        case(WEAPON_DAGGER):
	        if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("jab"); break; }
	        else if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("slice"); break; }
	        else if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("pierce"); break; }
	        else if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("thrust"); break; }
	        else { obj->value[3] = attack_lookup("jab"); break; }
        case(WEAPON_SPEAR):
	        if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("jab"); break; }
	        else if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("slice"); break; }
	        else if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("pierce"); break; }
	        else if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("thrust"); break; }
	        else { obj->value[3] = attack_lookup("pierce"); break; }
        case(WEAPON_STAFF):
	        if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("pound");	break; }
	        else if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("thwack"); break; }
	        else if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("beating"); break; }
	        else if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("crunch"); break; }
	        else { obj->value[3] = attack_lookup("whack"); break; }
        case(WEAPON_MACE):
	        if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("pound"); break; }
	        else if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("crush"); break; }
	        else if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("beating"); break; }
	        else if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("crunch"); break; }
	        else { obj->value[3] = attack_lookup("pound"); break; }
        case(WEAPON_AXE):
	        if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("cleave"); break; }
	        else if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("crunch"); break; }
	        else if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("slash"); break; }
	        else if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("chop"); break; }
	        else { obj->value[3] = attack_lookup("cleave"); break; }
        case(WEAPON_FLAIL):
	        if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("pound");	break; }
	        else if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("crush"); break; }
	        else if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("beating"); break; }
	        else if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("crunch"); break; }
	        else { obj->value[3] = attack_lookup("pound"); break; }
        case(WEAPON_WHIP):
	        if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("whip"); break; }
	        else if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("flog"); break; }
	        else if ( number_percent() < 10 ) { obj->value[3] = attack_lookup("sting"); break; }
	        else { obj->value[3] = attack_lookup("whip"); break; }
		}

	}
	/* now let's find a valid material */
	switch( obj->item_type )
	{
	default:
		obj->material = str_dup("iron");
		weight += 10;
		break;
	case ITEM_FOOD:
		obj->material = str_dup("food");
		weight += 1;
		break;
	case ITEM_LOCKPICK:
		if ( chance <= 10 ) {
			obj->material = str_dup("iron");
			weight += 10;
			break;
		}
		if ( chance <= 60 ) {
			obj->material = str_dup("steel");
			weight += 10;
			break;
		}
		if ( chance <= 75 ) {
			obj->material = str_dup("hardwood");
			weight += 5;
			break;
		}
		if ( chance <= 90 ) {
			obj->material = str_dup("bone");
			weight += 2;
			break;
		}
		if ( chance <= 10 ) {
			obj->material = str_dup("temperedsteel");
			weight += 5;
			break;
		}
		else {
			obj->material = str_dup("iron");
			weight += 5;
			break;
		}
	case ITEM_PILL:
		obj->material = str_dup("food");
		weight -= 10;
		break;
	case ITEM_COMM_DEV:
		obj->material = str_dup("cuendillar");
		weight -= 5;
		break;
	case ITEM_SCROLL:
		obj->material = str_dup("paper");
		weight += 20;
		break;
	case ITEM_POTION:
		obj->material = str_dup("softleather");
		weight += 1;
		break;
	case ITEM_ARMOR:
		if ( chance <= 10 ) {
			obj->material = str_dup("bronze");
			weight += 8;
			break; }
		if ( chance <= 25 ) {
			obj->material = str_dup("softleather");
			weight += 1;
			break; }
		if ( chance <= 60 ) {
			obj->material = str_dup("steel");
			weight += 10;
			break; }
		if ( chance <= 75 ) {
			obj->material = str_dup("hardleather");
			weight += 3;
			break; }
		if ( chance <= 80 ) {
			obj->material = str_dup("bronze");
			weight += 7;
			break; }
		if ( chance <= 90 ) {
			obj->material = str_dup("bone");
			weight += 4;
			break; }
		if ( chance <= 95 ) {
			obj->material = str_dup("sungwood");
			weight += 5;
			break; }
		if ( chance <= 10 ) {
			obj->material = str_dup("temperedsteel");
			weight += 9;
			break; }
		else {
			obj->material = str_dup("iron");
			weight += 9;
			break; }
	case ITEM_WEAPON:
		if ( chance <= 1 ) {
			obj->material = str_dup("powerwrought");
			weight -= 5;
			break; }
		if ( chance <= 15 ) {
			obj->material = str_dup("bronze");
			weight += 1;
			break; }
		if ( chance <= 35 ) {
			obj->material = str_dup("iron");
			weight += 3;
			break; }
		if ( chance <= 85 ) {
			obj->material = str_dup("steel");
			weight += 4;
			break; }
		if ( chance <= 90 ) {
			obj->material = str_dup("bone");
			weight += 2;
			break; }
		if ( chance <= 100 ) {
			obj->material = str_dup("temperedsteel");
			weight += 3;
			break; }
	}
	/* we now have a material, assign a color type corresponding to the material */
	if ( !str_cmp(obj->material,"iron") )
	{
		free_string(mat_short);
		if ( chance <= 15 )	     strcat(mat_short,"cold-gray");
		else if ( chance <= 35 ) strcat(mat_short,"hard-gray");
		else if ( chance <= 55 ) strcat(mat_short,"dull-gray");
		else if ( chance <= 75 ) strcat(mat_short,"bright-gray");
		else if ( chance <= 85 ) strcat(mat_short,"misty-gray");
		else strcat(mat_short,"gray-white");
	}
	else if ( !str_cmp(obj->material,"steel") )
	{
		free_string(mat_short);
		if ( chance <= 15 )	     strcat(mat_short,"cold-gray");
		else if ( chance <= 35 ) strcat(mat_short,"hard-gray");
		else if ( chance <= 55 ) strcat(mat_short,"dull-gray");
		else if ( chance <= 75 ) strcat(mat_short,"bright-gray");
		else if ( chance <= 85 ) strcat(mat_short,"misty-gray");
		else strcat(mat_short,"gray-white");
	}
	else if ( !str_cmp(obj->material,"hardwood") )
	{
		free_string(mat_short);
		if ( chance <= 15 )	     strcat(mat_short,"light-brown");
		else if ( chance <= 35 ) strcat(mat_short,"dark-brown");
		else if ( chance <= 55 ) strcat(mat_short,"nearly black");
		else if ( chance <= 75 ) strcat(mat_short,"reddish-brown");
		else if ( chance <= 85 ) strcat(mat_short,"dark red");
		else strcat(mat_short,"dark brown");
	}
	else if ( !str_cmp(obj->material,"bone") )
	{
		free_string(mat_short);
		if ( chance <= 25 )	     strcat(mat_short,"dark-gray");
		else if ( chance <= 50 ) strcat(mat_short,"pale white");
		else if ( chance <= 75 ) strcat(mat_short,"bleached white");
		else strcat(mat_short,"gray-white");
	}
	else if ( !str_cmp(obj->material,"temperedsteel") )
	{
		free_string(mat_short);
		if ( chance <= 15 )	     strcat(mat_short,"cold-gray");
		else if ( chance <= 25 ) strcat(mat_short,"cobalt-gray");
		else if ( chance <= 35 ) strcat(mat_short,"hard-gray");
		else if ( chance <= 55 ) strcat(mat_short,"blue-gray");
		else if ( chance <= 75 ) strcat(mat_short,"bright-gray");
		else if ( chance <= 85 ) strcat(mat_short,"silvery-gray");
		else strcat(mat_short,"gray-white");
	}
	else if ( !str_cmp(obj->material,"food") )
	{
		free_string(mat_short);
		strcat(mat_short,"plain");
	}
	else if ( !str_cmp(obj->material,"paper") )
	{
		free_string(mat_short);
		strcat(mat_short,"plain");
	}
	else if ( !str_cmp(obj->material,"softleather") )
	{
		free_string(mat_short);
		if ( chance <= 15 )	     strcat(mat_short,"pale brown");
		else if ( chance <= 35 ) strcat(mat_short,"light brown");
		else if ( chance <= 55 ) strcat(mat_short,"dark brown");
		else if ( chance <= 75 ) strcat(mat_short,"brownish-gray");
		else if ( chance <= 85 ) strcat(mat_short,"reddish-brown");
		else strcat(mat_short,"gray-white");
	}
	else if ( !str_cmp(obj->material,"bronze") )
	{
		free_string(mat_short);
		strcat(mat_short,"plain");
	}
	else if ( !str_cmp(obj->material,"hardleather") )
	{
		free_string(mat_short);
		if ( chance <= 15 )	     strcat(mat_short,"pale brown");
		else if ( chance <= 35 ) strcat(mat_short,"light brown");
		else if ( chance <= 55 ) strcat(mat_short,"dark brown");
		else if ( chance <= 75 ) strcat(mat_short,"brownish-gray");
		else if ( chance <= 85 ) strcat(mat_short,"reddish-brown");
		else strcat(mat_short,"gray-white");
	}
	else if ( !str_cmp(obj->material,"sungwood") )
	{
		free_string(mat_short);
		if ( chance <= 15 )	     strcat(mat_short,"light-brown");
		else if ( chance <= 35 ) strcat(mat_short,"dark-brown");
		else if ( chance <= 55 ) strcat(mat_short,"nearly black");
		else if ( chance <= 75 ) strcat(mat_short,"reddish-brown");
		else if ( chance <= 85 ) strcat(mat_short,"dark red");
		else strcat(mat_short,"dark brown");
	}
	else if ( !str_cmp(obj->material,"powerwrought") )
	{
		free_string(mat_short);
		if ( chance <= 15 )	     strcat(mat_short,"cold-gray");
		else if ( chance <= 35 ) strcat(mat_short,"hard-gray");
		else if ( chance <= 55 ) strcat(mat_short,"dull-gray");
		else if ( chance <= 75 ) strcat(mat_short,"bright-gray");
		else if ( chance <= 85 ) strcat(mat_short,"misty-gray");
		else strcat(mat_short,"gray-white");
	}
	else
	{
		free_string(mat_short);
		if ( chance <= 15 )	     strcat(mat_short,"cold-gray");
		else if ( chance <= 35 ) strcat(mat_short,"hard-gray");
		else if ( chance <= 55 ) strcat(mat_short,"dull-gray");
		else if ( chance <= 75 ) strcat(mat_short,"bright-gray");
		else if ( chance <= 85 ) strcat(mat_short,"misty-gray");
		else strcat(mat_short,"gray-white");
	}

	switch( obj->item_type )
	{
	default: break;
	case ITEM_ARMOR:
		if ( chance <= 5 )	     {
			free_string(item_name);
			strcat(item_name,"thigh-guard");
			obj->wear_flags   = ITEM_TAKE;
			obj->wear_flags   += ITEM_WEAR_THIGH;
			break;
		}
		else if ( chance <= 10 ) {
			free_string(item_name);
			strcat(item_name,"thigh-plate");
			obj->wear_flags   = ITEM_TAKE;
			obj->wear_flags   += ITEM_WEAR_THIGH;
			break;
		}
		else if ( chance <= 15 ) {
			free_string(item_name);
			strcat(item_name,"greave");
			obj->wear_flags   = ITEM_TAKE;
			obj->wear_flags   += ITEM_WEAR_THIGH;
			break;
		}
		else if ( chance <= 20 ) {
			free_string(item_name);
			strcat(item_name,"belt");
			obj->wear_flags   = ITEM_TAKE;
			obj->wear_flags   += ITEM_WEAR_WAIST;
			break;
		}
		else if ( chance <= 25 ) {
			free_string(item_name);
			strcat(item_name,"shin-guard");
			obj->wear_flags   = ITEM_TAKE;
			obj->wear_flags   += ITEM_WEAR_CALF;
			break;
		}
		else if ( chance <= 30 ) {
			free_string(item_name);
			strcat(item_name,"bracer");
			obj->wear_flags   = ITEM_TAKE;
			obj->wear_flags   += ITEM_WEAR_WRIST;
			break;
		}
		else if ( chance <= 35 ) {
			free_string(item_name);
			strcat(item_name,"wrist-guard");
			obj->wear_flags   = ITEM_TAKE;
			obj->wear_flags   += ITEM_WEAR_WRIST;
			break;
		}
		else if ( chance <= 45 ) {
			free_string(item_name);
			strcat(item_name,"helm");
			obj->wear_flags   = ITEM_TAKE;
			obj->wear_flags   += ITEM_WEAR_HEAD;
			break;
		}
		else if ( chance <= 55 ) {
			free_string(item_name);
			strcat(item_name,"set of arm-guards");
			obj->wear_flags   = ITEM_TAKE;
			obj->wear_flags   += ITEM_WEAR_ARMS;
			break;
		}
		else if ( chance <= 65 ) {
			free_string(item_name);
			strcat(item_name,"backplate");
			obj->wear_flags   = ITEM_TAKE;
			obj->wear_flags   += ITEM_WEAR_BACK;
			break;
		}
		else if ( chance <= 70 ) {
			free_string(item_name);
			strcat(item_name,"breastplate");
			obj->wear_flags   = ITEM_TAKE;
			obj->wear_flags   += ITEM_WEAR_BODY;
			break;
		}
		else if ( chance <= 75 ) {
			free_string(item_name);
			strcat(item_name,"pair of gauntlets");
			obj->wear_flags   = ITEM_TAKE;
			obj->wear_flags   += ITEM_WEAR_HANDS;
			break;
		}
		else if ( chance <= 85 ) {
			free_string(item_name);
			strcat(item_name,"helmet");
			obj->wear_flags   = ITEM_TAKE;
			obj->wear_flags   += ITEM_WEAR_HEAD;
			break;
		}
		else {
			free_string(item_name);
			strcat(item_name,"helmet");
			obj->wear_flags   = ITEM_TAKE;
			obj->wear_flags   += ITEM_WEAR_HEAD;
			break;
		}
	}

	obj->weight     = weight;
	obj->condition  = number_range(weight*9,weight*11);
        if ( obj->condition > 100 )
            obj->condition = 100;
	obj->cost       = number_range(weight*9,weight*11) * 2;
	obj->guild      = str_dup("Loner");

	sprintf( buf_name, "%s %s %s", mat_short, obj->material, item_name );
	free_string( obj->name );
	obj->name = str_dup( buf_name );

	sprintf( buf_short, "a %s %s %s", mat_short, obj->material, item_name );
	free_string( obj->short_descr );
	obj->short_descr = str_dup( buf_short );

	sprintf( buf_long, "Here lies a %s %s %s.", mat_short, obj->material, item_name );
	free_string( obj->description );
	obj->description = str_dup( buf_long );

	return obj;
}

int rand_spell( )
{
    if (number_percent() < 10 ) return skill_lookup("cure blinding");
    else if (number_percent() < 10 ) return skill_lookup("cure critical");
    else if (number_percent() < 10 ) return skill_lookup("cure disease");
    else if (number_percent() < 10 ) return skill_lookup("cure serious");
    else if (number_percent() < 10 ) return skill_lookup("cure light");
    else if (number_percent() < 10 ) return skill_lookup("cure poison");
    else if (number_percent() < 10 ) return skill_lookup("heal");
    else if (number_percent() < 10 ) return skill_lookup("defense");
    else if (number_percent() < 10 ) return skill_lookup("vigor");
    else if (number_percent() < 10 ) return skill_lookup("shield");
    else if (number_percent() < 10 ) return skill_lookup("fire shield");
    else if (number_percent() < 10 ) return skill_lookup("large");
    else if (number_percent() < 10 ) return skill_lookup("glow");
    else if (number_percent() < 10 ) return skill_lookup("see shadow");
    else if (number_percent() < 10 ) return skill_lookup("see light");
    else if (number_percent() < 10 ) return skill_lookup("seeing");
    else if (number_percent() < 10 ) return skill_lookup("see hidden");
    else if (number_percent() < 10 ) return skill_lookup("see invis");
    else if (number_percent() < 10 ) return skill_lookup("smell poison");
    else return skill_lookup("invert");
}
OBJ_DATA *make_rand_special( OBJ_DATA *fObj )
{
	int chance = number_range(1,12);
    AFFECT_DATA *af = new_affect();
	switch( chance )
	{
		case 1:
    		af->location   =   APPLY_STR;
		    af->modifier   =   number_range(1,20);
			break;
		case 2:
    		af->location   =   APPLY_DEX;
		    af->modifier   =   number_range(1,20);
			break;
		case 3:
    		af->location   =   APPLY_CON;
		    af->modifier   =   number_range(1,20);
			break;
		case 4:
    		af->location   =   APPLY_WIS;
		    af->modifier   =   number_range(1,20);
			break;
		case 5:
    		af->location   =   APPLY_INT;
		    af->modifier   =   number_range(1,20);
			break;
		case 6:
    		af->location   =   APPLY_CHA;
		    af->modifier   =   number_range(1,20);
			break;
		case 7:
    		af->location   =   APPLY_INS;
		    af->modifier   =   number_range(1,20);
			break;
		case 8:
    		af->location   =   APPLY_REF;
		    af->modifier   =   number_range(1,20);
			break;
		case 9:
    		af->location   =   APPLY_HITROLL;
		    af->modifier   =   number_range(1,20);
			break;
		case 10:
    		af->location   =   APPLY_DAMROLL;
		    af->modifier   =   number_range(1,20);
			break;
		case 11:
    		af->location   =   APPLY_HIT;
		    af->modifier   =   ( number_range(1,10) * 5 );
			break;
		case 12:
    		af->location   =   APPLY_MANA;
		    af->modifier   =   ( number_range(1,10) * 5 );
			break;
		case 13:
    		af->location   =   APPLY_MOVE;
		    af->modifier   =   ( number_range(1,10) * 5 );
			break;
		case 14:
    		af->location   =   APPLY_AC;
		    af->modifier   =   -( number_range(1,10) * 5 );
			break;
	}
	af->where	   =   TO_OBJECT;
    af->type       =   -1;
   	af->duration   =   -1;
   	af->bitvector  =   0;
	af->level      =   250;
    af->next       =   fObj->affected;
	fObj->affected =   af;
	fObj->cost += ( af->modifier * 20 );
	return( fObj );
}

OBJ_DATA *make_treasure_item( int level )
{
	OBJ_DATA *obj;

	bool remort = FALSE;
	int oWeight;

	if ( level < 1 ) level = 1;
	/*if ( number_percent() / 2 > number_percent() ) remort = TRUE;*/

	obj = create_object( get_obj_index(OBJ_VNUM_GEM), 0 );

	rand_item( obj, level );
	oWeight = number_range(obj->weight/4,obj->weight/3);
	obj->level = 1;

	switch ( obj->item_type )
	{

	case ITEM_WEAPON:
		if ( obj->item_type != ITEM_WEAPON )
			break; /* what the fuck happened... boom */
		if ( !CAN_WEAR(obj, ITEM_TAKE) ) /* set take if not set */
			obj->wear_flags   = ITEM_TAKE;
		obj->value[1]     = remort ? 2 : 1; /* if the obj is remort, times 2 */
		obj->value[2]     = number_range( oWeight * 1.75, oWeight * 2.5 ); /* sick */
	break;

	case ITEM_ARMOR:
		obj->value[0]     = remort ? number_range( oWeight, oWeight * 2 ) /* more sick */
									: number_range( oWeight * 2, oWeight * 3 );
		obj->value[1]     = remort ? number_range( oWeight, oWeight * 2 )
									: number_range( oWeight * 2, oWeight * 3 );
		obj->value[2]     = remort ? number_range( oWeight, oWeight * 2 )
									: number_range( oWeight * 2, oWeight * 3 );
		obj->value[3]     = remort ? number_range( oWeight, oWeight * 2 )
									: number_range( oWeight * 2, oWeight * 3 );
	break;

	case ITEM_LOCKPICK:
		obj->condition    = level / 2;
		obj->value[0]     = level / 3 + obj->condition; /* might be too high */
	break;

	case ITEM_SCROLL:
	case ITEM_POTION:
	case ITEM_PILL:
		obj->value[0]     = level / 2;
		obj->value[1]     = rand_spell();
		obj->value[2]     = rand_spell();
		obj->value[3]     = rand_spell();
		obj->value[4]     = rand_spell();
	break;
	}
	if ( obj->condition > 100 )
		obj->condition = 100;
	make_rand_special( obj );
	return ( obj );
}
