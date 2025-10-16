get_next_line

A minimalist, dependency-free implementation of the classic get_next_line function in C.
It reads from a file descriptor and returns exactly one line per call (including the trailing newline when present), handling partial reads and buffering under the hood.

Repository layout and language breakdown are based on the files currently in this repo (get_next_line.c, get_next_line.h, get_next_line_utils.c, bonus variants, and a sample test2.txt). The repository is 100% C. 
GitHub

✨ Features

Single-line return: each call returns the next line (or NULL on EOF/error).

Configurable buffer size: set with the BUFFER_SIZE macro at compile time.

No external deps: uses only the C standard library/POSIX read(2).

Bonus (multi-FD): optional bonus version that keeps a separate buffer per file descriptor, allowing interleaved reads.

📦 Project Structure
get_next_line.c
get_next_line.h
get_next_line_utils.c
get_next_line_bonus.c
get_next_line_bonus.h
get_next_line_utils_bonus.c
test2.txt


(File names reflect the repository contents.) 
GitHub

📚 API
// get_next_line.h
char    *get_next_line(int fd);


Contract:

Returns a heap-allocated string containing the next line read from fd.
The returned string includes the newline '\n' when one was read.

Returns NULL on EOF (no more data) or error.

Caller must free() the returned string.

Notes:

Reading uses an internal buffer of size BUFFER_SIZE (define at compile time).

The function should be reentrant across calls for the same fd.

Bonus variant supports multiple FDs concurrently (one stash per fd).

🔧 Building

This repository doesn’t include a Makefile, so compile directly with gcc (or your preferred compiler).
Set the buffer size via -D BUFFER_SIZE=<N>; typical values range from 1 to 4096.

Build the mandatory version
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
    get_next_line.c get_next_line_utils.c \
    -o gnl_demo

Build the bonus (multi-FD) version
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
    get_next_line_bonus.c get_next_line_utils_bonus.c \
    -o gnl_bonus_demo


If you integrate get_next_line into another project, just add the *.c files to your build and include get_next_line.h.

▶️ Usage Examples
1) Read a file line by line
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_next_line.h"

int main(void)
{
    int   fd = open("test2.txt", O_RDONLY);
    if (fd < 0) return 1;

    char *line;
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);   // line already contains '\n' if present
        free(line);
    }
    close(fd);
    return 0;
}


Compile and run:

gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
    get_next_line.c get_next_line_utils.c demo.c -o demo

./demo

2) Read from standard input (stdin)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "get_next_line.h"

int main(void)
{
    char *line;
    while ((line = get_next_line(STDIN_FILENO)) != NULL)
    {
        // Echo each line back
        printf(">> %s", line);
        free(line);
    }
    return 0;
}

3) Bonus: Interleave multiple file descriptors
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_next_line_bonus.h"

int main(void)
{
    int fd1 = open("test2.txt", O_RDONLY);
    int fd2 = open("test2.txt", O_RDONLY); // example uses same file twice

    char *l1 = get_next_line(fd1);
    char *l2 = get_next_line(fd2);
    printf("fd1: %s", l1); free(l1);
    printf("fd2: %s", l2); free(l2);

    close(fd1);
    close(fd2);
    return 0;
}

🧠 Implementation Notes

Buffering strategy: The function accumulates data read in chunks of BUFFER_SIZE until a newline is found or EOF is reached, then returns the next line slice.

Persistent stash: Any extra data after the newline is kept for the next call.

Memory management: Each returned line is dynamically allocated; it’s the caller’s responsibility to free() it.

Edge cases handled:

Empty files and last line without trailing newline.

Very long lines (longer than BUFFER_SIZE) via repeated reads.

BUFFER_SIZE <= 0 should yield NULL.

read() errors and invalid fds result in NULL.

✅ Testing Tips

Try different buffer sizes to stress the logic:

for N in 1 2 4 8 16 32 64 128 1024; do
  gcc -D BUFFER_SIZE=$N get_next_line.c get_next_line_utils.c test_sanity.c -o t$N
  ./t$N
done


Test files:

test2.txt is included for quick checks. 
GitHub

Create files with very long lines or without trailing newlines.

Pipe data from commands:

echo -e "a\nb\nc" | ./gnl_demo

🔍 Complexity

Time: Amortized O(L) per returned line (L = line length), plus O(B) per chunk read.

Space: O(L) for the line + up to O(B) for internal stash per active fd.

🧩 Integration Guidelines

Include the appropriate header:

#include "get_next_line.h" (mandatory)

#include "get_next_line_bonus.h" (bonus)

Add the corresponding .c files to your build.

Define BUFFER_SIZE globally in your build system (e.g., compiler flag) to keep headers clean.

⚠️ Limitations

Relies on POSIX read(2) semantics. On non-POSIX systems you may need adaptations.

Not thread-safe unless you guard calls per fd at a higher level.

Large numbers of simultaneously open FDs can increase memory usage in the bonus version.
