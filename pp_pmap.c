/************************************
Author: Danielle Wisemiller
Course: CS4420 (OS)
Date: 02-16-2018
************************************/

#include <procfs.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#define BUFF_SIZE 1000

void str_append(char* string, char c)
{
	int length = strlen(string);
	string[length] = c;
}

int main(int argc, char **argv)
{
	//error testing -- for arguments and process numbers
	if(argc < 2)
	{
		printf("%s \n", "Error: not enough arguments. Hint: run with PID");
		printf("%s \n","Exiting...");
		exit(-1);
	}
	else if (argc > 2) 
	{
		printf("%s \n","Error: Too many args");
		printf("%s \n","Exiting...");
		exit(-1);
	}
	else
	{
		//process number test
		int long test = strtol(argv[1], NULL, 10);
		if (test == 0)
			{
				printf("%s \n","Error: Input was not a number");
				printf("%s \n","Exiting...");
				exit(-1);
			}
		//valid process number - start 
		char buffer [BUFF_SIZE];
		prmap_t p;
		int size = sizeof(prmap_t);
		sprintf (buffer, "/proc/%s/map", argv[1]);
		
		//open file and read data
		int fd = open(buffer, O_RDONLY, 0);
		if(fd>0)
		{
			//print formatted header
			printf("%10s%18s%20s%20s%30s \n", "Start", "End","Size","Permissions","Path");
			printf("%s \n", "----------------------------------------------------------------------------------------------------");
			while(read(fd, &p, size) > 0)
			{
				char perms [5] = ""; 
				if(p.pr_mflags & MA_READ)
					str_append(perms,'r');
				else
					str_append(perms,'-');
				if(p.pr_mflags & MA_WRITE)
					str_append(perms,'w');
				else
					str_append(perms,'-');
				if(p.pr_mflags & MA_EXEC)
					str_append(perms,'x');
				else
					str_append(perms,'-');

				char mappings [BUFF_SIZE];
				//check path
				if(p.pr_mflags & MA_ANON)
				{
					if(p.pr_mflags & MA_STACK)
						strcpy(mappings, "[ stack ]");
					else
						strcpy(mappings,"[ anon ]");
				}
				else
				{
					const char* restrict path1 = p.pr_mapname;
					char buff [BUFF_SIZE];
					char finalpath [BUFF_SIZE];

					sprintf(finalpath, "/proc/%s/path/%s", argv[1], path1);
					int length = readlink(finalpath, buff, BUFF_SIZE);
					if(length != 1)
					{
						buff[length] = '\0';
						mappings[length] = '\0';	
						strcpy(mappings, buff);				
					}
				}
			//print results in formatted manner
			printf("%10x%20x%18u%0s%16s%35s \n", p.pr_vaddr, p.pr_vaddr+p.pr_size, p.pr_size/1024, "K", perms, mappings);
			}
		}
		//couldn't open file error
		else
		{
			printf("%s \n","Invalid Process Number, try again.");
			printf("%s \n","Exiting...");}
		}
}
