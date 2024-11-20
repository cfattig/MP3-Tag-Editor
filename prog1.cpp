/*************************************************************************//**
 * @file 
 *
 * @mainpage program 1 - MP3 Tag Editor - ID3v1.1
 * 
 * @section course_section Course Information 
 *
 * @author Christian Fattig
 * 
 * @date Oct 7, 2016
 * 
 * @par Professor: 
 *         Roger Schrader
 * 
 * @par Course: 
 *         CSC 250 - M001 -  1:00-pm
 * 
 * @par Location: 
 *         CB - 107
 *
 * @section program_section Program Information 
 * 
 * @details 
 * This program is an mp3 tag editor that will read in information 
 * from the given files and store it into a structure that can then
 * be edited and manipulated to the needs of the user where the 
 * changes can then be saved and re-written to the file. Along with
 * being able to change the mp3 file tags, this program will also 
 * allow the user to play the currently accessed mp3 file via 
 * Windows Media Player through the system function.
 *
 * The program starts by checking if it has a valid argument count given
 * where, if valid, will proceed to open and check for the neccessary files
 * and will otherwise produce and error message and ussage statement while
 * terminating the program. One at a time, the program will access each 
 * mp3 file given and present the user with the information of the files
 * that will be stored in a structure and will give the user a menu of
 * options to edit the information with. The user can make any edits 
 * needed from this menu and if they are satisfied with their changes, they
 * can save them which will overwrite the previous tags in the file. This 
 * menu can then be exited by the user inputing the number "10" which will
 * close up the current file and open up the next one found in the list of
 * arguments. If there are no more files listed, the program will close up
 * all remaining files and terminate.
 *
 * The information regarding the files being read are stored in a series 
 * of character arrays of various sizes as well as a couple unsigned 
 * characters to store the track number and genre. The character arrays
 * can be changed via an edit function that asks the user for a range
 * of characters and will truncate the array if neccessary and will 
 * pad the remainder of the array with null terminators if given less
 * characters than required. The genre and track number fields are changed
 * by requesting an integer value between a certain range. If the value
 * is invalid, the data is untouched and an error message is presented.
 * For the genre, the program takes the number given and seeks to that
 * spot in the genres.bin file and reads in the genre. The play function
 * generates a string from both the path to Windows Media Player and the
 * path to the current directory. This string is then put into the system
 * function which allows the mp3 file to play. Once the user enters "10"
 * and every file has been tended to, the program safely terminates.
 *
 *
 * @section compile_section Compiling and Usage 
 *		Usage: prog1.exe file1.mp3 ... filen.mp3
 *
 * @par Compiling Instructions: 
 *		None
 * 
 * @par Usage: 
   @verbatim  
   c:\> prog1.exe 
   d:\> c:\bin\prog1.exe 
   @endverbatim 
 *
 * @section todo_bugs_modification_section Todo, Bugs, and Modifications
 * 
 * 
 * 
 * @par Modifications and Development Timeline: 
   @verbatim 
   Date          Modification 
   ------------  -------------------------------------------------------------- 
   Sep 16, 2016  Created argument usage and error statements
   Sep 18, 2016  Set up the menu and displaying file name and was
                 able to press 10 to skip to the next file
   Sep 19, 2016  Could open Mp3 file in binary, read, and display
                 the tags to the screen
   Sep 29, 2016  Error check for genres.bin and able to read
				 and display genre tags
   Sep 30, 2016  Could edit title, album, artist, year, and comment
   Oct 01, 2016  Fixed an overwrite bug in the edit function
   Oct 06, 2016  Could edit genre and track numbers as well as
                 save tag changes
   Oct 07, 2016  Able to play song
   Oct 09, 2016  Finished up documentation
   @endverbatim
 *
 *****************************************************************************/


#pragma pack(1)
#include"Mp3editor.h"


/*******************************************************************************
*                         Function Prototypes
******************************************************************************/

void menu(mp3Tag &song, ifstream &gfin, fstream &file, char* name);
void readIn(fstream &file, mp3Tag &song);
void write(fstream &file, mp3Tag &song);
void displayTags(mp3Tag song, ifstream &gfin);
void gOpen(ifstream &fin);
void displayField(char tag[], int size);
void play(char* name);


