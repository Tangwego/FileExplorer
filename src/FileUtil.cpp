#include "FileUtil.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

#include <mntent.h>
#include <sys/vfs.h>

#include "Log.h"

static const unsigned long long G = 1024*1024*1024ull;
static const unsigned long long M = 1024*1024;
static const unsigned long long K = 1024;
static char str[20];

char* kscale(unsigned long b, unsigned long bs)
{
    unsigned long long size = b * (unsigned long long)bs;
    bzero(str,sizeof(str));
    if (size > G)
    {
        sprintf(str, "%0.2f GB", size/(G*1.0));
        return str;
    }
    else if (size > M)
    {
        sprintf(str, "%0.2f MB", size/(1.0*M));
        return str;
    }
    else if (size > K)
    {
        sprintf(str, "%0.2f K", size/(1.0*K));
        return str;
    }
    else
    {
        sprintf(str, "%0.2f B", size*1.0);
        return str;
    }
}

QString getSize(unsigned int size)
{
    QString num;
    if (size > G)
    {
        num = QString("%1").number(size/(G*1.0),'f',2);
        return QString("%1GB").arg(num);
    }
    else if (size > M)
    {
        num = QString("%1").number(size/(M*1.0),'f',2);
        return QString("%1MB").arg(num);
    }
    else if (size > K)
    {
        num = QString("%1").number(size/(K*1.0),'f',2);
        return QString("%1KB").arg(num);
    }
    else
    {
        num = QString("%1").number(size*1.0,'f',2);
        return QString("%1B").arg(num);
    }
    return "0";
}


QList<FileAttr> FileUtil::getMountedDisks()
{
    QList<FileAttr> disks;
    FILE* mount_table;
    struct mntent *mount_entry;
    struct statfs s;
    unsigned long blocks_used;
    unsigned blocks_percent_used;
    mount_table = NULL;
    mount_table = setmntent("/etc/mtab", "r");
    if (!mount_table)
    {
        perror("set mount entry error!");
        return disks;
    }
    while (1) {
        const char *device;
        const char *mount_point;
        FileAttr attr;

        if (mount_table) {
            mount_entry = getmntent(mount_table);
            if (!mount_entry) {
                endmntent(mount_table);
                break;
            }
        }else
            continue;

        device = mount_entry->mnt_fsname;
        mount_point = mount_entry->mnt_dir;

        int ret = statfs(mount_point, &s);

        if (ret != 0)
        {
            perror("statfs failed!");
            continue;
        }

        if ((s.f_blocks > 0) || !mount_table )
        {
            blocks_used = s.f_blocks - s.f_bfree;
            blocks_percent_used = 0;
            if (blocks_used + s.f_bavail)
            {
                blocks_percent_used = (blocks_used * 100ULL
                        + (blocks_used + s.f_bavail)/2
                        ) / (blocks_used + s.f_bavail);
            }
            /* GNU coreutils 6.10 skips certain mounts, try to be compatible.  */
            if (strcmp(device, "rootfs") == 0 || strcmp(device, "udev") == 0
                    || strcmp(device, "tmpfs") == 0){
                fprintf(stdout, "It is not a avaliable file system [%s]!\n",device);
                continue;
            }

            attr.filePath = QString("%1").arg(mount_point);
            attr.fileName = QString("%1").arg(mount_point);
            attr.fileSize = QString("%1").arg(kscale(s.f_blocks, s.f_bsize));

            disks.append(attr);
        }
    }
    return disks;
}

