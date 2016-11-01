MODULES = pg_themis
DATA = pg_themis--1.0.sql
EXTENSION = pg_themis
OBJS = pg_themis.o


MODULE_big = pg_themis
SHLIB_LINK = -lthemis

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
