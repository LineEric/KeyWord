#pragma once

#ifdef KEY_WORD_EXPORTS
#define KEY_WORD_API __declspec(dllexport)
#else
#define KEY_WORD_API __declspec(dllimport)
#endif

extern "C"  KEY_WORD_API  char* Key_Word(char * msg);
