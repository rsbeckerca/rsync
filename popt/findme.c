/** \ingroup popt
 * \file popt/findme.c
 */

/* (C) 1998-2002 Red Hat, Inc. -- Licensing details are in the COPYING
   file accompanying popt source distributions, available from 
   ftp://ftp.rpm.org/pub/rpm/dist. */

#include "rsync.h"
#include "system.h"
#include "findme.h"

const char * findProgramPath(const char * argv0)
{
    char * path = getenv("PATH");
    char * pathbuf;
    char * start, * chptr;
    char * buf;
    size_t bufsize;

    if (argv0 == NULL) return NULL;	/* XXX can't happen */
    /* If there is a / in the argv[0], it has to be an absolute path */
    if (strchr(argv0, '/'))
	return xstrdup(argv0);

    if (path == NULL) return NULL;

    bufsize = strlen(path) + 1;
#if defined __TANDEM
    start = pathbuf = malloc(bufsize);
#else
    start = pathbuf = alloca(bufsize);
#endif
    if (pathbuf == NULL) return NULL;	/* XXX can't happen */
    strlcpy(pathbuf, path, bufsize);
    bufsize += sizeof "/" - 1 + strlen(argv0);
    buf = malloc(bufsize);
#if defined __TANDEM
    if (buf == NULL) {
	free(start);
	return NULL;	/* XXX can't happen */
    }
#else
    if (buf == NULL) return NULL;	/* XXX can't happen */
#endif

    chptr = NULL;
    /*@-branchstate@*/
    do {
	if ((chptr = strchr(start, ':')))
	    *chptr = '\0';
	snprintf(buf, bufsize, "%s/%s", start, argv0);

#if defined __TANDEM
	if (!access(buf, X_OK)) {
	    free(start);
	    return buf;
	}
#else
	if (!access(buf, X_OK))
	    return buf;
#endif

	if (chptr) 
	    start = chptr + 1;
	else
	    start = NULL;
    } while (start && *start);
    /*@=branchstate@*/

    free(buf);

#if defined __TANDEM
    free(start);
#endif
    return NULL;
}
