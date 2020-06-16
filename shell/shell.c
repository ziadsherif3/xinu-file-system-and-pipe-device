/* shell.c  -  shell */

#include <xinu.h>
#include <stdio.h>
#include "shprototypes.h"

struct inode *cwd = &fsystem.rt0;

/************************************************************************/
/* Table of Xinu shell commands and the function associated with each	*/
/************************************************************************/
const	struct	cmdent	cmdtab[] = {
	{"argecho",	TRUE,	xsh_argecho},
	{"clear",	TRUE,	xsh_clear},
	{"devdump",	FALSE,	xsh_devdump},
	{"echo",	FALSE,	xsh_echo},
	{"exit",	TRUE,	xsh_exit},
	{"help",	FALSE,	xsh_help},
	{"kill",	TRUE,	xsh_kill},
	{"memdump",	FALSE,	xsh_memdump},
	{"memstat",	FALSE,	xsh_memstat},
	{"ps",		FALSE,	xsh_ps},
	{"ramdisktest",	FALSE,	xsh_ramdisktest},
	{"sleep",	FALSE,	xsh_sleep},
	{"uptime",	FALSE,	xsh_uptime},
	{"?",		FALSE,	xsh_help},
	{"mkdir", 	TRUE,	xsh_mkdir},
	{"rmdir", 	TRUE,	xsh_rmdir},
	{"ls", 		FALSE,	xsh_ls},
	{"touch", 	TRUE,	xsh_touch},
	{"cat", 	FALSE,	xsh_cat},
	{"mv", 		TRUE,	xsh_mv},
	{"rm", 		TRUE,	xsh_rm},
	{"cp", 		TRUE,	xsh_cp},
	{"pwd",		FALSE,	xsh_pwd},
	{"cd", 		TRUE,	xsh_cd}

};

uint32	ncmd = sizeof(cmdtab) / sizeof(struct cmdent);

/************************************************************************/
/* shell  -  Provide an interactive user interface that executes	*/
/*	     commands.  Each command begins with a command name, has	*/
/*	     a set of optional arguments, has optional input or		*/
/*	     output redirection, and an optional specification for	*/
/*	     background execution (ampersand).  The syntax is:		*/
/*									*/
/*		   command_name [args*] [redirection] [&]		*/
/*									*/
/*	     Redirection is either or both of:				*/
/*									*/
/*				< input_file				*/
/*			or						*/
/*				> output_file				*/
/*									*/
/************************************************************************/

