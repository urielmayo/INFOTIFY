#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio_ext.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <time.h>
#include <linux/soundcard.h>

#define BUFFER_MAX 40000
#define LARGO 50
#define MSGBUFFER 80
#define IP_LENGTH 16
#define SI 1
#define NO 0
#define EXITO  0
#define ERROR  -1
#define CONNECTIONS_MAX 5
#define STRUCT_SIZE 20

#define MSEG 10
	/* sampling rate = velocidad de muestreo del audio a la entrada*/
#define RATE 48000
	/* sample size = Tamaño de muestra. Típicamente 8 o 16 bits */
#define SIZE 16
	/* 1 = mono 2 = stereo */
#define CHANNELS 2
	/* bits por bytes*/
#define BITS 8
	/* milisengundos por segundo */
#define MILI 1000

#ifndef FUNCTIONS_L_
#define FUNCTIONS_L_

//Crear el Logfile en caso de activacion
#define LOGFILEACT
#define LARGOLOG 105

//pusimos error_ xq sino concordaba el nombre con otra constatnte ya usada
typedef enum NivelLog {
    DEBUG,
    INFO,
    WARN,
    ERROR_,
    FATAL
}NivelLog;
#ifdef LOGFILEACT
void log_msg(NivelLog nivel,  char* archivo, int linea,const char* mensaje);
#endif //LOGFILEACT
#endif // FUNCTIONS_L_ 