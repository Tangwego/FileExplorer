#ifndef FILEEXPLORER_H
#define FILEEXPLORER_H

#include "BaseWindow.h"

#include "TreeView.h"
#include "TreeViewItem.h"

#include "ListView.h"
#include "ListViewItem.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>

#include <QGroupBox>
#include "PathEdit.h"
#include "QPushButton"

#include "FileUtil.h"

class FileExplorer : public BaseWindow
{
    Q_OBJECT

public:
    explicit FileExplorer(QWidget *parent = nullptr);
    ~FileExplorer();

private:
    void initComponent();
    void initQuickList();
    void initExplorerList();
    void resizeEvent(QResizeEvent *event);
    void initHeaderView();
    void initContentView();
    void initExplorerTree();
    void initQuickTree();
    void toggleShowMode(bool iconMode);


private:
    QGroupBox *mHeader;
    QSplitter *mContainer;
    QSplitter *mTreeView;
    QSplitter *mContent;
    TreeView *mWidget;
    QVBoxLayout *mLayout;
    ListView *mFileList;
    FileUtil *mUtil;
    char const* home = getenv("HOME");

private:
    int treeWidth = 240;

private slots:
    void doubleClickTreeView(QTreeWidgetItem *item, int column);
    void clickTreeView(QTreeWidgetItem *item, int column);
    void doubleClickListView(QListWidgetItem *item);


};

#endif
