#pragma once

#ifndef GUINSOODB_API
#ifdef _WIN32
#if defined(GUINSOODB_BUILD_LIBRARY) && !defined(GUINSOODB_BUILD_LOADABLE_EXTENSION)
#define GUINSOODB_API __declspec(dllexport)
#else
#define GUINSOODB_API __declspec(dllimport)
#endif
#else
#define GUINSOODB_API
#endif
#endif

#ifdef GUINSOODB_BUILD_LOADABLE_EXTENSION
#ifdef _WIN32
#define GUINSOODB_EXTENSION_API __declspec(dllexport)
#else
#define GUINSOODB_EXTENSION_API
#endif
#endif
