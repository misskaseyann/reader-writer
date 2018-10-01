# reader-writer
A Writer program that creates a shared memory segment, then repeatedly accepts an arbitrary user input string from the terminal and writes it into shared memory. A corresponding Reader program; two instances of it will attach to the same shared memory segment. The Readers will repeatedly read each new string from the shared memory and display the string to the screen.
