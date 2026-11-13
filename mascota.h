#ifndef MASCOTA_H
#define MASCOTA_H

#define MAX_MASCOTAS 200
#define TAM_NOMBRE 50
#define TAM_ESPECIE 30
#define TAM_RAZA 40
#define TAM_DUENO 60

typedef struct {
    int id;
    char nombre[TAM_NOMBRE];
    char especie[TAM_ESPECIE];
    char raza[TAM_RAZA];
    int edad;
    char nombreDueno[TAM_DUENO];
} Mascota;

int obtenerSiguienteId(const Mascota mascotas[], int cantidad);
int buscarPorDueno(const Mascota mascotas[], int cantidad, const char *nombreDueno,
                   int indices[], int capacidadIndices);
void listarMascotas(const Mascota mascotas[], int cantidad);
int eliminarPorId(Mascota mascotas[], int *cantidad, int id);

#endif
