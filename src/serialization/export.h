#pragma once

#ifdef WIN32
#ifdef MAO_SERIALIZATION_EXPORTS_H
#define MAO_SERIALIZATION_EXPORTS __declspec(dllexport)
#else
#define MAO_SERIALIZATION_EXPORTS __declspec(dllimport)
#endif  // MAO_REFLECTION_EXPORTS_H
#else
#define MAO_SERIALIZATION_EXPORTS
#endif  // WIN32