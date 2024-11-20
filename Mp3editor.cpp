/***************************************************************************//**
 * @file 
 * @brief all functions to help edit mp3 tags
 ******************************************************************************/

#pragma pack(1)
#include "Mp3editor.h"


/***************************************************************************//**
 * @author Christian Fattig
 *
 * @par Description:
 * This function is used to edit a given field of an mp3 file. It begins 
 * by prompting the user to input up to a certain ammount of characters. 
 * If the inputted field is larger than the requested size, it gets truncated
 * down to the max size allowed. If it is shorter than the max size, the
 * string then gets padded with null terminators up to the max character limit.
 *
 * @param[in,out] tag - holds the current mp3 file in use.
 * @param[in]     size - stores the name of the current mp3 file.
 *
 ******************************************************************************/
void edit(char tag[], int size)
{
	string temp;// holds the user given field

	cout << "Enter up to " << size << " characters: " << endl;
	cin.ignore();
	getline(cin, temp);
	
	//checks inputted size and truncates if needed
	if ((int)temp.length() > size)
		temp.resize(size);

	//pads the remainder of the string with "\0"
	while ((int)temp.length() < size)
		temp += '\0';

	//converts the new field into a cstyle string
	//and chops off the "\0" if given
	strncpy(tag, temp.c_str(), size);

	return;
}


/***************************************************************************//**
 * @author Christian Fattig
 *
 * @par Description:
 * This function prompts the user for an integer between 0 and 255. It will
 * check to make sure the number given is valid. If it is, the number gets
 * stored as an unsigned character. Otherwise the function displays an
 * error message and the number doesn't get changed.
 *
 * @param[out] tag - holds the unsigned character value for the track number.
 *
 ******************************************************************************/
void trackNumber(unsigned char &tag)
{
	int temp; //holds the user inputted value 

	cout << "Enter a new track  number between 1 and 255: ";

	cin >> temp;

	if (temp <= 0 || temp > 255)//checks that the number is valid
		cout << "Invalid choice, track number was not changed." << endl;
	else
		tag = (char)temp;//changes the number

	return;
}


/***************************************************************************//**
 * @author Christian Fattig
 *
 * @par Description:
 * This function reads in the current genre from genres.bin along with
 * the genre number. It then proceeds to list every available genre to
 * the user in two neatly formatted columns. The user is then prompted 
 * for a new genre number. From there, the number is checked for validity.
 * If the number is invalid, an error message is outputted and the data 
 * remains unchanged. Otherwise the genre field gets updated.
 *
 * @param[in,out] gfin - used for opening genres.bin.
 * @param[in,out] tag - holds the character value equivalent of the genre number.
 *
 ******************************************************************************/
void genre(ifstream &gfin, unsigned char &tag)
{
	char genre[30];//holds the genre name
	int temp;//user inputted genre number

	//finds the current genre
	gfin.seekg((int)tag * sizeof(char) * 30, ios::beg);
	gfin.read((char *)genre, sizeof(char) * 30);

	cout << "Current genre: " << (int)tag << ". " << genre << endl;

	gfin.seekg(0, ios::beg);

	//this loop displays all possible genres and numbers
	for (int i = 0; i < 148; i++)
	{
		if (i % 2 == 0)
			cout << endl;

		gfin.read((char *)genre, sizeof(char) * 30);

		cout << left << setw(1) << i << ". " << left << setw(22)
			<< genre << left << setw(15);
	}

	gfin.seekg(0, ios::beg);

	//prompts for new genre
	cout << endl << endl << "Enter a new genre number between 0 and 147: ";

	cin >> temp;

	//checks if number is valid
	if (temp < 0 || temp > 147)
	{
		cout << endl << "Invalid number, genre was not changed." << endl;
		return;
	}

	tag = (unsigned char)temp;//changes the genre number

	return;
}
