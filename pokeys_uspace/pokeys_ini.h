#ifndef POKEYS_INI_H
#define POKEYS_INI_H

#include <stdio.h>

#define MAX_PATH_LENGTH 512

// Funktion zur Initialisierung des INI-Dateipfads
void get_pokeys_ini_path(int device_id, char *buffer, size_t size);

// INI-Werte lesen
int ini_read_int(const char *section, const char *key, int default_value);
float ini_read_float(const char *section, const char *key, float default_value);
void ini_read_string(const char *section, const char *key, char *buffer,
                     size_t size, const char *default_value);

// INI-Werte schreiben
void ini_write_int(const char *section, const char *key, int value);
void ini_write_float(const char *section, const char *key, float value);
void ini_write_string(const char *section, const char *key, const char *value);

#endif // POKEYS_INI_H
