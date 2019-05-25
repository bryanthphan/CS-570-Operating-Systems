#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <ctype.h>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "command.h"

/** The chdir() function will make the directory named by the pathname pointed
*   to by the path argument to become the current working directory.
*/
void command::execute_cd(string command) {
	int ret = chdir(command.c_str());
	/** fails if ret equals -1, the cwd will not change*/
	if(ret == -1) {
		perror("Directory does not exist or is not accessible.");
	}
}

/** The getcwd() function copies an absolute pathname of the current working
*   directory to the array pointed to by buff, which is of length size
*/
void command::execute_pwd() {
	char cwd[256];
	/**if length of the cwd is not equal to NULL, cwd is produced */
	if(getcwd(cwd,sizeof(cwd)) != NULL) {
		printf("Current working directory is: %s\n",cwd);
	}
	else {
		/**Prints error */
		perror("Error: Unable to obtain current directory.");
	}
}

/**fork system call*/
void command::execute_fork(char *argv[]) {
	pid_t child = fork();
	int status;
	/**fork fails */
	if(child<0) {
		perror("Unable to spawn program");
		/**terminates the process*/
		exit(-1);
	}
	/** for the child process */
	else if(child == 0) {
		/**execute the command */
		if (execvp(*argv,argv)<0) {
			/** Prints which process was unable to execute */
			printf("Unable to execute %s\n",argv[0]);
		}
	}
	/** for the parent process (child>0)*/
	else if (child > 0){
		/** wait for the child process to finish before executing */
		if (wait(&status) == -1) {
			perror("Process exited with error");
		}
		else {
			perror("Process exited successfully");
		}
	}
}
/** executes the commands from the tokenized list and produces warnings/errors
* in some situations for example if we have a token cd < ls cs570, this should
* become cd cs570, since any argument after a > or < is ignored
* along with it
*/
void command::execute_commands(list<string> tokenList) {
	/**used for fork*/
	char *buff[100];
	/**pointer to pointer to int */
	char **p;
	p=buff;

	string current;
	int count = 0;
	/** used for | (pipe) token */
	bool pipe = false;
	/**used for <, >, & */
	bool both = false;
	/** used for "pwd" token */
	bool pwd = false;
	/** used for "cd" token */
	bool cd = false;

	/** iterates through tokenList */
	for(list<string>::iterator it = tokenList.begin(); it != tokenList.end(); it++) {
		count++;
		/**used for determining arguments (i.e. arguments after > <) */
		list<string>::iterator prev = --it;
		current = *++it;
		list<string>::iterator next = ++it;
		it--;
		/**if token is not cd or the end of the list or command terminator */
		if (current != "cd" && (next == tokenList.end() || *next == ";" || *next == "|")) {
			if(cd) {
				/**cd must be with an argument */
				if(count==2)
					execute_cd(current);
				/** if it's just cd or has more than one argument */
				else
					cout << "Error: Accepts exactly one argument." << endl;
			}
			/** if pwd is found by itself, we execute_pwd() */
			else if(pwd) {
				if (count == 1)
					execute_pwd();
			}
			/** if pwd is found in list, we execute_pwd()*/
			else if(current == "pwd") {
				execute_pwd();
			}
			/**sets the address of current to p, increment p
			* and execute execute_fork(buff);
			*/
			else {
				*p=&current[0u];
				p++;
				execute_fork(buff);
			}
			cd=false;
			count=0;
			/**if next is not the end of the list, keep going*/
			if(next!=tokenList.end())
				it++;
			continue;
		}
		/** if ; is found, we skip it and continue with the tokenList */
		else if(current == ";")
			continue;
		/**if | is found, we skip it by decrementing by one */
		else if(current == "|") {
			count = count-1;
			pipe = true;
		}
		/**if redirections are found, decrease count by 2 */
		else if(current == ">" || current == "<") {
			count = count-2;
			both = true;
		}
		/**if background operator is found */
		else if(current =="&") {
			both=true;
		}
		/**if pwd is found */
		else if (current == "pwd") {
			pwd = true;
		}
		/**if cd is found */
		else if(current == "cd") {
			/**if there's nothing after produce an error */
			if(next==tokenList.end()) {
				cout << "Error: Accepts exactly one argument." << endl;
				continue;
			}
			/**the next argument is the "command" i.e. cd cs570*/
			else {
				cd = true;
				current = *next;
			}
		}
		else {
			string command = current;
			/** address of command set to p*/
			*p=&command[0u];
			/**increment p*/
			p++;
		}
	}
	/** if <, >, and/or & is in the token string this warning will be produced */
	if (both==true)
		cout << "Warning: IO redirection and background not implemented" << endl;
	/** if | is found, this warning will be produced */
	if(pipe)
		cout << "Warning: Pipe not implemented" << endl;
}
