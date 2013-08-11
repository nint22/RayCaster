/***************************************************************
 
 RayCaster - A Simple C++ Ray Caster Game
 Copyright 2013 Core S2 - See License.txt for details
 
 This source file is developed and maintained by:
 + Jeremy Bridon jbridon@cores2.com
 
 File: VectorMath.cpp/h
 Desc: 2D and 3D vector class implementations; also supports
 generic vector maths, like dot-product and cross-product. Originally
 derived from my old project DwarfCraft, from Magi3:
 
 https://code.google.com/p/dwarfcraft/source/browse/Magi3/Vector2.h
 https://code.google.com/p/dwarfcraft/source/browse/Magi3/Vector3.h

***************************************************************/

#ifndef __VECTORMATH_H__
#define __VECTORMATH_H__

#include <math.h>

// Vector2 class; A two-dimensional vector class
template <typename Type> class Vector2
{
public:

    // Constructor
    Vector2()
    {
        x = (Type)0;
        y = (Type)0;
    }

    // Constructor
    Vector2(Type x, Type y)
    {
        this->x = x;
        this->y = y;
    }

    // Copy constructor
    Vector2(const Vector2 &obj)
    {
        x = obj.x;
        y = obj.y;
    }

    // Deconstructor
    ~Vector2()
    {
        // Nothing to do...
    }

    // Overloaded '=' operator
    Vector2 operator= (const Vector2 &obj)
    {
        x = obj.x;
        y = obj.y;
        return *this;
    }

    // Overloaded '+' operator
    Vector2 operator+ (const Vector2 &obj)
    {
        return Vector2<Type>(x + obj.x, y + obj.y);
    }

    // Overloaded '-' operator
    Vector2 operator- (const Vector2 &obj)
    {
        return Vector2<Type>(x - obj.x, y - obj.y);
    }

    // Overloaded '*' operator
    Vector2 operator* (const Vector2 &obj)
    {
        return Vector2<Type>(x * obj.x, y * obj.y);
    }

    // Overloaded '/' operator
    Vector2 operator/ (const Vector2 &obj)
    {
        return Vector2<Type>(x / obj.x, y / obj.y);
    }

    // Overloaded '+=' operator
    Vector2& operator+= (const Vector2 &obj)
    {
        *this = *this + obj;
        return *this;
    }

    // Overloaded '-=' operator
    Vector2& operator-= (const Vector2 &obj)
    {
        *this = *this - obj;
        return *this;
    }

    // Overloaded '*=' operator
    Vector2& operator*= (const Vector2 &obj)
    {
        *this = *this * obj;
        return *this;
    }

    // Overloaded '/=' operator
    Vector2& operator/= (const Vector2 &obj)
    {
        *this = *this / obj;
        return *this;
    }
    
    // Overloaded '+=' operator
    Vector2& operator+= (const Type &obj)
    {
        this->x += obj;
        this->y += obj;
        return *this;
    }
    
    // Overloaded '-=' operator
    Vector2& operator-= (const Type &obj)
    {
        this->x -= obj;
        this->y -= obj;
        return *this;
    }
    
    // Overloaded '*=' operator
    Vector2& operator*= (const Type &obj)
    {
        this->x *= obj;
        this->y *= obj;
        return *this;
    }
    
    // Overloaded '/=' operator
    Vector2& operator/= (const Type &obj)
    {
        this->x /= obj;
        this->y /= obj;
        return *this;
    }
    
    // overloaded '==' operator
    bool operator== (const Vector2 &obj)
    {
        if(obj.x == x && obj.y == y)
            return true;
        else
            return false;
    }

    // Returns the length of this vector
    double GetLength()
    {
        double X = (double)x;
        double Y = (double)y;
        return sqrt(X*X + Y*Y);
    }
    
    // Normalizes this vector
    void Normalize()
    {
        Type length = GetLength();
        x /= length;
        y /= length;
    }

    // Public types
    Type x, y;

};

// Cross product function
template <typename Type> Type Vector2Cross(Vector2<Type> VectorU, Vector2<Type> VectorV)
{
    // Output scaler
    return VectorU.x * VectorV.y - VectorU.y * VectorV.x;
}

// Dot product function
template <typename Type> double Vector2Dot(Vector2<Type> VectorU, Vector2<Type> VectorV)
{
    // Output vector
    double Total = 0.0f;
    
    // Set the vector
    Total += double(VectorU.x) * double(VectorV.x);
    Total += double(VectorU.y) * double(VectorV.y);
    
    // Return vector
    return Total;
}

