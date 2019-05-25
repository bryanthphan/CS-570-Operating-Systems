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

#include <readline/readline.h>
#include <readline/history.h>

/** prints the contents of token in brackets seperated by comma */
/** function not used */
void iterate_tokenList(list<string> tokenList) {
	for(list<string>:: iterator it = tokenList.begin(); it != tokenList.end(); it++) {
		/**if the first token, start with [ before preceeding with the others */
		if (it == tokenList.begin()) {
			cout << "[";
		}
		/**tokens that are not first/last (i.e. {},{}, ...) */
		cout<<"{" << *it;
		if(*it != tokenList.back()) {
			cout << "},";
		}
		/**last token end */
		else if(*it == tokenList.back()) {
			cout << "}]" << endl;
		}
	}
}
int main(int argc, char *argv[]){
	/**keeps looping until user wants to end program ctrl-C (for Mac) */
	while (true) {
		list<string> tokenList;
		command c;
		string buffer;
		char *tokens = readline ("> ");
		/** oneQuote = ' ' */
		bool oneQuote = false;
		/** two Quote = " " */
		bool twoQuote = false;
		bool escapeChar = false;
		/**ends program if user presses ctrl-d/ctrl-c */
		if(tokens == NULL) {
			cout<<endl;
			break;
		}
		/**loops through tokens until there is no more input*/
		while(*tokens) {
			/**checks for the escape character */
			if(escapeChar) {
				buffer.push_back(*tokens);
				escapeChar=false;
			}
			/**checks for the escape key */
			else if(*tokens == '\'' && !twoQuote) {
				if(oneQuote == false)
					oneQuote = true;
				else
					oneQuote = false;
			}
			/**checks for inputs with double quotes*/
			else if(*tokens == '\"' && !oneQuote) {
				if (twoQuote == false)
					twoQuote = true;
				else
					twoQuote = false;
			}
			/**checks for the backslash character that is not within
			*  single quotes
			**/
			else if(*tokens == '\\' && !oneQuote) {
				escapeChar = true;
			}
			/**checks for special characters |;<>& */
			else if(*tokens=='|'||*tokens==';'||*tokens=='<'||*tokens=='>'||*tokens=='&') {
				/**checks to see if special characters arent within quotes*/
				if(!oneQuote && !twoQuote) {
					/**if buffer empty add into the list */
					if(!buffer.empty()) {
						tokenList.push_back(buffer);
					}
					/**calls the buffer*/
					buffer = "";
					/**adds tokens to buffer (char to string)*/
					buffer.push_back(*tokens);
					/**adds buffer contents to list */
					tokenList.push_back(buffer);
					/**frees buffer */
					buffer = "";
				}
			}
			/**when no quotes found, tokenizes the input by spaces
			* the isspace() function checks for white-space created by space, tab,etc.
			*/
			else if(isspace(*tokens) && !oneQuote && !twoQuote) {
				/**add to list */
				if(!buffer.empty()) {
					tokenList.push_back(buffer);
				}
				/** free buffer */
				buffer = "";
			}
			/** if there are no special characters/tokenizations */
			else {
				buffer.push_back(*tokens);
			}
			/**increments to next token */
			tokens++;
		}
		/**adds final token */
		if(!buffer.empty()) {
			tokenList.push_back(buffer);
		}
		//calls the execute_commands function
		c.execute_commands(tokenList);

	} //while(true)
	return 0;
} //main 
