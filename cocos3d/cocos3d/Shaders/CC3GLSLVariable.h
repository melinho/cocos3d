/*
 * CC3GLSLVariable.h
 *
 * cocos3d 2.0.0
 * Author: Bill Hollings
 * Copyright (c) 2011-2013 The Brenwill Workshop Ltd. All rights reserved.
 * http://www.brenwill.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * http://en.wikipedia.org/wiki/MIT_License
 */

/** @file */	// Doxygen marker


#import "CC3Foundation.h"
#import "CC3OpenGLESUtility.h"
#import "CC3Matrix4x4.h"

@class CC3GLProgram;


/**
 * Indicates the scope of a GLSL variable.
 *
 * GLSL variable are automatically populated prior to drawing. This enumeration indicates
 * when and how often the variable needs to be populated.
 *
 * Most GLSL variables need to be populated anew as each node is drawn. But some variables, such
 * as lighting or camera content only needs to be populated once each time the scene is drawn,
 * and some other variables, such as bone matrices, need to be populated on each draw call.
 */
typedef enum {
	kCC3GLSLVariableScopeUnknown = 0,	/**< The scope of the variable is unknown. */
	kCC3GLSLVariableScopeScene,			/**< The scope of the variable is the entire scene. */
	kCC3GLSLVariableScopeNode,			/**< The scope of the variable is the current node. */
	kCC3GLSLVariableScopeDraw,			/**< The scope of the variable is the current draw call. */
} CC3GLSLVariableScope;

/** Returns a string representation of the specified GLSL variable scope. */
NSString* NSStringFromCC3GLSLVariableScope(CC3GLSLVariableScope scope);


#pragma mark -
#pragma mark CC3GLSLVariable

/**
 * Represents a variable used in a GLSL shader program. Different subclasses are used for
 * uniform variables and attribute variables.
 *
 * A variable may contain an int or float scalar, an int or float vector, a float matrix,
 * or an array of any of those types, as indicated by the type and size properties.
 */
@interface CC3GLSLVariable : NSObject <NSCopying> {
	CC3GLProgram* _program;
	NSString* _name;
	GLenum _type;
	GLenum _semantic : 16;
	GLint _location : 16;
	GLuint _index : 12;
	CC3GLSLVariableScope _scope : 4;
	GLint _size : 8;
	GLuint _semanticIndex : 8;
}

/** The GL program object containing this variable. */
@property(nonatomic, assign, readonly) CC3GLProgram* program;

/**
 * The index of this variable within the GL program object.
 * This is distinct from the location property.
 */
@property(nonatomic, assign, readonly) GLuint index;

/** 
 * The location of this variable within the GL program object.
 * This is distinct from the index property.
 */
@property(nonatomic, assign, readonly) GLint location;

/** The name of this variable in the GLSL shader source code. */
@property(nonatomic, retain, readonly) NSString* name;

/**
 * Returns a symbolic constant indicating the type of content held by this variable.
 *
 * The returned value depends on the type of variable being tracked, as determined by the
 * subclass. See the notes for this property in each subclass for more specific information.
 */
@property(nonatomic, readonly) GLenum type;

/** 
 * Returns the size of the variable content, in units of the type indicated by the type property.
 *
 * If the variable is declared as an array in the GLSL program, this property will return
 * the size of that array, otherwise it will return the value 1.
 */
@property(nonatomic, readonly) GLint size;

/**
 * A symbolic constant indicating the semantic meaning of this variable.
 *
 * The value of this property is typically one of values in the CC3Semantic enumeration,
 * but an application can define and use additional semantics beyond the values defined
 * by CC3Semantic. Additional semantics defined by the application should fall with the
 * range defined by the kCC3SemanticAppBase and kCC3SemanticMax constants, inclusively.
 *
 * The initial value of this property is kCC3SemanticNone.
 */
@property(nonatomic, assign) GLenum semantic;

