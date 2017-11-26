#ifndef CONVERT_FLAC_H
#define CONVERT_FLAC_H

#include "share/compat.h"
#include "FLAC/stream_decoder.h"

static FLAC__StreamDecoderWriteStatus write_callback(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client_data);
static void metadata_callback(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data);
static void error_callback(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data);

static FLAC__uint64 total_samples = 0;
static unsigned sample_rate = 0;
static unsigned channels = 0;
static unsigned bps = 0;


void convert();



#endif