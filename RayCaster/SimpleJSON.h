/***************************************************************
 
 RayCaster - A Simple C++ Ray Caster Game
 Copyright 2013 Core S2 - See License.txt for details
 
 This source file is developed and maintained by:
 + Jeremy Bridon jbridon@cores2.com
 
 File: SimpleJSON.cpp/h
 Desc: Simple JSON parsing custom code-base. Note that the
 implementation focuses on supporting arbitrary-large data, which
 means malloc / free is heavly used. Also note that many of the
 containers are standard C++ STL classes.

***************************************************************/

#ifndef __SIMPLEJSON_H__
#define __SIMPLEJSON_H__

// Forward declaration
namespace std
{
    class map;
    class vector;
    class string;
}

// The entire code-base resides here
namespace SimpleJSON
{

// Object-type
enum ObjectType
{
    ObjectType_Null,
    ObjectType_Object,
    ObjectType_String,
    ObjectType_Number,
    ObjectType_Array,
    ObjectType_Bool,
};

// Typedef our custom string-value mapping
class ObjectData;
typedef std::map< std::string, ObjectData > ObjectMap;
typedef std::vector< ObjectData > ObjectArray;

// Data structure
class ObjectData
{
    ObjectType m_type;
    typedef union {
        ObjectData m_object;
        string m_string;
        float m_number;
        bool m_boolean;
        ObjectArray m_array;
    } m_data;
};

// Given a file handle, parse and return an STL map
ObjectData ParseSimpleJSON( FILE* fileHandle );

}

#endif
