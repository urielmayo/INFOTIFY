/**
 * \file            struct.c
 * \brief           Funciones de manejo de estructura
 * \author          Grupo1
 * \date            2019.8.31
 * \details         Usar makefile para compilar y linkear (build)
 */
#include <struct.h>
#include <iniciar_secion.h>
#include <comunicacion.h>
/**
 * \fn      int registro_estructura(DATOS *clientes,int *tam)
 * \brief   modificar la estructura a partir del registro 
 * \author  Alejo Fernadez Bados
 * \date    2019.9.4
 * \brief   clientes direccion de las estructuras de datos de los clientes
 * \brief   tam  tamaño del array de clientes
 * \return  si se registro exitosamente o no
 */
int registro_estructura(DATOS **clientes,int *tam, int sockFd){
    int i = 0, salio = ERROR, existe = NO;
    DATOS aux;
        #ifdef LOGFILEACT
        char linea_log[LARGOLOG];
        #endif 
    //ingreso de datos
    aux = pedir_Datos(sockFd);
    while(existe==NO && i<*tam)
    {   
            #ifdef LOGFILEACT
            sprintf(linea_log,"comparando los datos de la estructura aux con los datos de la posicion [%d] del arreglo",i);
            log_msg(DEBUG,__FILE__,__LINE__,linea_log);
            #endif
        if (strcmp(aux.usuario,(*clientes + i)->usuario) == 0) 
        {
            existe = SI;
                #ifdef LOGFILEACT
                sprintf(linea_log,"el nombre de usuario %s ya existe y se encuantra en la posicion [%d] ",(*clientes+i)->usuario,i);
                log_msg(WARN,__FILE__,__LINE__,linea_log);
                #endif
        }
        i++;
    }
    //si no existe ya el nombre de usuario se lo registra
    if(existe == NO){
        #ifdef LOGFILEACT
        sprintf(linea_log,"el nombre de usuario %s no existe, se pasa a registrar",aux.usuario);
        log_msg(INFO,__FILE__,__LINE__,linea_log);
        #endif
        *clientes = realloc(*clientes,(*tam + 1)*sizeof(DATOS));
        if (*clientes)
        {
            strcpy((*clientes + i )->usuario, aux.usuario);
            strcpy((*clientes + i)->contra, aux.contra);
                #ifdef LOGFILEACT
                sprintf(linea_log,"copiando el nombre %s y la contra %s en la posicion [%d] del arreglo",clientes[i]->usuario,clientes[i]->contra,i);
                log_msg(DEBUG,__FILE__,__LINE__,linea_log);
                #endif
            (*clientes + i)->tiempo=time( NULL ); 
            salio = EXITO;
            *tam = *tam + 1;
            enviar("Usuario registrado exitosamente",sockFd);
        }
        else printf("Error al reasignar memoria\n");
        
    } 
    else
        enviar("el usuario ya existe, intente con otro nombre de usuario",sockFd);
    return salio;
}
/**
 * \fn      int ingreso_estructura(DATOS *clientes,int *tam)
 * \brief   ingreso de datos y corroboracion de los mismos
 * \author  Alejo Fernandez Bados
 * \date    2019.9.4
 * \param   clientes direccion de las estructuras de datos de los clientes
 * \param   tam  tamaño del array de clientes
 * \return  si ingreso exitosamente o no
 */
int ingreso_estructura(DATOS *clientes,int *tam, int sockFd){
    int i = 0, salio = ERROR, existe = NO;
    char msg[MSGBUFFER];
    DATOS aux;
        #ifdef LOGFILEACT
        char linea_log[LARGOLOG];
        #endif 
    aux = pedir_Datos(sockFd);

    for ( i = 0; i < *tam; i++)
    {   
            #ifdef LOGFILEACT
            sprintf(linea_log,"comparando los datos de la estructura aux con los datos de la posicion [%d] del arreglo",i);
            log_msg(DEBUG,__FILE__,__LINE__,linea_log);
            #endif
        if ((strcmp(aux.usuario,(clientes+i)->usuario)==0) && (strcmp(aux.contra,(clientes+i)->contra)==0))
        {
            existe = SI;
                #ifdef LOGFILEACT
                sprintf(linea_log,"se encontro una coincidencia entre aux y la posicion [%d]",i);
                log_msg(INFO,__FILE__,__LINE__,linea_log);
                #endif
            break;
        }
    }
    
    //si algun usuario coincide con el ingresado se le da la bienvenida
    if(existe == SI){
        clientes[i].tiempo=time( NULL ); 
            #ifdef LOGFILEACT
            sprintf(linea_log,"actualizando el horario de ingreso de %s",(clientes+i)->usuario);
            log_msg(INFO,__FILE__,__LINE__,linea_log);
            #endif
        sprintf(msg,"¡Bienvenido %s!",clientes[i].usuario);
        enviar(msg, sockFd);
        salio = EXITO;
    }
    else
        enviar("Datos ingresados erroneos",sockFd);
    return salio; 
}
/**
 * \fn      void eliminar_estructura(DATOS *clientes,int *tam)
 * \brief   Eliminacion del usuario pedido
 * \author  Ladislao Rodriguez
 * \date    2019.9.6
 * \param   clientes direccion del array de la estructura de los clientes
 * \param   tam cantidad de clientes
 */
