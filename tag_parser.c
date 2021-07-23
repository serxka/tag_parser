// This was some bullshit to find info for. As a future reference look into the
// `postgres/src/backend/tsearch` directory, specifically at the `wparser_def.c` 
// file, this contains the default full text parser for Postgres

#include "postgres.h"
#include "tsearch/ts_public.h"
#include "fmgr.h"
#include "utils/builtins.h"

// Character we break our tags on
#define BREAK_CHAR (',')

#define TYPE_TAG 1
#define TYPE_SEP 2

// Included from `fmgr.h`, is a magic block so Postgres know it can load this object
PG_MODULE_MAGIC;

// Wraps with the Postgres calling convention so it can call the functions, not 
// needed if function won't be called directly
PG_FUNCTION_INFO_V1(tagpsr_start);
PG_FUNCTION_INFO_V1(tagpsr_gettoken);
PG_FUNCTION_INFO_V1(tagpsr_end);
PG_FUNCTION_INFO_V1(tagpsr_lextype);

// Contains information about the state of our parser, text we are parsing, it's
// length and how much we have parsed so far
typedef struct {
	char *buf; // Text to parse
	int len; // Length of text
	int pos; // Cursor of parser
} parse_state_t;

// This is called at the start of parsing
// Args: char *text, int textlen
// Ret: void *user_data
Datum tagpsr_start(PG_FUNCTION_ARGS) {
	parse_state_t *p = palloc(sizeof(parse_state_t));
	p->buf = (char*)PG_GETARG_POINTER(0);
	p->len = PG_GETARG_INT32(1);
	p->pos = 0;

	PG_RETURN_POINTER(p);
}

// This is called each time a new token is requested
// Args: void *user_data, char** token_start, int *token_len
// Ret: int token_type
Datum tagpsr_gettoken(PG_FUNCTION_ARGS) {
	parse_state_t *p = (parse_state_t*)PG_GETARG_POINTER(0);
	char **token = (char**)PG_GETARG_POINTER(1);
	int *tlen = (int*)PG_GETARG_POINTER(2);
	
	int type; // Our return value
	int spos = p->pos; // Our starting position

	// Set the start of the token to our cursor
	*token = p->buf + p->pos;

	if (p->pos >= p->len) { // If we have parsed it all
		type = 0;
		goto ret;
	} else if (p->buf[p->pos] == BREAK_CHAR) { // If we have a break
		type = TYPE_SEP;
		// Read until there are no spaces left or end of string
		while (p->pos < p->len && p->buf[p->pos] == BREAK_CHAR)
			++p->pos;
	} else { // Otherwise it's part of our token
		type = TYPE_TAG;
		// Read until we hit a break or end of string
		while (p->pos < p->len && p->buf[p->pos] != BREAK_CHAR)
			++p->pos;
	}
	// Set our string length
	*tlen = p->pos - spos;
	// Return the type of lexeme
ret:	PG_RETURN_INT32(type);
}

// This is called when we have finished parsing, clean up
// resources here
// Args: void *user_data
Datum tagpsr_end(PG_FUNCTION_ARGS) {
	parse_state_t *p = (parse_state_t*)PG_GETARG_POINTER(0);
	pfree(p);
	PG_RETURN_VOID();
}

// This is called to get a table information about the word types
// Ret: LexDescr*
Datum tagpsr_lextype(PG_FUNCTION_ARGS) {
	// Two different lexeme types and a null terminator
	LexDescr *descr = palloc(sizeof(LexDescr) * (2 + 1));
	descr[0] = (LexDescr){TYPE_TAG, pstrdup("tag"),   pstrdup("Tag")          };
	descr[1] = (LexDescr){TYPE_SEP, pstrdup("break"), pstrdup("Tag separator")};
	descr[2] = (LexDescr){0};

	PG_RETURN_POINTER(descr);
}
