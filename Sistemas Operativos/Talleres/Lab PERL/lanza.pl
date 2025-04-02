#!/usr/bin/perl

#**************************************************************
#                       Pontificia Universidad Javeriana
#       Autor: Julián Pérez
#     Materia: Sistemas Operativos
#     Fichero: Automatización para ejecuciones por lotes
#         Descripcion: Fichero para la ejecución de ejecutables
#****************************************************************/

$Path = `pwd`;
chomp($Path);

printf($Path);

@Nombre_Ejecutable = ("mm_clasico");

@Size_Matriz = (500,1000);

@Num_Hilos = (1,2);

$Repeticiones = 30;

foreach $Nombre_Ejecutable (@Nombre_Ejecutable) {
    foreach $size (@Size_Matriz) {
        foreach $hilo (@Num_Hilos) {

            $file = "$Path/$Nombre_Ejecutable-".$size."-Hilos-".$hilo.".dat";

            for ($i = 0; $i < $Repeticiones; $i++) {

          	 system("$Path/$Nombre_Ejecutable $size $hilo  >> $file");

                #printf("$Path/$Nombre_Ejecutable $size $hilo \n");
            }

            close($file);

            $p = $p + 1;
        }
    }
}
