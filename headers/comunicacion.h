/**
 * \file            comunicacion.c
 * \brief           funciones de comunicacon
 * \author          Grupo1
 * \date            Nob 3, 2019
 * \details         Usar makefile para compilar y linkear (build)
 */


#include <define.h>
#include <iniciar_secion.h>
#include <menu.h>
#include <struct.h>
#include <files.h>

int crearSocket(int puerto);
int login(int port, char* ip);
int enviar_cancion(char * cancion , int clientSocketFd);
int enviar(char * buff, int socket);
void recibir(char *buff,int clientSocketFd);
void recibirint(int *opcion,int clientSocketFd);
int msg(char* buff, int clientFd,char * ip);