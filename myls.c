#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

typedef struct{
	unsigned char file_type;
	char host_perm[3];
	char group_perm[3];
	char others_perm[3];
	nlink_t links_num;
	char* host_name;
	char* group_name;
	off_t file_size;
	char time_min;
	char time_hour;
	char time_day;
	char* time_month;
	char* file_name;
}ls_variables;

int main(int argc, char** argv)
{
	char current_dir[] = ".";
	char current_file_path[] = "./";
	char* path = NULL;
	char file_path[255];
	DIR* opendir_return_value;
	struct dirent* readdir_return_value;
	ls_variables current_file;
	struct stat stat_buf;
	char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	if(argc == 1)
	{
		path = current_dir;
	}else
	{
		path = argv[1];
	}
	
	opendir_return_value = opendir(path);
	if(opendir_return_value == NULL)
	{
		printf("Error occured during excuting opendir\n");
		return -1;
	}
	
	while((readdir_return_value = readdir(opendir_return_value)) != NULL)
	{
		current_file.file_name = readdir_return_value->d_name;

		if(argc == 1)
		{
			strcpy(file_path, current_file_path);
		}else
		{
			strcpy(file_path, path);
		}
		strcat(file_path, current_file.file_name);
		int stat_return_value = stat(file_path, &stat_buf);
		if(stat_return_value != 0)
		{
			printf("Error occured during excuting stat\n");
			return -2;
		}
		current_file.links_num = stat_buf.st_nlink;
		current_file.file_size = stat_buf.st_size;
		struct passwd* getpwnam_return_value = getpwuid(stat_buf.st_uid);
		current_file.host_name = getpwnam_return_value->pw_name;
		struct group* getgrnam_return_value = getgrgid(stat_buf.st_gid);
		current_file.group_name = getgrnam_return_value->gr_name;

                switch(((stat_buf.st_mode)&S_IFMT))
                {
			case S_IFDIR:
				current_file.file_type = 'd';
				break;
			case S_IFLNK:
                                current_file.file_type = 'l';
                                break;
			case S_IFCHR:
                                current_file.file_type = 'c';
                                break;
			case S_IFBLK:
                                current_file.file_type = 'b';
                                break;
			default:
				current_file.file_type = '-';
                }



		/*Switch case for the owner permissions*/
		switch(((stat_buf.st_mode)&S_IRWXU))
		{
			case S_IRWXU:
				current_file.host_perm[0] = 'r';
				current_file.host_perm[1] = 'w';
				current_file.host_perm[2] = 'x';
				break;
			case S_IRUSR:
				current_file.host_perm[0] = 'r';
                                current_file.host_perm[1] = '-';
                                current_file.host_perm[2] = '-';
                                break;
			case S_IWUSR:
                                current_file.host_perm[0] = '-';
                                current_file.host_perm[1] = 'w';
                                current_file.host_perm[2] = '-';
                                break;
			case S_IXUSR:
                                current_file.host_perm[0] = '-';
                                current_file.host_perm[1] = '-';
                                current_file.host_perm[2] = 'x';
				break;
			case S_IRUSR|S_IWUSR:
                                current_file.host_perm[0] = 'r';
                                current_file.host_perm[1] = 'w';
                                current_file.host_perm[2] = '-';
                                break;
			case S_IRUSR|S_IXUSR:
                                current_file.host_perm[0] = 'r';
                                current_file.host_perm[1] = '-';
                                current_file.host_perm[2] = 'x';
                                break;
			case S_IWUSR|S_IXUSR:
                                current_file.host_perm[0] = '-';
                                current_file.host_perm[1] = 'w';
                                current_file.host_perm[2] = 'x';
                                break;
			default:
				current_file.host_perm[0] = '-';
                                current_file.host_perm[1] = '-';
                                current_file.host_perm[2] = '-';
		}

		/*Switch case for the group permissions*/
                switch(((stat_buf.st_mode)&S_IRWXG))
                {
                        case S_IRWXG:
                                current_file.group_perm[0] = 'r';
                                current_file.group_perm[1] = 'w';
                                current_file.group_perm[2] = 'x';
                                break;
                        case S_IRGRP:
                                current_file.group_perm[0] = 'r';
                                current_file.group_perm[1] = '-';
                                current_file.group_perm[2] = '-';
                                break;
                        case S_IWGRP:
                                current_file.group_perm[0] = '-';
                                current_file.group_perm[1] = 'w';
                                current_file.group_perm[2] = '-';
                                break;
                        case S_IXGRP:
                                current_file.group_perm[0] = '-';
                                current_file.group_perm[1] = '-';
                                current_file.group_perm[2] = 'x';
                                break;
			case S_IRGRP|S_IWGRP:
                                current_file.group_perm[0] = 'r';
                                current_file.group_perm[1] = 'w';
                                current_file.group_perm[2] = '-';
                                break;
                        case S_IRGRP|S_IXGRP:
                                current_file.group_perm[0] = 'r';
                                current_file.group_perm[1] = '-';
                                current_file.group_perm[2] = 'x';
                                break;
                        case S_IWGRP|S_IXGRP:
                                current_file.group_perm[0] = '-';
                                current_file.group_perm[1] = 'w';
                                current_file.group_perm[2] = 'x';
                                break;
                        default:
                                current_file.group_perm[0] = '-';
                                current_file.group_perm[1] = '-';
                                current_file.group_perm[2] = '-';
                }

		/*Switch case for the others permissions*/
                switch(((stat_buf.st_mode)&S_IRWXO))
                {
                        case S_IRWXO:
                                current_file.others_perm[0] = 'r';
                                current_file.others_perm[1] = 'w';
                                current_file.others_perm[2] = 'x';
                                break;
                        case S_IROTH:
                                current_file.others_perm[0] = 'r';
                                current_file.others_perm[1] = '-';
                                current_file.others_perm[2] = '-';
                                break;
                        case S_IWOTH:
                                current_file.others_perm[0] = '-';
                                current_file.others_perm[1] = 'w';
                                current_file.others_perm[2] = '-';
                                break;
                        case S_IXOTH:
                                current_file.others_perm[0] = '-';
                                current_file.others_perm[1] = '-';
                                current_file.others_perm[2] = 'x';
                                break;
			case S_IROTH|S_IWOTH:
                                current_file.others_perm[0] = 'r';
                                current_file.others_perm[1] = 'w';
                                current_file.others_perm[2] = '-';
                                break;
                        case S_IROTH|S_IXOTH:
                                current_file.others_perm[0] = 'r';
                                current_file.others_perm[1] = '-';
                                current_file.others_perm[2] = 'x';
                                break;
                        case S_IWOTH|S_IXOTH:
                                current_file.others_perm[0] = '-';
                                current_file.others_perm[1] = 'w';
                                current_file.others_perm[2] = 'x';
                                break;
                        default:
                                current_file.others_perm[0] = '-';
                                current_file.others_perm[1] = '-';
                                current_file.others_perm[2] = '-';
                }

		/*Switch case for special bits*/
		switch(((stat_buf.st_mode)&07000))
		{
			case S_ISUID:
				current_file.host_perm[2] = 's';
				break;
			case S_ISGID:
				current_file.group_perm[2] = 's';
				break;
			case S_ISVTX:
				current_file.others_perm[2] = 't';
				break;
			default:
		}

		struct tm *gmtime_return_value = gmtime(&stat_buf.st_ctime);
		current_file.time_min = gmtime_return_value->tm_min;
		current_file.time_hour = gmtime_return_value->tm_hour + 3;
		if(current_file.time_hour > 23)
		{
			current_file.time_hour -= 24;
		}
		current_file.time_day = gmtime_return_value->tm_mday;
		current_file.time_month = months[gmtime_return_value->tm_mon];
		
		printf("%c%c%c%c%c%c%c%c%c%c %ld %3s %s %ld \t %d:%d %2d %5s %2s\n",
				current_file.file_type,
				current_file.host_perm[0],
				current_file.host_perm[1],
				current_file.host_perm[2],
				current_file.group_perm[0],
                                current_file.group_perm[1],
                                current_file.group_perm[2],
				current_file.others_perm[0],
                                current_file.others_perm[1],
                                current_file.others_perm[2],
				current_file.links_num,
				current_file.host_name,
				current_file.group_name,
				current_file.file_size,
				current_file.time_hour,
				current_file.time_min,
				current_file.time_day,
				current_file.time_month,
				current_file.file_name);
	}
	closedir(opendir_return_value);
	return 0;
}
