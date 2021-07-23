-- Complain if script is sourced in psql, rather than by CREATE EXTENSION
\echo Use "CREATE EXTENSION tag_parser" to load this file \quit

-- Yoink out our phat functions
CREATE FUNCTION tagpsr_start(internal, int4)
RETURNS internal
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION tagpsr_gettoken(internal, internal, internal)
RETURNS internal
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION tagpsr_end(internal)
RETURNS void
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION tagpsr_lextype(internal)
RETURNS internal
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT IMMUTABLE;

-- Create a parser from these functions
CREATE TEXT SEARCH PARSER tag_parser (
	START = tagpsr_start,
	GETTOKEN = tagpsr_gettoken,
	END = tagpsr_end,
	LEXTYPES = tagpsr_lextype,
	HEADLINE = pg_catalog.prsd_headline
);
COMMENT ON TEXT SEARCH PARSER tag_parser IS 'Text Search Tag Parser';

-- Create a tsearch config for it
CREATE TEXT SEARCH CONFIGURATION tag_parser (PARSER = tag_parser);
ALTER TEXT SEARCH CONFIGURATION tag_parser ADD MAPPING FOR tag WITH simple;
COMMENT ON TEXT SEARCH CONFIGURATION tag_parser IS 'Search Configuration with Tag Parser';
