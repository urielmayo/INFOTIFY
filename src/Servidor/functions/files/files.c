/**
 * \file            files.c
 * \brief           funciones de archivos
 * \author          Grupo1
 * \date            2019.9.3
 * \details         Usar makefile para compilar y linkear (build)
 */
#include <files.h>
/**
 * \fn      int abrir_archivoesc(char* nombre_archivo,FILE** archivo)
 * \brief   abrir el archivo
 * \author  Uriel Mayo
 * \date    2019.9.6
 * \param  nombre del archivo
 * \param  archivo file pointer del archivo
 * \return  si abrio o no el archivo
 */
int abrir_archivoesc(char* nombre_archivo,FILE** archivo)
{
    int res = ERROR;

    *archivo = fopen(nombre_archivo,"w");
    if (*archivo){
        res = EXITO;
    }
    return res;
}
/**
 * \fn      int abrir_archivolec(char* nombre_archivo,FILE** archivo)
 * \brief   abrir el archivo de lectura
 * \author  Uriel Mayo
 * \date    2019.9.6
 * \param   nombre del archivo
 * \param   archivo file pointer del archivo
 * \return  si abrio o no el archivo
 */
int abrir_archivolec(char* nombre_archivo,FILE** archivo)
{
    int res = ERROR;

    *archivo = fopen(nombre_archivo,"r");
    if (*archivo){
        res = EXITO;
    }
    return res;
}
/**
 * \fn      int leer_linea(char* linea,FILE* fp)
 * \brief   leer una linea
 * \author  Uriel Mayo
 * \date    2019.9.6
 * \param   dirrecion del array donde se va a guardar el contenido de la linea
 * \param  archivo file pointer del archivo
 * \return  si se pudo leer o no la linea en cuestion
 */
int leer_linea(char* linea,FILE* fp){
    int comoSalio=ERROR;
    int i=0;
    char aux;
    if(fp!=NULL && feof(fp)==0){
        aux=fgetc(fp);
        while((aux!='\n') && feof(fp)==0){
            comoSalio=EXITO;
            *(linea+i)=aux;
            i++; 
            aux=fgetc(fp);
        }
        *(linea+i)='\0';
    }// se reemplaza el \n con un \0
    return comoSalio;
}
/**
 * \fn      int escribir_linea(FILE* fp, const char* linea)
 * \brief   escribir una linea
 * \author  Uriel Mayo
 * \date    2019.9.6
 * \param  archivo file pointer del archivo
 * \param   dirrecion del array donde se guarda lo que se quire escribir
 * \return  si hubo error o no
 */   
int escribir_linea(FILE* fp, const char* linea){
    
    int comoSalio=ERROR;
    int i=0;
    if(fp!=NULL){
        comoSalio=EXITO;
        while(*(linea+i)!='\0'){
            fprintf(fp,"%c",*(linea+i));
            i++;    
        }//reemplaza \0 con \n
        fprintf(fp,"\n");
    }
    return comoSalio;
}
/**
 * \fn      void cerrar_archivo(FILE* ptr_archivo)
 * \brief   cerrar el archivo
 * \author  Ladislao Rodriguez
 * \date    2019.9.6
 * \param  archivo file pointer del archivo
 */  
void cerrar_archivo(FILE* ptr_archivo)
{
    fclose(ptr_archivo);
}
/**
 * \fn      void ir_inicio_archivo(FILE* ptr_archivo)
 * \brief   funcion para ir al principio del archivo
 * \author  Ladislao Rodriguez
 * \date    2019.9.6
 * \param   archivo file pointer del archivo
 */  
void ir_inicio_archivo(FILE* ptr_archivo)
{
    fseek(ptr_archivo,0,SEEK_SET);
}