/***************************************************************************//**
 * @author Christian Fattig
 *
 * @par Description:
 * This is the starting point to the program. It will use the files that 
 * are given in the command prompt and open them up for editing. If a file
 * is empty, it will be skipped giving a message back that it was empty. If
 * an invalid argument count is given, an error will be printed as well as 
 * a usage statement followed by the termination of the program. once the
 * file has been open, the editor menu will be presented to the user. The
 * function will loop through the files until they have all been processed.
 *
 * @param[in] argc - the number of arguments from the command prompt.
 * @param[in] argv - a 2d array of characters containing the arguments.
 *
 * @returns 0 Program ran sucessfully
 * @returns 1 Program failed, invalid argument count was given
 *
 ******************************************************************************/
int main(int argc, char **argv)
{
	ifstream gfin; // genre.bin file
	fstream file; // holds current .mp3 file being used
	mp3Tag song; // structure for holding tags

	if (argc < 2) // checks for arguments and provides usage statement
	{
		cout << "Invalid argument count!" << endl;
		cout << "Usage: prog1.exe file1.mp3 file2.mp3 ... filen.mp3" << endl;
		return 1;
	}

	gOpen(gfin); // opens and checks the genre.bin file
	
	// starts at first file given, runs till last file given
	for (int i = 1; i < argc; i++) 
	{
        file.open(argv[i], ios::in | ios::out | ios::app | ios::binary);
			if (!file)
			{
				cout << "Could not open " << argv[i] << endl;
				return 0;
			}

			file.seekg(0, ios::end);

		if ((int)file.tellg() == 0)// checks to see if the current file is empty
			cout << endl << argv[i] << " is empty" << endl << endl;
		else
		{
			file.seekg(0, ios::beg);

			readIn(file, song);// fills the the structure's tags from the file
			cout << "Filename: " << argv[i] << endl;

			menu(song, gfin, file, argv[i]);//display editor menu
			file.close();
		}
	}

	gfin.close();

	return 0;
}


/***************************************************************************//**
 * @author Christian Fattig
 *
 * @par Description:
 * This function displays the information regarding the mp3 files as
 * well as an interactive menu for the user to edit this information.
 * It will prompt the user for an integer value between one and ten
 * and will run the function associated with that number. If an 
 * invalid number is given, an error message will be displayed and the
 * user will be re-prompted.
 *
 * @param[in,out] song - structure holding the mp3 file information.
 * @param[in,out] gfin - used for opening genres.bin.
 * @param[in,out] file - holds the current mp3 file in use.
 * @param[in]     name - stores the name of the current mp3 file.
 *
 ******************************************************************************/
void menu(mp3Tag &song, ifstream &gfin, fstream &file, char* name)
{
	int choice = 0; //user input for menu selection

	while (choice != 10)
	{
        displayTags(song, gfin); // displays the mp3 tags
        cout << "1) Edit Song Title" << endl << "2) Edit Artist Name" << endl
		        << "3) Edit Album Name" << endl << "4) Edit Albums Year" << endl
		        << "5) Edit Comment" << endl << "6) Edit Album Track" << endl
		        << "7) Edit Genre" << endl << "8) Play Song" << endl 
		        << "9) Save Changes" << endl << "10) Next File" << endl << endl;

		cin >> choice;

		switch (choice) // menu for editing options
		{
		case 1:
			edit(song.songTitle, 30);
			break;
		case 2:
			edit(song.artist, 30);
			break;
		case 3:
			edit(song.album, 30);
			break;
		case 4:
			edit(song.year, 4);
			break;
		case 5:
			edit(song.comment, 28);
			break;
		case 6:
			trackNumber(song.albumTrack);
			break;
		case 7:
			genre(gfin, song.genre);
			break;
		case 8:
			play(name);
			break;
		case 9:
			write(file, song);
			cout << "Your changes have been saved to the file" << endl;
			break;
		case 10:
			break;
		default:
			cout << "Invalid choice" << endl;
		}
	}
	return;
}


/***************************************************************************//**
 * @author Christian Fattig
 *
 * @par Description:
 * This function seeks to the last 128 bytes in the file and then reads
 * in the information in to the given structure. It will then seek back 
 * to the beginning of the file.
 *
 * @param[in,out] song - structure holding the mp3 file information.
 * @param[in] file - holds the current mp3 file in use.
 *
 ******************************************************************************/
