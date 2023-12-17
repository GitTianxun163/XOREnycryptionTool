#ifndef ENYCPT_H
#define ENYCPT_H

extern void enycptString(const char* plain,int length,const char* key,char* cipher);
extern void enycptFile(const char* plainpath,const char* key,const char* cipherpath);

#endif