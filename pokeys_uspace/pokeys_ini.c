/**
 * @file
 *
 */
#include "pokeys_ini.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char pokeys_ini_path[MAX_PATH_LENGTH] = { 0 };
extern uint32_t device_id;
extern bool ApplyIniSettings;

void tolower_str(char *dst, const char *src, size_t size) {
    size_t i;
    for (i = 0; i < size - 1 && src[i]; ++i)
        dst[i] = tolower((unsigned char)src[i]);
    dst[i] = '\0';
}

// **1. Pfad der INI-Datei ermitteln**
void get_pokeys_ini_path(int device_id, char *buffer, size_t size) {
    const char *ini_path = getenv("INI_FILE_NAME");
    if (!ini_path) {
        fprintf(stderr, "Fehler: INI_FILE_NAME nicht gesetzt.\n");
        return;
    }

    // Verzeichnis extrahieren
    strncpy(buffer, ini_path, size - 1);
    buffer[size - 1] = '\0'; // Ensure null-termination
    char *last_slash = strrchr(buffer, '/');
    if (last_slash) {
        *last_slash = '\0'; // Trenne den Dateinamen ab
    }

    // Endgültigen INI-Dateinamen setzen
    snprintf(buffer, size, "%s/pokeys_%d.ini", buffer, device_id);
}

void set_pokeys_ini_path(const char *path) {
    rtapi_print_msg(RTAPI_MSG_ERR, "set_pokeys_ini_path: %s\n", path);
    strncpy(pokeys_ini_path, path, sizeof(pokeys_ini_path) - 1);
    pokeys_ini_path[sizeof(pokeys_ini_path) - 1] = '\0'; // Ensure null-termination
}

// **2. INI-Datei nach Integer-Wert durchsuchen**
int ini_read_int(const char *section, const char *key, int default_value) {
    rtapi_print_msg(RTAPI_MSG_DBG, "ini_read_int: section='%s' key='%s' ini='%s' default_value=%i\n", section ? section : "NULL", key ? key : "NULL", pokeys_ini_path ? pokeys_ini_path : "NULL", default_value);

    FILE *fp = fopen(pokeys_ini_path, "r");
    if (!fp) {
        rtapi_print_msg(RTAPI_MSG_ERR, "ini_read_int: FAILED to open ini file '%s'\n", pokeys_ini_path);
        return default_value;
    }

    char line[256];
    int in_section = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '[') {
            if (in_section) {
                rtapi_print_msg(RTAPI_MSG_DBG, "ini_read_int: left section [%s] without finding key '%s'\n", section, key);
                break;
            }

            char current_section[64] = "";
            sscanf(line, "[%63[^]]", current_section);
            in_section = (strcmp(current_section, section) == 0);

            rtapi_print_msg(RTAPI_MSG_DBG, "ini_read_int: found section [%s] → %s\n", current_section, in_section ? "MATCH" : "no match");
            continue;
        }

        if (in_section && strchr(line, '=')) {
            char file_key[64];
            char *equalsign = strchr(line, '=');

            if (sscanf(line, "%63[^=]", file_key) == 1) {
                // Whitespace entfernen
                char *end = file_key + strlen(file_key) - 1;
                while (end > file_key && isspace((unsigned char)*end))
                    *end-- = '\0';

                // Case-insensitive Vergleich
                char file_key_lower[64];
                char key_lower[64];
                tolower_str(file_key_lower, file_key, sizeof(file_key_lower));
                tolower_str(key_lower, key, sizeof(key_lower));

                if (strcmp(file_key_lower, key_lower) == 0) {
                    char *val_start = equalsign + 1;
                    while (isspace((unsigned char)*val_start))
                        val_start++;

                    int value = atoi(val_start);
                    rtapi_print_msg(RTAPI_MSG_ERR, "ini_read_int: FOUND key='%s', value=%d\n", key, value);
                    fclose(fp);
                    return value;
                }
            }
        }
    }

    fclose(fp);
    rtapi_print_msg(RTAPI_MSG_ERR, "ini_read_int: FAILED to find key '%s' in section [%s], returning default=%d\n", key, section, default_value);
    return default_value;
}