QList<FileAttr> FileUtil::getFileList(QString pwd)
{
    QList<FileAttr> fileList, dirList, result;
    QString dirname;
    if(pwd == NULL || pwd.isNull() || pwd.isEmpty()){
        dirname = QString("%1").arg(".");
    }else{
        dirname = QString("%1").arg(pwd.toLocal8Bit().data());
    }

    DIR* dp=opendir(dirname.toLocal8Bit().data());

    if(NULL == dp)
    {
        perror("opendir");
        return result;
    }
    struct dirent* de;

    while((de = readdir(dp)) != NULL)
    {
        FileAttr attr;
        struct stat s;
        if('.' == de->d_name[0]) {
            attr.isHide = true;
            continue;
        }
        attr.filePath = QString("%1/%2").arg(dirname).arg(de->d_name);

        if(lstat(attr.filePath.toLocal8Bit().data(), &s) < 0)
        {
            perror("stat");
            continue;
        }

        char str[11] = {};
        get_mode(s.st_mode,str);//类型
        attr.fileMode = QString("%1").arg(str);

        struct passwd *passwd;
        passwd = getpwuid(s.st_uid);
//        struct passwd *pwd = getpwuid(getuid());
        attr.fileOwner= QString("%1").arg(passwd->pw_name);
        struct group *group;
        group = getgrgid(passwd->pw_gid);
        attr.fileGroup = QString("%1").arg(group->gr_name);
        attr.fileSize = getSize(s.st_size);

        attr.fileName = QString("%1").arg(de->d_name);
        attr.isHide = false;

        if(S_ISDIR(s.st_mode)){
            attr.fileType = DIRECTORY;
            dirList.append(attr);
        }else{
            attr.fileType = NORMAL_FILE;
            fileList.append(attr);
        }
    }
    closedir(dp);
    result.append(dirList);
    result.append(fileList);
    return result;
}

QList<FileAttr> FileUtil::findFiles(QString searchPath, QString fileName)
{
    findFileByName(searchPath, fileName);
    return mSearchFiles;
}

void FileUtil::findFileByName(QString path, QString name)
{
    QList<FileAttr> fileList = getFileList(path);
    for(int i = 0; i < fileList.size();i++){
        FileAttr temp = fileList.at(i);
        if(temp.fileName == name){
            mSearchFiles.append(temp);
        }
        if(temp.fileType == DIRECTORY){
            findFileByName(temp.filePath, name);
        }
    }
}


int _time(int year)//计算是否是闰年函数
{
   if((year%4==0 && year%100 !=0) || year%400 == 0)
    return 29;
   return 28;
}

QString time_ch(time_t num)//通过秒数来计算日期
{
   int year=1970;
   int month =1;
   int day =1;
   QString time_s;
   num = num + 8*3600;
   while(num >= 86400)
{
    num-=86400;
    day++;
    if(month==1 && day == 32)
    {
        month++;
        day =1;
    }
    else if(month == 2 && day ==_time(year)+1)
    {
        month++;
        day =1;
    }
    else if(month == 3 && day == 32)
    {
        month++;
        day =1;
    }
    else if(month == 4 && day == 31)
    {
        month++;
        day=1;
    }
    else if(month == 5 && day == 32)
    {
        month++;
        day=1;
    }
    else if(month == 6 && day == 31)
    {
        month++;
        day=1;
    }
    else if(month == 7 && day == 32)
    {
        month++;
        day=1;
    }
    else if(month == 8 && day == 32)
    {
        month++;
        day=1;
    }
    else if(month == 9 && day == 31)
    {
        month++;
        day=1;
    }
    else if(month == 10 && day == 32)
    {
        month++;
        day=1;
    }
    else if(month == 11 && day == 31)
    {
        month++;
        day=1;
    }
    else if(month == 12 && day == 32)
    {
        month=1;
        day=1;
        year++;
    }

   }
   int hour = num/3600;
   int minute =num/60 -hour*60;

   time_s = QString("%1%2 %3:%4")
           .arg(month).arg(day).arg(hour).arg(minute);

   return time_s;
}

char* get_mode(mode_t m,char* str)//显示文件读写权限函数
{
   if(S_ISREG(m))
    strcpy(str,"-");
   else if(S_ISDIR(m))
    strcpy(str,"d");
   else if(S_ISCHR(m))
    strcpy(str,"c");
   else if(S_ISBLK(m))
    strcpy(str,"b");
   else if(S_ISFIFO(m))
    strcpy(str,"f");
   else if(S_ISLNK(m))
    strcpy(str,"l");
   else if(S_ISSOCK(m))
    strcpy(str,"n");

   // 属主权限
   strcat(str,m&S_IRUSR?"r":"-");
   strcat(str,m&S_IWUSR?"w":"-");
   strcat(str,m&S_IXUSR?"x":"-");

   // 同组权限
   strcat(str,m&S_IRGRP?"r":"-");
   strcat(str,m&S_IWGRP?"w":"-");
   strcat(str,m&S_IXGRP?"x":"-");

   // 其它权限
   strcat(str,m&S_IROTH?"r":"-");
   strcat(str,m&S_IWOTH?"w":"-");
   strcat(str,m&S_IXOTH?"x":"-");

   return str;
}

FileUtil::FileUtil()
{

}
