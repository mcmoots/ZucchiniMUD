Hi there. This area comes from my long-defunct playground of a mud -
fold, spindle, and mutilate all you want, but keep the original builder's name in the
credits.

You'll need to make a few changes before booting the area up in a stock
ROM; see below for complete instructions.

-Kazeus

---

How To Re-Stockify Zucchini Areas:

I: AREADATA

 I've added a levels field to this section of the file; delete at will.

II: ROOMS

 Stock rom has an odd habit of saving exit flags as integers, rather
 than as flags, which causes things like EX_INFURIATING to vanish at
 every reboot. I've rewritten my save/load functions so that exit flags
 are saved as flags. This snippet is fairly simple, and I highly
 recommend it as it makes adding new exit flags much easier.

 http://www.the-infinite.org/lists/romlist/1998/07/msg00529.html
 for instructions.

 Otherwise, you'll have to go through all the exits and change them back
 to stock format. Exit numbers in the areafiles are given as a sum of
 corresponding numbers for each exit flag. Zucchini's exit flags were
 as follows:

 EX_ISDOOR      1
 EX_CLOSED      2
 EX_LOCKED      4
 EX_HIDDEN      8 
 EX_PICKPROOF   16
 EX_NOPASS      32
 EX_EASY	64
 EX_HARD	128
 EX_INFURIATING 256
 EX_NOCLOSE     512
 EX_NOLOCK      1024

 So, for example, an exit flag value of 39 would correspond to a closed,
 locked, no_pass door; in stock rom, the door would be coded as 2, closed
 and locked. The no_pass flag cannot be saved in a stock area file.

III: MOBPROGS

 I used the standard Ivan OLC mobprogs, but added a name field. This is
 the ~-delimited string below the vnum. You MUST remove this field
 before trying to boot the area or everything will fuck up.
