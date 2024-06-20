// MIT License
//
// Copyright (c) 2024 Tomáš Mark
//

#ifndef __SOUNDS_H__
#define __SOUNDS_H__

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <stdio.h>
#include <string>
#include <chrono>
#include <thread>

class Sounds
{
public:
    Sounds(){};
    ~Sounds(){};
    
    // Metody třídy nejsou kompatibilní s ukazateli na funkce kvůli skrytému parametru this, který je předáván všem metodám třídy.
    static inline void data_callback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
    {
        ma_decoder *pDecoder = (ma_decoder *)pDevice->pUserData;
        if (pDecoder == NULL)
        {
            return;
        }

        ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

        (void)pInput;
    }

    int PlaySoundSync(std::string soundFilePath, int waitForPlayToFinishInMs)
    {
        ma_result result;
        ma_decoder decoder;
        ma_device_config deviceConfig;
        ma_device device;

        result = ma_decoder_init_file(soundFilePath.c_str(), NULL, &decoder);
        if (result != MA_SUCCESS)
        {
            printf("Could not load file: %s\n", soundFilePath.c_str());
            return -2;
        }

        deviceConfig = ma_device_config_init(ma_device_type_playback);
        deviceConfig.playback.format = decoder.outputFormat;
        deviceConfig.playback.channels = decoder.outputChannels;
        deviceConfig.sampleRate = decoder.outputSampleRate;
        deviceConfig.dataCallback = data_callback;
        deviceConfig.pUserData = &decoder;

        if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS)
        {
            printf("Failed to open playback device.\n");
            ma_decoder_uninit(&decoder);
            return -3;
        }

        if (ma_device_start(&device) != MA_SUCCESS)
        {
            printf("Failed to start playback device.\n");
            ma_device_uninit(&device);
            ma_decoder_uninit(&decoder);
            return -4;
        }

        // printf("Press Enter to quit...");
        // getchar();

        // required some time to play the sound until will be stopped
        std::this_thread::sleep_for(std::chrono::milliseconds(waitForPlayToFinishInMs));

        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);

        return 0;
    }

private:
};

#endif // __SOUNDS_H__