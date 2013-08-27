/***************************************************************
 
 RayCaster - A Simple C++ Ray Caster Game
 Copyright 2013 Core S2 - See License.txt for details
 
 This source file is developed and maintained by:
 + Jeremy Bridon jbridon@cores2.com
 
 File: SimpleJSON.cpp/h
 Desc: Simple JSON parsing custom code-base. Note that the
 implementation focuses on supporting arbitrary-large data, which
 means new / delete is heavily used. Also note that many of the
 containers are standard C++ STL classes.

***************************************************************/

#ifndef __SIMPLEJSON_H__
#define __SIMPLEJSON_H__

// Forward declaration
#include <map>
#include <vector>
#include <string>

// The entire code-base resides here
namespace SimpleJSON
{

// Type of the value
enum JsonType
{
    JsonType_Null,
    JsonType_Bool,
    JsonType_Array,
    JsonType_Object,
    JsonType_Number,
    JsonType_String,
};

// Typedef our custom string-value mapping
struct JsonValue;
typedef std::map< std::string, JsonValue* > JsonObject;         // Map of key-string => values
typedef std::pair< std::string, JsonValue* > JsonObjectPair;    // Key definition for the above
typedef std::vector< JsonValue* > JsonArray;                    // Array of values

// Data structure
struct JsonValue
{
    // Type & data
    JsonType m_type;
    union {
        bool m_boolean;
        JsonArray* m_array;
        JsonObject* m_object;
        float m_number;
        std::string* m_string;
    } m_data;

    // Initialize data to NULL
    JsonValue();

    // Helpful type-specific initializers
    // Note that for array & object, the pointers are gifted and thus are destroyed here
    JsonValue( const bool givenBool );
    JsonValue( JsonArray* givenArray );
    JsonValue( JsonObject* givenObject );
    JsonValue( const float givenFloat );
    JsonValue( const std::string& givenString );

    // Clean release (recursive)
    ~JsonValue();
};

// Given a file handle, parse and return an STL map
bool ParseSimpleJSON( FILE* fileHandle, JsonValue* rootValue );

} // End of namespace

#endif
