#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <endian.h>
#include <AL/al.h>
#include <AL/alc.h>

#include "convertflac.h"

void list_audio_devices(const ALCchar *devices);
void manage_error();
char *get_PCM_info(const char *filename, uint32_t *dataSize, uint32_t *freq);

int main(int argc, char **argv)
{
  const char *inFilename = "infile.flac";
  const char *outFilename = "out.wav";

  printf("Hello FLAC World\n");

  printf("Starting convert flac to wav");
  convertFlacToWav(inFilename, outFilename);
  printf("Conversion done!");

  uint32_t dataSize;
  uint32_t freq;
  char *data = get_PCM_info(outFilename, &dataSize, &freq);

  list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));

  ALCdevice *device;

  device = alcOpenDevice(NULL);
  if (!device)
  {
    printf("No device \n");
    exit(-1);
  }
  else
  {
    ALCcontext *context;
    context = alcCreateContext(device, NULL);
    if (!alcMakeContextCurrent(context))
    {
      printf("No context");
      exit(-1);
    }
    else
    {
      ALuint buffer;
      ALuint source;

      alGenBuffers((ALuint)1, &buffer);
      manage_error();

      alBufferData(buffer, AL_FORMAT_STEREO16, data, dataSize, freq);
      manage_error();

      alGenSources((ALuint)1, &source);
      manage_error();

      alSourcei(source, AL_BUFFER, buffer);
      manage_error();

      alSourcePlay(source);
      manage_error();
    }
  }
}

void list_audio_devices(const ALCchar *devices)
{
  const ALCchar *device = devices, *next = devices + 1;
  size_t len = 0;

  printf("Devices list:\n");
  printf("----------\n");
  while (device && *device != '\0' && next && *next != '\0')
  {
    printf("Mon device est %s\n", device);
    len = strlen(device);
    device += (len + 1);
    next += (len + 2);
  }
  printf("----------\n");
}

void manage_error()
{
  ALCenum error;

  error = alGetError();
  if (error != AL_NO_ERROR)
  {
    printf("Une petit erreur");
  }
}

char *get_PCM_info(const char *filename, uint32_t *dataSize, uint32_t *freq)
{
  FILE *fichier = fopen(filename, "rb");

  if (fichier == NULL)
  {
    printf("No file");
    exit(-1);
  }
  else
  {
    char typeblocid[5];
    typeblocid[4] = 0;
    fread(typeblocid, 4, 1, fichier);
    printf("FileTypeBlocId %s \n", typeblocid);

    uint32_t size = 0;
    fread(&size, 4, 1, fichier);
    printf("File size %0.2f \n", (float)size / 1024 / 1024);

    char format[5];
    format[4] = 0;
    fread(format, 4, 1, fichier);
    printf("Format %s \n", format);

    char formatBlocId[5];
    formatBlocId[4] = 0;
    fread(formatBlocId, 4, 1, fichier);
    printf("Format Bloc Id %s \n", formatBlocId);

    uint32_t blocSize = 0;
    fread(&blocSize, 4, 1, fichier);
    printf("Bloc size %u \n", blocSize);

    uint16_t audioFormat = 0;
    fread(&audioFormat, 2, 1, fichier);
    if (audioFormat == 1)
    {
      printf("Format PCM \n");
    }
    else
    {
      printf("Format inconnu \n");
      exit(1);
    }

    uint16_t nbCanaux = 0;
    fread(&audioFormat, 2, 1, fichier);
    printf("Nombre de canaux %u \n", audioFormat);

    fread(freq, 4, 1, fichier);
    printf("Frequence %u \n", *freq);

    uint32_t bytePerSec;
    fread(&bytePerSec, 4, 1, fichier);
    printf("Bytes per seconds %u \n", bytePerSec);

    uint16_t bytePerBloc = 0;
    fread(&bytePerBloc, 2, 1, fichier);
    printf("Byte per bloc %u \n", bytePerBloc);

    uint16_t bitsPerSample = 0;
    fread(&bitsPerSample, 2, 1, fichier);
    printf("bitsPerSample %u \n", bitsPerSample);

    char constdata[5];
    constdata[4] = 0;
    fread(constdata, 4, 1, fichier);
    printf("Data Bloc Id %s \n", constdata);

    fread(dataSize, 4, 1, fichier);
    printf("DataSize %u \n", *dataSize);

    char *data = malloc(*dataSize);
    fread(data, 1, *dataSize, fichier);

    return data;
  }
}