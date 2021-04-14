/**
 * \file            menu.c
 * \brief           funciones del menu
 * \author          Grupo1
 * \date            2019.9.4
 * \details         Usar makefile para compilar y linkear (build)
 */
#include <menu.h>
#include <iniciar_secion.h>
#include <comunicacion.h>
#include <dirs.h>
/**
 * \fn      int menu_principal(DATOS *usuarios,int *tam)
 * \brief   funcion del menu principal
 * \author  Uriel Mayo
 * \date    2019.9.4
 * \param   usuarios estructuras de datos de los clientes
 * \param   tam  tamaño del array de clientes
 * \param   clientSocketFd fd del socket cliente
 * \return  si la funcion tuvo exito o no
 */
int menu_principal(DATOS *usuarios,int *tam, int clientSocketFd)
{
    int opcion, comoFue = ERROR, salir = NO;
        #ifdef LOGFILEACT
        char linea_log[LARGOLOG];
        #endif 
    while (salir == NO)
    {//Eleccion de la accion a realizar
        enviar("1.Ingresar\n2. Registrarse\n3. Salir",clientSocketFd);
        enviar("Por favor elegir una opcion:",clientSocketFd);
        scanfInt(&opcion,clientSocketFd);

            #ifdef LOGFILEACT
            sprintf(linea_log,"el usuario eligio la opcion %d",opcion);
            log_msg(INFO,__FILE__,__LINE__,linea_log);
            #endif 
    
        switch (opcion) 
        {
            case 1: //iniciar sesion
                    comoFue = ingreso_estructura(usuarios,tam,clientSocketFd);
                    if (comoFue == EXITO){
                        bubble(usuarios,*tam);
                        salir = SI;
                    }                   
                    break;

            case 2: //registrarse
                    comoFue = registro_estructura(&usuarios,tam,clientSocketFd);
                    if (comoFue == EXITO){
                        bubble(usuarios,*tam);
                        salir = SI;
                    } 
                    break;
            
            case 3: //salir
                    enviar("Hasta luego",clientSocketFd);
                    comoFue = -2;
                    break;
            
            default:enviar("Por favor intente de nuevo",clientSocketFd);
                    #ifdef LOGFILEACT
                    log_msg(WARN,__FILE__,__LINE__,"No se eligio una opcion valida");
                    #endif
                    break;
        }
    }
    return comoFue;
}
/**
 * \fn      int segundo_menu(DATOS *usuarios,int *tam)
 * \brief   funcion del segundo menu
 * \author  Alejo Fernadez Bados
 * \date    2019.9.4
 * \param   usuarios estructuras de datos de los clientes
 * \param   tam  tamaño del array de clientes
 * \param   clientSocketFd fd del socket del cliente con el cual se interactua
 * \param   ip puntero  char con la ip del ususario
 * \return  si hubo un cambio o no que requiera una actualizacon del archivo
 */
int segundo_menu(DATOS *usuarios,int *tam, int clientSocketFd, char* ip)
{
    int opcion, terminar = NO, comoFue = EXITO, actualizar = NO;
    char buff[200];
    char cancion[20];
    #ifdef LOGFILEACT
    char linea_log[LARGOLOG];
    #endif 
    while (terminar == NO)
    {//Eleccion de la accion a realizar
        enviar("1. Eliminar cuenta\n2. Escuchar una cancion\n3. Salir",clientSocketFd);
        enviar("Por favor elegir una opcion:",clientSocketFd);
        recibirint(&opcion,clientSocketFd);
            #ifdef LOGFILEACT
            sprintf(linea_log,"el usuario eligio la opcion %d del segundo menu",opcion);
            log_msg(INFO,__FILE__,__LINE__,linea_log);
            #endif
        switch (opcion) {
        
        case 1: //eliminar cuenta
                eliminar_estructura(&usuarios,tam);
                enviar("Hasta luego, gracias por usar nuestra app!",clientSocketFd);
                terminar = SI;
                actualizar = SI;
                break;
        
        case 2: //escuchar una cancion
                printf("Cliente [%s]: cancion solicitada\n",ip);
                comoFue = showSongs(clientSocketFd);
                if(comoFue == EXITO){
                    /*se le pide al usuario q elija la cancion, indefinidamente hasta que el usuario ingrese una opcion valida*/
                    do{
                        enviar("Elegi una cancion:",clientSocketFd);
                        comoFue = obtener_nombre(cancion,clientSocketFd);
                    }while(comoFue == -2);
                    
                    if (comoFue == ERROR){//si no se pudo abrir el directorio
                        terminar = SI;
                        break;
                    }
                    menu_cancion(cancion,buff,clientSocketFd);
                }
                else enviar("Error al ver las canciones disponibles\n",clientSocketFd);
                terminar = SI;
                break;
        
        case 3: //salir
                enviar("Hasta luego",clientSocketFd);
                terminar = SI;
                break;
        
        default:enviar("Por favor intente de nuevo\n",clientSocketFd);
                #ifndef LOGFILEACT
                log_msg(WARN,__FILE__,__LINE__,"No se eligio una opcion valida");
                #endif 
        }
    }
    return actualizar;
}

/**
 * \fn      void menu_cancion(char *nombre,char * buff)
 * \brief   funcion del menu de canciones
 * \author  Alejo Fernadez Bados
 * \date    2019.10.5
 * \param   nombre nombre de la cancion reproducida 
 * \param   buff nombres de canciones
 * \param   clientSocketFd fd del socket del cliente
 * \return  no retorna ningun valor
 */
