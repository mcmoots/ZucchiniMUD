/****************************************************************************
 * OLC Delete Code copyright 2001/2002                                      *
 * Markanth : dlmud@dlmud.com                                               *
 * Devil's Lament : dlmud.com port 3778                                     *
 * Web Page : http://www.dlmud.com                                          *
 *                                                                          *
 * Allows for deletion of areas, rooms, objects, mobs, and mprogs.          *
 *                                                                          *
 * All I ask in return is that you give me credit on your mud somewhere     *
 * or email me if you use it.                                               *
 ***************************************************************************/


>>> OLC.H <<<

DECLARE_OLC_FUN (mpedit_delete);
DECLARE_OLC_FUN (medit_delete);
DECLARE_OLC_FUN (oedit_delete);
DECLARE_OLC_FUN (redit_delete);

void clean_area_links args ((AREA_DATA * target));
void unlink_mprog args ((int vnum));
void unlink_obj_index args ((OBJ_INDEX_DATA * pObj));
void unlink_reset args ((ROOM_INDEX_DATA * pRoom, RESET_DATA * pReset));
void unlink_room_index args ((ROOM_INDEX_DATA * pRoom));
void unlink_mob_index args ((MOB_INDEX_DATA * pMob));

>>> OLC_MPCODE.C <<<

a) Add functions:

void unlink_mprog (int pnum)
{
	PROG_LIST *list, *list_next;
	MOB_INDEX_DATA *pMob;
	int vnum = 0;
	int nMatch = 0, count, pos = 0;

	for (vnum = 0; nMatch < top_mob_index; vnum++)
		{
			if ((pMob = get_mob_index (vnum)) == NULL)
				continue;

			nMatch++;
			count = -1;
			for (list = pMob->mprogs; list != NULL; list = list_next)
				{
					list_next = list->next;
					count++;

					if (list->vnum != pnum)
						continue;

					if (count == 0)
						{
							REMOVE_BIT (pMob->mprog_flags,
										pMob->mprogs->trig_type);
							list = pMob->mprogs;
							pMob->mprogs = list->next;
							free_mprog (list);
						}
					else
						{
							while ((list_next = list->next) && (++pos < count))
								list = list_next;

							if (list_next)
								{
									REMOVE_BIT (pMob->mprog_flags,
												list_next->trig_type);
									list->next = list_next->next;
									free_mprog (list_next);
								}
						}
				}
		}
}

MPEDIT (mpedit_delete)
{
	MPROG_CODE *curr, *prev;
	int value = atoi (argument);
	AREA_DATA *ad;

	if (IS_NULLSTR (argument) || value < 1)
		{
			if (ch)
				send_to_char ("Syntax : mpedit create [vnum]\n\r", ch);
			return FALSE;
		}

	if (get_mprog_index (value) == NULL)
		{
			if (ch)
				send_to_char ("MPEdit : Mob program not found.\n\r", ch);
			return FALSE;
		}

	ad = get_vnum_area (value);
	if (ad == NULL)
		{
			if (ch)
				send_to_char ("MPEdit : Mob program not assigned to an area.\n\r", ch);
			return FALSE;
		}

	if (ch && !IS_BUILDER (ch, ad))
		{
			send_to_char ("MPEdit : Insufficient security to create MobProgs.\n\r", ch);
			return FALSE;
		}

	unlink_mprog (value);
	prev = NULL;
	for (curr = mprog_list; curr != NULL; prev = curr, curr = curr->next)
		{
			if (curr->vnum != value)
				continue;
			if (prev == NULL)
				mprog_list = mprog_list->next;
			else
				prev->next = curr->next;

			top_mprog_index--;

			free_mpcode (curr);
		}
	SET_BIT (ad->area_flags, AREA_CHANGED);
	if (ch)
		send_to_char ("MobProgram Code Deleted.\n\r", ch);

	return TRUE;
}

