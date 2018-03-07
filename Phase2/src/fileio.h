void dump_buffer(void *buffer, int buffer_size);

int write_buffer(char *fname, void *buffer, int buffer_size);

int set_buffer(char **destination, void *source, int buffer_size);

int ReadFile(char *name, char** buffer);

char *remove_ext (char* mystr, char dot, char sep);