//
//  alsa.h
//  fcdsdr
//
//  Created by Albin Stigö on 21/11/2017.
//  Copyright © 2017 Albin Stigo. All rights reserved.
//

#pragma once

#include <alsa/asoundlib.h>
#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif
    
    // These are very convenient in loops and stuff
    typedef struct {
        int32_t l;
        int32_t r;
    } sample32_t;
    
    snd_pcm_t* alsa_pcm_handle(const char* pcm_name,
                               unsigned int channels,
                               unsigned int rate,
                               const unsigned int periods,
                               snd_pcm_uframes_t frames,
                               snd_pcm_format_t format,
                               snd_pcm_stream_t stream);
    
#ifdef __cplusplus
}
#endif