b) Add to do_mpedit():

	if (!str_cmp (command, "delete"))
		{
			if (IS_NULLSTR (argument))
				{
					send_to_char ("Syntax: mpedit delete [vnum]\n\r", ch);
					return;
				}
			mpedit_delete (ch, argument);
			return;
		}

c) Add to mpedit_table[]:

	{"delete", mpedit_delete},

>>> OLC.C <<<

a) Add to do_medit():

			else if (!str_cmp (arg1, "delete"))
				{
					medit_delete (ch, argument);
					return;
				}

b) Add to do_oedit():

			else if (!str_cmp (arg1, "delete"))
				{
					oedit_delete (ch, argument);
					return;
				}

c) Add to do_redit():

	else if (!str_cmp (arg1, "delete"))
		{
			redit_delete (ch, argument);
			return;
		}

d) Add function before do_aedit():

void clean_area_links (AREA_DATA * target)
{
	int vnum;
	char buf[20];

	for (vnum = target->min_vnum; vnum <= target->max_vnum; vnum++)
		{
			if (get_mprog_index (vnum) != NULL)
				{
					sprintf (buf, "%ld", vnum);
					if (!mpedit_delete (NULL, buf))
						bugf ("Unable to delete Mprog %ld", vnum);
				}
			if (get_room_index (vnum) != NULL)
				{
					sprintf (buf, "%ld", vnum);
					if (!redit_delete (NULL, buf))
						bugf ("Unable to delete Room %ld", vnum);
				}
			if (get_obj_index (vnum) != NULL)
				{
					sprintf (buf, "%ld", vnum);
					if (!oedit_delete (NULL, buf))
						bugf ("Unable to delete Object %ld", vnum);
				}
			if (get_mob_index (vnum) != NULL)
				{
					sprintf (buf, "%ld", vnum);
					if (!medit_delete (NULL, buf))
						bugf ("Unable to delete Mobile %ld", vnum);
				}
		}
}

e) Add to do_aedit():

	else if (!str_cmp (arg, "delete"))
		{
			AREA_DATA *curr, *prev;
			char filename[MIL];

			if (ch->pcdata->security < 9)
				{
					send_to_char("AEdit : Seguridad insuficiente para delete area.\n\r", ch);
					return;
				}
			if (!is_number (argument)
				|| !(pArea = get_area_data (atoi (argument))))
				{
					send_to_char ("That area vnum does not exist.\n\r", ch);
					return;
				}

			clean_area_links (pArea);
			logf ("Done unlinking area %s.", pArea->name);
			sprintf (filename, "%s%s", AREA_DIR, pArea->file_name);
			unlink (filename);
			prev = NULL;
			for (curr = area_first; curr != NULL;
				 prev = curr, curr = curr->next)
				{
					if (curr == pArea)
						{
							if (prev == NULL)
								area_first = area_first->next;
							else
								prev->next = curr->next;

							free_area (curr);
							send_to_char ("Area deleted.\n\r", ch);
							do_asave (NULL, "changed");
						}
				}
			return;
		}


>>> OLC_ACT.C <<<

a) Add functions:

void unlink_reset (ROOM_INDEX_DATA * pRoom, RESET_DATA * pReset)
{
	RESET_DATA *prev, *wReset;

	prev = pRoom->reset_first;
	for (wReset = pRoom->reset_first; wReset; wReset = wReset->next)
		{
			if (wReset == pReset)
				{
					if (pRoom->reset_first == pReset)
						{
							pRoom->reset_first = pReset->next;
							if (!pRoom->reset_first)
								pRoom->reset_last = NULL;
						}
					else if (pRoom->reset_last == pReset)
						{
							pRoom->reset_last = prev;
							prev->next = NULL;
						}
					else
						prev->next = prev->next->next;

					if (pRoom->reset_first == pReset)
						pRoom->reset_first = pReset->next;

					if (!pRoom->reset_first)
						pRoom->reset_last = NULL;
				}

			prev = wReset;
		}
}

