/***************************************************************
 
 RayCaster - A Simple C++ Ray Caster Game
 Copyright 2013 Core S2 - See License.txt for details
 
 This source file is developed and maintained by:
 + Jeremy Bridon jbridon@cores2.com
 
 File: Utilities.cpp/h
 Desc: Dozen helpful structures or functions for general use.
 Read the inline function definition for details on the purpose
 and usage of each func.

***************************************************************/

#ifndef __UTILITIES_H__
#define __UTILITIES_H__

// Window includes
#ifdef _WIN32
    
    #pragma warning(disable:4996)
    #include <windows.h>
    
    // Need to define infinity
    #ifndef INFINITY
        #define INFINITY (std::numeric_limits<float>::infinity())
    #endif
    
// Linux includes
#elif __linux__

	// Todo...

// Apple/OSX includes
#elif __APPLE__

	// Todo...

#endif

// OpenGL Uiser Interface extensions
#include "SDL.h"

// Standard includes
#include <sys/types.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

// C++ specific includes
#include <iostream>
using namespace std;

// Define PI
#define UtilPI (3.141592653589793)

// A custom assert function-like macro
// Replace the UtilAssert call with a location setting (file/line) function and then correct util assert
// This is done via a Variadic Macro; C99 standard
// Nov-6-2010: Small bug fix, removed semi-colon on the end
#define UtilAssert(...) __UtilAssert(__FILE__, __LINE__, __VA_ARGS__)
#define UtilAbort(...) __UtilAssert(__FILE__, __LINE__, false, __VA_ARGS__)

// Internal assertion functions
// Checks if the assertion is true (if given); if not a critical error is posted. See __UtilAssert(...)
// If Assertion is false, it posts the failure string, file name, and line number in a pop-up window or stdout
// There is a variable param list at the end to support printf-like arguments
void __UtilAssert(const char* FileName, int LineNumber, bool Assertion, const char* FailText, ...);

// Unix and Win32 wrappers for high-resolution sleep
// Takes in a fraction of a second (as a float)
void UtilSleep(float SleepTime);

// Random number generator; "Linear Congruential Generator"
// Based on http://en.wikipedia.org/wiki/Linear_congruential_generator
class UtilRand
{
public:
    
    // Constructs a random number generator (RNG) based on a seed
    UtilRand(unsigned int seed);
    UtilRand(const char* seed);
    
    // Returns a random number between 0 and UINT_MAX
    unsigned int Rand();
    
    // Get the seed that was generated
    inline unsigned int GetSeed() { return seed; }
    
private:
    
    // The internal seed / previous numbers
    unsigned int seed;
    
    // Constants based on the LCG algo (comes from GCC's chosen numbers)
    static const unsigned int a = 1103515245;
    static const unsigned int c = 12345;
};

// Windows version
#if _WIN32

    // High-resolution clock (In fraction of seconds)
    class UtilHighresClock
    {
    public:
        
        UtilHighresClock(bool StartTiming = false)
        {
            // Setup the high-resolution timer
            QueryPerformanceFrequency(&TicksPerSec);
            
            // High-resolution timers, iniitalized timer
            QueryPerformanceCounter(&startTime);
            
            // Start off the timer
            Start(); Stop();
            
            // Star the timer if we want
            if(StartTiming)
                Start();
        }
        
        void Stop()
        {
            QueryPerformanceCounter(&endTime);
        }
        
        // In seconds
        float GetTime()
        {
            // Measure the cycle time
            cpuTime.QuadPart = endTime.QuadPart - startTime.QuadPart;
            return (float)cpuTime.QuadPart / (float)TicksPerSec.QuadPart;
        }
        
        void Start()
        {
            QueryPerformanceCounter(&startTime);
        }
        
    private:
        LARGE_INTEGER TicksPerSec;
        LARGE_INTEGER startTime, endTime, cpuTime;
    };

// UNIX / Linux / OSX interface
#else

    // High-resolution clock (In fraction of seconds)
    class UtilHighresClock
    {
    public:
        
        UtilHighresClock(bool StartTiming = false)
        {
            // High-resolution timers, iniitalized timer
            gettimeofday(&startTime, NULL);
            
            // Start off the timer
            Start(); Stop();
            
            // Star the timer if we want
            if(StartTiming)
                Start();
        }
        
        void Stop()
        {
            gettimeofday(&endTime, NULL);
        }
        
        float GetTime()
        {
            // Measure the cycle time (Convert from micro to milli to seconds)
            // If the end time is one second more than the start time, modify the
            float secondDif = float(endTime.tv_sec - startTime.tv_sec);
            return float(float(endTime.tv_usec) / 1000.0f - float(startTime.tv_usec) / 1000.0f) / 1000.0f + secondDif;
        }
        
        void Start()
        {
            gettimeofday(&startTime, NULL);
        }
        
    private:
        struct timeval startTime, endTime;
    };

// End of UNIX version
#endif

#endif
