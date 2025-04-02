#include "fft.h"

arm_cfft_radix4_instance_f32 scfft;


uint16_t ADC_1_Value_DMA[FFT_LENGTH]={0};//ADC采样数组


float fft_inputbuf[FFT_LENGTH * 2];  // FFT 复数输入（实部+虚部）
float fft_outputbuf[FFT_LENGTH];     // FFT 幅值输出

SignalInfo_t signal_info_real;//最后的信号信息

void FFT_INIT(void)
{
	  arm_cfft_radix4_init_f32(&scfft,FFT_LENGTH,0,1);
	
		HAL_TIM_Base_Start(&htim3);//tim3定时触发ADc
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_1_Value_DMA, FFT_LENGTH);
	
}



//分析FFT输出数组
SignalInfo_t analyze_fft(float *fft_outputbuf, int fft_length, float fs) 
{
    SignalInfo_t signal_info = {0};

    // 直流分量
    signal_info.dc_offset = fft_outputbuf[0] / fft_length;

    float max_value1 = 0, max_value2 = 0;
    int max_index1 = 0, max_index2 = 0;

    
    for (int i = 1; i < fft_length / 2; i++) {
        if (fft_outputbuf[i] > max_value1) {
            max_value2 = max_value1;  
            max_index2 = max_index1;
            max_value1 = fft_outputbuf[i];
            max_index1 = i;
        } else if (fft_outputbuf[i] > max_value2) {
            max_value2 = fft_outputbuf[i];
            max_index2 = i;
        }
    }

    // 计算谐波频率
    signal_info.main_freq = (float)max_index1 * fs / fft_length;
    signal_info.second_freq = (float)max_index2 * fs / fft_length;

    // 计算幅值
    signal_info.main_amp = (2.0f * max_value1) / fft_length;
    signal_info.second_amp = (2.0f * max_value2) / fft_length;

    return signal_info;
}

//转化ADC 然后进行FFT分析
SignalInfo_t capture_and_FFT(uint32_t FFT_LENGTH_, uint16_t *ADC_1_Value_DMA, float SAMPLING_RATE_)
{
	  SignalInfo_t signal_info = {0};

	  

    arm_cfft_radix4_instance_f32 scfft;  // FFT 结构体
    

    // 初始化 FFT 结构
    arm_cfft_radix4_init_f32(&scfft, FFT_LENGTH_, 0, 1);

    // 整理 FFT 输入数组
    for (uint32_t i = 0; i < FFT_LENGTH_; i++) {
        fft_inputbuf[2 * i] = ADC_1_Value_DMA[i];  // 实部
        fft_inputbuf[2 * i + 1] = 0;              // 虚部
    }

    // 执行 FFT
    arm_cfft_radix4_f32(&scfft, fft_inputbuf);

    // 计算幅值
    arm_cmplx_mag_f32(fft_inputbuf, fft_outputbuf, FFT_LENGTH_);

    // 进行频谱分析（你自己的 analyze_fft 函数）
    signal_info = analyze_fft(fft_outputbuf, FFT_LENGTH_, SAMPLING_RATE_);
     
		return signal_info;
}