void readIn(fstream &file, mp3Tag &song)
{
	file.seekg(-128, ios::end); //seeks to the last 128 bytes in the file 
	file.read((char *) &song, sizeof(mp3Tag));//reads the info into song
	file.seekg(0, ios::beg); //seeks back to the beginning of the file

	return;
}


/***************************************************************************//**
 * @author Christian Fattig
 *
 * @par Description:
 * This function seeks to the last 128 bytes in the file. It then overwrites
 * those 128 bytes with the information stored in the structure. Finally, it
 * seeks back to the beginning of the file.
 *
 * @param[in,out] song - structure holding the mp3 file information.
 * @param[in,out] file - holds the current mp3 file in use.
 *
 ******************************************************************************/
void write(fstream &file, mp3Tag &song)
{
	file.seekg(-128, ios::end);//seeks to last 128 bytes in file
	file.write((char *)&song, sizeof(mp3Tag));//writes over those bytes
	file.seekg(0, ios::beg);//seeks back to beginning of file

	return;
}


/***************************************************************************//**
 * @author Christian Fattig
 *
 * @par Description:
 * Function specially designed to display the information stored in the 
 * structure without having information bleed to the screen from the other
 * tags in the structure. This function also handles reading the genre in 
 * from the file genres.bin.
 *
 * @param[in] song - structure holding the mp3 file information.
 * @param[in,out] gfin - used for opening genres.bin.
 *
 ******************************************************************************/
void displayTags(mp3Tag song, ifstream &gfin)
{
	char genre[30];//used to store the genre read in from genres.bin
	
	
	//fills genre
	gfin.seekg((int)song.genre * sizeof(char) * 30, ios::beg);
	gfin.read((char *)genre, sizeof(char) * 30);


	//used to display each field
	cout << endl << "Artist: ";
	displayField(song.artist, 30);
	cout << endl << "Track Number: " << (int)song.albumTrack << endl
		<< "Genre: " << genre << endl << "Album - Song Title: ";
	displayField(song.album, 30); 
	cout << " - ";
	displayField(song.songTitle, 30);
	cout << endl << "Year: ";
	displayField(song.year, 4);
	cout << endl << "Comment: ";
	displayField(song.comment, 28);
    cout << endl << endl;
	
	return;
}


/***************************************************************************//**
 * @author Christian Fattig
 *
 * @par Description:
 * This function is used for opening and error checking genres.bin. If
 * genres.bin is not detected, the program displays an error message and
 * then safely terminates.
 *
 * @param[in,out] fin - used for opening genres.bin.
 *
 ******************************************************************************/
void gOpen(ifstream &fin)
{
	fin.open("genres.bin", ios::in | ios::binary);//opens file in binary
	if (!fin)
	{
		cout << "Error, genres.bin was not found" << endl;
		exit(1);
	}

	return;
}


/***************************************************************************//**
 * @author Christian Fattig
 *
 * @par Description:
 * This function displays an individual field to the screen to the extent
 * of the size of the tag given.
 *
 * @param[in]     tag - holds the information of a field in the structure
 * @param[in]     size - the number of characters the field can hold
 *
 ******************************************************************************/
void displayField(char tag[], int size)
{
	//displays characters until size is reached
	for (int i = 0; i < size && tag[i] != '\0'; i++)
		cout << tag[i];

	return;
}


/***************************************************************************//**
 * @author Christian Fattig
 *
 * @par Description:
 * This function combines both the paths for Windows Media Player and for
 * the directory for the current file. This combined string is then put into
 * the system function which will play it for the user.
 *
 * @param[in] name - stores the name of the current mp3 file.
 *
 ******************************************************************************/
void play(char* name)
{
	
	char mp3[250];//holds the path to the current directory
	// will hold the entire string 
	char full[500] = "\"\"c:\\Program Files (x86)\\Windows Media Player\\wmplayer.exe\" \"";


	_getcwd(mp3, 250);//fill mp3 with current directory path

	//combines the paths
	strcat(full, mp3);
	strcat(full, "\\");
	strcat(full, name);
	strcat(full, "\"\"");

	cout << "Paths: " << full << endl;
	system(full);//plays the song

	return;
}