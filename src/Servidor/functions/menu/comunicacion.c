/**
 * \file            comunicacion.c
 * \brief           funciones de comunicacon
 * \author          Grupo1
 * \date            2019.11.4
 * \details         Usar makefile para compilar y linkear (build)
 */

#include <comunicacion.h>

/**
 * \fn      int crearSocket(int puerto)
 * \brief   funcion para crear el socket del servidor
 * \author  Grupo 1
 * \date    2019.11.4
 * \param   puerto puerto al que se va a conectar el socket
 * \return  valor de exito o error
 */
int crearSocket(int puerto)
{
    int sockFd, enable;
    struct sockaddr_in datosServer;

    sockFd = socket(AF_INET,SOCK_STREAM,0); //creo el socket
    if (sockFd == -1)
    {
        printf("Error al crear socket\n");
            #ifdef LOGFILEACT
            log_msg(FATAL,__FILE__,__LINE__,"Error en socket()");
            #endif
        return -1;
    }
    
    //lleno la estructura
    datosServer.sin_family = AF_INET;
    datosServer.sin_addr.s_addr = INADDR_ANY; //direccion ip, en este caso es la local host
    datosServer.sin_port = htons(puerto); //numero de puerto de conexion
    memset(datosServer.sin_zero,0,8);

    //evito que se concten al puerto solicitado
    enable = 1;
    setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));

    //enlazo el socket con la ip y con el puerto
    if (bind(sockFd,(struct sockaddr*)&datosServer, sizeof (datosServer)) == -1)
    {
        printf("Error en bind\n");

        #ifdef LOGFILEACT
        log_msg(FATAL,__FILE__,__LINE__,"Error en funcion bind()");
        #endif

        return -1;
    }
    //empiezo a escuchar
    if (listen(sockFd,CONNECTIONS_MAX) == -1)
    {
        printf("Error en listen\n");
            #ifdef LOGFILEACT
            log_msg(FATAL,__FILE__,__LINE__,"Error en listen()");
            #endif
        return -1;
    }

    #ifdef LOGFILEACT
    log_msg(INFO,__FILE__,__LINE__,"Servidor Conectado");
    #endif

    return sockFd; //se retorna el fd del socket creado
}

/**
 * \fn      int enviar(char * buff, int socket)
 * \brief   funcion para enviar mensajes al cliente
 * \author  Grupo 1
 * \date    2019.11.4
 * \param   buff puerto a char, sirve para colocar una cadena de texto
 * \param   socket fd del socket al cual se envia el mensaje
 * \return  valor de exito o error
 */
int enviar(char * buff, int socket){
    int sentBytes, comoFue = EXITO;
    #ifdef LOGFILEACT
    char linea_log[LARGOLOG];
    #endif
    
    sentBytes = send(socket, buff, MSGBUFFER, 0); //se envia el mensaje de texto
    
    if (sentBytes <= 0) { //si no se pudo enviar
        printf("Error en send()");
            #ifdef LOGFILEACT
            sprintf(linea_log,"Error en funcion send()");
            log_msg(ERROR_,__FILE__,__LINE__,linea_log);
            #endif
        comoFue = ERROR;    
    }
    return comoFue;
}

/**
 * \fn      void recibir(char *buff,int clientSocketFd)
 * \brief   funcion para recibir mensajes del cliente
 * \author  Grupo 1
 * \date    2019.11.4
 * \param   buff puerto a char, sirve para colocar una cadena de texto
 * \param   clientSocketFd fd del socket del cual se recibe el mensaje
 * \return  no retorna ningun valor
 */
void recibir(char *buff,int clientSocketFd){
    int Bytes;

    Bytes = recv(clientSocketFd,buff, MSGBUFFER, 0); //se recibe el mensaje 
    buff[Bytes] = '\0'; //se coloca un caracter nule para asegurar el string
}

/**
 * \fn      void recibirint(int *opcion,int clientSocketFd)
 * \brief   funcion para recibir mensajes del cliente y pasar el mensaje a un int
 * \author  Grupo 1
 * \date    2019.11.4
 * \param   opcion puerto a int, por el cual se pasa por referencia el resultado
 * \param   clientSocketFd fd del socket del cual se recibe el mensaje
 * \return  no retorna ningun valor
 */
void recibirint(int *opcion,int clientSocketFd){
    
    //esta funcion es igual a recibir, solo que lo recibido se pasa a variable entera con atoi()
    char buffer[MSGBUFFER];
    int Bytes;
    Bytes = recv(clientSocketFd,buffer, MSGBUFFER, 0);
    buffer[Bytes] = '\0';
    *opcion = atoi(buffer);
}


/**
 * \fn      int enviar_cancion(char* cancion , int clientSocketFd)
 * \brief   funcion para recibir mensajes del cliente y pasar el mensaje a un int
 * \author  Grupo 1
 * \date    2019.10.27
 * \param   cancion puntero a char con el nombre del archivo de la cancion elegida
 * \param   clientSocketFd fd del socketal cual se le va a enviar la cancion
 * \return  valor de exito o error
 */
int enviar_cancion(char* cancion , int clientSocketFd){
    FILE * fd;
    int enviados;
    char direccion[25];
    char buffer[BUFFER_MAX];
        #ifdef LOGFILEACT
        char linea_log[LARGOLOG];
        int i = 1;
        #endif
    sprintf(direccion,"media/%s",cancion);

    fd = fopen(direccion, "r"); //se abre el archivo en forma de lectura
    if(fd)
    {
            #ifdef LOGFILEACT
            sprintf(linea_log,"abriendo archivo %s para enviar al cliente",direccion);
            log_msg(INFO,__FILE__,__LINE__,linea_log);
            #endif
        enviar("/enviocancion",clientSocketFd); //se le avisa al cliente que va a recibir una cancion
        do
        {   
            //se leen de a 40000 bytes y se envian al cliente hasta que haya finalizado el archivo
            fread(buffer,1,BUFFER_MAX, fd);
            enviados = send(clientSocketFd, buffer,BUFFER_MAX,0);
            usleep(10000); //se puso un delay porque sino se pierden algunos envios
                #ifdef LOGFILEACT
                sprintf(linea_log,"Paquete Nro %d de %d bytes",i,enviados);
                log_msg(DEBUG,__FILE__,__LINE__,linea_log);
                i++;
                #endif
            if(enviados < 0){
                printf("Error servidor\n");
                    #ifdef LOGFILEACT
                    sprintf(linea_log,"Error en funcion send() al enviar cancion");
                    log_msg(ERROR_,__FILE__,__LINE__,linea_log);
                    #endif
                break;
            }
        }while (enviados > 0 && !(feof(fd)));

        //se cierra el archivo y se envia un mensaje de fin para decirle al cliente que ya termino el envio
        fclose(fd);
        enviados = send(clientSocketFd,"terminado",MSGBUFFER,0);
        printf("envio finalizado\n");
        return EXITO;
            #ifdef LOGFILEACT
            sprintf(linea_log,"Envio exitoso");
            log_msg(INFO,__FILE__,__LINE__,linea_log);
            #endif
    }
    else{ //en caso de que no se haya podido enviar la cancion
        enviar("la cancion no se encuentra disponible",clientSocketFd);
            #ifdef LOGFILEACT
            sprintf(linea_log,"No se encontro la cancion para enviar");
            log_msg(WARN,__FILE__,__LINE__,linea_log);
            #endif
        return ERROR;
    }    
}