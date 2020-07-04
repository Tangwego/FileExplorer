#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QTreeWidget>
#include "FileUtil.h"

class TreeView : public QTreeWidget
{
public:
    explicit TreeView(QWidget *parent = nullptr);
    void setAttr(FileAttr attr){
        mFileAttr = attr;
    };

    FileAttr getAttr(){
        return mFileAttr;
    };

private:
    FileAttr mFileAttr;

};

#endif // TREEVIEW_H
