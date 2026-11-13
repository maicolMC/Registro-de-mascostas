#ifndef ARCHIVO_H
#define ARCHIVO_H

#include "mascota.h"

/* Devuelve 1 si pudo cargar (tambien si el archivo aun no existe), 0 ante error. */
int cargarMascotas(const char *ruta, Mascota mascotas[], int capacidad,
                   int *cantidad, int *lineasOmitidas);

/* Devuelve 1 si todos los registros se escribieron correctamente. */
int guardarMascotas(const char *ruta, const Mascota mascotas[], int cantidad);

#endif
