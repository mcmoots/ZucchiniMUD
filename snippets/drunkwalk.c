From: eidsod01@condor.stcloudstate.edu

>         In the interest of role-playing I'm forced to ask myself a question.
> Why are there drunk talk codes and no drunk WALK codes?  Has anyone taken
> the time to convert circles drunk walk, made their own, or is this something
> that needs to be done yet?

There *are* drunk walk snippets for merc... or maybe she wrote it 
herself... Umm... yeah, if you'd like to, I could throw one 
together really fast... of course you could do that yourself...


add to the top of move_char()

    if( ch->pcdata->condition[COND_DRUNK] >= 5  /* Is char. drunk? */
        && number_percent() < 50 )		/* 50-50 chance */
    {
	act("$n stumbles around drunk.",ch,NULL,NULL,TO_ROOM);
	act("You stumble around drunk.",ch,NULL,NULL,TO_CHAR);

					/* Add this for ROT only */
	if(ch->align <0)
	    door = number_range(0,11);
	else
					/* End of "Add this for ROT only" */
	    door = number_range(0,5);		/* Find new direction to 
						   leave in */
    }

This should do the job... just make sure to add this before the checks 
for if he exit actually exists... that way you don't have to do the 
checks for existing door twice.... Also, he "add for ROT only" - lines, 
ROT has alignment sensitive exits... (numbered 7-11 for the "evil" ones)


Nick

P.S> I make *NO* garanties whatsoever that the above code actually 
works... I just woke up, and am still trying to find out whick of hte 
three monitors is the *real* one...
