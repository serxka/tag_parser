EXTENSION = tag_parser
DATA = tag_parser--0.1.0.sql

MODULES = tag_parser
REGRESS = tag_parser

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
