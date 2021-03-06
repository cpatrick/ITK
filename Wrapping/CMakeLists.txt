IF("${ITK_COMMON_BUILD_TYPE}" MATCHES "SHARED")
  SET(BUILD_SHARED_LIBS ON)
ENDIF("${ITK_COMMON_BUILD_TYPE}" MATCHES "SHARED")

#-----------------------------------------------------------------------------
# Explicit Instantiation config
OPTION(ITK_EXPLICIT_INSTANTIATION "Try to explicitly build some of the core templates explictitly rather than over and over again." OFF)
MARK_AS_ADVANCED(ITK_EXPLICIT_INSTANTIATION)
# Display a warning message if we try to build explicit instantiations on windows with
# shared libraries. VXL is not linking using shared libs on windows.
IF(ITK_EXPLICIT_INSTANTIATION AND WIN32 AND BUILD_SHARED_LIBS)
  MESSAGE("Explicit Instiantiation does not work on Windows platform with shared libraries. Turn BUILD_SHARED_LIBS OFF.")
ENDIF(ITK_EXPLICIT_INSTANTIATION AND WIN32 AND BUILD_SHARED_LIBS)

#-----------------------------------------------------------------------------
# Find wrapping language API libraries.
IF(ITK_CSWIG_TCL)
  FIND_PACKAGE(TCL)
  # Hide useless settings provided by FindTCL.
  FOREACH(entry TCL_LIBRARY_DEBUG
                TK_LIBRARY_DEBUG
                TCL_STUB_LIBRARY
                TCL_STUB_LIBRARY_DEBUG
                TK_STUB_LIBRARY
                TK_STUB_LIBRARY_DEBUG
                TK_WISH)
    SET(${entry} "${${entry}}" CACHE INTERNAL "This value is not used by ITK.")
  ENDFOREACH(entry)
ENDIF(ITK_CSWIG_TCL)

IF(ITK_CSWIG_PYTHON)
  INCLUDE(${CMAKE_ROOT}/Modules/FindPythonLibs.cmake)
  FIND_PROGRAM(PYTHON_EXECUTABLE
    NAMES python python2.3 python2.2 python2.1 python2.0 python1.6 python1.5 
    PATHS
    [HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\2.3\\InstallPath]
    [HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\2.2\\InstallPath]
    [HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\2.1\\InstallPath]
    [HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\2.0\\InstallPath]
    [HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\1.6\\InstallPath]
    [HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\1.5\\InstallPath]
    )
  MARK_AS_ADVANCED(PYTHON_EXECUTABLE)
ENDIF(ITK_CSWIG_PYTHON)

IF(ITK_CSWIG_JAVA)
  FIND_PACKAGE(Java)
  FIND_PACKAGE(JNI)
ENDIF(ITK_CSWIG_JAVA)

IF(ITK_CSWIG_PERL)
  FIND_PACKAGE(Perl)
  FIND_PACKAGE(PerlLibs)
ENDIF(ITK_CSWIG_PERL)

MARK_AS_ADVANCED(ITK_CSWIG_PYTHON)
MARK_AS_ADVANCED(ITK_CSWIG_TCL)
MARK_AS_ADVANCED(ITK_CSWIG_JAVA)
MARK_AS_ADVANCED(ITK_CSWIG_PERL)
MARK_AS_ADVANCED(CABLE_INDEX)
MARK_AS_ADVANCED(CSWIG)
MARK_AS_ADVANCED(GCCXML)

# set a variable to determine if 
# the CSwig directory should be used
SET(ITK_CSWIG_DIR 0)
IF(ITK_CSWIG_TCL)
  SET(ITK_CSWIG_DIR 1)
ENDIF(ITK_CSWIG_TCL)
IF(ITK_CSWIG_PYTHON)
  SET(ITK_CSWIG_DIR 1)
ENDIF(ITK_CSWIG_PYTHON)
IF(ITK_CSWIG_JAVA)
  SET(ITK_CSWIG_DIR 1)
ENDIF(ITK_CSWIG_JAVA)

IF(ITK_CSWIG_DIR)
  SUBDIRS(CSwig)
ENDIF(ITK_CSWIG_DIR)

IF(USE_WRAP_ITK)
  add_subdirectory(WrapITK)
  # be sure that wrapitk is not used with the older system
  FOREACH(lang TCL PYTHON JAVA)
    IF(ITK_CSWIG_${lang} AND WRAP_ITK_${lang})
      MESSAGE(FATAL_ERROR "ITK_CSWIG_${lang} and WRAP_ITK_${lang} can't be used together. Please disable one of them.")
    ENDIF(ITK_CSWIG_${lang} AND WRAP_ITK_${lang})
  ENDFOREACH(lang)
ENDIF(USE_WRAP_ITK)

