#ifndef INCLUDE_INI_H_
#define INCLUDE_INI_H_

typedef struct ini_key ini_key_t;
typedef struct ini_section ini_section_t;
typedef struct ini_parser ini_parser_t;

struct ini_key
{
    char *name;
    char *value;
    ini_key_t *next;
};

struct ini_section
{
    char *name;
    ini_key_t *head;
    ini_section_t *next;
};

struct ini_parser
{
    ini_section_t *sections;
};

int ini_parser_load(ini_parser_t *parser, const char *path);
const char *ini_parser_get(const ini_parser_t *parser, const char *section, const char *key);
void ini_parser_free(ini_parser_t *parser);

#endif // INCLUDE_INI_PARSER_H_
