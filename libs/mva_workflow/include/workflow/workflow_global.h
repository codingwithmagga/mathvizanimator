#ifndef WORKFLOW_GLOBAL_H
#define WORKFLOW_GLOBAL_H

/**
 * @brief Allows for windows exported symbols
 *
 * Windows requries a special modifier to allow them to export correctly.
 * This macro allows that while leaving Mac OS X and Linux alone.
 *
 * While Qt can tell us if it was make for WIN32, MAC, or LINUX, It cannot
 * tell us if we are being statically or dynamically linked. That is why
 * this is using the CMake variables instead of Qt
 * 
 * This export symbol is only necessary for classes which are inherited from QObject.
 * All other classes are exported correctly using 
 *   set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
 * in CMake. 
 */
#if defined(_WIN32)
// cmake makes this variable if your a shared library (projectname_EXPORTS)
#if defined(workflow_EXPORTS)
#define WORKFLOW_EXPORT Q_DECL_EXPORT
#else
#define WORKFLOW_EXPORT Q_DECL_IMPORT
#endif
#else
#define WORKFLOW_EXPORT
#endif

#endif // WORKFLOW_GLOBAL_H
