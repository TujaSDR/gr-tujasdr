//
//  tujavolk.h
//  libtujavolk
//
//  Created by Albin Stigö on 2018-12-21.
//  Copyright © 2018 Albin Stigo. All rights reserved.
//

#ifndef tujavolk_h
#define tujavolk_h

#include <stdint.h>
#include <arm_neon.h>
#include <volk/volk.h>

// int32_t to float32
static inline void
volk_32i_s32f_convert_32f_neon(float* outputVector,
                               const int32_t* inputVector,
                               const float scalar,
                               unsigned int num_points) {
    float* outputVectorPtr = outputVector;
    const int32_t* inputVectorPtr = inputVector;
    const float iScalar = 1.0 / scalar;
    unsigned int number;
    unsigned int quarter_points = num_points / 4;
    int32x4_t input_vec;
    float32x4_t ouput_vec, iscalar_vec;
    
    iscalar_vec = vdupq_n_f32(iScalar);
    
    for(number = 0; number < quarter_points; number++) {
        // load s32
        input_vec = vld1q_s32(inputVectorPtr);
        // convert s32 to f32
        ouput_vec = vcvtq_f32_s32(input_vec);
        // scale
        ouput_vec = vmulq_f32(ouput_vec, iscalar_vec);
        // store
        vst1q_f32(outputVectorPtr, ouput_vec);
        // move pointers ahead
        outputVectorPtr+=4;
        inputVectorPtr+=4;
    }
    
    // deal with the rest
    for(number = quarter_points * 4; number < num_points; number++) {
        *outputVectorPtr++ = ((float)(*inputVectorPtr++)) * iScalar;
    }
}

// float to int
static inline void
volk_32f_s32f_convert_32i_neon(int32_t* outputVector,
                               const float* inputVector,
                               const float scalar,
                               unsigned int num_points) {
    int32_t* outputVectorPtr = outputVector;
    const float* inputVectorPtr = inputVector;
    unsigned int number;
    unsigned int quarter_points = num_points / 4;
    float32x4_t input_vec, scalar_vec;
    int32x4_t ouput_vec;
    
    float min_val = -2147483648; // There seems to be a bug in volk kernels
    float max_val = 2147483647;  // regarding the lower bound?
    float r;
    
    scalar_vec = vdupq_n_f32(scalar);
    
    for(number = 0; number < quarter_points; number++) {
        // load f32
        input_vec = vld1q_f32(inputVectorPtr);
        // scale, this is saturating
        input_vec = vmulq_f32(input_vec, scalar_vec);
        // convert f32 to s32
        ouput_vec = vcvtq_s32_f32(input_vec);
        // store
        vst1q_s32(outputVectorPtr, ouput_vec);
        // move pointers ahead
        outputVectorPtr+=4;
        inputVectorPtr+=4;
    }
    
    // deal with the rest
    for(number = quarter_points * 4; number < num_points; number++) {
        r = *inputVectorPtr++ * scalar;
        if(r > max_val)
            r = max_val;
        else if(r < min_val)
            r = min_val;
        *outputVectorPtr++ = (int32_t)(r);
    }
}

// float32 to int16_t
static inline void
volk_32f_s32f_convert_16i_neon(int16_t* outputVector,
                               const float* inputVector,
                               const float scalar,
                               unsigned int num_points) {
    
    int16_t* outputVectorPtr = outputVector;
    const float* inputVectorPtr = inputVector;
    unsigned int number = 0;
    unsigned int quarter_points = num_points / 4;
    float32x4_t input_vec, scalar_vec;
    int32x4_t ouput32_vec;
    int16x4_t ouput16_vec;
    
    float min_val = -32768;
    float max_val = 32767;
    float r;
    
    scalar_vec = vdupq_n_f32(scalar);
    
    // I'm not sure if it's better to do 8 at at time?
    for(number = 0; number < quarter_points; number++) {
        input_vec = vld1q_f32(inputVectorPtr);
        // scale, this is saturating
        input_vec = vmulq_f32(input_vec, scalar_vec);
        // convert f32 to s32
        ouput32_vec = vcvtq_s32_f32(input_vec);
        // narrow to 16 bits
        ouput16_vec = vmovn_s32(ouput32_vec);
        // store
        vst1_s16(outputVectorPtr, ouput16_vec);
        // move pointers ahead
        outputVectorPtr+=4;
        inputVectorPtr+=4;
    }
    
    for(number = quarter_points * 4; number < num_points; number++) {
        r = *inputVectorPtr++ * scalar;
        if(r > max_val)
            r = max_val;
        else if(r < min_val)
            r = min_val;
        *outputVectorPtr++ = (int16_t)(r); // rintf??
    }
}

#endif /* tujavolk_h */

