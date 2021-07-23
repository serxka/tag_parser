CREATE EXTENSION tag_parser;
SELECT alias,token,lexemes FROM ts_debug('tag_parser', 'red,red_hair,red-hair,red hair,red_hairs');

