Online Social editting
======================

(c) 1996 Erwin S. Andreasen <erwin@pip.dknet.dk>

See the file "License" first for license information.

This module has been tested with MERC as well as Envy 1.0 and 2.0. It should
work under Linux, SunOS as well as BSDI.

Installation
============

This should be rather easy. Insert the social-edit file in your Makefile.

Check out and modify the value of SOCIAL_FILE in social-edit.c.

Also, insert printf_to_char in somewhere, like comm.c. Remember to include
the prototype for it in merc.h. printf_to_char is a rather handy thing,
which I found in John Booth's EOD source. Also, remeber to add 

#include <stdargs.h>

at top of comm.c. I am not 100% sure how portable this is.

The file sedit.hlp contains a help entry.


Step 1 - saving the old social table:
======

Be sure that CONST_SOCIAL is defined in sedit-social.c.

Make a call to save_social_table() somewhere, for example at the end of
boot_db(). You might want to declare the prototypes for load_social_table()
and save_social_table() in merc.h:

void load_social_table();
void save_social_table();

Start up the MUD, make sure the file is saved, shutdown.


Step 2 - removing the old socials.
======

Remove the whole social table from interp.c. Then replace, in merc.h, the
old const social table, with this non-constant one:

extern struct social_type *social_table;

Also, change the social_type structure in merc.h, removing all the "const".


Step 3 - adding loading of social table
======

Remove the #define CONST_SOCIAL. Add call to load_social_table somewhere in
boot_db(). Compile. Run. Try the commands out, but be SURE to havea backup
of the social file around.

Note: there are maxSocial socials in the social_table, but the table is one
larger than that. The last entry has its name == "", for backwards
compatibility with whatever uses social_table now.

Also add the SEDIT command, used for online editting of the socials.
The procedure for adding commands is in the doc files accompanying
MERC/Envy. This should be an immortal command


Technical issues
================

For easy backwards compatbility, I use realloc to allocate the new larger
social table when a new social is created. I do not know if this will
fragment the memory. The alternative would be a linked list of socials, but
I felt lazy. This way, the new table is nicely backwards compatible.

The whole social table is written to disk when a social is added. The social
file contains first the number of socials in the file, followed by the
socials.


Social Editting and ROM
=======================

This is what Whiplash <whiplash@armory.com> writes:

>in db2.c
>comment out load_socials( )  /* ROM uses the social table stuff here,
>                                not in interp.c, needed to comment a
>                                line or two in db.c in ref. to load_socials
>                              */

As I run Envy2.0, and not ROM, I can be of no further assistance.


Updates since last version
==========================

Added smash_tilde() to do_sedit.







