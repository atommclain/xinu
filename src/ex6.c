/* ex6.c - additem */

int	mutex;
int	a[100];
int	n = 0;

/*------------------------------------------------------------------------
 *  additem  --  obtain exclusive access to array 'a' and add item to it
 *------------------------------------------------------------------------
 */
additem(item)
{
	wait(mutex);
	a[n++] = item;
	signal(mutex);
}
