/* CS3540 - Programming in C in a UNIX Environment
 * Instructor: Jonathan Higgins
 * Nathaniel Klein
 * Homework #7
 * November 10, 2014
*/

/* Homework #6 (problem 4.16, p.142 in Advanced Programming in the UNIX Environment
 * by Richard Stevens), asked the following: "Does the UNIX System have a fundamental
 * limitation on the depth of a directory tree?  To find out, write a program that
 * creates a directory and then changes to that directory, in a loop.  Make certain
 * that the length of the absolute pathname of the leaf of this directory is greater
 * than your system's PATH_MAX limit."
 *
 * This assignment, Homework #7, was an extension of Homework #6.  It required that
 * we add the functionality to delete the created directories in all cases, regardless
 * of the manner in which the program terminates.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>

void unwind(int cur_depth);
void err_sys(const char* message);

int main()
{
	
	const char* dir = "new";					// new nested directories we will be making and destroying, each called "new"
	int size = PATH_MAX + 1;					// setting size to greater than PATH_MAX allows us to test for end of tree
	char* path;									// path will hold the current working directory (cwd)
	char* buf_cwd = malloc(size);				// allocating space for cwd
	char* buf_orig = malloc(size);				// allocating space for orig_path
	char* orig_path = getcwd(buf_orig,size);	// path we start with

	int i = 0;									// counter used to track directory depth throughout
	
	// while loop continues while true

	while(1)
	{
		// makes a single directory within a directory.  The first part of the if statement runs
      	//unless the attempt fails, in which case it calls unwind()
		
		if (mkdir(dir, S_IRWXU | S_IRWXG | S_IRWXO) < 0)
			unwind(i);

		// changes directories toward the bottom of the tree.  The first part of the if statement
      	// runs unless the attempt fails, in which case it calls unwind()
		
		if (chdir(dir) < 0)
			unwind(i);

		// prints the associated number of the created folder
		
		printf("%d\n", i);

		// Returns the buffer object and stores it in path. The buffer object contains an absolute
      	//pathname of the working directory.
		
		path = getcwd(buf_cwd, size);

		// If path is null for any reason.  Most likely because previous line was not able to get
		// the current working directory.  This could happen for any number of reasons, but if it
      	// does, we can't guarantee that we reached a depth of "size", so we have to rewind so we
    	// can try again.

		if(path == NULL)
		{
        
	        // Calling err_sys() here will end loop by returning 0, but we must first rewind, so
	        // we simply do a printf() instead (see err_sys() method below).
			//err_sys("Failed to get cwd\n");

			printf("Path has reached a NULL state\n");
			unwind(i);
		}

		// increase the depth at which a new directory is made
		i++;
	}

   // free memory allocations
   free(buf_cdw);
   free(buf_orig);

	return 0;
}

// continues deleting directories from the bottom up until root directory (protected)

void unwind(int cur_depth)
{

	while (cur_depth > 0)
	{
		chdir("..");
		rmdir(dir);
		--cur_depth;
	}
}

// Lets user know what's going on in case of problem. Used in HW6, but not needed in this one,
// because we must rewind regardless of why.  This will end loop by returning 0, but we must
// first rewind, which is why we will only do a printf() above.

void err_sys(const char* message)
{
	printf("%s\n", message);
	exit(0);
}
