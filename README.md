# Sistema de Registro de Mascotas

Base inicial para el proyecto de Algoritmos y Estructuras de Datos. El programa está escrito en C y se puede ampliar por módulos.

## Funciones incluidas

- Registrar mascotas con ID, nombre, especie, raza, edad y nombre del dueño.
- Buscar coincidencias por nombre del dueño.
- Listar y eliminar mascotas por ID.
- Cargar los registros al iniciar y guardarlos en `mascotas.txt` después de cada cambio.
- Validar entradas vacías, edades entre 0 y 100, opciones del menú y capacidad del arreglo.
- Rechazar el carácter `|` en los campos de texto, porque separa los datos del archivo.

Los registros se mantienen en un arreglo de estructuras con capacidad máxima de 200 mascotas. El archivo de texto guarda un registro por línea con el formato `id|nombre|especie|raza|edad|dueño`. Las líneas mal formadas se omiten al cargar y se informa cuántas fueron omitidas.

## Archivos

- `main.c`: menú, lectura y validación de entradas, y coordinación de operaciones.
- `mascota.h` / `mascota.c`: estructura y funciones para buscar, listar y eliminar registros.
- `archivo.h` / `archivo.c`: lectura y escritura del archivo de datos.

## Compilar y ejecutar

Con GCC o MinGW:

```sh
gcc -std=c11 -Wall -Wextra -pedantic main.c mascota.c archivo.c -o registro_mascotas
```

En Windows, ejecutar `registro_mascotas.exe`; en Linux o macOS, ejecutar `./registro_mascotas`.

El archivo `mascotas.txt` se crea en el directorio desde el que se ejecuta el programa. Si no existe al iniciar, el programa comienza con una lista vacía.

## Ideas para ampliar

- Añadir edición de datos y filtros por especie.
- Separar la captura de entradas en un módulo independiente.
- Permitir más de 200 registros usando memoria dinámica.
- Añadir pruebas automatizadas y casos de prueba documentados.

## Estado del proyecto

Esta es una base inicial. Antes de la entrega, el equipo debe probar los flujos, preparar capturas o simulaciones y completar el documento explicativo solicitado por la asignatura.