/**
 * When the semantic refers to an element of a structure that may have multiple instances,
 * this property indicates to which instance this variable refers.
 *
 * This property is a zero-based index. For variables that do not appear in multiple structures,
 * this property will always be zero.
 *
 * As an example, there may be mulitple lights in a scene, each tracked in the GLSL by a
 * structure, one element of which might be the diffuse color property. For the variable
 * associated with the diffuse color of the third light, the value of the semantic property
 * would be kCC3SemanticLightColorDiffuse and the value of this property would be 2.
 *
 * On the other hand, for variables that represent an array of non-structure values, there will
 * only be one instance of the variable, but the size property of that variable will indicate
 * how many values are being managed by that single variable. For these types of variables,
 * the value of this property will always be zero.
 * 
 * As an example of an array of scalar values. If a single GLSL variable is defined to track the
 * ambient color of multiple lights (ie- defined as an array of vec4), the value of the semantic
 * property would be kCC3SemanticLightColorDiffuse, the value of the size property would be the
 * size of the array as defined in the GLSL, and the value of this property would be zero.
 * In this case, the values of the diffuse colors of all lights will be set as a single array.
 *
 * The initial value of this property is zero.
 */
@property(nonatomic, assign) GLuint semanticIndex;

/**
 * Indicates the scope of a GLSL variable.
 *
 * GLSL variable are automatically populated prior to drawing. This property indicates
 * when and how often the variable needs to be populated.
 *
 * Most GLSL variables need to be populated anew as each node is drawn. But some variables, such
 * as lighting or camera content only needs to be populated once each time the scene is drawn,
 * and some other variables, such as bone matrices, need to be populated on each draw call.
 */
@property(nonatomic, assign) CC3GLSLVariableScope scope;


#pragma mark Allocation and initialization

/** Initializes this instance at the specified index within the specified program. */
-(id) initInProgram: (CC3GLProgram*) program atIndex: (GLuint) index;

/** Allocates and initializes an autoreleased instance at the specified index within the specified program. */
+(id) variableInProgram: (CC3GLProgram*) program atIndex: (GLuint) index;

/**
 * Returns a newly allocated (retained) copy of this instance. The new copy will be an instance
 * of the specified class, whose content is copied from this instance.
 *
 * Care should be taken when choosing the class to be instantiated. If the class is different
 * than that of this instance, the populateFrom: method of that class must be compatible with
 * the contents of this instance.
 *
 * As with all copy behaviour, the returned instance is retained. It is the responsiblity of the
 * invoker to manage the lifecycle of the returned instance and perform the corresponding invocation
 * of the release method at the appropriate time.
 *
 * Subclasses that extend copying should not override this method, but should override the
 * populateFrom: method instead.
 */
-(id) copyAsClass: (Class) aClass;

/**
 * Returns a newly allocated (retained) copy of this instance. The new copy will be an instance
 * of the specified class, whose content is copied from this instance.
 *
 * Care should be taken when choosing the class to be instantiated. If the class is different
 * than that of this instance, the populateFrom: method of that class must be compatible with
 * the contents of this instance.
 *
 * As with all copy behaviour, the returned instance is retained. It is the responsiblity of the
 * invoker to manage the lifecycle of the returned instance and perform the corresponding invocation
 * of the release method at the appropriate time.
 *
 * Subclasses that extend copying should not override this method, but should override the
 * populateFrom: method instead.
 */
-(id) copyWithZone: (NSZone*) zone asClass: (Class) aClass;

/**
 * Template method that populates this instance from the specified other instance.
 *
 * This method is invoked automatically during object copying via the copy or copyWithZone: method.
 * In most situations, the application should use the copy method, and should never need to invoke
 * this method directly.
 *
 * Subclasses that add additional instance state (instance variables) should extend copying by
 * overriding this method to copy that additional state. Superclass that override this method should
 * be sure to invoke the superclass implementation to ensure that superclass state is copied as well.
 */
-(void) populateFrom: (CC3GLSLVariable*) another;

/** Returns a detailed description of this instance. */
-(NSString*) fullDescription;

@end


#pragma mark -
#pragma mark CC3GLSLAttribute

