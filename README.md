# miniRPEL

This project contains a interpreter of a very small programing language, miniREPL. The interpreter 
would be able to parse source code file or run an interactive session as a REPL (Read Eval Print Loop).

This is just a hobby project to increase my computer science skills and C proficiency. Feel free to suggest 
or comment anything.

## Build

Be sure that you have installed gcc and make on your computer. Clone the repo and jump into the repo directory.
Then use make to build the program in release mode.

    make

If you want to debug, be sure to compile it in debug mode:

    make DEBUG=YES

To clean any artifact just run:

    make clean
