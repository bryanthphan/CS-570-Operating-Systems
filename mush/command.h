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

#include <readline/readline.h>
#include <readline/history.h>

using namespace std;

class command {
	private:
		/**list of strings*/
		list<string> *tokenList;
	public:
		/**chdir() function */
		void execute_cd(string command);
		/**getpwd() function */
		void execute_pwd();
		/**fork system call */
		void execute_fork(char *argv[]);
		/** retrieve tokens after tokenization and execute commands */
		void execute_commands(list<string> tokenList);

};
