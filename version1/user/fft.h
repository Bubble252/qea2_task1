#ifndef FFT_H
#define FFT_H

#include "arm_math.h"
#include "main.h"
#include "tim.h"
#include "adc.h"

#define FFT_LENGTH		1024 		//FFT数组大小
#define SAMPLING_RATE 250000  // ADC采样频率
//fft分析结构体
typedef struct {
    float dc_offset;   // 直流分量
    float main_freq;   // 主谐波频率
    float main_amp;    // 主谐波幅值
    float second_freq; // 第二谐波频率
    float second_amp;  // 第二大幅值
} SignalInfo_t;

void FFT_INIT(void);



SignalInfo_t capture_and_FFT(uint32_t FFT_LENGTH_, uint16_t *ADC_1_Value_DMA, float SAMPLING_RATE_);

#endif