# cshell
Concurrent shell that processes user input, built in C with pThreads.

User inputs command in console (i.e. cat input.txt | wc), and each sub-command (cat and wc) is made into a separate thread.

Project is work in progress. Only basic functionality works currently.

Test using ./shell

When ">" character appears in console, input cat input.txt.

This will create two threads, one for the cat sub-command and one for the print sub-command (which is implicitly added to the end of each command to print output to the console).
