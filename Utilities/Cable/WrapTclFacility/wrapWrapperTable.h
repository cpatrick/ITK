/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    wrapWrapperTable.h
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
#ifndef _wrapWrapperTable_h
#define _wrapWrapperTable_h

#include "wrapUtils.h"
#include <map>

namespace _wrap_
{  

// All wrappers inherit from WrapperBase.
class WrapperBase;

/**
 * A class to maintain a table of the type wrappers that have
 * been registered with its interpreter.
 */
class _wrap_EXPORT WrapperTable
{
public:
  WrapperTable(Tcl_Interp*);
  
  bool Exists(const Type* type) const;
  void SetWrapper(const Type*, WrapperBase*);
  WrapperBase* GetWrapper(const Type*) const;
  
private:
  /**
   * The Tcl interpreter to which this InstanceTable object is attached.
   */
  Tcl_Interp* m_Interpreter;

  typedef std::map<const Type*, WrapperBase*>  WrapperMap;
  /**
   * Map from type to wrapper function.
   */
  WrapperMap m_WrapperMap;
  
public:
  static WrapperTable* GetForInterpreter(Tcl_Interp*);
  
private:
  typedef std::map<const Tcl_Interp*, WrapperTable*>  InterpreterWrapperTableMap;
  static InterpreterWrapperTableMap interpreterWrapperTableMap;
};

} // namespace _wrap_

#endif