/** Represents an attribute variable used in a GLSL shader program.  */
@interface CC3GLSLAttribute : CC3GLSLVariable

/**
 * Returns a symbolic constant indicating the type of content held by this variable.
 *
 * The returned value will be one of the following symbolic constants:
 *   - GL_FLOAT, GL_FLOAT_VEC2, GL_FLOAT_VEC3, GL_FLOAT_VEC4,
 *   - GL_FLOAT_MAT2, GL_FLOAT_MAT3, or GL_FLOAT_MAT4
 */
@property(nonatomic, readonly) GLenum type;

@end


#pragma mark -
#pragma mark CC3GLSLUniform

/** Represents a uniform variable used in a GLSL shader program.  */
@interface CC3GLSLUniform : CC3GLSLVariable {
	size_t _varLen;
	GLvoid* _varValue;
}

/**
 * Returns a symbolic constant indicating the type of content held by this variable.
 *
 * The returned value will be one of the following symbolic constants:
 *   - GL_FLOAT, GL_FLOAT_VEC2, GL_FLOAT_VEC3, GL_FLOAT_VEC4,
 *   - GL_INT, GL_INT_VEC2, GL_INT_VEC3, GL_INT_VEC4,
 *   - GL_BOOL, GL_BOOL_VEC2, GL_BOOL_VEC3, GL_BOOL_VEC4,
 *   - GL_FLOAT_MAT2, GL_FLOAT_MAT3, GL_FLOAT_MAT4,
 *   - GL_SAMPLER_2D, GL_SAMPLER_CUBE
 */
@property(nonatomic, readonly) GLenum type;


#pragma mark Accessing uniform values

/**
 * Sets the value of this uniform to the specified value.
 *
 * If this uniform has been declared as an array, this method sets the value of the first
 * element in the array.
 *
 * The type property of this instance can be any value other than one of matrix types.
 * If the type property indicates an integer type, the float is converted to an integer.
 * If the type property indicates a vector type with more than one component, the second
 * and third components are set to zero and the fourth component is set to one.
 */
-(void) setFloat: (GLfloat) value;

/**
 * Sets the element at the specified index in this uniform to the specified value.
 *
 * The specified index must be less than the value of the size property. This method may
 * still be used when this uniform has not been declared as an array. In this case, the
 * value of the size property will be one, and so the specified index must be zero.
 *
 * The type property of this instance can be any value other than one of matrix types.
 * If the type property indicates an integer type, the float is converted to an integer.
 * If the type property indicates a vector type with more than one component, the second
 * and third components are set to zero and the fourth component is set to one.
 */
-(void) setFloat: (GLfloat) value at: (GLuint) index;

/**
 * Sets the value of this uniform to the specified value.
 *
 * If this uniform has been declared as an array, this method sets the value of the first
 * element in the array.
 *
 * The type property of this instance can be any value other than one of matrix types.
 * If the type property indicates an integer type, the floats are converted to integers.
 * If the type property indicates a scalar, the X component of the specified point is used.
 * If the type property indicates a vector type with more than two components, the third
 * component is set to zero and the fourth component is set to one.
 */
-(void) setPoint: (CGPoint) value;

/**
 * Sets the element at the specified index in this uniform to the specified value.
 *
 * The specified index must be less than the value of the size property. This method may
 * still be used when this uniform has not been declared as an array. In this case, the
 * value of the size property will be one, and so the specified index must be zero.
 *
 * The type property of this instance can be any value other than one of matrix types.
 * If the type property indicates an integer type, the floats are converted to integers.
 * If the type property indicates a scalar, the X component of the specified point is used.
 * If the type property indicates a vector type with more than two components, the third
 * component is set to zero and the fourth component is set to one.
 */
-(void) setPoint: (CGPoint) value at: (GLuint) index;

