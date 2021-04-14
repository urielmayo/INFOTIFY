/**
 * \file            functions.c
 * \brief           funciones de interracion entre el archivo y la estructura y funcion del logfile
 * \author          Grupo1
 * \date            2019.8.9
 * \details         Usar makefile para compilar y linkear (build)
 **/
#include <iniciar_secion.h>
/**
 * \fn      copiar_datos(DATOS *clientes,int *tam)
 * \brief   copia los datos del archivo a la estructura
 * \author  Alejo Fernadez Bados
 * \date    2019.9.4
 * \param   clientes direccion de las estructuras de datos de los clientes
 * \param   tam  tamaño del array de clientes
 * \return  si puedo abrir el archivo o no
 */
int copiar_datos(DATOS **clientes,int *tam){
    char  usuario_registrado[100];
    int verificacion = EXITO,comoFue=EXITO,i,j=0;
    char* aux;
    FILE * archivo;  
        #ifdef LOGFILEACT
        char linea_log[LARGOLOG];
        #endif 
    comoFue = abrir_archivolec("clientes.csv",&archivo);
    //validacion de que se abrio el archivo como corresponde
    if (comoFue == ERROR) {
        printf("Error al abrir el archivoo\n");
            #ifdef LOGFILEACT
            sprintf(linea_log,"Error al abrir el archivo");
            log_msg(FATAL,__FILE__,__LINE__,linea_log);
            #endif 
    }
    //comprabacion de si ya existe el nombre de usuario
    if (verificacion != ERROR) {
            #ifdef LOGFILEACT
            sprintf(linea_log,"archivo abierto en formato de lectura con exito");
            log_msg(INFO,__FILE__,__LINE__,linea_log);
            #endif 
        
        j = contar_lineas(archivo);
        *clientes = malloc(j*sizeof(DATOS));
        if (*clientes)
        {   
            j = 0;
            verificacion = leer_linea(usuario_registrado,archivo);
            while (verificacion != ERROR )
            {   
                aux=strtok(usuario_registrado,",");
                i = 0;
                
                while(*(aux+i)!='\0'){
                    (*clientes+ j)->usuario[i]=*(aux+i);
                    i++;
                }
                (*clientes+ j)->usuario[i]='\0';
                
                aux = strtok(NULL,",");
                i = 0;
                while(*(aux+i)!='\0'){
                    (*clientes + j)->contra[i]=*(aux+i);
                    i++;
                }
                (*clientes + j)->contra[i]='\0';
                
                aux = strtok(NULL,"\n");
                (*clientes + j)->tiempo=atoi(aux);
                    #ifdef LOGFILEACT
                    sprintf(linea_log,"copiando la linea %d del archivo 'clientes.csv' el la posicion [%d] del arreglo",j+1,j);
                    log_msg(DEBUG,__FILE__,__LINE__,linea_log);
                    #endif 
                j++;
                verificacion = leer_linea(usuario_registrado,archivo);
            }
            *tam = j;

            cerrar_archivo(archivo);
        }    
            #ifdef LOGFILEACT
            sprintf(linea_log,"cerrando el archivo");
            log_msg(INFO,__FILE__,__LINE__,linea_log);
            #endif 
    }
    return comoFue;
}
/**
 * \fn      int actualizar_archivo(DATOS *clientes,int *tam)
 * \brief   actualizar los datos modificados en la estructura al archivo
 * \brief   tam  tamaño del array de clientes
 * \author  Alejo Fernadez Bados
 * \date    2019.9.4
 * \param   clientes direccion de las estructuras de datos de los clientes
 * \param   tam  tamaño del array de clientes
 * \return  si puedo abrir el archivo o no
 */
int actualizar_archivo(DATOS *clientes,int *tam){
    int i,comoFue = EXITO;
    char usuario_nuevo[100];
    FILE * archivo;
        #ifdef LOGFILEACT
        char linea_log[LARGOLOG];
        #endif 
    //copiar_datos(&aux, &auxtam);
    comoFue = abrir_archivoesc("clientes.csv",&archivo);
    //validacion de que se abrio el archivo como corresponde
    if (comoFue == ERROR) {
            #ifdef LOGFILEACT
            log_msg(FATAL,__FILE__,__LINE__,"Error al abrir el archivo");
            #endif 
    }
    else{
        for(i=0;i<*tam;i++){
            sprintf(usuario_nuevo,"%s,%s,%ld",clientes[i].usuario,clientes[i].contra,clientes[i].tiempo);
            comoFue = escribir_linea(archivo,usuario_nuevo);
                #ifdef LOGFILEACT
                sprintf(linea_log,"Escribiendo linea %d",i);
                log_msg(DEBUG,__FILE__,__LINE__,linea_log);
                #endif 
        }
    }
    cerrar_archivo(archivo);
    return comoFue;
}
/**
 * \fn      void log_msg(NivelLog nivel, const char* archivo, int linea, const char* mensaje)
 * \brief   generar un archivo log
 * \author  Alejo Fernandez Bados
 * \date    2019.8.18
 * \param   nivel tipo de log
 * \param   archivo archivo donde estoy ejecutando la funcion
 * \param   linea linea donde se ejecuto la funcion
 * \param   mensaje mensaje que yo quiero poner en el log
 */


void log_msg(NivelLog nivel, char* archivo, int linea,const char* mensaje){
    time_t tiempo;
    FILE *fp;
    char* nvl[]={"DEBUG","INFO","WARN","ERROR_","FATAL"};
    fp=fopen("informe.log","a");
    tiempo = time( NULL );
    fprintf(fp,"%ld %s %s:%d: %s\n",tiempo,nvl[nivel],archivo,linea,mensaje);
    fclose(fp);
}

/**
 * \fn      int contar_lineas(FILE* fp)
 * \brief   contar las lineas que tiene el archivo
 * \author  Uriel Mayo
 * \date    2019.9.29
 * \param   fp puntero a FILE 
 * \return  cuantas lineas conto
 */
int contar_lineas(FILE* fp)
{
    int verif, contLineas = 0;
    char linea[60];
    do {
        verif = leer_linea(linea,fp);
        if (verif != ERROR) {
            contLineas++;
        }
    } while (verif != ERROR);
    rewind(fp);
    return contLineas;
}