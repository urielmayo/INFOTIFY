/**
 * \file            menu.H
 * \brief           funciones del menu
 * \author          Grupo1
 * \date            Sep 3, 2019
 * \details         Usar makefile para compilar y linkear (build)
 */
#include <define.h>
#include <struct.h>

//#ifndef STRUCT_H_
//#define STRUCT_H_

//#ifndef MAIN_H_
//#define MAIN_H_ 
int menu_principal(DATOS *usuarios,int *tam, int clientSocketFd);
int segundo_menu(DATOS *usuarios,int *tam, int clientSocketFd, char* ip);
void menu_cancion(char *nombre,char * buff,int clienteSocketFd);
int obtener_nombre(char *cancion, int clientSocketFd);
void scanfInt(int* var_entera,int clientSocketFd);


//#endif /* MAIN_H_*/
//#endif /* STRUCT_H_ */