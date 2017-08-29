/*
*	Jergus Lysy (374217)
*	Napisane a testovane v Dev-C++ 4.9.9.2
*	Iba CMD app
*/

#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <io.h>
#include "grfinger.h"

struct raw_image {
       char name[50];
       unsigned char *img;
       unsigned int width;
       unsigned int height;
       int res;
       unsigned char ftmpl[GR_MAX_SIZE_TEMPLATE];
       int ftmpl_size;
       char finger;
};

int contextId = 0;
int index = 0;
struct raw_image imgs[30];

void WriteEvent(char *idSensor, GRCAP_FINGER_EVENTS event)
{
	switch (event) {
		case GR_PLUG:
			printf("Sensor: %s Event: Plugged\n",idSensor);
            break;
		case GR_UNPLUG:
			printf("Sensor: %s Event: Unplugged\n",idSensor);
            break;
		case GR_FINGER_DOWN:
			printf("Sensor: %s Event: Finger placed\n",idSensor);
            break;
		case GR_FINGER_UP:
			printf("Sensor: %s Event: Finger removed\n",idSensor);
            break;
		case GR_IMAGE:
			printf("Sensor: %s Event: Image captured\n",idSensor);
            break;
		default:
			printf("Sensor: %s Event: %i\n",idSensor, event);            
	}
}

void CALLBACK FingerEventHandler(char* idSensor, GRCAP_FINGER_EVENTS event)
{
     WriteEvent(idSensor, event);
}

void CALLBACK ImageEventHandler(char* idSensor, unsigned int width, unsigned int height, unsigned char* rawImage, int res) 
{
     int ret;
     
     imgs[index].img = (unsigned char *) malloc(width * height + 100);
     if (imgs[index].img == NULL)
        exit(EXIT_FAILURE);
     memcpy(imgs[index].img, rawImage, width * height);
     imgs[index].width = width;
     imgs[index].height = height;
     imgs[index].res = res;
     
     imgs[index].ftmpl_size = GR_MAX_SIZE_TEMPLATE;
     ret = GrExtract(imgs[index].img, imgs[index].width, imgs[index].height, imgs[index].res,
           (char*)imgs[index].ftmpl, &imgs[index].ftmpl_size, GR_DEFAULT_CONTEXT);
     if (ret < 0) {
        printf("GrExtract returned %d\n", ret);
        imgs[index].ftmpl_size = 0;
     }
     WriteEvent(idSensor, GR_IMAGE);
     index++;
}

void CALLBACK StatusEventHandler(char* idSensor, GRCAP_STATUS_EVENTS event)
{
     WriteEvent(idSensor, event);
     if (event == GR_PLUG)
        GrCapStartCapture(idSensor, FingerEventHandler, ImageEventHandler); 
     else if (event == GR_UNPLUG)
         GrCapStopCapture(idSensor);
}

void comp_fprints()
{
     int in = 0;
     int score, match, i;
     char fin_in = 'A';
     
     while (in < index) {
           if (imgs[in].finger) {
              in++;
              continue;
           }
           printf("Finger %c\n", fin_in);
           imgs[in].finger = fin_in;
           for (i = in + 1; i < index; i++) {
               if (!imgs[i].finger) {
                  match = GrVerify((char*)imgs[in].ftmpl,(char*)imgs[i].ftmpl, &score, contextId);
                  if (match == GR_MATCH) {
                     imgs[i].finger = imgs[in].finger;
                     printf("\t%s\n", imgs[i].name);
                  }
               }
           }
           printf("\t%s\n", imgs[in].name);
           fin_in++;
           in++;
     }
}

int main(int argc, char **argv)
{
    struct _finddata_t fdata;
    char path[256];
    intptr_t fret;
    int ret, i;
    
    if (argc < 2) {
       printf("Need folder containing fingerprints as a parameter!\n");
       return EXIT_FAILURE;
    }
    
    ret = GrInitialize();
    if (ret != GR_OK) {
       printf("Invalid license!\n");
       return EXIT_FAILURE;
    }
    ret = GrCreateContext(&contextId);
    if (ret != GR_OK) {
       printf("GrCreateContext returned %d!\n", ret);
       return EXIT_FAILURE;
    }
    ret = GrCapInitialize(&StatusEventHandler);
    if (ret != GR_OK) {
       printf("GrCapInitialize returned %d!\n");
       return EXIT_FAILURE;
    }

    sprintf(path, "%s\\*.bmp", argv[1]);
    fret = _findfirst(path, &fdata);
    if (fret == -1) {
       printf("No suitable file found!\n");
       return EXIT_FAILURE;
    }

    do {
        strcpy(imgs[index].name, fdata.name);
        sprintf(path, "%s\\%s", argv[1], fdata.name);
        GrCapLoadImageFromFile(path, 500);
    } while (!_findnext(fret, &fdata));

    _findclose(fret);

    printf("\nComparing fingerprints...\n\n");
    comp_fprints();

    for (i = 0; i < index; i++)
        free(imgs[i].img);

    GrCapFinalize();
    GrDestroyContext(contextId);
    GrFinalize();
    system("PAUSE");
    
    return EXIT_SUCCESS;
}
