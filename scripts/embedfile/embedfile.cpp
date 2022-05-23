// See https://stackoverflow.com/questions/11813271/embed-resources-eg-shader-code-images-into-executable-library-with-cmake
#include <stdlib.h>
#include <stdio.h>
#include <string>

FILE* open_or_exit(const char* fname, const char* mode)
{
  FILE* f = fopen(fname, mode);
  if (f == NULL) {
    perror(fname);
    exit(EXIT_FAILURE);
  }
  return f;
}

int main(int argc, char** argv)
{
  if (argc < 3) {
    return EXIT_FAILURE;
  }

  const char* symfile = argv[1];
  FILE* in = open_or_exit(argv[2], "r");

  FILE* out = open_or_exit(symfile, "w");

  std::string sym = symfile;
    
    auto pos = sym.find(".");
    
    if (pos != std::string::npos) {
        sym = sym.substr(0, pos);
    }

  fprintf(out, "#include <string>\n\n");
  fprintf(out, "static const char data[] = {\n");

  unsigned char buf[256];
  size_t nread = 0;
  size_t linecount = 0;
  do {
    nread = fread(buf, 1, sizeof(buf), in);
    size_t i;
    for (i=0; i < nread; i++) {
      fprintf(out, "0x%02x, ", buf[i]);
      if (++linecount == 10) { fprintf(out, "\n"); linecount = 0; }
    }
  } while (nread > 0);
  if (linecount > 0) fprintf(out, "\n");
  fprintf(out, "0};\n");
  fprintf(out, "std::string get_%s() { return data; }\n\n",sym.data());

  fclose(in);
  fclose(out);

  return EXIT_SUCCESS;
}
