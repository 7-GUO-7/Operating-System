#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#define MAX_LINE 50 /* The maximum length command */

int wait_or_not = 1;
char input[50];
char args[50][50];
char history[50] = {0};
int num = 0;
int detect()
{
		if(num == 0)
			return 0;	
		if(strcmp(args[0], "!!")==0)
		{
			if(!history[0])
			{
				printf("No commands in history! \n");
				return 0;
			}
			
				strcpy(input, history);
			return detect();
		}
	
	int i = 0;
	while(input[i])
	{
		history[i] = input[i];
		i++;
	}
	strcpy(history,input);
	if(strcmp(args[0], "exit")==0)
		exit(0);
		if(strcmp(args[num-1],"&")==0)
			{wait_or_not = 0;
				num--;}
	return 1;
}
	
int execute()
{
	int type = 0,i,j; // 0:as usual 1:"<" 2:">" 3:"|"
	char *filename;
	char *tmp_1[MAX_LINE/2+1];
	char *tmp_2[MAX_LINE/2+1];
	for(i = 0;i<num;i++)
		tmp_1[i] = args[i];
	tmp_1[i] = NULL;
	for(i = 0;i < num;i++)
	{
		if(strcmp(args[i],">") == 0)
		{
			filename = args[i+1];
			type = 2;
			tmp_1[i] = NULL;
		}
		else if(strcmp(args[i], "<")==0)
		{
			filename = args[i+1];
			type = 1;
			tmp_1[i] = NULL;
		}
		else if(strcmp(args[i],"|")==0)
		{
			type = 3;
			tmp_1[i] = NULL;
			for(j = i+1;j<num;j++)
			{
				tmp_2[j-i-1] = args[j];
			}
			tmp_2[j-i-1] = NULL;
		}
	}
	int pid = fork();
	int in, out;
	int fds[2];
	if(pid < 0)
	{
		perror("there is a fork error!!");
		exit(0);
	}
	else if(pid > 0)
	{
		if(wait_or_not == 1)
			waitpid(pid,NULL,0);
		wait_or_not = 1;
	}
	else
	{
		switch(type)
		{
			case 0:
			execvp(tmp_1[0],tmp_1);
			break;
			case 1:
			in = open(filename, O_RDONLY);
			dup2(in,STDIN_FILENO);
			execvp(tmp_1[0],tmp_1);
			close(in);
			break;
			case 2:
			out = open(filename, O_WRONLY|O_CREAT, 0666);
			dup2(out, STDOUT_FILENO);
			execvp(tmp_1[0],tmp_1);
			close(out);
			break;
			case 3:
			pipe(fds);
			int pids = fork();
			if(pids<0)
			{
				perror("there is a fork error!");
				exit(0);
			}
			else if(pids>0)
			{
				close(fds[1]);
				dup2(fds[0], STDIN_FILENO);
				execvp(tmp_2[0],tmp_2);
			}
			else
			{
				close(fds[0]);
				dup2(fds[1], STDOUT_FILENO);
				execvp(tmp_1[0],tmp_1);
			}
		break;
		}
	}
	return 1;
}

int main(void)
{


int should_run = 1; /* flag to determine when to exit program */
while (should_run)
{
printf("osh > ");
fflush(stdout);
gets(input);
char *p = input;
strcat(input," ");
int t = 0, index = 0;
while(*p)
{
    if(!isspace(*p))
    {
    	args[index][t] = *p;
    	t++;
    }
    if(isspace(*p))
    {
    	args[index][t] = '\0';
    	index++;
    	t = 0;
    }
    p++;

}
num = index;
// for(int i = 0;i < strlen(args);i++)
    // printf ("%s \n", args[i]);
int tmp = detect();
if(tmp == 0)
	continue;
execute();

/**
* After reading user input, the steps are:
* (1) fork a child process using fork()
* (2) the child process will invoke execvp()
* (3) parent will invoke wait() unless command included &
*/
}

return 0;

}