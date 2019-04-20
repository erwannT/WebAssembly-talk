#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include "share/compat.h"
#include "FLAC/stream_decoder.h"

#include "convertflac.h"

static FLAC__bool write_little_endian_uint16(FILE *f, FLAC__uint16 x)
{
    return fputc(x, f) != EOF &&
           fputc(x >> 8, f) != EOF;
}

static FLAC__bool write_little_endian_int16(FILE *f, FLAC__int16 x)
{
    return write_little_endian_uint16(f, (FLAC__uint16)x);
}

static FLAC__bool write_little_endian_uint32(FILE *f, FLAC__uint32 x)
{
    return fputc(x, f) != EOF &&
           fputc(x >> 8, f) != EOF &&
           fputc(x >> 16, f) != EOF &&
           fputc(x >> 24, f) != EOF;
}

void convertFlacToWav(const char* inFileName, const char* outFileName)
{
    FLAC__bool ok = true;
    FLAC__StreamDecoder *decoder = 0;
    FLAC__StreamDecoderInitStatus init_status;
    FILE *fout;

    if ((fout = fopen(outFileName, "wb")) == NULL)
    {
        printf( "ERROR: opening out.wav for output\n");
        exit(1);
    }

    if ((decoder = FLAC__stream_decoder_new()) == NULL)
    {
        printf( "ERROR: allocating decoder\n");
        fclose(fout);
        exit(1);
    }

    (void)FLAC__stream_decoder_set_md5_checking(decoder, true);

    init_status = FLAC__stream_decoder_init_file(decoder, inFileName, write_callback, metadata_callback, error_callback, /*client_data=*/fout);
    if (init_status != FLAC__STREAM_DECODER_INIT_STATUS_OK)
    {
        printf( "ERROR: initializing decoder: %s\n", FLAC__StreamDecoderInitStatusString[init_status]);
        ok = false;
    }

    if (ok)
    {
        ok = FLAC__stream_decoder_process_until_end_of_stream(decoder);
        printf("decoding: %s\n", ok ? "succeeded" : "FAILED");
        printf( "   state: %s\n", FLAC__StreamDecoderStateString[FLAC__stream_decoder_get_state(decoder)]);
    }

    FLAC__stream_decoder_delete(decoder);
    fclose(fout);
}

FLAC__StreamDecoderWriteStatus write_callback(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 *const buffer[], void *client_data)
{
    FILE *f = (FILE *)client_data;
    const FLAC__uint32 total_size = (FLAC__uint32)(total_samples * channels * (bps / 8));
    size_t i;

    (void)decoder;

    if (total_samples == 0)
    {
        fprintf(stderr, "ERROR: this example only works for FLAC files that have a total_samples count in STREAMINFO\n");
        return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
    }
    if (channels != 2 || bps != 16)
    {
        fprintf(stderr, "ERROR: this example only supports 16bit stereo streams\n");
        return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
    }
    if (frame->header.channels != 2)
    {
        fprintf(stderr, "ERROR: This frame contains %d channels (should be 2)\n", frame->header.channels);
        return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
    }
    if (buffer[0] == NULL)
    {
        fprintf(stderr, "ERROR: buffer [0] is NULL\n");
        return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
    }
    if (buffer[1] == NULL)
    {
        fprintf(stderr, "ERROR: buffer [1] is NULL\n");
        return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
    }

    /* write WAVE header before we write the first frame */
    if (frame->header.number.sample_number == 0)
    {
        if (
            fwrite("RIFF", 1, 4, f) < 4 ||
            !write_little_endian_uint32(f, total_size + 36) ||
            fwrite("WAVEfmt ", 1, 8, f) < 8 ||
            !write_little_endian_uint32(f, 16) ||
            !write_little_endian_uint16(f, 1) ||
            !write_little_endian_uint16(f, (FLAC__uint16)channels) ||
            !write_little_endian_uint32(f, sample_rate) ||
            !write_little_endian_uint32(f, sample_rate * channels * (bps / 8)) ||
            !write_little_endian_uint16(f, (FLAC__uint16)(channels * (bps / 8))) || /* block align */
            !write_little_endian_uint16(f, (FLAC__uint16)bps) ||
            fwrite("data", 1, 4, f) < 4 ||
            !write_little_endian_uint32(f, total_size))
        {
            fprintf(stderr, "ERROR: write error\n");
            return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
        }
    }

    /* write decoded PCM samples */
    for (i = 0; i < frame->header.blocksize; i++)
    {
        if (
            !write_little_endian_int16(f, (FLAC__int16)buffer[0][i]) || /* left channel */
            !write_little_endian_int16(f, (FLAC__int16)buffer[1][i])    /* right channel */
        )
        {
            fprintf(stderr, "ERROR: write error\n");
            return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
        }
    }

    return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

void metadata_callback(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data)
{
    (void)decoder, (void)client_data;

    /* print some stats */
    if (metadata->type == FLAC__METADATA_TYPE_STREAMINFO)
    {
        /* save for later */
        total_samples = metadata->data.stream_info.total_samples;
        sample_rate = metadata->data.stream_info.sample_rate;
        channels = metadata->data.stream_info.channels;
        bps = metadata->data.stream_info.bits_per_sample;

        fprintf(stderr, "sample rate    : %u Hz\n", sample_rate);
        fprintf(stderr, "channels       : %u\n", channels);
        fprintf(stderr, "bits per sample: %u\n", bps);
        fprintf(stderr, "total samples  : %llu \n", total_samples);
    }
}

void error_callback(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data)
{
    (void)decoder, (void)client_data;

    fprintf(stderr, "Got error callback: %s\n", FLAC__StreamDecoderErrorStatusString[status]);
}