void unlink_obj_index (OBJ_INDEX_DATA * pObj)
{
	int iHash;
	OBJ_INDEX_DATA *iObj, *sObj;

	iHash = pObj->vnum % MAX_KEY_HASH;

	sObj = obj_index_hash[iHash];

	if (sObj->next == NULL)

		obj_index_hash[iHash] = NULL;
	else if (sObj == pObj)

		obj_index_hash[iHash] = pObj->next;
	else
		{

			for (iObj = sObj; iObj != NULL; iObj = iObj->next)
				{
					if (iObj == pObj)
						{
							sObj->next = pObj->next;
							break;
						}
					sObj = iObj;
				}
		}
}

void unlink_room_index (ROOM_INDEX_DATA * pRoom)
{
	int iHash;
	ROOM_INDEX_DATA *iRoom, *sRoom;

	iHash = pRoom->vnum % MAX_KEY_HASH;

	sRoom = room_index_hash[iHash];

	if (sRoom->next == NULL)

		room_index_hash[iHash] = NULL;
	else if (sRoom == pRoom)

		room_index_hash[iHash] = pRoom->next;
	else
		{

			for (iRoom = sRoom; iRoom != NULL; iRoom = iRoom->next)
				{
					if (iRoom == pRoom)
						{
							sRoom->next = pRoom->next;
							break;
						}
					sRoom = iRoom;
				}
		}
}

void unlink_mob_index (MOB_INDEX_DATA * pMob)
{
	int iHash;
	MOB_INDEX_DATA *iMob, *sMob;

	iHash = pMob->vnum % MAX_KEY_HASH;

	sMob = mob_index_hash[iHash];

	if (sMob->next == NULL)

		mob_index_hash[iHash] = NULL;
	else if (sMob == pMob)

		mob_index_hash[iHash] = pMob->next;
	else
		{

			for (iMob = sMob; iMob != NULL; iMob = iMob->next)
				{
					if (iMob == pMob)
						{
							sMob->next = pMob->next;
							break;
						}
					sMob = iMob;
				}
		}
}

MEDIT (medit_delete)
{
	CHAR_DATA *wch, *wnext;
	MOB_INDEX_DATA *pMob;
	RESET_DATA *pReset, *wReset;
	ROOM_INDEX_DATA *pRoom;
	char arg[MIL];
	int pIndex, i;
	int mcount, rcount, iHash;
	bool foundmob = FALSE;
	bool foundobj = FALSE;

	if (IS_NULLSTR (argument))
		{
			if (ch)
				send_to_char ("Syntax:  medit delete [vnum]\n\r", ch);
			return FALSE;
		}

	one_argument (argument, arg);

	if (is_number (arg))
		{
			pIndex = atoi (arg);
			pMob = get_mob_index (pIndex);
		}
	else
		{
			if (ch)
				send_to_char ("That is not a number.\n\r", ch);
			return FALSE;
		}

	if (!pMob)
		{
			if (ch)
				send_to_char ("No such mobile.\n\r", ch);
			return FALSE;
		}

	stop_editing (pMob);

	SET_BIT (pMob->area->area_flags, AREA_CHANGED);

	if (top_vnum_mob == pIndex)
		for (i = 1; i < pIndex; i++)
			if (get_mob_index (i))
				top_vnum_mob = i;

	top_mob_index--;

	rcount = 0;
	mcount = 0;

	for (iHash = 0; iHash < MAX_KEY_HASH; iHash++)
		{
			for (pRoom = room_index_hash[iHash]; pRoom; pRoom = pRoom->next)
				{

					for (wch = pRoom->people; wch; wch = wnext)
						{
							wnext = wch->next_in_room;
							if (wch->pIndexData == pMob)
								{
									extract_char (wch, TRUE);
									mcount++;
								}
						}

					for (pReset = pRoom->reset_first; pReset; pReset = wReset)
						{
							wReset = pReset->next;
							switch (pReset->command)
								{
								case 'M':
									if (pReset->arg1 == pIndex)
										{
											foundmob = TRUE;

											unlink_reset (pRoom, pReset);
											free_reset_data (pReset);

											rcount++;
											SET_BIT (pRoom->area->area_flags,
													 AREA_CHANGED);

										}
									else
										foundmob = FALSE;

									break;
								case 'E':
								case 'G':
									if (foundmob)
										{
											foundobj = TRUE;

											unlink_reset (pRoom, pReset);
											free_reset_data (pReset);

											rcount++;
											SET_BIT (pRoom->area->area_flags,
													 AREA_CHANGED);

										}
									else
										foundobj = FALSE;

									break;
								case '0':
									foundobj = FALSE;
									break;
								case 'P':
									if (foundobj && foundmob)
										{
											unlink_reset (pRoom, pReset);
											free_reset_data (pReset);

											rcount++;
											SET_BIT (pRoom->area->area_flags,
													 AREA_CHANGED);
										}
								}
						}
				}
		}

	unlink_mob_index (pMob);

	pMob->area = NULL;
	pMob->vnum = 0;

	free_mob_index (pMob);

	if (ch)
		{
			printf_to_char (ch, "Removed mobile vnum %ld and %d resets.\n\r",
						  pIndex, rcount);
			printf_to_char(ch, "%d mobiles were extracted from the mud.\n\r",
						  mcount);
		}
	return TRUE;
}


