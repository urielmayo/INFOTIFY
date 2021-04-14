/**
 * \file            functions.c
 * \brief           funciones para setear audio
 * \author          Grupo1
 * \date            2019.11.3
 * \details         Usar makefile para compilar y linkear (build)
 */

#include <play.h>

/**
 * \fn      void set_audio_params(int fd, audio_params * valor)
 * \brief   funcion para setear los parametros del audio
 * \author  Uriel Mayo
 * \date    2019.10.6
 * \param   fd entero de un file descriptor
 * \param	valor puntero a una estructura de tipo audio_params
 * \return  no retorna ningun valor
 */
void set_audio_params (int fd, audio_params * valor)
{
	int arg;		/* argumento para ioctl */
	int	status;		/* salida de ioctl */

	/* seteamos los parametros de muestreo  */
	arg = valor->t_muestra;	   /* arg = Tamaño de muestra */
	status = ioctl(fd, SOUND_PCM_WRITE_BITS, &arg); 

	if (status == -1) 
		perror("Error con comando SOUND_PCM_WRITE_BITS");

	// en caso de que el tamaño de la muestra de audio no sea soportado
	if (arg != valor->t_muestra)
	{
		fprintf (stderr,"Tamaño de muestras no soportado. Se programó %d\n",arg);
		valor->t_muestra = arg;
	}

	/* mono o stereo */
	arg = valor->c_canales;
	status = ioctl(fd, SOUND_PCM_WRITE_CHANNELS, &arg);
	if (status == -1)
		perror("Error en comando SOUND_PCM_WRITE_CHANNELS");
	if (arg != valor->c_canales)
	{
		fprintf (stderr,"Cantidad de canales no soportado. Se programó %d\n",arg);
		valor->c_canales = arg;
	}


	/* Velocidad de Muestreo */
	arg = valor->v_muestreo;
	status = ioctl(fd, SOUND_PCM_WRITE_RATE, &arg);
	if (status == -1)
		perror("Error en comando SOUND_PCM_WRITE_RATE");
	if (arg != valor->v_muestreo)
	{
		fprintf (stderr,"Velocidad de muestreo no soportada. Se programó %d\n",arg);
		valor->v_muestreo = arg;
	}
	return;
}