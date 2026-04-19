#include "liblinrecopy.h"

const int file_per = 0755;

size_t get_file_size(char* path)
{
	FILE* src_fptr = fopen(path, "r");
        if (src_fptr == NULL)
        {
                printf("hh");
                return -1;
        }

	fseek(src_fptr, 0, SEEK_END);
	size_t sizeoffile = ftell(src_fptr);
	fclose(src_fptr);

	return sizeoffile;
}

int copy_file(char* src, char* dst)
{
	char file_content_buffer[MAX_PATH];
	ssize_t read_count;

	int src_file = open(src, O_RDONLY);
	if (src_file < 0)
	{
		printf("[LINRECOPY] Failed to open source file: %s\n", src);
		return -1;
	}

	int dst_file = open(dst, O_WRONLY | O_CREAT | O_EXCL, file_per);
	if (dst_file < 0)
	{
		printf("[LINRECOPY] Failed to open destination file: %s\n", dst);
		close(src_file);
		return -1;
	}

	while (read_count = read(src_file, file_content_buffer, sizeof(file_content_buffer)), read_count > 0)
	{
		while(read_count > 0)
		{
			char* out_pointer = file_content_buffer;
			ssize_t written_count = write(dst_file, out_pointer, read_count);
			if (written_count >= 0)
			{
				read_count -= written_count;
				out_pointer += written_count;
			}
			else if (errno != EINTR)
			{
				close(dst_file);
				close(src_file);

				return -1;
			}
		}
	}
	close(dst_file);
	close(src_file);

	return 0;
}

int create_dir_recur(char* path)
{
	char temp_store[MAX_PATH];
	strcpy(temp_store, path);
	size_t length = strlen(temp_store);
	if (temp_store[length - 1] == '/')
	{
		temp_store[length - 1] = 0;
	}

	for (char* p = temp_store + 1; *p; p++)
	{
		if (*p == '/')
		{
			*p = 0;
			mkdir(temp_store, file_per);
			*p = '/';
		}
	}
	return mkdir(temp_store, file_per);
}


void verify_dir_exists(char* path, int correct)
{
	struct stat st = {0};

	if (stat(path, &st) == -1)
	{
		if (correct)
		{
			printf("Correcting Invalid Directory: %s\n", path);
			mkdir(path, file_per);
		}
		else
		{
			printf("Invalid Path: %s\n", path);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		if (!S_ISDIR(st.st_mode))
		{
			printf("Invalid Path: %s\nIs not a directory.\n", path);
			exit(EXIT_FAILURE);
		}
		else
		{
			printf("Verified\n");
		}
	}
}


void copy_dir_recursive(char* dir_from, char* dir_to)
{
	const char* illegal_chars = "<>:/\\|?*";

	char src_buff[MAX_PATH];
	char dst_buff[MAX_PATH];

	size_t length = strlen(dir_to);
	char dir_to_clean[MAX_PATH];
	strcpy(dir_to_clean, dir_to);
	if (dir_to_clean[length - 1] == '/')
	{
		dir_to_clean[length - 1] = 0;
	}

	char src_path[MAX_PATH];
	sprintf(src_path, "%s\\*", dir_from);
	DIR* dir;
	dir = opendir(dir_from);
	struct dirent *ent;
	struct stat stat_buff;
        int status;
	


	if (dir != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			sprintf(src_buff, "%s/%s", dir_from, ent->d_name);
		       	sprintf(dst_buff, "%s/%s", dir_to_clean, ent->d_name);
			if (strcmp(ent->d_name, "..") == 0 || strcmp(ent->d_name, ".") == 0)
                        {
                        	continue;
                        }

			status = stat(src_buff, &stat_buff);
			if (status == -1)
			{
				fprintf(stderr, "[LINRECOPY] Failed to retrieve file info. File: %s\n, ERROR CODE: %d\n", ent->d_name, status);
			}
			printf("%d\n", stat_buff.st_mode);
			if (S_ISDIR(stat_buff.st_mode))
			{

				int res = create_dir_recur(dst_buff);
				if (res != 0)
				{
					printf("[LINRECOPY] Failed to create directory: %s. ERROR CODE: %d\n", dst_buff, res);
				}
				else
				{
					copy_dir_recursive(src_buff, dst_buff);
				}
			}
			else
			{
				if (strpbrk(ent->d_name, illegal_chars) != NULL)
				{
					printf("Skipping %s due to invalid chars\n", src_buff);
				}
				else
				{
					if (copy_file(src_buff, dst_buff) != 0)
					{
						printf("[LINRECOPY] Failed to copy: %s\n", src_buff);
					}
					else
					{
						printf("Copied Bytes from: %s -> %s\n", src_buff, dst_buff);
					}
				}
			}
		}
		closedir(dir);
	}
}
