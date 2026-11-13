#include "archivo.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_LINEA_ARCHIVO 256

static int convertirEntero(const char *texto, int *valor) {
    char *fin;
    long numero;

    errno = 0;
    numero = strtol(texto, &fin, 10);
    if (texto == fin || *fin != '\0' || errno == ERANGE ||
        numero < 0 || numero > 2147483647L) {
        return 0;
    }

    *valor = (int)numero;
    return 1;
}

static int convertirLinea(char *linea, Mascota *mascota) {
    char *campos[6];
    char *token;
    int total = 0;

    token = strtok(linea, "|\r\n");
    while (token != NULL && total < 6) {
        campos[total++] = token;
        token = strtok(NULL, "|\r\n");
    }

    if (total != 6 || token != NULL ||
        !convertirEntero(campos[0], &mascota->id) || mascota->id <= 0 ||
        !convertirEntero(campos[4], &mascota->edad) || mascota->edad > 100 ||
        strlen(campos[1]) >= TAM_NOMBRE || strlen(campos[2]) >= TAM_ESPECIE ||
        strlen(campos[3]) >= TAM_RAZA || strlen(campos[5]) >= TAM_DUENO ||
        campos[1][0] == '\0' || campos[2][0] == '\0' ||
        campos[3][0] == '\0' || campos[5][0] == '\0') {
        return 0;
    }

    strcpy(mascota->nombre, campos[1]);
    strcpy(mascota->especie, campos[2]);
    strcpy(mascota->raza, campos[3]);
    strcpy(mascota->nombreDueno, campos[5]);
    return 1;
}

int cargarMascotas(const char *ruta, Mascota mascotas[], int capacidad,
                   int *cantidad, int *lineasOmitidas) {
    FILE *archivo;
    char linea[TAM_LINEA_ARCHIVO];
    int cargadas = 0;
    int omitidas = 0;

    if (ruta == NULL || mascotas == NULL || cantidad == NULL ||
        capacidad <= 0 || lineasOmitidas == NULL) {
        return 0;
    }

    *cantidad = 0;
    *lineasOmitidas = 0;
    archivo = fopen(ruta, "r");
    if (archivo == NULL) {
        return errno == ENOENT;
    }

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        Mascota temporal;
        size_t longitud = strlen(linea);

        if (longitud > 0 && linea[longitud - 1] != '\n' && !feof(archivo)) {
            int caracter;
            while ((caracter = fgetc(archivo)) != '\n' && caracter != EOF) {
                /* Descarta el resto de una linea demasiado larga. */
            }
            omitidas++;
            continue;
        }

        if (convertirLinea(linea, &temporal) && cargadas < capacidad) {
            mascotas[cargadas++] = temporal;
        } else {
            omitidas++;
        }
    }

    if (ferror(archivo)) {
        fclose(archivo);
        return 0;
    }
    if (fclose(archivo) != 0) {
        return 0;
    }

    *cantidad = cargadas;
    *lineasOmitidas = omitidas;
    return 1;
}

int guardarMascotas(const char *ruta, const Mascota mascotas[], int cantidad) {
    FILE *archivo;
    int i;
    int correcto = 1;

    if (ruta == NULL || mascotas == NULL || cantidad < 0) {
        return 0;
    }

    archivo = fopen(ruta, "w");
    if (archivo == NULL) {
        return 0;
    }

    for (i = 0; i < cantidad; i++) {
        if (fprintf(archivo, "%d|%s|%s|%s|%d|%s\n",
                    mascotas[i].id, mascotas[i].nombre, mascotas[i].especie,
                    mascotas[i].raza, mascotas[i].edad,
                    mascotas[i].nombreDueno) < 0) {
            correcto = 0;
            break;
        }
    }

    if (fclose(archivo) != 0) {
        correcto = 0;
    }
    return correcto;
}
