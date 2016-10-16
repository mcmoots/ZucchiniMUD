/********************************************************************************
 * Memory logging code v2.0                                                     *
 * Markanth : dlmud@dlmud.com                                                   *
 * Devil's Lament : dlmud.com port 3778                                      *
 * Web Page : http://www.dlmud.com                                              *
 *                                                                              *
 * Provides a line number and file name logging for recycling bad memory.       *
 *                                                                              *
 * All I ask in return is that you give me credit on your mud somewhere         *
 * or email me if you use it, or if you make any changes.                       *
 ********************************************************************************/

>>> MERC.H <<<

1) add these macros:

/* use for free'ing and str_dup'ing in 1 shot */
#define replace_string( pstr, nstr )            \
{                                               \
  _free_string( (pstr), __FILE__, __LINE__ );   \
  pstr = str_dup( (nstr) );                     \
} 

#define free_mem( pMem, sMem )  _free_mem(pMem, sMem, __FILE__, __LINE__)

#define free_string( pstr )  _free_string(pstr, __FILE__, __LINE__)


2) change free_string and free_mem prototypes to these:

void _free_mem args ((void *pMem, int sMem, char *file, int line));
void _free_string args ((char *pstr, char *file, int line));

>>> DB.C <<<

1) Replace SKEY macro with this one:

#define	SKEY( string, field )                       \
if ( !str_cmp( word, string ) )                     \
{									                \
    _free_string( field, __FILE__, __LINE__ );      \
	field = fread_string( fp );                     \
	fMatch = TRUE;                                  \
	break;                                          \
}

3) replace free_mem() with something like this one:

void _free_mem (void *pMemV, int sMem, char *file, int line )
{
  int iList;
  int *magic;
  char *pMem = pMemV;

  pMem -= sizeof (*magic);
  magic = (int *) pMem;

  if (*magic != MAGIC_NUM)
  {
    bugf ("Attempt to recyle invalid memory of size %d -> %s:%d", sMem, file, line);
    bug ((char *) pMem + sizeof (*magic), 0);
    return;
  }

  *magic = 0;
  sMem += sizeof (*magic);

  for (iList = 0; iList < MAX_MEM_LIST; iList++)
  {
    if (sMem <= rgSizeList[iList])
      break;
  }

  if (iList == MAX_MEM_LIST)
  {
    bug ("Free_mem: size %d too large. -> %s:%d", sMem, file, line);
    exit (1);
  }

  *((void **) pMem) = rgFreeList[iList];
  rgFreeList[iList] = pMem;
  return;
}

4) replace free_string() with this one:

void _free_string (char *pstr, char *file, int line)
{
  if (pstr == NULL || pstr == &str_empty[0] ||
      (pstr >= string_space && pstr < top_string))
    return;

  _free_mem (pstr, strlen (pstr) + 1, file, line);
  return;
}


>>> SAVE.C <<<

1) replace KEYS macro with this one:

#define	KEYS( literal, field, value )               \
if ( !str_cmp( word, literal ) )                    \
{										            \
	free_string_plus(field, __FILE__, __LINE__);    \
	field  = value;                                 \
	fMatch = TRUE;                                  \
	break;                                          \
}


______________________
Markanth : dlmud@dlmud.com     
Devil's Lament : dlmud.com port 3778     
Web Page : http://dlmud.akbearsden.com