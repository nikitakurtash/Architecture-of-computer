#include "stdafx.h"
#include <conio.h> 
#include <windows.h>
#include "inc\fmod.hpp"
#include "inc\fmod_errors.h"
#include <iostream>
#pragma comment(lib, "fmodex_vc.lib")
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
FMOD_RESULT result;
FMOD::System * system; 
result = FMOD::System_Create(&system);
result = system->init(2, FMOD_INIT_NORMAL, 0);
FMOD::Sound * sound; // sound
FMOD::Channel * channel; // sound channel
result = system->createSound("ns.mp3", FMOD_3D, 0, &sound); // creating sound
result = system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
//result = channel->setPaused(false);
//getch();

FMOD_VECTOR front = { 0.0f, 0.0f, -1.0f };
FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };

for (int i = -100000; i < 1000000; i++) {
    FMOD_VECTOR listenerpos = { 0.0f, 0.0f, 0.0f };
    FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };
    result = system->set3DListenerAttributes(0, &listenerpos, &velocity, &front, &up);
    result = system->update();

    std::cout << i << ", ";
}

 //playing sound (assigning it to a channel)
 //channel->setPaused(false);


//system("pause");
//getch();

	return 0;
}




//
//#include <math.h>
//#include "stdafx.h"
//#include <iostream>
//#include <conio.h> 
//#include <stdio.h>
//#include <windows.h>
//#include "inc\fmod.hpp"
//#include "inc\fmod_errors.h"
//#pragma comment(lib, "fmodex_vc.lib")
//using namespace std;
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//    FMOD::System* system;
//    FMOD::System_Create(&system);
//    system->init(16, FMOD_INIT_NORMAL, 0);
//    FMOD::Sound* sound;
//    FMOD::Channel* channel = NULL;
//    FMOD::DSP* dsp = 0;
//    int key = 0;
//    unsigned int version;
//    FMOD_RESULT res = system->set3DSettings(166, 0.2, -15);
//    res = system->getVersion(&version);
//    res = system->createSound("ns.mp3", FMOD_SOFTWARE, 0, &sound);
//    res = system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
//    res = system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
//
//    do
//    {
//        if (_kbhit())
//        {
//            key = _getch();
//
//            switch (key)
//            {
//            case ' ':
//            {
//                bool initialize;
//                res = dsp->getActive(&initialize);
//                if (initialize)
//                {
//                    res = dsp->remove();
//                    std::cout << "Pitch shift is disabled.\n";
//                }
//                else
//                {
//                    res = system->addDSP(dsp, 0);
//                    res = dsp->setParameter(FMOD_DSP_PITCHSHIFT_PITCH, 1.5f); 
//                    std::cout << "Pitch shift is enabled.\n";
//                }
//                break;
//            }
//            }
//        }
//
//        system->update();
//
//        {
//            bool paused = 0;
//            bool dspPitchShift_active;
//            dsp->getActive(&dspPitchShift_active);
//            if (channel)
//            {
//                res = channel->getPaused(&paused);
//            }
//        }
//
//    } while (key != 100);
//
//    res = sound->release();
//    res = system->close();
//    res = system->release();
//    return 0;
//}
