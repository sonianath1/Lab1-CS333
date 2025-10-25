// Sonia Nath - mystat.c
//readlink() - display file status
//memset()
//getpwuid()
//getgrgrid()
//strcat()
//localtime()
//gmtime()
//strftime()



#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <errno.h>


// function prototypes
void print_file_info(const char* file_name);
void file_type(const char* file_name);
//void print_time(const char* label, time_t t);
//void print_time();
void print_time(const char* file_name);
char* file_perms(const char* file_name, char* permissions);

static const int BUFFER = 100;


int main(int argc, char *argv[])
{

	for (int i = 1; i < argc; ++i)
		print_file_info(argv[i]);

	exit(EXIT_SUCCESS);
}



void print_file_info(const char* file_name)
{
	struct stat sb;
	char permissions[11];
	struct passwd* pw;
	struct group* gr;

	if (lstat(file_name, &sb) == -1)
	{
		perror("lstat");
		return;
	}
/*
	if (realpath(file_name, abs_path))
		printf("File: %s\n", abs_path);
	else
		printf("File: %s\n", file_name); // e.g., for dangling symlink
*/
	printf("File: %s\n", file_name);
	file_type(file_name);

	printf("  Device ID number:         %ld\n", (long) sb.st_dev);
	printf("  I-node number:            %ld\n", (long) sb.st_ino);

	file_perms(file_name, permissions);	

	printf("  Mode:                     %-10s        (%03o in octal)\n"
			, permissions, sb.st_mode & 0777);

	printf("  Link count:               %ld\n", (long) sb.st_nlink);

	pw = getpwuid(sb.st_uid);
	gr = getgrgid(sb.st_gid);

	printf("  Owner Id:                 %-16s (UID = %d)\n"
			, pw ? pw->pw_name : "unknown", sb.st_uid);
	printf("  Group Id:                 %-16s (GID = %d)\n"
			, gr ? gr->gr_name : "unknown", sb.st_gid);

	printf("  Preferred I/O block size: %ld bytes\n", (long) sb.st_blksize);
	printf("  File size:                %ld bytes\n", (long) sb.st_size);
	printf("  Blocks allocated:         %ld\n", (long) sb.st_blocks);

	print_time(file_name);
}


void file_type(const char* file_name)
{
	struct stat sb;
	struct stat target_sb; 
	char link_target[PATH_MAX];

	if (lstat(file_name, &sb) == -1)
	{
		perror("lstat");
		return;
	}

	printf("  File type:                ");
	if (S_ISREG(sb.st_mode))
		printf("regular file\n");
	else if (S_ISDIR(sb.st_mode))
		printf("directory\n");
	else if (S_ISCHR(sb.st_mode))
		printf("character device\n");
	else if (S_ISBLK(sb.st_mode))
		printf("block device\n");
	else if (S_ISFIFO(sb.st_mode))
		printf("FIFO/pipe\n");
	else if (S_ISSOCK(sb.st_mode))
		printf("socket\n");
	else if (S_ISLNK(sb.st_mode))
	{
		ssize_t len = readlink(file_name, link_target, sizeof(link_target) - 1);
                if (len != -1)
                {
                        link_target[len] = '\0';
                        // Check if the target exists using stat (not lstat)
                        if (stat(file_name, &target_sb) == -1)
                                printf("Symbolic link - with dangling destination\n");
                        else
                                printf("Symbolic link -> %s\n", link_target);
                }
                else
                        printf("Symbolic link - with dangling destination\n");
	}
	else
		printf("unknown\n");
}


char* file_perms(const char* file_name, char* permissions)
{
	struct stat sb;

	if (lstat(file_name, &sb) == -1)
	{
		perror("lstat");
		return permissions;
	}


	// first char deterimiend by file type 
	if (S_ISDIR(sb.st_mode))
		permissions[0] = 'd';
	else if (S_ISLNK(sb.st_mode))
		permissions[0] = 'l';
	else if (S_ISCHR(sb.st_mode))
		permissions[0] = 'c';
	else if (S_ISBLK(sb.st_mode))
		permissions[0] = 'b';
	else if (S_ISFIFO(sb.st_mode))
		permissions[0] = 'p';
	else if (S_ISSOCK(sb.st_mode))
		permissions[0] = 's';
	else
		permissions[0] = '-';

	// owner permissions
	if (sb.st_mode & S_IRUSR)
		permissions[1] = 'r';
	else
		permissions[1] = '-';


	if (sb.st_mode & S_IWUSR)
		permissions[2] = 'w';
	else
		permissions[2] = '-';

	if (sb.st_mode & S_IXUSR)
		permissions[3] = 'x';
	else
		permissions[3] = '-';

	// group permissions
	if (sb.st_mode & S_IRGRP)
		permissions[4] = 'r';
	else
		permissions[4] = '-';

	if (sb.st_mode & S_IWGRP)
		permissions[5] = 'w';
	else
		permissions[5] = '-';

	if (sb.st_mode & S_IXGRP)
		permissions[6] = 'x';
	else
		permissions[6] = '-';

	// other permissions
	if (sb.st_mode & S_IROTH)
		permissions[7] = 'r';
	else
		permissions[7] = '-';

	if (sb.st_mode & S_IWOTH)
		permissions[8] = 'w';
	else
		permissions[8] = '-';

	if (sb.st_mode & S_IXOTH)
		permissions[9] = 'x';
	else
		permissions[9] = '-';

	// null terminate string
	permissions[10] = '\0';

	return permissions;
}



void print_time(const char* file_name)
{
	struct stat sb;
	const char* labels[] = {"Last file access:", "Last file modification:", "Last status change:"};
	time_t times[3];
	char buffer[BUFFER];
	struct tm* tm;

	if (lstat(file_name, &sb) == -1)
	{
		perror("lstat");
		return;
	}

	times[0] = sb.st_atime;
	times[1] = sb.st_mtime;
	times[2] = sb.st_ctime;

	for (int i = 0; i < 3; ++i)
		printf(" %-26s %ld (seconds since the epoch)\n", labels[i], (long)times[i]);

	for (int i =0; i < 3; ++i)
	{
		// Local time
		tm = localtime(&times[i]);
		if (tm != NULL)
		{
			strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S %z (%Z) %a", tm);
			printf(" %-26s %s (local)\n", labels[i], buffer);
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		// GMT time
		tm = gmtime(&times[i]);
		if (tm != NULL)
		{
			strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S %z (%Z) %a", tm);
			printf(" %-26s %s (GMT)\n", labels[i], buffer);
		}
	}
}
