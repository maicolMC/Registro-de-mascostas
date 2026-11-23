#include "archivo.h"
#include "mascota.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARCHIVO_DATOS "mascotas.txt"
#define TAM_ENTRADA 256

/* Lee una linea completa; devuelve 0 al llegar al fin de la entrada. */
static int leerLinea(const char *mensaje, char *destino, size_t capacidad) {
    size_t longitud;

    printf("%s", mensaje);
    if (fgets(destino, (int)capacidad, stdin) == NULL) {
        return 0;
    }

    longitud = strlen(destino);
    if (longitud > 0 && destino[longitud - 1] == '\n') {
        destino[--longitud] = '\0';
    } else if (!feof(stdin)) {
        int caracter;
        while ((caracter = getchar()) != '\n' && caracter != EOF) {
            /* Descarta el exceso para que no se convierta en otra respuesta. */
        }
        printf("La entrada es demasiado larga. Intenta de nuevo.\n");
        return -1;
    }

    if (longitud > 0 && destino[longitud - 1] == '\r') {
        destino[--longitud] = '\0';
    }

    return 1;
}

static void quitarEspacios(char *texto) {
    char *inicio = texto;
    size_t longitud;

    while (isspace((unsigned char)*inicio)) {
        inicio++;
    }
    if (inicio != texto) {
        memmove(texto, inicio, strlen(inicio) + 1);
    }

    longitud = strlen(texto);
    while (longitud > 0 && isspace((unsigned char)texto[longitud - 1])) {
        texto[--longitud] = '\0';
    }
}

static int leerTextoObligatorio(const char *mensaje, char *destino, size_t capacidad) {
    int resultado;

    do {
        resultado = leerLinea(mensaje, destino, capacidad);
        if (resultado == 0) {
            return 0;
        }
        if (resultado < 0) {
            continue;
        }

        quitarEspacios(destino);
        if (destino[0] == '\0') {
            printf("El campo no puede quedar vacio.\n");
        } else if (strlen(destino) < 3) {
            printf("El campo debe tener al menos 3 caracteres.\n");
        } else if (strchr(destino, '|') != NULL) {
            printf("No se permite el caracter | porque separa los campos del archivo.\n");
        } 
        else {
            ///* Rechazar si el texto solo tiene numeros*///
            int soloNumeros = 1;
            size_t i;

            for (i = 0; destino[i] != '\0'; i++) {
                if (!isdigit((unsigned char)destino[i]) && !isspace((unsigned char)destino[i])) {
                    soloNumeros = 0;
                    break;
                }       
            }
            if (soloNumeros) {
                printf("El campo no puede ser solo numeros.\n");
            } else {
                return 1;
            }
        }
    } while (1);
}

static int leerEnteroRango(const char *mensaje, int minimo, int maximo, int *valor) {
    char entrada[TAM_ENTRADA];
    char *inicioNumero;
    char *fin;
    long numero;
    int resultado;

    while (1) {
        resultado = leerLinea(mensaje, entrada, sizeof(entrada));
        if (resultado == 0) {
            return 0;
        }
        if (resultado < 0) {
            continue;
        }

        errno = 0;
        numero = strtol(entrada, &fin, 10);
        inicioNumero = fin;
        while (isspace((unsigned char)*fin)) {
            fin++;
        }
        if (entrada == inicioNumero || *fin != '\0' || errno == ERANGE ||
            numero < minimo || numero > maximo) {
            printf("Ingresa un numero entero entre %d y %d.\n", minimo, maximo);
            continue;
        }

        *valor = (int)numero;
        return 1;
    }
}

static int capturarMascota(Mascota *mascota, int id) {
    mascota->id = id;
    printf("\nRegistro de mascota (ID asignado: %d)\n", id);

    if (!leerTextoObligatorio("Nombre de la mascota: ", mascota->nombre, TAM_NOMBRE) ||
        !leerTextoObligatorio("Especie: ", mascota->especie, TAM_ESPECIE) ||
        !leerTextoObligatorio("Raza: ", mascota->raza, TAM_RAZA) ||
        !leerEnteroRango("Edad (0-100): ", 0, 100, &mascota->edad) ||
        !leerTextoObligatorio("Nombre del dueno: ", mascota->nombreDueno, TAM_DUENO)) {
        return 0;
    }

    return 1;
}

