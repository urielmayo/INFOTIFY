/**
 * \file            socket.c
 * \brief           funciones para crear socket cliente y analizar el mensaje recibido
 * \author          Grupo1
 * \date            2019.11.3
 * \details         Usar makefile para compilar y linkear (build)
 */

#include <comunicacion.h>
#include <play.h>
#define CLR "\033[2J\033[1;1H"

/**
 * \fn      int login(int port, char* ip)
 * \brief   funcion para crear socket
 * \author  Uriel Mayo
 * \date    2019.11.4
 * \param   port puerto de conexion (mismo que el del servidor)
 * \param   ip puntero a char con el string del ip
 * \return  valor de exito o error
 */

int login(int port, char* ip)
{
    typedef struct sockaddr_in sockaddr_in;
    
    struct hostent* hostData;
    sockaddr_in datosServer; 
    int sockFd;

    hostData = gethostbyname(ip); //se obtiene la ip
    if (!hostData)
    {
        printf("Error en gethostbyname()\n");
        return -1;
    }

    sockFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //se crea el socket

    datosServer.sin_family = AF_INET; //Flia de conecciones
    memcpy(&(datosServer.sin_addr), hostData->h_addr_list[0], hostData->h_length); //ip
    datosServer.sin_port = htons(port); //numero de puerto de conexion
    memset(&(datosServer.sin_zero), 0, 8);

    if (connect(sockFd,(struct sockaddr*)&datosServer, sizeof(struct sockaddr)) == -1) //se conecta el socket
    {
        printf("Erro en connect()\n");
        return -1;
    }

    return sockFd; //se retorna el fd del socket
}

/**
 * \fn      int msg(char* buffer, int clientFd)
 * \brief   funcion para analizar mensaje recibido
 * \author  Grupo 1
 * \date    Nov 4, 2019
 * \param   buffer puntero a char con el string de los mensajes a enviar
 * \param   clientFd fd del socket 
 * \return  valor de exito o error
 */
int msg(char* buffer, int clientFd, char* ip)
{
    int sentBytes, clientOnline = 1;
    char fileN[25];
    FILE* fd;

     if(strcmp(buffer,"Bienvenido a Infotify!") == 0){ //si llega mensaje de bienvenida
        sleep(1);
        printf(CLR);
     }

    if(strcmp(buffer,"Por favor elegir una opcion:") == 0) //si se pide elegir una opcion
    {
        scanf("%s",buffer);
        sentBytes = send(clientFd, buffer, 5, 0);
        printf(CLR);
        if (sentBytes <= 0) {
            printf("Error en send()");
            clientOnline = 0;
        }
    }

    else if(strcmp(buffer,"Ingresar nombre usuario:") == 0) //si se pide ingresar nombre d usuario
    {
        scanf("%s",buffer);
        sentBytes = send(clientFd, buffer, STRUCT_SIZE, 0);
        if (sentBytes <= 0) {
            printf("Error en send()");
            clientOnline = 0;
        }
    }

    else if(strcmp(buffer,"Ingresar contraseña:") == 0) //si se pide ingresar contraseña
    {
        scanf("%s",buffer);
        sentBytes = send(clientFd, buffer, STRUCT_SIZE, 0);
        if (sentBytes <= 0) {
            printf("Error en send()");
            clientOnline = 0;
        }
    }
    
    if(strcmp(buffer,"Elegi una cancion:") == 0){ //si se pide ingresar una opcion de las canciones a elegir
        scanf("%s",buffer);
        sentBytes = send(clientFd, buffer, 5, 0);
        if (sentBytes <= 0) {
            printf("Error en send()");
            clientOnline = 0;
        }
    }
    else if(strcmp(buffer,"/enviocancion") == 0) //si se va a recibir una cancion
    {
        sprintf(fileN,"temp_%s",ip);
        printf("Descargando cancion...\n");
        fd = fopen(fileN, "w"); //se crea el archivo binario sobre el cual se va a escribir
        if (fd){
            sentBytes = recv(clientFd, buffer, BUFFER_MAX, 0); //se recibe la primer tanda de 40000 bytes
            
            while (sentBytes > 0) //se repite el proceso hasta que llegue el mensaje de que se termino el envio
            {
                fwrite(buffer,1,BUFFER_MAX, fd);
                if (strcmp("terminado",buffer) == 0) break;
                sentBytes = recv(clientFd, buffer, BUFFER_MAX,0); 
            }
            fclose(fd);
            play(fileN); //se pone la cancion
        
        send(clientFd,"/terminoenvio", MSGBUFFER, 0); //se le avisa al servidor que termino el envio
        }
        else {
            printf("No se pudo recibir la cancion\n");
            clientOnline = 0;
        }
    }
    else if(strcmp(buffer,"/repitocancion") == 0) //si se quiere repetir la cancion
    {
        play(fileN);
        send(clientFd,"/terminoenvio", MSGBUFFER, 0);
    }
    //si llega el mensaje de cierre
    else if(strcmp(buffer,"Hasta luego") == 0 || strcmp(buffer,"Hasta luego, gracias por usar nuestra app!") == 0){
        clientOnline = 0;
    }
    
    return clientOnline;
}