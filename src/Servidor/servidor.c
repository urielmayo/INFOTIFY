/**
 * \file            servidor.c
 * \brief           funciones de Servidor
 * \author          Grupo1
 * \date            2019.10.30
 * \details         Usar makefile para compilar y linkear (build)
 */


#include <comunicacion.h>
#define TEMP_FILE "temp"
#define CLR "\033[2J\033[1;1H"

void son_signal();
void proceso_hijo(int sockCliente, struct sockaddr_in clientData, DATOS* clientes, int tam);

/**
	\fn      int main()
	\brief   Programa del Servidor
	\author  Alejo Fernadez Bados
	\date    2019.10.30
	\return  0
*/

int main(int argc, char* argv[]) 
{
    int clientLength, listenSocketFd, clientSocketFd;
    int pid;
    struct sockaddr_in clientData;
    DATOS *clientes;
    int tam;
    #ifdef LOGFILEACT
    char linealog[LARGOLOG];
    #endif

    signal(SIGCHLD, son_signal);
    
    if (argc < 2) {
        printf("Debe ingresar el puerto como parámetro\n");
        printf("Uso: %s <PUERTO>\n", argv[0]);
        return -1;
    }

    //Abro un socket tcp
    listenSocketFd = crearSocket( atoi(argv[1]) );
    if (listenSocketFd == -1) {
        printf("Error al crear socket\n");
        return -1;
    }
    
    // Server corriendo esperando conexiones
    printf("Server conectado en el puerto: %d\n", atoi(argv[1]) );

    copiar_datos(&clientes, &tam);
    #ifdef LOGFILEACT
    log_msg(INFO,__FILE__,__LINE__,"Copiando datos");
    #endif

    while (1) 
    {
        printf("Esperando conexiones...\n");
        clientSocketFd = accept(listenSocketFd, (struct sockaddr*) &clientData, (socklen_t*) &clientLength);

        pid = fork();


        if (pid < 0){
            printf("Error al crear proceso hijo\n");
                #ifdef LOGFILEACT
                sprintf(linealog,"Error al crear proceso hijo");
                log_msg(ERROR_,__FILE__,__LINE__,linealog);
                #endif

            return -1;
        }

        else if (pid == 0){
                #ifdef LOGFILEACT
                sprintf(linealog,"Creando proceso hijo para atender cliente");
                log_msg(INFO,__FILE__,__LINE__,linealog);
                #endif
            close(listenSocketFd);
            copiar_datos(&clientes, &tam);
            proceso_hijo(clientSocketFd, clientData, clientes, tam);
            return 0;
        }

        else{
            close(clientSocketFd);
        }
    }
    close(listenSocketFd);
    printf("Servidor finalizado\n");
        #ifdef LOGFILEACT
        sprintf(linealog,"Creando proceso hijo para atender cliente");
        log_msg(INFO,__FILE__,__LINE__,linealog);
        #endif
    return 0;
}

/**
	\fn      void son_signal()
	\brief   Handler par recibir señal de proceso hijo terminado y enterrarlo
	\author  Grupo 1
	\date    2019.11.9
	\return  no retorna ningun valor
*/
void son_signal() //cada vez que un proceso hijo termina, se viene a esta funcion
{
    // 16. "Sepulto" al hijo
    while (waitpid((pid_t)(-1), 0, WNOHANG) > 0) {
        printf("Cliente desconectado!\n");
    }
}

/**
 * \fn      void proceso_hijo(int socketCliente, struct sockaddr_in datosCliente, DATOS* clientes, int tam)
 * \brief   funcion con la que un proceso hijo realiza la comunicacion con el cliente
 * \author  Uriel Mayo
 * \date    2019.11.9
 * \param   socketCliente fd del socket cliente
 * \param   datosCliente estructura de tipo sockaddr_in con datos del cliente
 * \param   clientes vector de estructuras de tipo DATOS
 * \param   tam cantidad de estructuras en el vector
 * \return  valor de exito o error
 */

void proceso_hijo(int socketCliente, struct sockaddr_in datosCliente, DATOS* clientes, int tam)
{
    int status, actualizar; //tamaño del mensaje recibido en bytes
    #ifdef LOGFILEACT
    char linealog[LARGOLOG];
    #endif
    /*se toma la ip de forma dinamica con un puntero a char*/
    char* ip = (char*) malloc(sizeof(inet_ntoa((struct in_addr)datosCliente.sin_addr)));
    if (ip == NULL) return;
    strcpy(ip, inet_ntoa((struct in_addr)datosCliente.sin_addr));
    
    printf("Se conecto alguien desde la ip: %s\n",ip);
    
    #ifdef LOGFILEACT
    sprintf(linealog,"Nuevo cliente conectado desde la ip %s",ip);
    log_msg(INFO,__FILE__,__LINE__,linealog);
    #endif

    send(socketCliente, "Bienvenido a Infotify!", MSGBUFFER,0);

    status = menu_principal(clientes, &tam, socketCliente); //se pasa al menu principal
    if (status == EXITO)
    {
        /*si hubo algun registro, inicio de socion o se borro una cuenta, se actualiza el archivo csv*/
        actualizar_archivo(clientes, &tam);
        actualizar = segundo_menu(clientes, &tam, socketCliente, ip);
        if (actualizar == SI){
            actualizar_archivo(clientes, &tam);
        }
    }
    /*en caso de que el usuario decida salir o haya algun error de conexion, se libera la memoria y se termina la funcion*/
    else if (status == -2)
    {
        free(ip);
        return;
    }
    else {
        printf("Error de conexion\n");
        free(ip);
    }
}