process	shell (
		did32	dev		/* ID of tty device from which	*/
	)				/*   to accept commands		*/
{

	// char  biff[3] = "hi\0";
	// char  bdff[6] = "rd0:/s\0";
	// printf("%s\n",bdff);
	// printf("%s\n",biff);
	// char out[3];
	// did32 f;
	// printf("Control output: %d\n",control(FSYSTEM, FCREATE, (int32) bdff, 0));
	// f = open(FSYSTEM, bdff, "w");
	// printf("write output: %d\n",write(f,biff, 3));
	// close(f);
	// f = open(FSYSTEM, bdff, "r");
	// printf("read output: %d\n",read(f,out,3));
	// close(f);
	// printf("%s\n\n\n\n\n\n\n", out);


	char	buf[SHELL_BUFLEN];	/* Input line (large enough for	*/
					/*   one line from a tty device	*/
	int32	len;			/* Length of line read		*/
	char	tokbuf[SHELL_BUFLEN +	/* Buffer to hold a set of	*/
			SHELL_MAXTOK];  /* Contiguous null-terminated	*/
					/* Strings of tokens		*/
	int32	tlen;			/* Current length of all data	*/
					/*   in array tokbuf		*/
	int32	tok[SHELL_MAXTOK];	/* Index of each token in	*/
					/*   array tokbuf		*/
	int32	toktyp[SHELL_MAXTOK];	/* Type of each token in tokbuf	*/
	int32	ntok;			/* Number of tokens on line	*/
	pid32	child;			/* Process ID of spawned child	*/
	bool8	backgnd;		/* Run command in background?	*/
	char	*outname, *inname;	/* Pointers to strings for file	*/
					/*   names that follow > and <	*/
	did32	stdinput, stdoutput;	/* Descriptors for redirected	*/
					/*   input and output		*/
	int32	i;			/* Index into array of tokens	*/
	int32	j;			/* Index into array of commands	*/
	int32	msg;			/* Message from receive() for	*/
					/*   child termination		*/
	int32	tmparg;			/* Address of this var is used	*/
					/*   when first creating child	*/
					/*   process, but is replaced	*/
	char	*src, *cmp;		/* Pointers used during name	*/
					/*   comparison			*/
	bool8	diff;			/* Was difference found during	*/
					/*   comparison			*/
	char	*args[SHELL_MAXTOK];	/* Argument vector passed to	*/
					/*   builtin commands		*/

	/* Print shell banner and startup message */

	fprintf(dev, "\n\n%s%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
		SHELL_BAN0,SHELL_BAN1,SHELL_BAN2,SHELL_BAN3,SHELL_BAN4,
		SHELL_BAN5,SHELL_BAN6,SHELL_BAN7,SHELL_BAN8,SHELL_BAN9);

	fprintf(dev, "%s\n\n", SHELL_STRTMSG);

	/* Continually prompt the user, read input, and execute command	*/

	while (TRUE) {

		/* Display prompt */

		fprintf(dev, SHELL_PROMPT);

		/* Read a command */

		len = read(dev, buf, sizeof(buf));

		/* Exit gracefully on end-of-file */

		if (len == EOF) {
			break;
		}

		/* If line contains only NEWLINE, go to next line */

		if (len <= 1) {
			continue;
		}

		buf[len] = SH_NEWLINE;	/* terminate line */

		/* Parse input line and divide into tokens */

		ntok = lexan(buf, len, tokbuf, &tlen, tok, toktyp);

		/* Handle parsing error */

		if (ntok == SYSERR) {
			fprintf(dev,"%s\n", SHELL_SYNERRMSG);
			continue;
		}

		/* If line is empty, go to next input line */

		if (ntok == 0) {
			fprintf(dev, "\n");
			continue;
		}

		/* If last token is '&', set background */

		if (toktyp[ntok-1] == SH_TOK_AMPER) {
			ntok-- ;
			tlen-= 2;
			backgnd = TRUE;
		} else {
			backgnd = FALSE;
		}


		/* input/output redirection is none */

		outname = inname = NULL;
		if ( (ntok >=3) && ( (toktyp[ntok-2] == SH_TOK_LESS)
				   ||(toktyp[ntok-2] == SH_TOK_GREATER))){
			if (toktyp[ntok-1] != SH_TOK_OTHER) {
				fprintf(dev,"%s\n", SHELL_SYNERRMSG);
				continue;
			}
			if (toktyp[ntok-2] == SH_TOK_LESS) {
				inname =  &tokbuf[tok[ntok-1]];
			} else {
				outname = &tokbuf[tok[ntok-1]];
			}
			ntok -= 2;
			tlen = tok[ntok];
		}


		if ( (ntok >=3) && ( (toktyp[ntok-2] == SH_TOK_LESS)
				   ||(toktyp[ntok-2] == SH_TOK_GREATER))){
			if (toktyp[ntok-1] != SH_TOK_OTHER) {
				fprintf(dev,"%s\n", SHELL_SYNERRMSG);
				continue;
			}
			if (toktyp[ntok-2] == SH_TOK_LESS) {
				if (inname != NULL) {
				    fprintf(dev,"%s\n", SHELL_SYNERRMSG);
				    continue;
				}
				inname = &tokbuf[tok[ntok-1]];
			} else {
				if (outname != NULL) {
				    fprintf(dev,"%s\n", SHELL_SYNERRMSG);
				    continue;
				}
				outname = &tokbuf[tok[ntok-1]];
			}
			ntok -= 2;
			tlen = tok[ntok];
		}


		/* Verify remaining tokens are type "other" */

		for (i=0; i<ntok; i++) {
			if (toktyp[i] != SH_TOK_OTHER) {
				break;
			}
		}
		if ((ntok == 0) || (i < ntok)) {
			fprintf(dev, SHELL_SYNERRMSG);
			continue;
		}

		stdinput = stdoutput = dev;

		/* Lookup first token in the command table */

		for (j = 0; j < ncmd; j++) {
			src = cmdtab[j].cname;
			cmp = tokbuf;
			diff = FALSE;
			while (*src != NULLCH) {
				if (*cmp != *src) {
					diff = TRUE;
					break;
				}
				src++;
				cmp++;
			}
			if (diff || (*cmp != NULLCH)) {
				continue;
			} else {
				break;
			}
		}

		/* Handle command not found */

		if (j >= ncmd) {
			fprintf(dev, "command %s not found\n", tokbuf);
			continue;
		}

		/* Handle built-in command */

		if (cmdtab[j].cbuiltin) { /* No background or redirect. */
			if (inname != NULL || outname != NULL || backgnd){
				fprintf(dev, SHELL_BGERRMSG);
				continue;
			} else {
				/* Set up arg vector for call */

				for (i=0; i<ntok; i++) {
					args[i] = &tokbuf[tok[i]];
				}

				/* Call builtin shell function */

				if ((*cmdtab[j].cfunc)(ntok, args)
							== SHELL_EXIT) {
					break;
				}
			}
			continue;
		}

		/* Open files and redirect I/O if specified */

		if (inname != NULL) {
			fprintf(dev, SHELL_INERRMSG, inname);
			continue;
		}
		if (outname != NULL) {
			//printf("outname: %s", outname);
			if ((stdoutput = open(FSYSTEM, outname, "w")) == NOTFOUND) { /* File not found */
				control(FSYSTEM, FCREATE, (int32)outname, 0);
				stdoutput = open(FSYSTEM, outname, "w");
				proctab[currpid].nfprdesc--;
				proctab[currpid].prdesc[proctab[currpid].pprdesc] = -1;
			}
			else if (stdoutput == SYSERR) {
				fprintf(dev, SHELL_OUTERRMSG, outname);
				continue;
			}
			else { /* Mark the file as closed because the child will close it as its stdoutput */
				proctab[currpid].nfprdesc--;
				proctab[currpid].prdesc[proctab[currpid].pprdesc] = -1;
			}
		}

		/* Spawn child thread for non-built-in commands */

		child = create(cmdtab[j].cfunc,
			SHELL_CMDSTK, SHELL_CMDPRIO,
			cmdtab[j].cname, 2, ntok, &tmparg);

		/* If creation or argument copy fails, report error */

		if ((child == SYSERR) ||
		    (addargs(child, ntok, tok, tlen, tokbuf, &tmparg)
							== SYSERR) ) {
			fprintf(dev, SHELL_CREATMSG);
			continue;
		}

		/* Set stdinput and stdoutput in child to redirect I/O */

		proctab[child].prdesc[0] = stdinput;
		proctab[child].prdesc[1] = stdoutput;

		msg = recvclr();
		resume(child);
		if (! backgnd) {
			msg = receive();
			while (msg != child) {
				msg = receive();
			}
		}
    }

    /* Terminate the shell process by returning from the top level */

    fprintf(dev,SHELL_EXITMSG);
    return OK;
}
