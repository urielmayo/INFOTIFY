/**
 * \file            functions.h
 * \brief           funciones
 * \author          Grupo1
 * \date            Ago 9, 2019
 * \details         Usar makefile para compilar y linkear (build)
 */
#include <define.h>
#include <files.h>
#include <struct.h>
#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

int copiar_datos(DATOS **clientes,int *tam);
int actualizar_archivo(DATOS *clientes,int *tam);
int contar_lineas(FILE * fp);

#endif /* FUNCTIONS_H_ */