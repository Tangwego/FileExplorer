#ifndef TREEVIEWITEM_H
#define TREEVIEWITEM_H

#include <QTreeWidgetItem>
#include "FileUtil.h"

class TreeViewItem : public QTreeWidgetItem
{
public:
    explicit TreeViewItem();
    void clear(){
        QList<QTreeWidgetItem *> itemList = takeChildren();
        int count = itemList.size();
        for(int i = 0; i < count; i++){
            removeChild(itemList.at(i));
        }
    };
    void setAttr(FileAttr attr){
        mFileAttr = attr;
    };

    FileAttr getAttr(){
        return mFileAttr;
    };

    void setDepth(int depth){
        mDepth = depth;
    };

    int depth(){
        return mDepth;
    }

    void addItem(TreeViewItem *item){
        addChild(item);
    };

private:
    FileAttr mFileAttr;
    int mDepth = -1;


};

#endif // TREEVIEWITEM_H
