#pragma once

#ifdef WIN32
#ifdef MAO_JSON_EXPORTS_H
#define MAO_JSON_EXPORTS __declspec(dllexport)
#else
#define MAO_JSON_EXPORTS __declspec(dllimport)
#endif  // MAO_JSON_EXPORTS_H
#else
#define MAO_JSON_EXPORTS
#endif  // WIN32