/***************************************************************
 
 RayCaster - A Simple C++ Ray Caster Game
 Copyright 2013 Core S2 - See License.txt for details
 
***************************************************************/

#include "SimpleJSON.h"

// Includes
#include <map>
#include <stdio.h>
#include <ctype.h>
using namespace std;

/*** Private ***/

// Given the file handle, return the next token; skips whitespaces, etc.
// Note that the returned pointer is malloc'ed, so make sure to release it!
static const char* GetToken( FILE* fileHandle )
{
    static const int bufferLength = 512;
    char buffer[bufferLength] = { '\0' };

    int charCount = 0;
    int nextChar = 0;
    bool isStringLiteral = 0;

    while( (nextChar = fgetc(fileHandle)) != EOF )
    {
        // Skip if whitespace but not in a string literal
        if( !isStringLiteral && isspace(nextChar) )
            continue;

        // If it's an object, array, or key-value association character, read it off and bail out
        if( !isStringLiteral && (nextChar == '{' || nextChar == '}' || nextChar == '[' || nextChar == ']' || nextChar == ':') )
        {
            buffer[0] = nextChar;
            charCount = 1;
            break;
        }
        // Is it the start of a string literal?
        else if( !isStringLiteral && nextChar == '"' )
        {
            // Flip state on/off
            isStringLiteral = true;
            break;
        }
        // Read off the buffer
        else if( isStringLiteral )
        {
            // Escape-char sequence
            if( nextChar == '\\' )
            {
                int escapeChar = fgetc(fileHandle);
                switch(escapeChar)
                {
                    case '"':   nextChar = '"';     break;
                    case '\\':  nextChar = '\\';    break;
                    case '/':   nextChar = '/';     break;
                    case 'b':   nextChar = '\b';    break;
                    case 'f':   nextChar = '\f';    break;
                    case 'n':   nextChar = '\n';    break;
                    case 'r':   nextChar = '\r';    break;
                    case 't':   nextChar = '\t';    break;
                    default:    return false; // Fails out
                }
            }

            // Place character, handled if control character
            buffer[charCount++] = nextChar;
        }
    }

    // Null-terminate string and duplicate the result onto the buffer..
    buffer[charCount] = '\0';
    return strdup(buffer);
}

// Puts the token back into the given file-buffer; correctly pushes the letters in the
// correct way so that re-reading the buffer, through GetToken(...), will return correct results
static void UngetToken( FILE* fileHandle, const char* tokenString )
{
    ungetc( ' ', fileHandle );
    const int tokenLength = strlen(tokenString);
    for(int i = tokenLength - 1; i >= 0; i--)
        ungetc( tokenString[i], fileHandle );
}

// Given a string, returns true if it is a well-formed JSON string, else false on error
static bool ParseString( const char* tokenString )
{
    if(tokenString == NULL)
        return false;
    
    int tokenLength = strlen(tokenString);
    return ( tokenString[0] == '"' && tokenString[tokenLength - 1] == '"' );
}

// Given a string, parse as a float, with the syntax of "[+|-]DIGITS[.][DIGITS][(e|E)[+|-]DIGITS]"
// Returns true on success, false on failure / error
// Note that this can even parse a number-string like "-3.01967E-20"
// Note that we could also use atof(...) from the cstd-lib, but that's no fun..
static bool ParseNumber( const char* tokenString )
{
    const int tokenLength = strlen(tokenString);
    int tokenIndex = 0;
    if(tokenLength <= 0)
        return false;
    
    bool isNegative = false;
    if( tokenString[0] == '-' )
    {
        tokenIndex++;
        isNegative = true;
    }
    else if( tokenString[0] == '+' )
    {
        tokenIndex++;
    }

    // Read in the integer up to the end / dot
    int tokenEndIndex = tokenLength - 1;
    for(int i = tokenIndex; i < tokenLength; i++)
    {
        if( isdigit(tokenString[i]) )
        {
            tokenEndIndex = i - 1;
            break;
        }
    }

    // Re-read from right to left to parse
    int factor = 1;
    int sum = 0;
    for(int i = tokenEndIndex; i >= tokenIndex; i--)
    {
        sum += (tokenString[i] - '0') * factor;
        factor *= 10;
    }
    tokenIndex = tokenEndIndex;

    // TODO: Read off the rest of the string, and deal with the 'e'/'E' character..
}

