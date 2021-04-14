#include <define.h>

#ifndef MYCALLBACK
#define MYCALLBACK
//--------------

#include <opencv2/core/core_c.h>
#include <opencv2/core/types_c.h>
#include <opencv2/highgui/highgui_c.h>
//--------------

#define PLAY "src/Cliente/functions/audio/data/PLAY.png"
#define PAUSE "src/Cliente/functions/audio/data/PAUSE.png"

void myCallback(int event, int x, int y, int flags, void *data);

#endif

#define TEMP_FILE "temp"
typedef struct
	{
		int v_muestreo;
		int t_muestra;
		int c_canales;

	} audio_params;

void play(const char* trackName);
void set_audio_params (int fd, audio_params * valor);