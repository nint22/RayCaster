/***************************************************************
 
 RayCaster - A Simple C++ Ray Caster Game
 Copyright 2013 Core S2 - See License.txt for details
 
***************************************************************/

// Main include
#include "SimpleJSON.h"
using namespace SimpleJSON;

// Win32: suppress the security warning associated with C-string functions
#ifdef WIN32
    #pragma warning(disable : 4996)
#endif

/*** Forward Declare ***/

static char* GetToken( FILE* fileHandle );
static void UngetToken( FILE* fileHandle, char** tokenString );
static bool ParseString( const char* tokenString );
static bool ParseNumber( const char* tokenString, float* fValue );
static bool ParseBoolean( const char* tokenString, bool* boolValueOut );
static bool ParseValue( FILE* fileHandle, JsonValue** valueOut );
static bool ParseObject( FILE* fileHandle, JsonObject** objectOut );
static bool ParseArray( FILE* fileHandle, JsonArray** arrayOut );

/*** Private ***/

// Given the file handle, return the next token; skips whitespaces, etc.
// Note that the returned pointer is malloc'ed, so make sure to release it!
static char* GetToken( FILE* fileHandle )
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
// Note that the given token string will also be deleted through a delete[]
static void UngetToken( FILE* fileHandle, char** tokenString )
{
    // Put string back on track
    ungetc( ' ', fileHandle );
    const int tokenLength = strlen(*tokenString);
    for(int i = tokenLength - 1; i >= 0; i--)
        ungetc( (*tokenString)[i], fileHandle );

    // Release full string
    delete[] tokenString;
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
static bool ParseNumber( const char* tokenString, float* fValue )
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
    *fValue = 1.0f;
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
static bool ParseValue( FILE* fileHandle, JsonValue** valueOut )
{
    // Initialize working memory
    *valueOut = NULL;
    float fValue;
    bool bValue;
    JsonObject* jsonObject = NULL;
    JsonArray* jsonArray = NULL;

    // Peek (and then un-get) the string token
    char* tokenString = GetToken(fileHandle);
    
    // Given the token, figure out the type and parse it appropriately
    if( ParseString(tokenString) )
    {
        *valueOut = new JsonValue( std::string(tokenString) );
        delete[] tokenString;
    }
    else if( ParseNumber(tokenString, &fValue) )
    {
        *valueOut = new JsonValue( fValue );
        delete[] tokenString;
    }
    else if( ParseBoolean(tokenString, &bValue) )
    {
        *valueOut = new JsonValue( bValue );
        delete[] tokenString;
    }

    // To test for object / array, we must put back the string
    else
    {
        UngetToken(fileHandle, &tokenString);

        if( ParseObject(fileHandle, &jsonObject) )
        {
            *valueOut = new JsonValue( jsonObject );
        }
        else if( ParseArray(fileHandle, &jsonArray) )
        {
            *valueOut = new JsonValue( jsonArray );
        }
    }
    
    // Done; return true (success) if the valueOut is set
    return ((*valueOut) != NULL) ? true : false;
}

// Parse the given object stream; returns true on success, false on error
// Does recursive parsing as needed
static bool ParseObject( FILE* fileHandle, JsonObject** objectOut )
{
    // Initialize working memory
    bool isValid = true;
    *objectOut = new JsonObject();
    
    // Must start with '{'
    char* tokenString = GetToken( fileHandle );
    if( tokenString == NULL || tokenString[0] != '{' )
    {
        delete[] tokenString;
        isValid = false;
    }
    
    // Keep reading
    while( isValid )
    {
        // We're expecting a "key-string : value" format
        tokenString = GetToken( fileHandle );
        bool isObjectEnd = (tokenString != NULL && tokenString[0] == '}');
        bool isKeyString = ParseString( tokenString );
        std::string KeyString = tokenString;
        delete[] tokenString;

        // If we're at the end of the key-value list, break-out!
        if( isObjectEnd )
            break;

        // Expecting a colon (key:value)
        tokenString = GetToken( fileHandle );
        bool isColon = (tokenString != NULL && tokenString[0] == ':');
        delete[] tokenString;

        // Expecting a value (could be object, string, array, etc.)
        JsonValue* jsonValue;
        bool isValue = ParseValue( fileHandle, &jsonValue );

        // Error checking
        if( !isKeyString || !isColon || !isValue )
        {
            isValue = false;
        }
        // Post result
        else
        {
            (*objectOut)->insert( JsonObjectPair(KeyString, jsonValue) );
        }
    }

    // Release buffer if on error
    if(!isValid)
    {
        // Release map
        for(JsonObject::iterator it = (*objectOut)->begin(); it != (*objectOut)->end(); ++it)
        {
            delete it->second;
        }

        delete *objectOut;
        *objectOut = NULL;
    }
    return isValid;
}

// Parse the given array stream; returns true on success, false on error
// Does recursive parsing as needed
static bool ParseArray( FILE* fileHandle, JsonArray** arrayOut )
{
    // Initialize working memory
    bool isValid = true;
    *arrayOut = new JsonArray();

    // Must start with '[' character
    char* tokenString = GetToken( fileHandle );
    if( tokenString == NULL || tokenString[0] != '[' )
    {
        delete[] tokenString;
        return false;
    }

    // Go through the array...
    while( isValid )
    {
        // Peek at end for end-condition
        tokenString = GetToken( fileHandle );
        if( tokenString == NULL )
        {
            isValid = false;
            break;
        }
        // End of array, break out!
        else if( tokenString[0] == ']' )
        {
            delete[] tokenString;
            break;
        }

        // Put the value back, in case it's data for the next array
        UngetToken( fileHandle, &tokenString );

        // Parse the object in question
        JsonValue* jsonValue;
        if(!ParseValue(fileHandle, &jsonValue))
        {
            isValid = false;
            break;
        }

        // Save off object
        (*arrayOut)->push_back( jsonValue );

        // Peek at the next token, reading off any comma-characters
        tokenString = GetToken( fileHandle );
        if( tokenString != NULL && tokenString[0] == ',')
        {
            delete[] tokenString;
        }
        // Some sort of error
        else if(tokenString == NULL)
        {
            isValid = false;
            break;
        }
        // Else, put back token for next read
        else
        {
            UngetToken( fileHandle, &tokenString );
        }
    }

    // Release buffer if on error
    if(!isValid)
    {
        // Release array
        while( !(*arrayOut)->empty() )
        {
            JsonValue* jsonValue = (*arrayOut)->back();
            (*arrayOut)->pop_back();

            delete jsonValue;
        }

        delete *arrayOut;
        *arrayOut = NULL;
    }
    return isValid;
}

/*** Public ***/

JsonValue::JsonValue()
{
    m_type = JsonType_Null;
    memset( this, 0, sizeof(this) );
}

JsonValue::JsonValue( const bool givenBool )
{
    m_type = JsonType_Bool;
    m_data.m_boolean = givenBool;
}

JsonValue::JsonValue( JsonArray* /*gifted*/ givenArray )
{
    m_type = JsonType_Array;
    m_data.m_array = givenArray;
}

JsonValue::JsonValue( JsonObject* /*gifted*/ givenObject )
{
    m_type = JsonType_Object;
    m_data.m_object = givenObject;
}

JsonValue::JsonValue( const float givenFloat )
{
    m_type = JsonType_Number;
    m_data.m_number = givenFloat;
}

JsonValue::JsonValue( const std::string& givenString )
{
    m_type = JsonType_String;
    m_data.m_string = new std::string(givenString);
}

JsonValue::~JsonValue()
{
    if( m_type == JsonType_String && m_data.m_string != NULL )
    {
        delete m_data.m_string;
    }
    else if( m_type == JsonType_Object && m_data.m_object != NULL )
    {
        // Release all key->value pairs
        for(JsonObject::iterator it = m_data.m_object->begin(); it != m_data.m_object->end(); ++it)
        {
            delete it->second;
        }
        delete m_data.m_object;
    }
    else if( m_type == JsonType_Array && m_data.m_array != NULL )
    {
        // Release all elements
        while( !m_data.m_array->empty() )
        {
            JsonValue* jsonValue = m_data.m_array->back();
            m_data.m_array->pop_back();

            delete jsonValue;
        }
        delete m_data.m_array;
    }
}

bool ParseSimpleJSON( FILE* fileHandle, JsonValue* rootValue )
{
    // All valid JSON files start as anonymous root-objects or root-arrays
    char* firstToken = GetToken( fileHandle );
    if(firstToken == NULL)
        return false;

    bool isObject = (firstToken[0] == '{');
    bool isArray = (firstToken[0] == '[');
    bool isValid = false;

    UngetToken( fileHandle, &firstToken );

    if( isObject )
    {
        rootValue->m_type = JsonType_Object;
        isValid = ParseObject(fileHandle, &(rootValue->m_data.m_object));
    }
    else if( isArray )
    {
        rootValue->m_type = JsonType_Array;
        isValid = ParseArray(fileHandle, &(rootValue->m_data.m_array));
    }

    return isValid;
}