/**
 * Sets the value of this uniform to the specified value.
 *
 * If this uniform has been declared as an array, this method sets the value of the first
 * element in the array.
 *
 * The type property of this instance can be any value other than one of matrix types.
 * If the type property indicates an integer type, the floats are converted to integers.
 * If the type property indicates a scalar, the X component of the specified vector is used.
 * If the type property indicates a vector type with fewer than three components, the X & Y
 * components will be used. If the type property indicates a vector type with more than three
 * components, fourth component is set to one.
 */
-(void) setVector: (CC3Vector) value;

/**
 * Sets the element at the specified index in this uniform to the specified value.
 *
 * The specified index must be less than the value of the size property. This method may
 * still be used when this uniform has not been declared as an array. In this case, the
 * value of the size property will be one, and so the specified index must be zero.
 *
 * The type property of this instance can be any value other than one of matrix types.
 * If the type property indicates an integer type, the floats are converted to integers.
 * If the type property indicates a scalar, the X component of the specified vector is used.
 * If the type property indicates a vector type with fewer than three components, the X & Y
 * components will be used. If the type property indicates a vector type with more than three
 * components, fourth component is set to one.
 */
-(void) setVector: (CC3Vector) value at: (GLuint) index;

/**
 * Sets the value of this uniform to the specified value.
 *
 * If this uniform has been declared as an array, this method sets the value of the first
 * element in the array.
 *
 * The type property of this instance can be any value other than one of matrix types.
 * If the type property indicates an integer type, the floats are converted to integers.
 * If the type property indicates a scalar, the X component of the specified vector is used.
 * If the type property indicates a vector type with fewer than four components, the X & Y,
 * or X, Y & Z components are used.
 */
-(void) setVector4: (CC3Vector4) value;

/**
 * Sets the element at the specified index in this uniform to the specified value.
 *
 * The specified index must be less than the value of the size property. This method may
 * still be used when this uniform has not been declared as an array. In this case, the
 * value of the size property will be one, and so the specified index must be zero.
 *
 * The type property of this instance can be any value other than one of matrix types.
 * If the type property indicates an integer type, the floats are converted to integers.
 * If the type property indicates a scalar, the X component of the specified vector is used.
 * If the type property indicates a vector type with fewer than four components, the X & Y,
 * or X, Y & Z components are used.
 */
-(void) setVector4: (CC3Vector4) value at: (GLuint) index;

/**
 * Sets the value of this uniform to the specified value.
 *
 * If this uniform has been declared as an array, this method sets the value of the first
 * element in the array.
 *
 * The type property of this instance can be any value other than one of matrix types.
 * If the type property indicates an integer type, the floats are converted to integers.
 * If the type property indicates a scalar, the X component of the specified vector is used.
 * If the type property indicates a vector type with fewer than four components, the X & Y,
 * or X, Y & Z components are used.
 */
-(void) setQuaternion: (CC3Quaternion) value;

/**
 * Sets the element at the specified index in this uniform to the specified value.
 *
 * The specified index must be less than the value of the size property. This method may
 * still be used when this uniform has not been declared as an array. In this case, the
 * value of the size property will be one, and so the specified index must be zero.
 *
 * The type property of this instance can be any value other than one of matrix types.
 * If the type property indicates an integer type, the floats are converted to integers.
 * If the type property indicates a scalar, the X component of the specified vector is used.
 * If the type property indicates a vector type with fewer than four components, the X & Y,
 * or X, Y & Z components are used.
 */
-(void) setQuaternion: (CC3Quaternion) value at: (GLuint) index;

/**
 * Sets the value of this uniform to the specified value.
 *
 * If this uniform has been declared as an array, this method sets the value of the first
 * element in the array.
 *
 * The type property of this instance must be GL_FLOAT_MAT3.
 */
-(void) setMatrix3x3: (CC3Matrix3x3*) value;

/**
 * Sets the element at the specified index in this uniform to the specified value.
 *
 * The specified index must be less than the value of the size property. This method may
 * still be used when this uniform has not been declared as an array. In this case, the
 * value of the size property will be one, and so the specified index must be zero.
 *
 * The type property of this instance must be GL_FLOAT_MAT3.
 */
-(void) setMatrix3x3: (CC3Matrix3x3*) value at: (GLuint) index;