OEDIT (oedit_delete)
{
	OBJ_DATA *obj, *obj_next;
	OBJ_INDEX_DATA *pObj;
	RESET_DATA *pReset, *wReset;
	ROOM_INDEX_DATA *pRoom;
	char arg[MIL];
	int pIndex, i;
	int rcount, ocount, iHash;

	if (IS_NULLSTR (argument))
		{
			if (ch)
				send_to_char ("Syntax:  oedit delete [vnum]\n\r", ch);
			return FALSE;
		}

	one_argument (argument, arg);

	if (is_number (arg))
		{
			pIndex = atoi (arg);
			pObj = get_obj_index (pIndex);
		}
	else
		{
			if (ch)
				send_to_char ("That is not a number.\n\r", ch);
			return FALSE;
		}

	if (!pObj)
		{
			if (ch)
				send_to_char ("No such object.\n\r", ch);
			return FALSE;
		}

	stop_editing (pObj);

	SET_BIT (pObj->area->area_flags, AREA_CHANGED);

	if (top_vnum_obj == pIndex)
		for (i = 1; i < pIndex; i++)
			if (get_obj_index (i))
				top_vnum_obj = i;

	top_obj_index--;

	ocount = 0;
	for (obj = object_list; obj; obj = obj_next)
		{
			obj_next = obj->next;

			if (obj->pIndexData == pObj)
				{
					extract_obj (obj);
					ocount++;
				}
		}

	rcount = 0;
	for (iHash = 0; iHash < MAX_KEY_HASH; iHash++)
		{
			for (pRoom = room_index_hash[iHash]; pRoom; pRoom = pRoom->next)
				{
					for (pReset = pRoom->reset_first; pReset; pReset = wReset)
						{
							wReset = pReset->next;
							switch (pReset->command)
								{
								case 'O':
								case 'E':
								case 'P':
								case 'G':
									if ((pReset->arg1 == pIndex)
										|| ((pReset->command == 'P')
											&& (pReset->arg3 == pIndex)))
										{
											unlink_reset (pRoom, pReset);
											free_reset_data (pReset);

											rcount++;
											SET_BIT (pRoom->area->area_flags,
													 AREA_CHANGED);

										}
								}
						}
				}
		}

	unlink_obj_index (pObj);

	pObj->area = NULL;
	pObj->vnum = 0;

	free_obj_index (pObj);

	if (ch)
		{
			printf_to_char (ch, "Removed object vnum %ld and %d resets.\n\r",
						  pIndex, rcount);

			printf_to_char(ch, "%d occurences of the object"
						  " were extracted from the mud.\n\r", ocount);

		}
	return TRUE;
}