void eliminar_estructura(DATOS **clientes,int *tam){
    int i = 0;
    #ifdef LOGFILEACT
    char linea_log[LARGOLOG];
    sprintf(linea_log,"eliminando la cuenta de %s",(*clientes)->usuario);
    log_msg(DEBUG,__FILE__,__LINE__,linea_log);
    #endif
    //se sobreescribe sobre la posicion 0 xq al ordenar por ultimo inicio siempre va estar primero
    for(i=0;i<(*tam-1);i++){
        strcpy((*clientes + i)->usuario,(*clientes + i + 1)->usuario);
        strcpy((*clientes + i)->contra,(*clientes + i + 1)->contra);
        (*clientes + i)->tiempo = (*clientes + i + 1)->tiempo;
        #ifdef LOGFILEACT
        sprintf(linea_log,"pasando los datos de la posicion [%d] a la posicion [%d]",i+1,i);
        log_msg(DEBUG,__FILE__,__LINE__,linea_log);
        #endif
    } 
    *clientes = realloc(*clientes,(*tam - 1)*sizeof(DATOS));
    if (*clientes)
    {
        *tam = (*tam) - 1;
    }
    else printf("ERROR DE MEMORIA\n");
            #ifdef LOGFILEACT
            sprintf(linea_log,"Creando proceso hijo para atender cliente");
            log_msg(INFO,__FILE__,__LINE__,linea_log);
            #endif
}
/**
 * \fn      void bubble(DATOS * user,int tam)
 * \brief   ordenar los clientes por hora de ingreso o registro
 * \author  Alejo Fernandez Bados
 * \date    2019.8.31
 * \param   user dirrecion del array de la estructura de los clientes
 * \param   tam cantidad de clientes
 */
void bubble(DATOS * user,int tam){
    int estaOrdenado = NO,i;
    int ultimoDesordenado;
        #ifdef LOGFILEACT
        char linea_log[LARGOLOG];
        #endif 
    ultimoDesordenado = tam-1;    
    while(estaOrdenado==NO) {
        estaOrdenado = SI;
        // si el tiempo es menor se lo cambia de lugar
        for (i = 0; i < ultimoDesordenado; i++) {
            if (user[i].tiempo < user[i+1].tiempo) {
                swap_struct(&user[i],&user[i+1]);
                estaOrdenado = NO;
                    #ifdef LOGFILEACT
                    sprintf(linea_log,"cambio de lugar de la poscion %d con la posicion %d",i+1,i);
                    log_msg(DEBUG,__FILE__,__LINE__,linea_log);
                    #endif
            }
        }
        ultimoDesordenado--;
    }
}
/**
 * \fn      void swap_struct(DATOS*a,DATOS*b)
 * \brief   swap entre dos estructuras
 * \author  Ladislao Rodriguez
 * \date    2019.9.6
 * \param   primera estructura
 * \param   segunda estructura
 */
void swap_struct(DATOS*a,DATOS*b)
{
    DATOS aux;
    aux = *a;
    *a=*b;
    *b = aux;
}
/**
 * \fn      DATOS pedir_Datos()
 * \brief   obtener datos del usuario
 * \author  Uriel Mayo
 * \date    2019.10.3
 * \return  datos del usuario igrensados
 */
DATOS pedir_Datos(int sockFd)
{
    DATOS dato_nuevo;

    enviar("Ingresar nombre usuario:",sockFd);
    recv(sockFd,dato_nuevo.usuario,20,0);
    
    enviar("Ingresar contraseña:",sockFd);
    recv(sockFd,dato_nuevo.contra,20,0);

    return dato_nuevo;
}