/**
 * Sets the 4x4 value of this uniform from the specified 4x3 value, adding the last identity row.
 *
 * If this uniform has been declared as an array, this method sets the value of the first
 * element in the array.
 *
 * The type property of this instance must be GL_FLOAT_MAT4.
 */
-(void) setMatrix4x3: (CC3Matrix4x3*) value;

/**
 * Sets the 4x4 element at the specified index in this uniform to the specified 4x3 value,
 * adding the last identity row.
 *
 * The specified index must be less than the value of the size property. This method may
 * still be used when this uniform has not been declared as an array. In this case, the
 * value of the size property will be one, and so the specified index must be zero.
 *
 * The type property of this instance must be GL_FLOAT_MAT4.
 */
-(void) setMatrix4x3: (CC3Matrix4x3*) value at: (GLuint) index;

/**
 * Sets the value of this uniform to the specified value.
 *
 * If this uniform has been declared as an array, this method sets the value of the first
 * element in the array.
 *
 * The type property of this instance must be GL_FLOAT_MAT4.
 */
-(void) setMatrix4x4: (CC3Matrix4x4*) value;

/**
 * Sets the element at the specified index in this uniform to the specified value.
 *
 * The specified index must be less than the value of the size property. This method may
 * still be used when this uniform has not been declared as an array. In this case, the
 * value of the size property will be one, and so the specified index must be zero.
 *
 * The type property of this instance must be GL_FLOAT_MAT4.
 */
-(void) setMatrix4x4: (CC3Matrix4x4*) value at: (GLuint) index;

/**
 * Sets the value of this uniform to the specified value.
 *
 * If this uniform has been declared as an array, this method sets the value of the first
 * element in the array.
 *
 * The type property of this instance can be any value other than one of matrix types.
 * If the type property indicates a float type, the integer is converted to a float.
 * If the type property indicates a vector type with more than one component, the
 * remaining components are set to zero.
 */
-(void) setInteger: (GLint) value;

/**
 * Sets the element at the specified index in this uniform to the specified value.
 *
 * The specified index must be less than the value of the size property. This method may
 * still be used when this uniform has not been declared as an array. In this case, the
 * value of the size property will be one, and so the specified index must be zero.
 *
 * The type property of this instance can be any value other than one of matrix types.
 * If the type property indicates a float type, the integer is converted to a float.
 * If the type property indicates a vector type with more than one component, the
 * remaining components are set to zero.
 */
-(void) setInteger: (GLint) value at: (GLuint) index;

/**
 * Sets the value of this uniform to the specified value.
 *
 * If this uniform has been declared as an array, this method sets the value of the first
 * element in the array.
 *
 * The type property of this instance can be any value other than one of matrix types.
 * If the type property indicates a float type, the integer is converted to a float.
 * If the type property indicates a scalar, the X component of the specified point is used.
 * If the type property indicates a vector type with more than two components, the
 * remaining components are set to zero.
 */
-(void) setIntPoint: (CC3IntPoint) value;

/**
 * Sets the element at the specified index in this uniform to the specified value.
 *
 * The specified index must be less than the value of the size property. This method may
 * still be used when this uniform has not been declared as an array. In this case, the
 * value of the size property will be one, and so the specified index must be zero.
 *
 * The type property of this instance can be any value other than one of matrix types.
 * If the type property indicates a float type, the integers are converted to floats.
 * If the type property indicates a scalar, the X component of the specified point is used.
 * If the type property indicates a vector type with more than two components, the
 * remaining components are set to zero.
 */
-(void) setIntPoint: (CC3IntPoint) value at: (GLuint) index;

/**
 * Sets the value of this uniform to the specified value.
 *
 * If this uniform has been declared as an array, this method sets the value of the first
 * element in the array.
 *
 * The type property of this instance can be any value other than one of matrix types.
 * If the type property indicates a float type, the integers are converted to floats.
 * If the type property indicates a scalar, the X component of the specified vector is used.
 * If the type property indicates a vector type with fewer than three components, the X & Y
 * components will be used. If the type property indicates a vector type with more than three
 * components, the fourth component is set to zero.
 */
