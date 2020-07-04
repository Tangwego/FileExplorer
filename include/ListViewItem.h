#ifndef LISTVIEWITEM_H
#define LISTVIEWITEM_H

#include <QListWidgetItem>
#include "FileUtil.h"

class ListViewItem : public QListWidgetItem
{
public:
    explicit ListViewItem();

    void setAttr(FileAttr attr){
        mFileAttr = attr;
    };

    FileAttr getAttr(){
        return mFileAttr;
    };

private:
    FileAttr mFileAttr;


};

#endif // LISTVIEWITEM_H
