#ifndef UTIL_H
#define UTIL_H

void *file_contents(const char *filename, GLint *length);
void *read_tga(const char *filename, int *width, int *height);

GLuint load_texture_png(const char *filename);

void computeFPS(double* fpsCount);

double getElapsedTimeInSeconds();

#endif /* UTIL_H */