-(void) setIntVector: (CC3IntVector) value;

/**
 * Sets the element at the specified index in this uniform to the specified value.
 *
 * The specified index must be less than the value of the size property. This method may
 * still be used when this uniform has not been declared as an array. In this case, the
 * value of the size property will be one, and so the specified index must be zero.
 *
 * The type property of this instance can be any value other than one of matrix types.
 * If the type property indicates a float type, the integers are converted to floats.
 * If the type property indicates a scalar, the X component of the specified vector is used.
 * If the type property indicates a vector type with fewer than three components, the X & Y
 * components will be used. If the type property indicates a vector type with more than three
 * components, fourth component is set to zero.
 */
-(void) setIntVector: (CC3IntVector) value at: (GLuint) index;

/**
 * Sets the value of this uniform to the specified value.
 *
 * If this uniform has been declared as an array, this method sets the value of the first
 * element in the array.
 *
 * The type property of this instance can be any value other than one of matrix types.
 * If the type property indicates a float type, the integers are converted to floats.
 * If the type property indicates a scalar, the X component of the specified vector is used.
 * If the type property indicates a vector type with fewer than four components, the X & Y,
 * or X, Y & Z components are used.
 */
-(void) setIntVector4: (CC3IntVector4) value;

/**
 * Sets the element at the specified index in this uniform to the specified value.
 *
 * The specified index must be less than the value of the size property. This method may
 * still be used when this uniform has not been declared as an array. In this case, the
 * value of the size property will be one, and so the specified index must be zero.
 *
 * The type property of this instance can be any value other than one of matrix types.
 * If the type property indicates a float type, the integers are converted to floats.
 * If the type property indicates a scalar, the X component of the specified vector is used.
 * If the type property indicates a vector type with fewer than four components, the X & Y,
 * or X, Y & Z components are used.
 */
-(void) setIntVector4: (CC3IntVector4) value at: (GLuint) index;

/**
 * Sets the value of this uniform to the specified value.
 *
 * If this uniform has been declared as an array, this method sets the value of the first
 * element in the array.
 *
 * The type property of this instance can be any value other than one of matrix types.
 * If the type property indicates a float type, the boolean is converted to a float.
 * If the type property indicates a vector type with more than one component, the
 * remaining components are set to zero.
 */
-(void) setBoolean: (BOOL) value;

/**
 * Sets the element at the specified index in this uniform to the specified value.
 *
 * The specified index must be less than the value of the size property. This method may
 * still be used when this uniform has not been declared as an array. In this case, the
 * value of the size property will be one, and so the specified index must be zero.
 *
 * The type property of this instance can be any value other than one of matrix types.
 * If the type property indicates a float type, the boolean is converted to a float.
 * If the type property indicates a vector type with more than one component, the
 * remaining components are set to zero.
 */
-(void) setBoolean: (BOOL) value at: (GLuint) index;

/**
 * Sets the value of this uniform to the specified value.
 *
 * If this uniform has been declared as an array, this method sets the value of the first
 * element in the array.
 *
 * The type property of this instance can be any value other than one of matrix types.
 * If the type property indicates an float type, the integers are normalized to floats between
 * 0 and 1. If the type property indicates a scalar, the R component of the specified color is
 * used. If the type property indicates a vector type with fewer than three components, the
 * R & G components are used. If the type property indicates a vector type with four components,
 * the A component is set to 255 (or 1 if float type).
 */
-(void) setColor: (ccColor3B) value;

/**
 * Sets the element at the specified index in this uniform to the specified value.
 *
 * The specified index must be less than the value of the size property. This method may
 * still be used when this uniform has not been declared as an array. In this case, the
 * value of the size property will be one, and so the specified index must be zero.
 *
 * The type property of this instance can be any value other than one of matrix types.
 * If the type property indicates an float type, the integers are normalized to floats between
 * 0 and 1. If the type property indicates a scalar, the R component of the specified color is
 * used. If the type property indicates a vector type with fewer than three components, the
 * R & G components are used. If the type property indicates a vector type with four components,
 * the A component is set to 255 (or 1 if float type).
 */
