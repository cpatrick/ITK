/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    wrapWrapperBase.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#ifndef _wrapWrapperBase_h
#define _wrapWrapperBase_h

#include "wrapUtils.h"
#include "wrapArgument.h"

#include <vector>
#include <map>

namespace _wrap_
{

class FunctionBase;
class Constructor;
class Method;
class StaticMethod;
class ConversionTable;
class WrapperTable;
class InstanceTable;

/**
 * An individual wrapper class is a specialization of this template.
 */
template <class T>
class Wrapper;

/**
 * Implement functionality common to all wrapper classes.  An individual
 * wrapper class should inherit from this class.
 */
class _wrap_EXPORT WrapperBase
{
public:
  WrapperBase(Tcl_Interp* interp, const String& wrappedTypeName);
  virtual ~WrapperBase();
  
  const Type* GetWrappedTypeRepresentation() const;
  
  Tcl_Interp* GetInterpreter() const;
  void CreateResultCommand(const String& name, const Type* type) const;
  void AddInstance(const String& name, void* object) const;
  String CreateTemporary(void* object, const CvQualifiedType&) const;
  
  CvQualifiedType GetObjectType(Tcl_Obj* obj) const;
  Argument GetObjectArgument(Tcl_Obj* obj) const;
  ConversionFunction GetConversionFunction(const CvQualifiedType& from,
                                           const Type* to) const;
  
  /**
   * The type of a wrapper function for a Tcl interpreter call-back.
   */
  typedef int (*WrapperFunction)(ClientData, Tcl_Interp*, int, Tcl_Obj* CONST[]);

  /**
   * Get the wrapper function for a Tcl interpreter call-back for commands
   * referencing the name of the wrapped type.
   *
   * This function should handle instance creation and the calling of
   * static member functions of the type.
   */
  virtual WrapperFunction GetClassWrapperFunction() const =0;
  
  /**
   * Get the wrapper function for a Tcl interpreter call-back for commands
   * referencing instances of the wrapped type.
   *
   * This function should handle method calls on instances, references
   * to instances, and pointers to instances of the type.
   */
  virtual WrapperFunction GetObjectWrapperFunction() const =0;
  
protected:
  typedef std::vector<FunctionBase*> CandidateFunctions;
  void AddFunction(Method*);
  void AddFunction(Constructor*);
  void NoNameSpecified() const;
  void NoMethodSpecified() const;
  void UnknownConstructor(const CvQualifiedTypes& argumentTypes) const;
  void UnknownMethod(const String& methodName,
                     const CvQualifiedTypes& argumentTypes,
                     const CandidateFunctions& = CandidateFunctions()) const;
  void UnknownInstance(const String& objectName) const;
  void ReportErrorMessage(const String& errorMessage) const;
  void FreeTemporaries(int objc, Tcl_Obj*CONST objv[]) const;
  int ObjectWrapperDispatch(ClientData ,int, Tcl_Obj* CONST[]) const;
  const WrapperBase* FindMethodWrapper(const String& name) const;
  bool HasMethod(const String& name) const;
  int ClassWrapperDispatch(ClientData, int, Tcl_Obj* CONST[]) const;
  int CallWrappedFunction(int, Tcl_Obj* CONST[], bool) const;

protected:
  /**
   * The Tcl interpreter to which this wrapper is attached.
   */
  Tcl_Interp*    m_Interpreter;
  
  /**
   * The name of the wrapped type.
   */
  const String   m_WrappedTypeName;
  
  /**
   * The table of conversion functions for this wrapper's interpreter.
   */
  ConversionTable* m_ConversionTable;
  
  /**
   * The table of object instances for this wrapper's interpreter.
   */
  InstanceTable* m_InstanceTable;
  
  /**
   * The table of registered wrappers for this wrapper's interpreter.
   */
  WrapperTable*  m_WrapperTable;
  
  /**
   * The TypeSystem's representation for this wrapped type.
   */
  const ClassType*    m_WrappedTypeRepresentation;
  
  /**
   * The name of a constructor of the type.  This is the type name without
   * template arguments or namespace qualification.
   */
  String m_ConstructorName;
  
  typedef std::vector<Constructor*> Constructors;
  
  /**
   * The constructors available to this wrapper.
   */
  Constructors m_Constructors;
  
  typedef std::multimap<String, Method*> MethodMap;
  
  /**
   * Map from method name to method wrapper.  Duplicate names are
   * allowed.  The method dispatch function needs to know about all
   * possible methods.  This is defined here, but must be filled in by
   * calls from a subclass to AddFunction.
   */
  MethodMap m_MethodMap;
};

} // namespace _wrap_

#endif