// Vector3 class; A three-dimensional vector class
template <typename Type> class Vector3
{
public:
    
    // Constructor
    Vector3()
    {
        x = (Type)0;
        y = (Type)0;
        z = (Type)0;
    }
    
    // Constructor
    Vector3(Type x, Type y, Type z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    
    // Copy constructor
    Vector3(const Vector3 &obj)
    {
        x = obj.x;
        y = obj.y;
        z = obj.z;
    }
    
    // Deconstructor
    ~Vector3()
    {
        // Nothing to do...
    }
    
    // Overloaded '=' operator
    Vector3 operator= (const Vector3 &obj)
    {
        x = obj.x;
        y = obj.y;
        z = obj.z;
        return *this;
    }
    
    /*** Scalars ***/
    
    // Overloaded '+' operator
    Vector3 operator+ (const Type &obj)
    {
        return Vector3<Type>(x + obj, y + obj, z + obj);
    }
    
    // Overloaded '-' operator
    Vector3 operator- (const Type &obj)
    {
        return Vector3<Type>(x - obj, y - obj, z - obj);
    }
    
    // Overloaded '*' operator
    Vector3 operator* (const Type &obj)
    {
        return Vector3<Type>(x * obj, y * obj, z * obj);
    }
    
    // Overloaded '/' operator
    Vector3 operator/ (const Type &obj)
    {
        return Vector3<Type>(x / obj, y / obj, z / obj);
    }
    
    // Overloaded '+=' operator
    Vector3& operator+= (const Type &obj)
    {
        *this = *this + Vector3<Type>(obj, obj, obj);
        return *this;
    }
    
    // Overloaded '-=' operator
    Vector3& operator-= (const Type &obj)
    {
        *this = *this - Vector3<Type>(obj, obj, obj);
        return *this;
    }
    
    // Overloaded '*=' operator
    Vector3& operator*= (const Type &obj)
    {
        *this = *this * Vector3<Type>(obj, obj, obj);
        return *this;
    }
    
    // Overloaded '/=' operator
    Vector3& operator/= (const Type &obj)
    {
        *this = *this / Vector3<Type>(obj, obj, obj);
        return *this;
    }    
    
    /*** Non-scalars ***/
    
    // Overloaded '+' operator
    Vector3 operator+ (const Vector3 &obj)
    {
        return Vector3<Type>(x + obj.x, y + obj.y, z + obj.z);
    }
    
    // Overloaded '-' operator
    Vector3 operator- (const Vector3 &obj)
    {
        return Vector3<Type>(x - obj.x, y - obj.y, z - obj.z);
    }

    // Overloaded '-' operator for unary purposes:
    friend Vector3 operator- (const Vector3 &obj)
    {
        return Vector3<Type>(-obj.x, -obj.y, -obj.z);
    }
    
    // Overloaded '*' operator
    Vector3 operator* (const Vector3 &obj)
    {
        return Vector3<Type>(x * obj.x, y * obj.y, z * obj.z);
    }
    
    // Overloaded '/' operator
    Vector3 operator/ (const Vector3 &obj)
    {
        return Vector3<Type>(x / obj.x, y / obj.y, z / obj.z);
    }

    // Overloaded '+=' operator
    Vector3& operator+= (const Vector3 &obj)
    {
        *this = *this + obj;
        return *this;
    }
    
    // Overloaded '-=' operator
    Vector3& operator-= (const Vector3 &obj)
    {
        *this = *this - obj;
        return *this;
    }
    
    // Overloaded '*=' operator
    Vector3& operator*= (const Vector3 &obj)
    {
        *this = *this * obj;
        return *this;
    }
    
    // Overloaded '/=' operator
    Vector3& operator/= (const Vector3 &obj)
    {
        *this = *this / obj;
        return *this;
    }
    
    // overloaded '==' operator
    bool operator== (const Vector3 &obj)
    {
        if(obj.x == x && obj.y == y && obj.z == z)
            return true;
        else
            return false;
    }
    
    // overloaded '!=' operator
    bool operator!= (const Vector3 &obj)
    {
        if(obj.x != x || obj.y != y || obj.z != z)
            return true;
        else
            return false;
    }
    
    // Returns the length of this vector
    double GetLength()
    {
        double X = (double)x;
        double Y = (double)y;
        double Z = (double)z;
        return sqrt(X*X + Y*Y + Z*Z);
    }
    
    // Normalizes this vector
    void Normalize()
    {
        Type length = GetLength();
        x /= length;
        y /= length;
        z /= length;
    }
    
    // Public types
    Type x, y, z;
};

// Cross product function
template <typename Type> Vector3<Type> Vector3Cross(Vector3<Type> VectorU, Vector3<Type> VectorV)
{
    // Output vector
    Vector3<Type> OutVector;
    
    // Set the vector
    OutVector.x = (VectorU.y * VectorV.z - VectorU.z * VectorV.y);
    OutVector.y = (VectorU.z * VectorV.x - VectorU.x * VectorV.z);
    OutVector.z = (VectorU.x * VectorV.y - VectorU.y * VectorV.x);
    
    // Return vector
    return OutVector;
}

// Dot product function
template <typename Type> double Vector3Dot(Vector3<Type> VectorU, Vector3<Type> VectorV)
{
    // Output vector
    double Total = 0.0f;
    
    // Set the vector
    Total += double(VectorU.x) * double(VectorV.x);
    Total += double(VectorU.y) * double(VectorV.y);
    Total += double(VectorU.z) * double(VectorV.z);
    
    // Return vector
    return Total;
}

// Convert a vector3 from float to integer (rounding down)
inline Vector3<int> Vector3ftoi(Vector3<float> fV)
{
    return Vector3<int>((int)fV.x, (int)fV.y, (int)fV.z);
}

// Convert a vector3 from integer to float
inline Vector3<float> Vector3itof(Vector3<int> iV)
{
    return Vector3<float>((float)iV.x, (float)iV.y, (float)iV.z);
}

// Short-hand pre-typed vectors
typedef Vector2<int> Vector2i;
typedef Vector3<int> Vector3i;

typedef Vector2<float> Vector2f;
typedef Vector3<float> Vector3f;

typedef Vector2<double> Vector2d;
typedef Vector3<double> Vector3d;

#endif
