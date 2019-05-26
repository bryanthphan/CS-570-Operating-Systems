The start of this assignment is to use a tokernizer (i.e. tokenizer.cpp) to parse out lines read from the user or a redirected file. 
The token list should then be analyzed for commands which are separated by command terminators (semicolon and pipe: ; |). 
Each command will be executed by a function in file command

  Commands containing tokens for file redirections “<” or “>” will be stripped of
  the file redirect and its argument. Commands containing the background operator
  “&” will be stripped of the background operator. For example: [{cat}{bozo.tzt},
  {>},{clowns.txt}] becomes : [{cat}{bozo.tzt}]. In both cases, the warning “IO
  redirection and background not implemented” will be produced.

  Commands starting with the token cd will accept exactly one argument, e.g. “cd
  ..” or “cd cs570”. If there is not exactly one argument, the error message “Accepts 
  exactly one argument” pops up. I used the chdir system call in the man pages (man chdir) to 
  change the directory to the requested directory. If chdir fails, the message “Directory does 
  not exist or is not accessible.” pops up. 

  Commands with the single token “pwd” will call the getcwd system call (man
  getcwd) to obtain the current directory. If successful, the current working
  directory will be printed. On error, write “Unable to obtain current directory”.
  
  Otherwise, the command is assumed to be a program to be launched. For this
  you will need to execute a fork system call (man fork) to create a new process.
  The parent process will wait (man -s 2 wait)1 for the child to complete and will
  then continue executing. The child will use execvp (man execvp) to attempt to
  load in the specified program and its arguments. The argv pointer in this system
  call is a pointer to an array of pointers that you will need to construct. argv[0]
  should point to your program to be executed with argv[1] pointing to the first
  argument, and so on. The last entry in argv must be NULL. 
   
  If the fork fails, "Unable to spawn program" appears (this should not happen). 
  If the execvp does not work (e.g. it returns), "Unable to execute %s" pops up where
  %s is the program name. 
  Once the child has exited, the return wait will execute either "Process exited with error" 
  or "Process exited successfully". 

As this shell is truly underachieving, when users connect commands with a
pipe symbol “|”, the commands are treated as if they were separated by a “;” along
with a warning “Pipe not implemented”. 

In order to run the program on your terminal, type the command "make" followed by "mush" in order to compile it. 
