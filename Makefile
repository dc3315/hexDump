CC=gcc # The GNU C compiler.
# Below are some useful warnings.
CFLAGS=-Wall -pedantic -std=c99 \
			 -Wextra -m64 -Wshadow -Wpointer-arith \
			 -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -c

# What sources are we compiling ? 
SOURCES=hdmain.c

# Dependencies.
hd.exe: hdmain.o
	$(CC) hdmain.o -o hd.exe

hdmain.o: hdmain.c
	$(CC) $(CFLAGS) hdmain.c

# Clean all unwanted files.
clean:
	    rm *.o hd.exe
