/**
 * \file            play.c
 * \brief           funciones para crear socket cliente y analizar el mensaje recibido
 * \author          Grupo1
 * \date            2019.11.3
 * \details         Usar makefile para compilar y linkear (build)
 */

#include <play.h>

int value = 0; //variable global para pausar/reanudar reproduccion

/**
 * \fn      Play(const char* trackName)
 * \brief   funcion para setear los parametros del audio
 * \author  Grupo1
 * \date    2019.10.6
 * \param   trackName puntero a char con el nombre de el track elegido
 * \return  no retorna ningun valor
 */

void play(const char* trackName)
{
    int dspfd;		/* sound device file descriptor */
	FILE * rawfd = NULL;	/* sound device file descriptor */
	int status;		/* return status de system calls */
	audio_params config = {RATE, SIZE, CHANNELS};
	int buffsize;
	int salida = 0;
	IplImage *img1, *base;
	char tecla;

	unsigned int cinta, i_cinta; 
 	unsigned char * buff = NULL;

    // se carga el buffer con memoria
    buffsize = MSEG*config.v_muestreo*config.t_muestra*config.c_canales/BITS/MILI;
    buff = (unsigned char*) malloc(buffsize);

    rawfd = fopen(trackName,"r");
    if (rawfd == NULL)
    {
        printf("Error al abrir el archivo\n");
        salida = 2;
		return;
    }

    //se abre la placa de sonido
    if (!salida)
	{
		if ((dspfd = open("/dev/dsp", O_RDWR))<0)
		{ 
			fprintf(stderr,"Error en función open, Código de error: %s\n",strerror (dspfd)); 
			salida = 3;
		}
	}

    if (!salida)
	{
		/*Seteamos parámetros de audio y de interfaz grafica*/
		set_audio_params (dspfd, &config);

		img1 = cvLoadImage(PLAY, CV_LOAD_IMAGE_COLOR);
		base = (IplImage *) malloc(img1->nSize);
		memcpy(base, img1, img1->nSize);
		cvNamedWindow("Infotify", CV_WINDOW_AUTOSIZE);
		cvMoveWindow("Infotify", 100, 600);
		cvSetMouseCallback("Infotify", myCallback, (void *) img1);
		cvShowImage("Infotify", img1);

		/*Inicializamos la variable de tiempo y limpiamos la pantalla*/
		i_cinta = time(NULL);
		printf("\033[2J\n");
        // se empieza a reproducir la cancion , mientras se muestra el nombre de la cancion y el tiempo que lleva reproducida.
		while ((!salida) && (!feof(rawfd)) && (tecla != 27))
		{
			while(value == 1){ //si se presiona boton de pausa se inicia el while, si se presiona de nuevo termina
				cvWaitKey(1);
				if (value == 0) break; 
			}
			cvWaitKey(1);
				
			status = fread(buff, 1, buffsize, rawfd); /* lee */
			if (!salida)
			{
				cinta = time(NULL);
		 		printf("\033[1;1HReproduciendo: \t\t\033[0m(%02d:%02d)\n", (cinta-i_cinta)/60, (cinta-i_cinta)%60);

				status = write(dspfd, buff, buffsize); /* reproduce */
			}
		}
	}
	status = ioctl(dspfd, SOUND_PCM_SYNC, 0);
	if (status == -1) 
		perror("Error en comando SOUND_PCM_SYNC");
    // se libera la memoria y se cierra el archivo que abre la placa de sonido
	if (salida > 2) fclose(rawfd);
	if (salida > 3) close(dspfd);
	
	cvDestroyAllWindows(); //se cierran las imagenes y se liberan los punteros
	cvRelease((void**)&img1);
	
	if (buff) free (buff);
}

/**
        \fn     void myCallback(int event, int x, int y, int flags, void *data)
        \brief  Función que se ejecuta al producirse un evento con el mouse. se evalua cuando se libera la presion
				del boton izquierdo
        \author Grupo1
        \date   2019.11.12
        \param  event: evento que llamó a la función
        \param  x: posición x del mouse al momento llamar a la función.
        \param  y: posición y del mouse al momento llamar a la función.
        \param  flags: flags activos al momento llamar a la función (por ejemplo tecla ctrl presionada).
        \param  data: puntero a la estructura de la imagen 
*/

void myCallback(int event, int x, int y, int flags, void *data)
{
	char nombre[512];

	switch(event)
	{
		case CV_EVENT_LBUTTONUP:
			if ((x>146) && (x<216))
			{
				if ((y>10)&&(y<80)) 
				{
					if (value == 0)
					{
						strcpy(nombre,PAUSE);
						value ++;
						data = cvLoadImage(nombre, CV_LOAD_IMAGE_COLOR);
						cvShowImage("Infotify", data);

					}
					else if(value == 1)
					{
						strcpy(nombre,PLAY);
						value --;
						data = cvLoadImage(nombre, CV_LOAD_IMAGE_COLOR);
						cvShowImage("Infotify", data);
					}
				}
			}
		break;
	}
}