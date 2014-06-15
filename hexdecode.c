/* ********************************************************************** */
/**
 * @brief   Command line tool to decode hex-dumped text.
 * @author  eel3
 * @date    2014/06/05
 *
 * @par Compilers
 *   - GCC 4.6.3 (Ubuntu 12.04.3 LTS)
 *   - TDM-GCC 4.8.1 (Windows 7 64bit SP1)
 *   - Visual Studio 2010 (Windows 7 64bit SP1)
 */
/* ********************************************************************** */


#if defined(WIN32) || defined(WIN64)
#	if !defined(__MINGW32__) && !defined(__MINGW64__)
#		ifndef _CRT_SECURE_NO_WARNINGS
#			define _CRT_SECURE_NO_WARNINGS
#		endif /* ndef _CRT_SECURE_NO_WARNINGS */
#	endif /* !defined(__MINGW32__) && !defined(__MINGW64__) */
#endif /* defined(WIN32) || defined(WIN64) */

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(WIN32) || defined(WIN64)
#	include <fcntl.h>
#	include <io.h>
#	ifndef STDIN_FILENO
#		define STDIN_FILENO 0
#	endif
#	ifndef STDOUT_FILENO
#		define STDOUT_FILENO 1
#	endif
#	define PATH_SEP '\\'
#else /* defined(WIN32) || defined(WIN64) */
#	define PATH_SEP '/'
#endif /* defined(WIN32) || defined(WIN64) */


/* ---------------------------------------------------------------------- */
/* Constant */
/* ---------------------------------------------------------------------- */

/** Error code. */
enum {
	E_OK = 0,       /**< OK (success). */
	E_NG = -1,      /**< NG (general failure). */
	E_EOF = -2      /**< Detect EOF. */
};


/* ---------------------------------------------------------------------- */
/* Function-like macro */
/* ---------------------------------------------------------------------- */

/* ====================================================================== */
/**
 * @brief  Return true if string s1 equal to s2.
 *
 * @param[in] s1  string 1.
 * @param[in] s2  string 2.
 *
 * @retval !=0  Equal.
 * @retval   0  Not equal.
 */
/* ====================================================================== */
#define  STREQ(s1, s2)  (((s1)[0] == (s2)[0]) && (strcmp((s1), (s2)) == 0))


/* ---------------------------------------------------------------------- */
/* Function */
/* ---------------------------------------------------------------------- */

/* ====================================================================== */
/**
 * @brief  Tiny copy of basename(3).
 *
 * @param[in] *name  (See man 3 basename).
 *
 * @return  (See man 3 basename).
 */
/* ====================================================================== */
static const char *
my_basename(const char * const name)
{
	const char *bn;

	assert(name != NULL);

	bn = strrchr(name, PATH_SEP);
	return (bn == NULL) ? name : bn+1;
}

/* ====================================================================== */
/**
 * @brief  Similar to fgetc(3),
 *         but this skips white-space charactors (see man 3 isspace).
 *
 * @param[in,out] *in  (See man 3 fgetc).
 *
 * @return  (See man 3 fgetc).
 */
/* ====================================================================== */
static int
fgetc_without_space(FILE * const in)
{
	int c;

	do {
		c = fgetc(in);
	} while ((c != EOF) && isspace((unsigned char) c));

	return c;
}

/* ====================================================================== */
/**
 * @brief  Decode hex-dumped text.
 *
 * @param[in]     *inname  Name of input stream.
 * @param[in,out] *in      Input stream.
 * @param[in,out] *out     Output stream.
 *
 * @retval E_OK   OK (success).
 * @retval E_NG   NG (failure).
 * @retval E_EOF  Detect EOF from output stream.
 */
/* ====================================================================== */
static int
hexdecode(const char * const inname, FILE * const in, FILE * const out)
{
	assert((inname != NULL) && (in != NULL) && (out != NULL));

	for (;;) {
		int c;
		char hex[3], *endp;
		unsigned long n;
		unsigned char outvalue;

		if ((c = fgetc_without_space(in)) == EOF) {
			return E_OK;
		}
		if (!isxdigit((unsigned char) c)) {
			(void) fprintf(stderr, "%s: detect not hex charactor\n", inname);
			return E_NG;
		}
		hex[0] = (char) (unsigned char) c;

		if ((c = fgetc_without_space(in)) == EOF) {
			(void) fprintf(stderr, "%s: unexpected end of file\n", inname);
			return E_NG;
		}
		if (!isxdigit((unsigned char) c)) {
			(void) fprintf(stderr, "%s: detect not hex charactor\n", inname);
			return E_NG;
		}
		hex[1] = (char) (unsigned char) c;

		hex[2] = '\0';
		errno = 0;
		n = strtoul(hex, &endp, 16);
		if ((n == ULONG_MAX) && (errno == ERANGE)) {
			assert(0);
			(void) fprintf(stderr, "%s: decode failed\n", inname);
			return E_NG;
		}
		if (*endp != '\0') {
			assert(0);
			(void) fprintf(stderr, "%s: decode failed\n", inname);
			return E_NG;
		}

		outvalue = (unsigned char) n;
		if (fwrite(&outvalue, 1, 1, out) != 1) {
			(void) fprintf(stderr, "cannot write decoded value\n");
			return E_EOF;
		}
	}
	/*NOTREACHED*/
}

/* ********************************************************************** */
/**
 * @brief  Main routine.
 *
 * @retval EXIT_SUCCESS  OK (success).
 * @retval EXIT_FAILURE  NG (failure).
 */
/* ********************************************************************** */
int
main(int argc, char *argv[])
{
	int err, retval;

	if ((argc == 2)
		&& (STREQ(argv[1], "-h") || STREQ(argv[1], "--help")))
	{
		(void) fprintf(stderr, "usage: %s [-h|--help] [file ...]\n", my_basename(argv[0]));
		return EXIT_FAILURE;
	}

#if defined(WIN32) || defined(WIN64)
	errno = 0;
	if (_setmode(STDOUT_FILENO, O_BINARY) == -1) {
		perror("_setmode");
		return EXIT_FAILURE;
	}
#endif /* defined(WIN32) || defined(WIN64) */

	if (argc <= 1) {
		err = hexdecode("(stdin)", stdin, stdout);
		retval = (err < 0) ? EXIT_FAILURE : EXIT_SUCCESS;
	} else {
		int i;
		const char * inname;
		FILE *in;

		retval = EXIT_SUCCESS;

		for (i = 1; i < argc; ++i) {
			if (STREQ(argv[i], "-")) {
				inname = "(stdin)";
				in = stdin;
			} else if (errno = 0, (in = fopen(argv[i], "r")) == NULL) {
				perror(argv[i]);
				retval = EXIT_FAILURE;
				continue;
			} else {
				inname = argv[i];
			}

			err = hexdecode(inname, in, stdout);
			if (err < 0) {
				retval = EXIT_FAILURE;
			}

			if (in != stdin) {
				(void) fclose(in);
			}

			if (err == E_EOF) {
				break;
			}
		}
	}

	return retval;
}