-(void) setColor: (ccColor3B) value at: (GLuint) index;

/**
 * Sets the value of this uniform to the specified value.
 *
 * If this uniform has been declared as an array, this method sets the value of the first
 * element in the array.
 *
 * The type property of this instance can be any value other than one of matrix types.
 * If the type property indicates an float type, the integers are normalized to floats between
 * 0 and 1. If the type property indicates a scalar, the R component of the specified color is
 * used. If the type property indicates a vector type with fewer than four components, the R & G,
 * or R, G & B components are used.
 */
-(void) setColor4B: (ccColor4B) value;

/**
 * Sets the element at the specified index in this uniform to the specified value.
 *
 * The specified index must be less than the value of the size property. This method may
 * still be used when this uniform has not been declared as an array. In this case, the
 * value of the size property will be one, and so the specified index must be zero.
 *
 * The type property of this instance can be any value other than one of matrix types.
 * If the type property indicates an float type, the integers are normalized to floats between
 * 0 and 1. If the type property indicates a scalar, the R component of the specified color is
 * used. If the type property indicates a vector type with fewer than four components, the R & G,
 * or R, G & B components are used.
 */
-(void) setColor4B: (ccColor4B) value at: (GLuint) index;

/**
 * Sets the value of this uniform to the specified value.
 *
 * If this uniform has been declared as an array, this method sets the value of the first
 * element in the array.
 *
 * The type property of this instance can be any value other than one of matrix types.
 * If the type property indicates an integer type, the floats are converted to integers
 * in the range 0 to 255. If the type property indicates a scalar, the R component of the
 * specified color is used. If the type property indicates a vector type with fewer than
 * four components, the R & G, or R, G & B components are used.
 */
-(void) setColor4F: (ccColor4F) value;

/**
 * Sets the element at the specified index in this uniform to the specified value.
 *
 * The specified index must be less than the value of the size property. This method may
 * still be used when this uniform has not been declared as an array. In this case, the
 * value of the size property will be one, and so the specified index must be zero.
 *
 * The type property of this instance can be any value other than one of matrix types.
 * If the type property indicates an integer type, the floats are converted to integers
 * in the range 0 to 255. If the type property indicates a scalar, the R component of the
 * specified color is used. If the type property indicates a vector type with fewer than
 * four components, the R & G, or R, G & B components are used.
 */
-(void) setColor4F: (ccColor4F) value at: (GLuint) index;

/** Sets the value of this uniform from the value of the specified uniform. */
-(void) setValueFromUniform: (CC3GLSLUniform*) uniform;

/**
 * Invoked after all of the content of the variable has been set using the set... methods,
 * in order to have the value of this variable set into the GL engine.
 *
 * The GL engine is only updated if the content of this variable has changed.
 * Returns whether the value has changed and was updated into the GL engine.
 *
 * The base implementation does nothing, and simply returns NO. Subclasses that actually
 * interact with the GL engine will override.
 *
 * This method is invoked automatically during uniform population.
 * The application normally never needs to invoke this method.
 */
-(BOOL) updateGLValue;

@end


#pragma mark -
#pragma mark CC3OpenGLESStateTrackerGLSLAttribute

/** Tracks the GL engine state for a attribute variable used in a GLSL shader program.  */
@interface CC3OpenGLESStateTrackerGLSLAttribute : CC3GLSLAttribute
@end


#pragma mark -
#pragma mark CC3OpenGLESStateTrackerGLSLUniform

/**
 * Tracks the GL engine state for a uniform variable used in a GLSL shader program.
 * Adds the ability to set the variable value in the GL engine. The GL state is tracked
 * and the GL engine is only updated if the setting the value actually changes the GL state.
 */
@interface CC3OpenGLESStateTrackerGLSLUniform : CC3GLSLUniform {
	GLvoid* _glVarValue;
}

@end