void menu_cancion(char *nombre,char * buff,int clientSocketFd){
    
    int salir = NO, opcion = 1, i = 0, comoFue = EXITO, seMando = NO;
    char finalizo[20];
    
        #ifdef LOGFILEACT
        char linea_log[105];
        #endif
    while (salir == NO)
    {//Eleccion de la accion a realizar
        if(i > 0){
            enviar("1. Escuchar nuevamente\n2. Escuchar otra cancion\n3. Salir\n",clientSocketFd);
            enviar("Por favor elegir una opcion:",clientSocketFd);
            scanfInt(&opcion,clientSocketFd);
                #ifdef LOGFILEACT
                sprintf(linea_log,"el usuario eligio la opcion %d del menu de canciones",opcion);
                log_msg(INFO,__FILE__,__LINE__,linea_log);
                #endif
        }
            switch (opcion) {
                case 1: //escuchar cancion por primera vez, o escuchar nuevamente
                        if(seMando == NO){ //si no se escucho la cancion
                            if (enviar_cancion(nombre,clientSocketFd) == EXITO){
                                recibir(finalizo,clientSocketFd);
                            }
                        }
                        else{ //si ya se escucho una vez, se le da la orden al usuario que la reproduzca
                            enviar("/repitocancion",clientSocketFd);
                            recibir(finalizo,clientSocketFd);
                        }
                        if (strcmp("/terminoenvio",finalizo) == 0){
                            seMando = SI;
                            break;
                        }
                
                case 2: //si se quiere escuchar otra cancion
                        comoFue = showSongs(clientSocketFd);
                        if(comoFue == EXITO){
                            //se le pide al usuario que eligia una opcion, hasta que sea valida
                            do{
                                enviar("Elegi una cancion:",clientSocketFd);
                                comoFue = obtener_nombre(nombre,clientSocketFd);
                                if(comoFue == EXITO) opcion = 1;
                                else salir = SI;
                            }while (comoFue == -2);

                            seMando = NO;
                            i = -1; //se pone -1 para que no vuelva a mostrar el menu
                            break;
                        }
                        else salir = SI;
                        break;
                
                case 3:// salir
                        enviar("Hasta luego",clientSocketFd);
                        salir = SI; 
                        break;
                default:enviar("Por favor intente de nuevo\n",clientSocketFd);
            }
        i++;
    }
}

/**
 * \fn      void scanfInt(int* var_entera)
 * \brief   funcion para chequear que el valor ingresado sea un numero
 * \author  Uriel Mayo
 * \date    2019.9.4
 * \param   puntero a entero
 * \return  no retorna ningun valor
 */
void scanfInt(int * var_entera,int clientSocketFd)
{
    int verificacion = EXITO, i;
    char s[5];

    recv(clientSocketFd,s,5, 0);
    //se toma lo ingresado como string
        
    for ( i = 0; (i < strlen(s)) ; i++)
    {
        //se chequea que toda la cadena (o el caracter) sea de numeros
        if (!((s[i] >= '0') && (s[i] <= '9')))
        {
            verificacion = ERROR; // si se encuentra un caracter no numerico en la cadena
            break;
        }
    }
    /* si todo lo ingresado es numero, se devuelve por referencia en forma entera usando atoi.
       de lo contrario, se devuleve un -1 */
    if (verificacion == ERROR) *var_entera = -1;

    else *var_entera = atoi(s);
}

/**
 * \fn      void obtener_nombre(char *cancion, int clientSocketFd)
 * \brief   funcion para obtener el nombre de la cancion
 * \author  Alejo Fernadez Bados
 * \date    2019.10.5
 * \param   cancion nombre de la cancion elegida
 * \param   clientSocketFd fd del socket del cliente
 * \return  no retorna ningun valor
 * 
 */
int obtener_nombre(char *cancion ,int clientSocketFd){
    
    int coincidencia = NO;
    int eleccion, i = 1;
    DIR* dp;
    struct dirent* entry;
        #ifdef LOGFILEACT
        char linea_log[LARGOLOG];
        #endif

    scanfInt(&eleccion,clientSocketFd);

    /* se abre la carpeta media y se busca el numero de fila que coincida con la eleccion del cliente*/
    dp = opendir("media");
    if (dp)
    {
        while ((entry = readdir(dp)) != NULL)
        {
            if ((strcmp(entry->d_name,".")) != 0 && (strcmp(entry->d_name,"..") != 0)){
                
                if (i == eleccion){
                    coincidencia = SI;
                    break;
                }
                i++;
            }
        }

        if (coincidencia == SI){ //si se encuentra la coindicencia
           strcpy(cancion,entry->d_name);
           return EXITO;
        }

        else{ //si no se encuentra ninguna coincidencia
            strcpy(cancion,"error");
            enviar("la cancion no se encuentra en el repertorio",clientSocketFd);
            return -2;
        }
        closedir(dp);
    }
    else { //si no se pudo abrir el directorio
        enviar("No se encontro la carpeta MEDIA",clientSocketFd);
            #ifdef LOGFILEACT
			sprintf(linea_log,"No se encontra la carpeta media en el directorio");
			log_msg(ERROR_,__FILE__,__LINE__,linea_log);
			#endif
        return ERROR;
    }
}