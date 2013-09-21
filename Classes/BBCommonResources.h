#ifndef BeatBlaster_BBCommonResources_h
#define BeatBlaster_BBCommonResources_h

#include<iostream>
#include<fstream>
#include "cocos2d.h"
USING_NS_CC;
#include <sys/timeb.h>


typedef struct  WAV_HEADER{
    char                RIFF[4];        // RIFF Header      Magic header
    uint32_t       ChunkSize;      // RIFF Chunk Size  
    char                WAVE[4];        // WAVE Header      
    char                fmt[4];         // FMT header       
    uint32_t       Subchunk1Size;  // Size of the fmt chunk                                
    uint16_t      AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM 
    uint16_t      NumOfChan;      // Number of channels 1=Mono 2=Sterio                   
    uint32_t       SamplesPerSec;  // Sampling Frequency in Hz                             
    uint32_t       bytesPerSec;    // bytes per second 
    uint16_t      blockAlign;     // 2=16-bit mono, 4=16-bit stereo 
    uint16_t      bitsPerSample;  // Number of bits per sample      
    char                Subchunk2ID[4]; // "data"  string   
    uint32_t       Subchunk2Size;  // Sampled data length    

}wav_hdr; 


//sysutils

float BB_getMillis();
CCPoint BBconvert_AngleAndMagnitudeToVelocityXY(float degrees, float velocityMagnitude);
bool isMovingOffScreen(CCPoint pos, CCPoint vel, CCSize size);


#endif