# SP-Project_1
Systems Programming - Project 1

Use:

	./sorter -c <column> -d [starting directory] -o [output directory]

	<column> - column to sort on
	[starting directory] - directory from which the program will start searching for csv files
	[output directory] - directory to output sorted files to

Algorithm Breakdown: We recusively traverse all directories, forking a new process for each new directory. That new process will in turn fork a sorting process for each csv file found. Recursion is responsible for traversing directories, forking is only responsible for sorting, not traversing.

	void Recurse(directory){
		while(currentFile != endOfFileList){ //Traverse list of files
			if(currentFile == Directory){ //Current file is a directory

				directory = openDir(currentDirectory);
				fork();

				if(childProcess){
					ProcessDir(directory);
					return;
				}

				else if(parentProcess){
					Recurse(directory);
				}
				currentFile = currentFile->next;

			}
		}
	}

	void ProcessDir(directory){
		while(directory)
		{
			if(.csv)	
			{	
				fork();
				if(childProcess)
				{
					file = openFile(currentFile);
					if(checkIfFileIsBad(file)){
							close(file);
							return; //This means the file is bad
					}
					sort(file);
					return;
				}

				//I don't think we need this conditional since it will continue traversing either way
				//else if(parentProcess)
				//{
				//	continueTraversing;
				//}
			}
			
		}
			
	}

Project Outline:

	1. Get parameters from command line.
	2.



Note: if file is bad, we can just ignore it and not sort that file. I guess terminate that fork process?  
