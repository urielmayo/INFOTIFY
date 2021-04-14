/**
 * \file            struct.h
 * \brief           Funciones de manejo de estructura
 * \author          Grupo1
 * \date            Ago 31, 2019
 * \details         Usar makefile para compilar y linkear (build)
 */
#include <define.h>
#include <files.h>

#ifndef STRUCT_H_
#define STRUCT_H_
typedef struct datos {
    char usuario[20];
    char contra[20];
    long tiempo;
} DATOS;
//Declaración de los prototipos de las funciones
int registro_estructura(DATOS **clientes,int *tam, int sockFd);
int ingreso_estructura(DATOS *clientes,int *tam, int sockFd);
void eliminar_estructura(DATOS **clientes,int *tam);
void bubble(DATOS * user,int tam);
void swap_struct(DATOS*a,DATOS*b);
DATOS pedir_Datos();
#endif /* FUNCTIONS_H_ */
