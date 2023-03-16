#pragma once
#ifdef WIN32
#ifdef MAO_THREAD_EXPORTS_H
#define MAO_THREAD_EXPORTS __declspec(dllexport)
#else
#define MAO_THREAD_EXPORTS __declspec(dllimport)
#endif  // MAO_REFLECTION_EXPORTS_H
#else
#define MAO_THREAD_EXPORTS
#endif  // WIN32