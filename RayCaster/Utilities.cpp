/***************************************************************
 
 RayCaster - A Simple C++ Ray Caster Game
 Copyright 2013 Core S2 - See License.txt for details
 
***************************************************************/

#include "Utilities.h"

void __UtilAssert(const char* FileName, int LineNumber, bool Assertion, const char* FailText, ...)
{
    // Validate assertion
    if(Assertion)
        return;
    
    // Validate the given error string
    if(FailText == NULL)
        FailText = "Undefined Error Text.";
    
    // Find the clean file name
    const char* CleanFileName = NULL;
    
    // Don't find the simple name if the string is null
    if(FileName != NULL)
    {
        // Thanks windows, for being such an evil file system
        // Note that the "+1" moves the pointer past the slash
        #ifdef _WIN32	
            CleanFileName = strrchr(FileName, '\\') + 1;
        #else
            CleanFileName = strrchr(FileName, '/') + 1;
        #endif
    }
    
    // Grab the variable argument list (See the "..." in the arg. list)
    va_list args;
    va_start(args, FailText);
    
    // Generate the failure string
    char FailString[256];
    vsprintf(FailString, FailText, args);
    
    // Release args list
    va_end(args);
    
    // Copy the given string
    char buffer[256];
    sprintf(buffer, "> Assertion in file %s:%d\n> Assertion message: \"%s\"\n", CleanFileName, LineNumber, FailString);
    
    // If windows... Write this out in a message box...
    #ifdef _WIN32
        
        // Present the message, break if user presses yes
        MessageBox(NULL, buffer, "Assertion Error", MB_ICONERROR);
        
    // Else, in a UNIX-like system... Write this via command line.
    #else
        
        // Write out to command line
        printf("> Magi3 - Assertion Error\n");
        printf("%s", buffer);
        
        // Print out the stack trace if this is GCC / glibc
        printf("> Dumping stack:\n\n");
        
        // Get the current backtrace
        void* backtrace_data[32];
        int backtrace_count;
        
        // Get and print
        backtrace_count = backtrace(backtrace_data, 32);
        backtrace_symbols_fd(backtrace_data, backtrace_count, STDOUT_FILENO);
        
    #endif
    
    // Exit the application
    exit(0);
}

// Static: it is an internal function
static void __UtilAssert(const char* FileName, int LineNumber, const char* FailText, ...)
{
    // Get multi-args list
    va_list args;
    va_start(args, FailText);
    
    // Force an assertion failure
    __UtilAssert(FileName, LineNumber, false, FailText, args);
    
    // Release args list
    va_end(args);
}

void UtilSleep(float SleepTime)
{
    #ifdef _WIN32
        Sleep(DWORD(1000.0f * SleepTime)); // Windows milli-second sleeps
    #else
        usleep(useconds_t(1000000.0f * SleepTime)); // Unix micro-second sleep
    #endif
}