REDIT (redit_delete)
{
	ROOM_INDEX_DATA *pRoom, *pRoom2;
	RESET_DATA *pReset;
	EXIT_DATA *ex;
	OBJ_DATA *Obj, *obj_next;
	CHAR_DATA *wch, *wnext;
	EXTRA_DESCR_DATA *pExtra;
	char arg[MIL];
	int pIndex, v;
	int i, iHash, rcount, ecount, mcount, ocount, edcount;

	if (IS_NULLSTR (argument))
		{
			if (ch)
				send_to_char ("Syntax:  redit delete [vnum]\n\r", ch);
			return FALSE;
		}

	one_argument (argument, arg);

	if (is_number (arg))
		{
			pIndex = atoi (arg);
			pRoom = get_room_index (pIndex);
		}
	else
		{
			if (ch)
				send_to_char ("That is not a number.\n\r", ch);
			return FALSE;
		}

	if (!pRoom)
		{
			if (ch)
				send_to_char ("No such room.\n\r", ch);
			return FALSE;
		}

	stop_editing (pRoom);

	SET_BIT (pRoom->area->area_flags, AREA_CHANGED);

	rcount = 0;

	for (pReset = pRoom->reset_first; pReset; pReset = pReset->next)
		{
			rcount++;
		}

	ocount = 0;
	for (Obj = pRoom->contents; Obj; Obj = obj_next)
		{
			obj_next = Obj->next_content;

			extract_obj (Obj);
			ocount++;
		}

	mcount = 0;
	for (wch = pRoom->people; wch; wch = wnext)
		{
			wnext = wch->next_in_room;
			if (IS_NPC (wch))
				{
					extract_char (wch, TRUE);
					mcount++;
				}
			else
				{
					send_to_char("This room is being deleted. Moving you somewhere safe.\n\r", wch);
					if (wch->fighting != NULL)
						stop_fighting (wch, TRUE);

					char_from_room (wch);

					char_to_room (wch, get_room_index (ROOM_VNUM_TEMPLE));
					wch->was_in_room = wch->in_room;
				}
		}

	ecount = 0;
	for (iHash = 0; iHash < MAX_KEY_HASH; iHash++)
		{
			for (pRoom2 = room_index_hash[iHash]; pRoom2; pRoom2 = pRoom2->next)
				{
					for (i = 0; i <= MAX_DIR; i++)
						{
							if (!(ex = pRoom2->exit[i]))
								continue;

							if (pRoom2 == pRoom)
								{

									ecount++;
									continue;
								}

							if (ex->u1.to_room == pRoom)
								{
									free_exit (pRoom2->exit[i]);
									pRoom2->exit[i] = NULL;
									SET_BIT (pRoom2->area->area_flags,
											 AREA_CHANGED);
									ecount++;
								}
						}
				}
		}

	edcount = 0;
	for (pExtra = pRoom->extra_descr; pExtra; pExtra = pExtra->next)
		{
			edcount++;
		}

	if (top_vnum_room == pIndex)
		for (v = 1; v < pIndex; v++)
			if (get_room_index (v))
				top_vnum_room = v;

	top_room--;

	unlink_room_index (pRoom);

	pRoom->area = NULL;
	pRoom->vnum = 0;

	free_room_index (pRoom);

	if (ch)
		{
			printf_to_char (ch, "Removed room vnum %ld, %d resets, %d extra "
						  "descriptions and %d exits.\n\r", pIndex, rcount, edcount,
						  ecount);
			printf_to_char (ch, "%d objects and %d mobiles were extracted "
						  "from the room.\n\r", ocount, mcount);
		}
	return TRUE;
}
