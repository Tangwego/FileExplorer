#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QListWidget>
#include "FileUtil.h"

class ListView : public QListWidget
{
public:
    explicit ListView(QWidget *parent = nullptr);

    void removeAll(){
        int n=count();
        for(int i=0;i<n;i++)
        {
            QListWidgetItem *item = takeItem(0);
            delete item;
        }
    };

    void setAttr(FileAttr attr){
        mFileAttr = attr;
    };

    FileAttr getAttr(){
        return mFileAttr;
    };

private:
    FileAttr mFileAttr;

};

#endif // LISTVIEW_H
