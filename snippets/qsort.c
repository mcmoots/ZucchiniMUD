On Fri, 26 Sep 1997, Dennis Reed wrote:

> >   I have a question.  It prolly seems simple to some of you . . . but it
> > is stumpin me  :)  I cant seem to think of a way to display a list in
> > order, from least to greatest, by level.  Well by any number I guess.

> There are two ways that I know of that you can do it.  One is to sort the
> list before displaying it.  (Bubble sorting is one way to do it.  There 
> are other ways to sort too).  You could also run a loop through it,

It's relatively easy to use qsort for this, once you figure out pointers
to functions.

You'd start with creating an array containing the data you want sorted
(probably pointers to the areas int his case rather the areas themselves). 
You also need a function that will take a *pointer* to two pieces of the
data you have stored and return -1/0/1 depending on which is bigger.
Something like:

void show_areas_sorted()
{
	AREA_DATA * areas[top_area];
	/* put in all aras in the areas array */

	qsort(
		areas,		/* Where does the data to sort start? */
		top_area,	/* How many elements? */
		sizeof(areas[0]), /* How big is each element? */
		compare_area);	/* What function to use to display? */

	/* Run through areas[] displaying it */
}

Note that the above code uses a gcc-specific extension of dynamically
sized auto arrays. If you don't have gcc, you could use alloca() to
allocate space "dymically" on the stack - space which is freed
automatically when leaving the function, together with other local
variables. Also, I am not certain if "top_area" is the number of areas
allocated, or 1 more than that. I assume in the code that it is the
former.

You will also have to write the compare_area() function which should
compare to areas:

int compare_area (const void *v1, const void *v2)
{
	AREA_DATA *a1 = *(AREA_DATA**)v1;
	AREA_DATA *a2 = *(AREA_DATA**)v2;

	if (a1->lvnum > a2->lvnum)
		return -1; 
	else if (a1->lvnum < a2->lvnum)
		return +1;
	else
		return 0;
}


The function will as mentioned above take two pointers to the type of data
you are sorting. Since what you are storing are *pointers* to areas, what
you get is pointers to pointers to areas - and they need to be cast as
shown above to be useful.

The function must return -1 if the first element is "bigger" then the
other, 0 if they are equal, and +1 if the second element is bigger. qsort
will sort the data so biggest is last.


See also: man qsort.


> For listing used/free vnums, I would suggest the function written by Erwin
> S. Andreasen that does just that, using the second method listed above.
> It is available on his web site in a file with several other useful
> commands at http://pip.dknet.dk/~erwin/ftp/wizutil.v3

I wrote it before I knew ILAB OLC existed - it has the fatal flaw of only
showing a 100-area slot as available if the first room in taht slot is
used.

==============================================================================
Erwin Andreasen   Herlev, Denmark <erwin@pip.dknet.dk>  UNIX System Programmer
<URL:http://pip.dknet.dk/~erwin/>         <*>           (not speaking for) DDE
==============================================================================
