# Create-problem

Este herramienta sirve principalmente para crear el .tex que es necesario para el posterior pdf de los problemas, además de crear los .ans y mostrar si se encontró algún error. 

## Archivos que debe tener:

- **content.tex:** Archivo que contiene la descripción del problema.
- **input.tex** Archivo que contiene cómo es el input del problema.
- **output.tex** Archivo que contiene cómo es el output del problema.
- **solution** Archivo que contiene el código solución del problema, por el momento solo está habilitado para python y c++.
- **test** Archivo donde está los casos de prueba..
    - **public**: Archivo que contiene los casos de prueba.
    - **private**: Archivo que contiene los casos privado.

El código de ejecución sería el siguiente:

`g++ solution.cc -o run; ./run <nombre_del_archivo>`