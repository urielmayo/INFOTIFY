/**
 * \file            files.h
 * \brief           funciones de archivos
 * \author          Grupo1
 * \date            Sep 3, 2019
 * \details         Usar makefile para compilar y linkear (build)
 */
#include <define.h>
#ifndef INFO_H_
#define INFO_H_

int abrir_archivoesc(char* nombre_archivo,FILE** archivo);
int abrir_archivolec(char* nombre_archivo,FILE** archivo);
int leer_linea(char* linea,FILE* ptr_archivo);
int escribir_linea(FILE* fp, const char* linea);
void ir_inicio_archivo(FILE* ptr_archivo);
void cerrar_archivo(FILE* ptr_archivo);

#endif