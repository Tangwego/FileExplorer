#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <QList>

typedef enum _FileType{
    EXECUTABLE, ASCII_TEXT, SHARED_LIBRARY, STATIC_LIBRARY,DIRECTORY, NORMAL_FILE,LINK_FILE,LINK_DIRECTORY
}FileType;

typedef struct _FileAttr{
    QString fileName;
    QString filePath;
    QString fileColor;
    QString fileOwner;
    QString fileGroup;
    QString fileMode;
    FileType fileType;
    bool isHide;
    QString fileSize;
} FileAttr;

int _time(int year);
QString time_ch(time_t num);
char* get_mode(mode_t m,char* str);
char* kscale(unsigned long b, unsigned long bs);

class FileUtil
{
public:
    FileUtil();

public:
    QList<FileAttr> getFileList(QString pwd);
    QList<FileAttr> getMountedDisks();
    QList<FileAttr> findFiles(QString searchPath, QString fileName);

private:
    void findFileByName(QString path, QString name);

private:
    QList<FileAttr> mSearchFiles;



};

#endif // FILEUTIL_H
