#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ini.h"

static char *ini_strdup(const char *src)
{
    size_t len;
    char *copy;

    if (src == NULL)
    {
        return NULL;
    }

    len = strlen(src);
    copy = (char *)malloc(len + 1);
    if (copy == NULL)
    {
        return NULL;
    }

    memcpy(copy, src, len + 1);
    return copy;
}

static char *trim_left(char *text)
{
    while (*text != '\0' && isspace((unsigned char)*text))
    {
        text++;
    }

    return text;
}

static void trim_right(char *text)
{
    size_t len = strlen(text);

    while (len > 0 && isspace((unsigned char)text[len - 1]))
    {
        text[len - 1] = '\0';
        len--;
    }
}

static char *trim(char *text)
{
    char *trimmed = trim_left(text);

    trim_right(trimmed);
    return trimmed;
}

static void strip_inline_comment(char *text)
{
    int in_whitespace = 1;

    while (*text != '\0')
    {
        if ((*text == ';' || *text == '#') && in_whitespace)
        {
            *text = '\0';
            return;
        }

        in_whitespace = isspace((unsigned char)*text);
        text++;
    }
}

static ini_section_t *find_section(const ini_parser_t *parser, const char *name)
{
    ini_section_t *section = parser->sections;

    while (section != NULL)
    {
        if (strcmp(section->name, name) == 0)
        {
            return section;
        }
        section = section->next;
    }

    return NULL;
}

static ini_key_t *find_key(const ini_section_t *section, const char *name)
{
    ini_key_t *key = section->head;

    while (key != NULL)
    {
        if (strcmp(key->name, name) == 0)
        {
            return key;
        }
        key = key->next;
    }

    return NULL;
}

static int append_section(ini_parser_t *parser, const char *name, ini_section_t **section_out)
{
    ini_section_t *section = (ini_section_t *)calloc(1, sizeof(ini_section_t));
    ini_section_t *tail;

    if (section == NULL)
    {
        return -1;
    }

    section->name = ini_strdup(name);
    if (section->name == NULL)
    {
        free(section);
        return -1;
    }

    if (parser->sections == NULL)
    {
        parser->sections = section;
    }
    else
    {
        tail = parser->sections;
        while (tail->next != NULL)
        {
            tail = tail->next;
        }
        tail->next = section;
    }

    *section_out = section;
    return 0;
}

static int set_key_value(ini_section_t *section, const char *name, const char *value)
{
    ini_key_t *key = find_key(section, name);
    char *value_copy;

    if (key != NULL)
    {
        value_copy = ini_strdup(value);
        if (value_copy == NULL)
        {
            return -1;
        }
        free(key->value);
        key->value = value_copy;
        return 0;
    }

    key = (ini_key_t *)calloc(1, sizeof(ini_key_t));
    if (key == NULL)
    {
        return -1;
    }

    key->name = ini_strdup(name);
    key->value = ini_strdup(value);
    if (key->name == NULL || key->value == NULL)
    {
        free(key->name);
        free(key->value);
        free(key);
        return -1;
    }

    if (section->head == NULL)
    {
        section->head = key;
    }
    else
    {
        ini_key_t *tail = section->head;
        while (tail->next != NULL)
        {
            tail = tail->next;
        }
        tail->next = key;
    }

    return 0;
}

int ini_parser_load(ini_parser_t *parser, const char *path)
{
    FILE *file;
    char line[1024];
    ini_section_t *current_section = NULL;

    if (parser == NULL || path == NULL)
    {
        return -1;
    }

    ini_parser_free(parser);

    file = fopen(path, "r");
    if (file == NULL)
    {
        return -1;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        char *content = trim(line);
        char *equals;

        strip_inline_comment(content);
        content = trim(content);
        if (*content == '\0')
        {
            continue;
        }

        if (*content == '[')
        {
            char *end = strchr(content, ']');

            if (end == NULL)
            {
                fclose(file);
                ini_parser_free(parser);
                return -1;
            }

            *end = '\0';
            content = trim(content + 1);
            if (*content == '\0')
            {
                fclose(file);
                ini_parser_free(parser);
                return -1;
            }

            current_section = find_section(parser, content);
            if (current_section == NULL && append_section(parser, content, &current_section) != 0)
            {
                fclose(file);
                ini_parser_free(parser);
                return -1;
            }
            continue;
        }

        equals = strchr(content, '=');
        if (equals == NULL || current_section == NULL)
        {
            fclose(file);
            ini_parser_free(parser);
            return -1;
        }

        *equals = '\0';
        {
            char *key = trim(content);
            char *value = trim(equals + 1);

            if (*key == '\0' || set_key_value(current_section, key, value) != 0)
            {
                fclose(file);
                ini_parser_free(parser);
                return -1;
            }
        }
    }

    fclose(file);
    return 0;
}

const char *ini_parser_get(const ini_parser_t *parser, const char *section, const char *key)
{
    ini_section_t *target_section;
    ini_key_t *target_key;

    if (parser == NULL || section == NULL || key == NULL)
    {
        return NULL;
    }

    target_section = find_section(parser, section);
    if (target_section == NULL)
    {
        return NULL;
    }

    target_key = find_key(target_section, key);
    if (target_key == NULL)
    {
        return NULL;
    }

    return target_key->value;
}

void ini_parser_free(ini_parser_t *parser)
{
    ini_section_t *section;

    if (parser == NULL)
    {
        return;
    }

    section = parser->sections;
    while (section != NULL)
    {
        ini_section_t *next_section = section->next;
        ini_key_t *key = section->head;

        while (key != NULL)
        {
            ini_key_t *next_key = key->next;
            free(key->name);
            free(key->value);
            free(key);
            key = next_key;
        }

        free(section->name);
        free(section);
        section = next_section;
    }

    parser->sections = NULL;
}
