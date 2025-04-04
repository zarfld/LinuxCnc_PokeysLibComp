/**
 * @file
 *
 */
#include "pokeys_ini.h"
#include <stdlib.h>
#include <string.h>

static char pokeys_ini_path[MAX_PATH_LENGTH] = { 0 };
extern uint32_t device_id;
extern bool ApplyIniSettings;

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

    rtapi_print_msg(RTAPI_MSG_ERR, "ini_read_int: %s %s (%s) default_value: %i\n", section, key, pokeys_ini_path,default_value);
    FILE *fp = fopen(pokeys_ini_path, "r");
    if (!fp) {
        rtapi_print_msg(RTAPI_MSG_ERR, "ini_read_int: FAILED to open ini file\n");
        return default_value;
    }

    rtapi_print_msg(RTAPI_MSG_ERR, "ini_read_int: %s %s  (%s)\n", section, key, pokeys_ini_path);
    char line[256];
    int in_section = 0;
    while (fgets(line, sizeof(line), fp)) {
        rtapi_print_msg(RTAPI_MSG_ERR, "ini_read_int: line='%s'\n", line);
        if (line[0] == '[') {
            char current_section[64] = "";
            sscanf(line, "[%63[^]]", current_section);  // robustere Erkennung
            in_section = (strcmp(current_section, section) == 0);
            rtapi_print_msg(RTAPI_MSG_ERR, "ini_read_int: found section [%s] → %s\n",
                            current_section, in_section ? "MATCH" : "no match");
        }
        if (in_section && strstr(line, key) && strchr(line, '=')) {
            fclose(fp);
            return atoi(strchr(line, '=') + 1);
        }
    }
    fclose(fp);
    return default_value;
}

// **3. INI-Datei nach Float-Wert durchsuchen**
float ini_read_float(const char *section, const char *key, float default_value) {
    FILE *fp = fopen(pokeys_ini_path, "r");
    if (!fp)
        return default_value;

    char line[256];
    int in_section = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '[') {
            in_section = (strncmp(line, section, strlen(section)) == 0);
        }
        if (in_section && strstr(line, key) && strchr(line, '=')) {
            fclose(fp);
            return atof(strchr(line, '=') + 1);
        }
    }
    fclose(fp);
    return default_value;
}

// **4. INI-Datei nach String durchsuchen**
void ini_read_string(const char *section, const char *key, char *buffer, size_t size, const char *default_value) {
    FILE *fp = fopen(pokeys_ini_path, "r");
    if (!fp) {
        strncpy(buffer, default_value, size - 1);
        buffer[size - 1] = '\0'; // Ensure null-termination
        return;
    }

    char line[256];
    int in_section = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '[') {
            in_section = (strncmp(line, section, strlen(section)) == 0);
        }
        if (in_section && strstr(line, key) && strchr(line, '=')) {
            strncpy(buffer, strchr(line, '=') + 1, size - 1);
            buffer[size - 1] = '\0';             // Ensure null-termination
            buffer[strcspn(buffer, "\r\n")] = 0; // Entferne Newline-Zeichen
            fclose(fp);
            return;
        }
    }
    fclose(fp);
    strncpy(buffer, default_value, size - 1);
    buffer[size - 1] = '\0'; // Ensure null-termination
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
