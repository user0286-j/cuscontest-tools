# Checkers

Modo de compilación:

`g++ -g -O1 -Wall -fstack-protector -D_FORTIFY_SOURCE=2 -fPIE -Wformat -Wformat-security -pedantic  -fPIE -Wl,-z,relro -Wl,-z,now -std=c++17 compare.cc -o run`

Los archivos necesarios para que funcione

- **judge_in**: Es el input principal del programa
- **judge_ans**: Es la respuesta generado por el código solución
- **feedback_dir/**: Carpeta necesaria donde verá los issues
- **user_out**: La salida generado por el concursante.

La forma de compilación es la siguiente

`./run judge_in2 judge_ans2 feedback_dir/ < user_out2`

Para la exportación para domjude debes comprimir en formato `.zip` los siguientes archivos:

- **build**: Necesario para ordenar la compilación.
- **compare.cc**: Archivo principal para la ejecución del checkers.
- **checker.h**: El encabezado donde contiene los datos importantes para el correcto funcionamiento del checkers

