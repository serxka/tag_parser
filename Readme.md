# tag_parser
A simple full text search parser for PostgreSQL, written in C. Is much simpler than the default tsearch parser, it will only split into lexeme's at a comma boundary. This can easily be changed to any other character in the source code by changing the definition of `BREAK_CHAR`

## Installation
Obviously PostgreSQL should be installed, `pg_config` will also be needed, along with basic GNU build environment (gmake, cc, etc..)
```sh
# Make sure these are run from an account with both a postgresql account and permissions to it's files
# This will build and install into the approiate directory for PostgreSQL to load
$ make install
# This do the opposite and remove and the installed version from the system
$ make uninstall
# This will do the same as install but also run the tests
$ make installcheck
```

### Contribution
This was just made as a small thing to use full text searching for a tag system in [watame](http://github.com/serxka/watame), if you have any improvements or additions please feel free to create a PR.

### Resources
- PostgreSQL Documentation
- PostgreSQL source: `postgres/src/backend/tsearch`
- [This Blog post](http://big-elephants.com/2015-10/writing-postgres-extensions-part-i)
