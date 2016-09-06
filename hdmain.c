#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

// Constants.
static const size_t quadWord = 16, doubleWord = 8;
static const char   pipe = '|';

// Function Prototypes.
char getPrintable(char c);
void checkIO(int argc, char** argv);
void PrintFormattedLine(unsigned int* byteCount, unsigned int bytesRead,
                        const unsigned char* byteBuffer);

int main(int argc, char** argv)
{
  checkIO(argc, argv); // check that the arguments are valid.
  FILE*         fileHandle = fopen(argv[1], "rb"); // "rb" to read in binary.
  unsigned char byteBuffer[quadWord];
  size_t        bytesRead;
  unsigned int  byteCount = 0;
  // Loop over the file lines.
  while ((bytesRead = fread(&byteBuffer[0], 1, quadWord, fileHandle)) ==
         quadWord)
  {
    PrintFormattedLine(&byteCount, bytesRead, &byteBuffer[0]);
  }
  // Print the last line.
  PrintFormattedLine(&byteCount, bytesRead, &byteBuffer[0]);
  exit(EXIT_SUCCESS);
}

// If the char is 'printable' return it, otherwise return '.'.
char getPrintable(char c)
{
  return (0x20 <= c && c < 0x79) ? c : '.';
}

// Handle input output checking.
void checkIO(int argc, char** argv)
{
  const char helpMessage[] = "Please provide 1 argument, no less, no more.\n"
                             "Usage: ./hd FILENAME\n";
  const char invalidFileName[] = "You supplied an invalid filename.\n";
  if (argc != 2)
  {
    fprintf(stderr, helpMessage);
    exit(EXIT_FAILURE); // usually 1.
  }
  struct stat st;
  if (stat(argv[1], &st) != 0) // invoke the OS to check file existence.
  {
    fprintf(stderr, "%s\n", argv[1]);
    fprintf(stderr, invalidFileName);
    exit(EXIT_FAILURE);
  }
}

// Print the line in expected format.
void PrintFormattedLine(unsigned int* byteCount, unsigned int bytesRead,
                        const unsigned char* byteBuffer)
{
  // Print the starting byte.
  fprintf(stdout, "%08x  ", *byteCount);
  // Increment the byte count by reference.
  *byteCount += bytesRead;
  for (size_t i = 0; i < bytesRead; ++i)
  {
    fprintf(stdout, "%02x ", byteBuffer[i]);
    if ((i + 1) % doubleWord == 0)
    {
      fprintf(stdout, "  ");
    }
  }
  const size_t remainingChars = quadWord - bytesRead;
  // Print empty space when there are no other characters.
  for (size_t i = 0; i < remainingChars; ++i)
  {
    fprintf(stdout, "   ");
    if ((i + 1 + bytesRead) % doubleWord == 0)
    {
      fprintf(stdout, "  ");
    }
  }
  // Suround the last print with pipe chars.
  fprintf(stdout, "%c", pipe);
  for (size_t i = 0; i < bytesRead; ++i)
  {
    fprintf(stdout, "%c", getPrintable(byteBuffer[i]));
  }
  fprintf(stdout, "%c", pipe);
  fprintf(stdout, "\n");
}
