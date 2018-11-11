#ifndef PV_MACROS_H
#define PV_MACROS_H

#define PV_DEFINE_STRUCT(name)                                                 \
  typedef struct name name;                                                    \
  struct name

#if defined(_MSC_VER) && defined(_DLL)

#if defined(PV_IMPLEMENTATION)
#define PV_EXPORT __declspec(dllexport)
#else
#define PV_EXPORT __declspec(dllimport)
#endif

#else
#define PV_EXPORT
#endif

#if defined(__cplusplus)
#define PV_BEGIN_HEADER extern "C" {
#define PV_END_HEADER }
#define PV_INLINE_FUNCTION inline
#else
#define PV_BEGIN_HEADER
#define PV_END_HEADER
#define PV_INLINE_FUNCTION static
#endif

#endif /* PV_MACROS_H */
