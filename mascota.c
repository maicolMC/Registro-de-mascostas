#include "mascota.h"

#include <stdio.h>
#include <string.h>

int obtenerSiguienteId(const Mascota mascotas[], int cantidad) {
    int mayorId = 0;
    int i;

    for (i = 0; i < cantidad; i++) {
        if (mascotas[i].id > mayorId) {
            mayorId = mascotas[i].id;
        }
    }

    return mayorId + 1;
}

int buscarPorDueno(const Mascota mascotas[], int cantidad, const char *nombreDueno,
                   int indices[], int capacidadIndices) {
    int encontradas = 0;
    int i;

    if (mascotas == NULL || nombreDueno == NULL || indices == NULL ||
        cantidad < 0 || capacidadIndices <= 0) {
        return 0;
    }

    for (i = 0; i < cantidad; i++) {
        if (strcmp(mascotas[i].nombreDueno, nombreDueno) == 0) {
            if (encontradas < capacidadIndices) {
                indices[encontradas] = i;
            }
            encontradas++;
        }
    }

    return encontradas;
}

void listarMascotas(const Mascota mascotas[], int cantidad) {
    int i;

    if (cantidad == 0) {
        printf("No hay mascotas registradas.\n");
        return;
    }

    printf("\n%-5s %-20s %-15s %-20s %-6s %s\n",
           "ID", "Nombre", "Especie", "Raza", "Edad", "Dueno");
    printf("--------------------------------------------------------------------------\n");

    for (i = 0; i < cantidad; i++) {
        printf("%-5d %-20s %-15s %-20s %-6d %s\n",
               mascotas[i].id,
               mascotas[i].nombre,
               mascotas[i].especie,
               mascotas[i].raza,
               mascotas[i].edad,
               mascotas[i].nombreDueno);
    }
}

int eliminarPorId(Mascota mascotas[], int *cantidad, int id) {
    int i;

    if (mascotas == NULL || cantidad == NULL || *cantidad < 0) {
        return 0;
    }

    for (i = 0; i < *cantidad; i++) {
        if (mascotas[i].id == id) {
            int j;
            for (j = i; j < *cantidad - 1; j++) {
                mascotas[j] = mascotas[j + 1];
            }
            (*cantidad)--;
            return 1;
        }
    }

    return 0;
}
