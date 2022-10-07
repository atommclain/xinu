/* sprintf.c - sprintf */

/*------------------------------------------------------------------------
 *  sprintf  --  format arguments and place output in a string
 *------------------------------------------------------------------------
 */
char	*sprintf(str, fmt, args)
	char *str;
	char *fmt;
{
	char	*s;
	int     sprntf();

	s = str;
	_doprnt(fmt, &args, sprntf, (int) &s);
	*s = '\0';
	return(str);
}

/*------------------------------------------------------------------------
 *  sprntf  --  routine called by doprnt to handle each character
 *------------------------------------------------------------------------
 */
static	sprntf(s, c)
	unsigned int	s;
	char		c;
{
	char	**pstr;

	pstr = (char **) (((long)&s & 0xFFFF0000) + s);
	if (pstr < &s)
		pstr = (char **) ((long)pstr + 0x10000);
	return(*(*pstr)++ = c);
}
