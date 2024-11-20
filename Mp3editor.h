/***************************************************************************//**
 * @file 
 * @brief header file to be used with Mp3editor.cpp
 ******************************************************************************/

//Includes
#pragma pack(1)
#ifndef _MP3EDITOR_H_
#define _MP3EDITOR_H_
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <string>
#include <direct.h>


using namespace std;

/*!
* @brief Holds information about a single mp3 file
*/
struct mp3Tag
{
	char header[3];				/*!< holds the word TAG */
	char songTitle[30];			/*!< the mp3 file's song title */
	char artist[30];			/*!< the mp3 file's artist */
	char album[30];				/*!< the mp3 file's album  */
	char year[4];				/*!< the mp3 file's year */
	char comment[28];			/*!< the mp3 file's comment */
	char zeroByte;				/*!< holds a null terminator */
	unsigned char albumTrack;	/*!< the mp3 file's album track number */
	unsigned char genre;		/*!< the mp3 file's genre number */
};


/*******************************************************************************
*                         Function Prototypes
******************************************************************************/

void edit(char tag[], int size);
void trackNumber(unsigned char &tag);
void genre(ifstream &gfin, unsigned char &tag);


#endif