static bool ParseBoolean( const char* tokenString, bool* boolValueOut )
{
    const int tokenLength = strlen(tokenString);
    char buffer[512];

    if(tokenLength > 5)
        return false;

    strncpy(buffer, tokenString, tokenLength + 1);
    for(int i = 0; i < tokenLength; i++)
        buffer[i] = tolower( buffer[i] );

    if( strcmp(buffer, "true") == 0 )
    {
        *boolValueOut = true;
        return true;
    }
    else if( strcmp(buffer, "false") == 0 )
    {
        *boolValueOut = false;
        return true;
    }
    else
    {
        return false;
    }
}

// Returns true on success, false on failure / error
// Posts the result on the given ObjectData object
static bool ParseValue( FILE* fileHandle, const ObjectData* objectData )
{
    // Peek (and then un-get) the string token
    const char* tokenString = GetToken(fileHandle);

    // Given the token, figure out the type and parse it appropriately
    if( ParseString(tokenString) )
    {
        objectData->m_type = ObjectType_String;
        objectData->m_data.m_string = std::string(tokenString);
    }
    else if( ParseNumber(tokenString, &objectData->m_data.m_number) )
    {
        objectData->m_type = ObjectType_Number;
    }
    else if( ParseBoolean(tokenString, &objectData->m_data.m_boolean) )
    {
        objectData->m_type = ObjectType_Bool;
    }

    // Special case: object / array
    else if( ParseObject(fileHandle, objectData) )
    {
        // TODO
    }
    else if( ParseArray(fileHandle, objectData) )
    {
        // TODO
    }

    // Done!
    UngetToken(fileHandle, tokenString);
}

// Parse the given object stream; returns true on success, false on error
// Does recursive parsing as needed
static bool ParseObject( FILE* fileHandle, const ObjectData* objectDataOut )
{
    char* token = NextToken( fileHandle );
    if(token == NULL)
        return false;

    bool objectStarts = (token[0] == '{');
    free(token);

    // Object starts
    while(objectStarts)
    {
        // We're expecting a key-string
        token = NextToken( fileHandle );
        bool isKeyString = ParseString( token );
        std::string KeyString = token;
        free(token);

        // Expecting a colon (key:value)
        token = NextToken( fileHandle );
        bool isColon = (token != NULL && token[0] == ':');
        free(token);

        // Expecting a value (could be object, string, array, etc.)
        ObjectData objectData;
        bool isValue = ParseValue( fileHandle, &objectData );

        // Error checking
        if( !isKeyString || !isColon || !isValue )
            return false;

    }
}

// Parse the given array stream; returns true on success, false on error
// Does recursive parsing as needed
static bool ParseArray( FILE* fileHandle, const ObjectArray* objectArrayOut )
{
    // Arrays will always have the pattern of "value, ..., value]"
    objectArrayOut->m_type = ObjectType_Array;

    // Go through the array...
    while( true )
    {
        // Parse the object
        ObjectData objectData;
        if(!ParseValue(fileHandle, &objectData))
            return false;

        // Save off object
        objectArrayOut->m_data.m_array.push_back( objectData );

        // What's next? End of array or another object?
        const char* tokenString = GetToken( fileHandle );
        if( tokenString == NULL )
        {
            return false;
        }
        // End of array, break out!
        else if( tokenString[0] == ']' )
        {
            free(tokenString);
            break;
        }
        // Just continue
        else if( tokenString[0] == ',' )
        {
            free(tokenString);
        }
        // Unknown token...
        else
        {
            free(tokenString);
            return false;
        }
    }

    // All good!
    return true;
}

/*** Public ***/

bool SimpleJSON::ParseSimpleJSON( FILE* fileHandle, const ObjectData* rootObject )
{
    // All valid JSON files start as anonymous root-objects or root-arrays
    char* firstToken = GetToken( fileHandle );
    bool isGood = false;

    if(firstToken == NULL)
    {
        return false;
    }
    else if(firstToken[0] == '{')
    {
        rootObject->m_type = ObjectType_Object;
        isGood = ParseObject(fileHandle, rootObject->m_data.m_object);
    }
    else if(firstToken[0] == '[')
    {
        rootObject->m_type = ObjectType_Array;
        isGood = ParseArray(fileHandle, rootObject->m_data.m_array);
    }

    free(firstToken);
    return isGood;
}
