/********************************************************************************
 * Skill affects code copyright 1999-2001                                       *
 * Markanth : dlmud@dlmud.com                                                   *
 * Devil's Lament : dlmud.com port 3778                                      *
 * Web Page : http://www.dlmud.com                                              *
 *                                                                              *
 * Provides ability for affects to affect skill modifiers.                      *
 *                                                                              *
 * All I ask in return is that you give me credit on your mud somewhere         *
 * or email me if you use it, or if you make any changes.                       *
 ********************************************************************************/
>>> MERC.H <<<

1) Add macro:

#define GET_SKILL(ch, sn)	((IS_NPC((ch))) ? 0 : (((ch)->pcdata->learned[(sn)]) < 101 ? \
                            (ch)->pcdata->learned[(sn)] : 100) + (ch)->pcdata->skill_mod[(sn)])

2) In pcdata structure after learned[] add:

    sh_int skill_mod[MAX_SKILL];

3) Add apply_type:

#define APPLY_SKILL 31  /* next free number */


>>> MAGIC.C  <<<

1) in spell_identify() and do_lore() if you have it:

            if ( paf->location != APPLY_NONE && paf->modifier != 0 )
            {
++                if ( paf->location == APPLY_SKILL && paf->type > 0 && paf->type < MAX_SKILL )
++                    sprintf ( buf, "Affects skill {W%s{x by {W%d{x",
++                              skill_table[paf->type].name, paf->modifier );
++                else
                    sprintf ( buf, "Affects {W%s{x by {W%d{x",
                              flag_string ( apply_flags, paf->location ),
                              paf->modifier );
                send_to_char ( buf, ch );


2) And again for enchanted objects:

            if ( paf->location != APPLY_NONE && paf->modifier != 0 )
            {
++                if ( paf->location == APPLY_SKILL && paf->type > 0 && paf->type < MAX_SKILL )
++                    sprintf ( buf, "Affects skill {W%s{x by {W%d{x",
++                              skill_table[paf->type].name, paf->modifier );
++                else
                    sprintf ( buf, "Affects {W%s{x by {W%d{x",
                              flag_string ( apply_flags, paf->location ),
                              paf->modifier );
                send_to_char ( buf, ch );

>>> TABLES.C <<<

1) in apply_flags[] add:

    {"skillaff", APPLY_SKILL, TRUE},


>>> OLC_ACT.C <<<

1) add to oedit_show():

++           if ( paf->location == APPLY_SKILL && paf->type > 0 && paf->type < MAX_SKILL )
++                sprintf ( buf, "[%4d] %-8d %-12s %s{x\n\r", cnt,
++                          paf->modifier, flag_string ( apply_flags, paf->location ),
++                         skill_table[paf->type].name );
++            else
                sprintf ( buf, "[%4d] %-8d %-12s{x\n\r", cnt, paf->modifier,
                          flag_string ( apply_flags, paf->location ) );

2) in oedit_addaff() add:

    a) Add variable:

    int skill = -1;

    b) change:

    argument = one_argument( argument, loc );
--    one_argument( argument, mod );
++    argument = one_argument(argument, mod);

    c) add before the affect:

    if ( value == APPLY_SKILL &&
         ( IS_NULLSTR ( argument ) || 
        ( skill = skill_lookup ( argument ) ) < 0 || 
        skill >= MAX_SKILL) )
    {
        send_to_char ( "Syntax: addaffect skillaff <mod> <skill name>\n\r", ch);
        return FALSE;
    }


    d) Change

--        pAf->type = -1;
++        pAf->type = skill;

>>> HANDLER.C <<<

1) In reset_char() Add:

        a)
                        case APPLY_SKILL:
                            if ( !IS_NPC ( ch ) && af->type > 0 && af->type < MAX_SKILL )
                                ch->pcdata->skill_mod[af->type] -= mod;
                            break;
        b)
                    case APPLY_SKILL:
                        if ( !IS_NPC ( ch ) && paf->type > 0 && paf->type < MAX_SKILL )
                            ch->pcdata->skill_mod[af->type] -= mod;
                        break;
        c)
                    case APPLY_SKILL:
                        if ( !IS_NPC ( ch ) && paf->type > 0 && paf->type < MAX_SKILL )
                            ch->pcdata->skill_mod[af->type] += mod;
                        break;

        d)
                case APPLY_SKILL:
                    if ( !IS_NPC ( ch ) && paf->type > 0 && paf->type < MAX_SKILL )
                        ch->pcdata->skill_mod[af->type] += mod;
                break;

        e)
            case APPLY_SKILL:
                if ( !IS_NPC ( ch ) && paf->type > 0 && paf->type < MAX_SKILL )
                    ch->pcdata->skill_mod[af->type] += mod;
                break;

2) Add to affect_modify():

        case APPLY_SKILL:
            if ( !IS_NPC ( ch ) && paf->type > 0 && paf->type < MAX_SKILL )
                ch->pcdata->skill_mod[paf->type] += mod;
            break;

>>> END <<<

Now in things like do_practice() and do_skills()
replace things like ch->pcdata->learned[sn] with GET_SKILL(ch, sn).

ie. grep -n \>learned *.c

Not all references you will want changed to GET_SKILL macro.

Voila, now you have skill affects.
Markanth.