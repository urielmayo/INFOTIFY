/**
 * \file            cliente.c
 * \brief           funciones de Cliente
 * \author          Grupo1
 * \date            2019.11.3
 * \details         Usar makefile para compilar y linkear (build)
 */
#include <play.h>
#include <comunicacion.h>

/**
	\fn      int main()
	\brief   Programa Cliente
	\author  Alejo Fernadez Bados
	\date    2019.10.31
	\return  0
*/
int main(int argc, char* argv[]) {
    
    char fileN[25];
    int messageSize = 0, clientOnline;
    char buffer[BUFFER_MAX];
    //Declaro el file descriptor para el socket de la conexión
    int clientSocketFd;

    // Obtengo el número de IP y de puerto pasados como parámetros del main
    if (argc < 3) {
        printf("Debe ingresar la IP y el puerto como parametros\n");
        printf("Uso: %s <IP> <PUERTO>\n", argv[0]);
        return -1;
    } 
    
    clientSocketFd = login(atoi(argv[2]),argv[1]);
    
    if(clientSocketFd == -1)
    {
        printf("no se pudo conectar con el servidor\n");
        return -1;
    }

    //Cliente conectado al servidor
    printf("Cliente conectado al servidor\n");
    clientOnline = 1;
     while (clientOnline == 1) {
        messageSize = recv(clientSocketFd, buffer, MSGBUFFER, 0);
        buffer[messageSize] = '\0';
        if (messageSize <= 0){
            printf("Error en recv()\n");
            clientOnline = 0;
        }

        else
        {
            // Muestra mensaje recibido
            if(strcmp(buffer,"/enviocancion")!=0) printf("\n%s\n", buffer);
            clientOnline = msg(buffer,clientSocketFd,argv[1]);
        }
    }
    sprintf(fileN,"temp_%s",argv[1]);
    remove(fileN);
    close(clientSocketFd);
    printf("Conexion con servidor finalizada\n");
    return 0;
}