// **3. INI-Datei nach Float-Wert durchsuchen**
float ini_read_float(const char *section, const char *key, float default_value) {
    rtapi_print_msg(RTAPI_MSG_DBG, "ini_read_float: section='%s' key='%s' ini='%s' default_value=%f\n", section ? section : "NULL", key ? key : "NULL", pokeys_ini_path ? pokeys_ini_path : "NULL", default_value);

    FILE *fp = fopen(pokeys_ini_path, "r");
    if (!fp) {
        rtapi_print_msg(RTAPI_MSG_ERR, "ini_read_float: FAILED to open ini file '%s'\n", pokeys_ini_path);
        return default_value;
    }

    char line[256];
    int in_section = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '[') {
            if (in_section) {
                rtapi_print_msg(RTAPI_MSG_DBG, "ini_read_float: left section [%s] without finding key '%s'\n", section, key);
                break;
            }

            char current_section[64] = "";
            sscanf(line, "[%63[^]]", current_section);
            in_section = (strcmp(current_section, section) == 0);

            rtapi_print_msg(RTAPI_MSG_DBG, "ini_read_float: found section [%s] → %s\n", current_section, in_section ? "MATCH" : "no match");
            continue;
        }

        if (in_section && strchr(line, '=')) {
            char file_key[64];
            char *equalsign = strchr(line, '=');

            if (sscanf(line, "%63[^=]", file_key) == 1) {
                // Whitespace am Ende entfernen
                char *end = file_key + strlen(file_key) - 1;
                while (end > file_key && isspace((unsigned char)*end))
                    *end-- = '\0';

                // Case-insensitive Vergleich
                char file_key_lower[64];
                char key_lower[64];
                tolower_str(file_key_lower, file_key, sizeof(file_key_lower));
                tolower_str(key_lower, key, sizeof(key_lower));

                if (strcmp(file_key_lower, key_lower) == 0) {
                    char *val_start = equalsign + 1;
                    while (isspace((unsigned char)*val_start))
                        val_start++;

                    float value = (float)atof(val_start);
                    rtapi_print_msg(RTAPI_MSG_ERR, "ini_read_float: FOUND key='%s', value=%f\n", key, value);
                    fclose(fp);
                    return value;
                }
            }
        }
    }

    fclose(fp);
    rtapi_print_msg(RTAPI_MSG_ERR, "ini_read_float: FAILED to find key '%s' in section [%s], returning default=%f\n", key, section, default_value);
    return default_value;
}

// **4. INI-Datei nach String durchsuchen**
void ini_read_string(const char *section, const char *key, char *buffer, size_t size, const char *default_value) {
    rtapi_print_msg(RTAPI_MSG_DBG, "ini_read_string: section='%s' key='%s' ini='%s' default='%s'\n", section ? section : "NULL", key ? key : "NULL", pokeys_ini_path ? pokeys_ini_path : "NULL", default_value ? default_value : "NULL");

    FILE *fp = fopen(pokeys_ini_path, "r");
    if (!fp) {
        rtapi_print_msg(RTAPI_MSG_ERR, "ini_read_string: FAILED to open ini file '%s'\n", pokeys_ini_path);
        strncpy(buffer, default_value, size - 1);
        buffer[size - 1] = '\0';
        return;
    }

    char line[256];
    int in_section = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '[') {
            if (in_section) {
                rtapi_print_msg(RTAPI_MSG_DBG, "ini_read_string: left section [%s] without finding key '%s'\n", section, key);
                break;
            }

            char current_section[64] = "";
            sscanf(line, "[%63[^]]", current_section);
            in_section = (strcmp(current_section, section) == 0);

            rtapi_print_msg(RTAPI_MSG_DBG, "ini_read_string: found section [%s] → %s\n", current_section, in_section ? "MATCH" : "no match");
            continue;
        }

        if (in_section && strchr(line, '=')) {
            char file_key[64];
            char *equalsign = strchr(line, '=');

            if (sscanf(line, "%63[^=]", file_key) == 1) {
                // Whitespace rechts vom Key entfernen
                char *end = file_key + strlen(file_key) - 1;
                while (end > file_key && isspace((unsigned char)*end))
                    *end-- = '\0';

                // Case-insensitive Vergleich
                char file_key_lower[64], key_lower[64];
                tolower_str(file_key_lower, file_key, sizeof(file_key_lower));
                tolower_str(key_lower, key, sizeof(key_lower));

                if (strcmp(file_key_lower, key_lower) == 0) {
                    // Wert extrahieren (Whitespace nach '=' überspringen)
                    char *val_start = equalsign + 1;
                    while (isspace((unsigned char)*val_start))
                        val_start++;

                    strncpy(buffer, val_start, size - 1);
                    buffer[size - 1] = '\0';

                    // Zeilenumbrüche entfernen
                    buffer[strcspn(buffer, "\r\n")] = '\0';

                    rtapi_print_msg(RTAPI_MSG_ERR, "ini_read_string: FOUND key='%s', value='%s'\n", key, buffer);
                    fclose(fp);
                    return;
                }
            }
        }
    }

    fclose(fp);
    rtapi_print_msg(RTAPI_MSG_ERR, "ini_read_string: FAILED to find key '%s' in section [%s], returning default='%s'\n", key, section, default_value);
    strncpy(buffer, default_value, size - 1);
    buffer[size - 1] = '\0';
}

// **5. INI-Werte schreiben (ersetzen oder hinzufügen)**
void ini_write_int(const char *section, const char *key, int value) {
    FILE *fp = fopen(pokeys_ini_path, "a"); // Anhängen, wenn Datei existiert
    if (!fp)
        return;

    fprintf(fp, "[%s]\n%s=%d\n", section, key, value);
    fclose(fp);
}

void ini_write_float(const char *section, const char *key, float value) {
    FILE *fp = fopen(pokeys_ini_path, "a");
    if (!fp)
        return;

    fprintf(fp, "[%s]\n%s=%.2f\n", section, key, value);
    fclose(fp);
}

void ini_write_string(const char *section, const char *key, const char *value) {
    FILE *fp = fopen(pokeys_ini_path, "a");
    if (!fp)
        return;

    fprintf(fp, "[%s]\n%s=%s\n", section, key, value);
    fclose(fp);
}
