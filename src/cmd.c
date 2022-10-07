/* cmd.c */

#include <conf.h>
#include <kernel.h>
#include <shell.h>
#include <cmd.h>

/* Initialize shell command structure */

extern	int
	shell(),	x_bpool(),	x_cat(),	x_cd(),
	x_close(),	x_color(),	x_cp(),		x_date(),
	x_debug(),	x_devs(),	x_echo(),	x_eject(),
	x_exit(),	x_format(),	x_help(),	x_kill(),
	x_lf(),		x_mem(),	x_mount(),	x_mv(),
	x_panic(),	x_ps(),		x_pwd(),	x_read(),
	x_rm(),		x_scroll(),	x_shutdown(),	x_sleep(),
	x_sync(),	x_unmou(),	x_who(),	x_write();

struct	cmdent	cmds[]  = {		/* shell commands		*/
/* Commands:	 name		Builtin?	procedure		*/

		"bpool",	FALSE,		x_bpool,
		"cat",		FALSE,		x_cat,
		"cd",		TRUE,		x_cd,
		"close",	FALSE,		x_close,
		"color",	FALSE,		x_color,
		"cp",		FALSE,		x_cp,
		"date",		FALSE,		x_date,
		"debug",	TRUE,		x_debug,
		"devs",		FALSE,		x_devs,
		"echo",		FALSE,		x_echo,
		"eject",	FALSE,		x_eject,
		"exit",		TRUE,		x_exit,
		"format",	FALSE,		x_format,
		"help",		FALSE,		x_help,
		"kill",		TRUE,		x_kill,
		"lf",		FALSE,		x_lf,
		"logout",	TRUE,		x_exit,
		"mem",		FALSE,		x_mem,
		"mount",	FALSE,		x_mount,
		"mv",		FALSE,		x_mv,
		"panic",	FALSE,		x_panic,
		"ps",		FALSE,		x_ps,
		"pwd",		FALSE,		x_pwd,
		"read",		FALSE,		x_read,
		"rm",		FALSE,		x_rm,
		"scroll",	FALSE,		x_scroll,
		SHNAME,		FALSE,		shell,
		"sh",		FALSE,		shell,
		"shutdown",	FALSE,		x_shutdown,
		"sleep",	FALSE,		x_sleep,
		"sync",		FALSE,		x_sync,
		"time",		FALSE,		x_date,
		"unmount",	FALSE,		x_unmou,
		"who",		FALSE,		x_who,
		"write",	FALSE,		x_write,
		"?",		FALSE,		x_help};

int	ncmds = sizeof(cmds)/sizeof(struct cmdent);