static void mostrarMenu(void) {
    printf("\n=== Sistema de Registro de Mascotas ===\n");
    printf("1. Registrar mascota\n");
    printf("2. Buscar por nombre del dueno\n");
    printf("3. Listar mascotas\n");
    printf("4. Eliminar mascota por ID\n");
    printf("0. Guardar y salir\n");
}

int main(void) {
    Mascota mascotas[MAX_MASCOTAS];
    int cantidad = 0;
    int omitidas = 0;
    int opcion;

    if (!cargarMascotas(ARCHIVO_DATOS, mascotas, MAX_MASCOTAS,
                        &cantidad, &omitidas)) {
        fprintf(stderr, "No se pudo leer %s.\n", ARCHIVO_DATOS);
        return EXIT_FAILURE;
    }
    printf("Se cargaron %d mascota(s) desde %s.\n", cantidad, ARCHIVO_DATOS);
    if (omitidas > 0) {
        printf("Aviso: se omitieron %d linea(s) invalidas o fuera de capacidad.\n", omitidas);
    }

    while (1) {
        mostrarMenu();
        if (!leerEnteroRango("Selecciona una opcion: ", 0, 4, &opcion)) {
            printf("\nFin de entrada. Cerrando el programa.\n");
            break;
        }

        if (opcion == 0) {
            break;
        } else if (opcion == 1) {
            Mascota nueva;
            if (cantidad >= MAX_MASCOTAS) {
                printf("Se alcanzo el limite de %d mascotas.\n", MAX_MASCOTAS);
                continue;
            }
            if (!capturarMascota(&nueva, obtenerSiguienteId(mascotas, cantidad))) {
                printf("No se completo el registro.\n");
                continue;
            }
            mascotas[cantidad++] = nueva;
            if (!guardarMascotas(ARCHIVO_DATOS, mascotas, cantidad)) {
                cantidad--;
                printf("No se pudo guardar el registro; el cambio fue cancelado.\n");
            } else {
                printf("Mascota registrada y guardada.\n");
            }
        } else if (opcion == 2) {
            char dueno[TAM_DUENO];
            int indices[MAX_MASCOTAS];
            int encontradas;
            int i;

            if (!leerTextoObligatorio("Nombre del dueno a buscar: ", dueno, sizeof(dueno))) {
                printf("Busqueda cancelada.\n");
                continue;
            }
            encontradas = buscarPorDueno(mascotas, cantidad, dueno,
                                         indices, MAX_MASCOTAS);
            if (encontradas == 0) {
                printf("No se encontraron mascotas para ese dueno.\n");
            } else {
                printf("Se encontraron %d mascota(s):\n", encontradas);
                for (i = 0; i < encontradas && i < MAX_MASCOTAS; i++) {
                    listarMascotas(&mascotas[indices[i]], 1);
                }
            }
        } else if (opcion == 3) {
            listarMascotas(mascotas, cantidad);
        } else if (opcion == 4) {
            int id;
            Mascota respaldo[MAX_MASCOTAS];
            int cantidadAnterior = cantidad;

            if (!leerEnteroRango("ID de la mascota a eliminar: ", 1, 2147483647, &id)) {
                printf("Eliminacion cancelada.\n");
                continue;
            }
            memcpy(respaldo, mascotas, (size_t)cantidad * sizeof(Mascota));
            if (!eliminarPorId(mascotas, &cantidad, id)) {
                printf("No existe una mascota con ese ID.\n");
                continue;
            }
            if (!guardarMascotas(ARCHIVO_DATOS, mascotas, cantidad)) {
                memcpy(mascotas, respaldo, (size_t)cantidadAnterior * sizeof(Mascota));
                cantidad = cantidadAnterior;
                printf("No se pudo guardar el cambio; la eliminacion fue cancelada.\n");
            } else {
                printf("Mascota eliminada y archivo actualizado.\n");
            }
        }
    }

    if (!guardarMascotas(ARCHIVO_DATOS, mascotas, cantidad)) {
        fprintf(stderr, "No se pudieron guardar los datos al salir.\n");
        return EXIT_FAILURE;
    }

    printf("Datos guardados. Hasta pronto.\n");
    return EXIT_SUCCESS;
}
