#include "liblinrecopy.h"

void Help()
{
	printf("[LINRECOPY] Help\n"
		"Author: Adam Croft(Adock90)\n"
		"Date Created: 19/4/26\n"
		"Documentation\n"
		"\twinrecopy.exe [SourcePath] [DestinationPath]\n"
		"\t\t[SourcePath]: The Starting Point of where you want your files to be copied from\n"
		"\t\t[DestinationPath]: The Ending Point of where you want your file to be copied to\n"
		"Examples:\n"
		"\t\tlinrecopy /bin/* /media/user/USB \n"
		"\t\tlinrecopy ~/Desktop/Projects/ ~/Documents/Projects\n"
	);

	exit(1);
}


int main(int argc, char* argv[])
{
	if (argc > 3 || argc < 3)
	{
		Help();
	}
	
	char* src = argv[1];
	char* dst = argv[2];

	verify_dir_exists(src, 0);
	verify_dir_exists(dst, 1);

	copy_dir_recursive(src, dst);

	return 0;
}
