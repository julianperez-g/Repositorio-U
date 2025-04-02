#!/usr/bin/perl

#**************************************************************
#                       Pontificia Universidad Javeriana
#       Autor: Julián Pérez
#     Materia: Sistemas Operativos
#     Fichero: Automatización para ejecuciones por lotes
#     Descripción: Script para ejecutar un programa en diferentes 
#                  configuraciones de tamaño de matriz y número de hilos
#**************************************************************/

# Obtener el directorio actual de trabajo y almacenarlo en $Path
$Path = `pwd`;  # `pwd` ejecuta el comando en la terminal y obtiene el directorio actual
chomp($Path);   # Elimina el salto de línea al final de la cadena

# Imprimir el directorio actual
printf($Path);

# Lista con los nombres de los ejecutables a probar
@Nombre_Ejecutable = ("mm_clasico");

# Lista con los diferentes tamaños de matrices a usar en la ejecución
@Size_Matriz = (500, 1000);

# Lista con los diferentes números de hilos a utilizar en la ejecución
@Num_Hilos = (1, 2);

# Número de veces que se ejecutará cada configuración
$Repeticiones = 30;

# Iterar sobre cada ejecutable en la lista
foreach $Nombre_Ejecutable (@Nombre_Ejecutable) {
    
    # Iterar sobre cada tamaño de matriz en la lista
    foreach $size (@Size_Matriz) {
        
        # Iterar sobre cada número de hilos en la lista
        foreach $hilo (@Num_Hilos) {

            # Construir el nombre del archivo de salida donde se guardarán los resultados
            $file = "$Path/$Nombre_Ejecutable-".$size."-Hilos-".$hilo.".dat";

            # Ejecutar el programa $Repeticiones veces con la configuración actual
            for ($i = 0; $i < $Repeticiones; $i++) {

                # Ejecutar el programa y redirigir la salida al archivo correspondiente
                system("$Path/$Nombre_Ejecutable $size $hilo  >> $file");

                # Imprimir la línea de ejecución en consola (descomentar si se quiere visualizar)
                # printf("$Path/$Nombre_Ejecutable $size $hilo \n");
            }

            # Cerrar el archivo (aunque no es necesario aquí, pues `system` gestiona la salida)
            close($file);

            
            $p = $p + 1;
        }
    }
}
