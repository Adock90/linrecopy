#ifndef LIBLINRECOPY_H
#define LIBLINRECOPY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#define MAX_PATH 4096


void verify_dir_exists(char* path, int correct);


void copy_dir_recursive(char* dir_from, char* dir_to);

#endif
