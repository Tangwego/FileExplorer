#include "FileExplorer.h"
#include "Log.h"

#include <QDesktopServices>
#include <QUrl>

void initFileTree(QString fileName, FileUtil *mUtil, TreeViewItem *item);
void initFileList(QString fileName, FileUtil *mUtil, ListView *list);

FileExplorer::FileExplorer(QWidget *parent) : BaseWindow(parent)
{
    setWindowTitle(FE_TITLE);
    initComponent();
}

FileExplorer::~FileExplorer()
{

}

void FileExplorer::resizeEvent(QResizeEvent *event)
{
    mTreeView->setFixedSize(treeWidth, height() * 85 / 100);
    mWidget->resize(treeWidth, mTreeView->height());
    mContent->resize(width() - treeWidth, height());
    mHeader->resize(width(), height() * 15 / 100);
    mContainer->resize(width(), height() * 85 / 100);
    mContainer->move(0,height() * 15 / 100);
    mLayout->update();
}

void FileExplorer::initComponent()
{
    mHeader = new QGroupBox(this);
    mContainer = new QSplitter(this);
    mTreeView = new QSplitter(mContainer);
    mContent = new QSplitter(mContainer);
    mWidget = new TreeView(mTreeView);
    mLayout = new QVBoxLayout(this);
    mUtil = new FileUtil();

    mWidget->setHeaderHidden(true);
    mWidget->setColumnCount(1);
//    mWidget->setRootIsDecorated(false);
//    mWidget->setItemsExpandable(true);
    mTreeView->setFixedSize(treeWidth, height() * 85 / 100);
    mWidget->resize(treeWidth, mTreeView->height());
    mContent->resize(width() - treeWidth, height());
    mHeader->resize(width(), height() * 15 / 100);
    mContainer->resize(width(), height() * 85 / 100);

    mContainer->move(0,height() * 15 / 100);

    initHeaderView();
    initContentView();

    initQuickTree();
    initExplorerTree();

    mWidget->expandToDepth(0);
    connect(mWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(doubleClickTreeView(QTreeWidgetItem *, int)));
    connect(mWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(clickTreeView(QTreeWidgetItem *, int)));

    QHBoxLayout *contentLayout = new QHBoxLayout(mContainer);

    mTreeView->addWidget(mWidget);
    contentLayout->addWidget(mTreeView);
    contentLayout->addWidget(mContent);
    mContainer->setLayout(contentLayout);
    mLayout->addWidget(mHeader);
    mLayout->addWidget(mContainer);
    setLayout(mLayout);
}

void FileExplorer::initHeaderView()
{
    QHBoxLayout *headerLayout = new QHBoxLayout(mHeader);

    QPushButton *leftBtn = new QPushButton();
    leftBtn->resize(20, height());
    leftBtn->setCursor(Qt::PointingHandCursor);
    leftBtn->setFixedSize(22,22);
    leftBtn->setStyleSheet("QPushButton{border:1px solid #000;border-image:url(:/images/icon/left_dark.png); background:transparent;} \
                              QPushButton:hover{border-image:url(:/images/icon/left_active.png)} \
                              QPushButton:pressed{border-image:url(:/images/icon/left_active.png)}");
    leftBtn->setFlat(true);

    QPushButton *rightBtn = new QPushButton();
    rightBtn->resize(20, height());
    rightBtn->setCursor(Qt::PointingHandCursor);
    rightBtn->setFixedSize(22,22);
    rightBtn->setStyleSheet("QPushButton{border:1px solid #000;border-image:url(:/images/icon/right_dark.png); background:transparent;} \
                              QPushButton:hover{border-image:url(:/images/icon/right_active.png)} \
                              QPushButton:pressed{border-image:url(:/images/icon/right_active.png)}");
    rightBtn->setFlat(true);

    QPushButton *topBtn = new QPushButton();
    topBtn->resize(20, height());
    topBtn->setCursor(Qt::PointingHandCursor);
    topBtn->setFixedSize(22,22);
    topBtn->setStyleSheet("QPushButton{border:1px solid #000;border-image:url(:/images/icon/top_dark.png); background:transparent;} \
                              QPushButton:hover{border-image:url(:/images/icon/top_active.png)} \
                              QPushButton:pressed{border-image:url(:/images/icon/top_active.png)}");
    topBtn->setFlat(true);

    QPushButton *refreshBtn = new QPushButton();
    refreshBtn->resize(20, height());
    refreshBtn->setCursor(Qt::PointingHandCursor);
    refreshBtn->setFixedSize(22,22);
    refreshBtn->setStyleSheet("QPushButton{border:1px solid #000;border-image:url(:/images/icon/refresh_dark.png); background:transparent;} \
                              QPushButton:hover{border-image:url(:/images/icon/refresh_active.png)} \
                              QPushButton:pressed{border-image:url(:/images/icon/refresh_active.png)}");
    refreshBtn->setFlat(true);

    headerLayout->addWidget(leftBtn);
    headerLayout->addWidget(rightBtn);
    headerLayout->addWidget(topBtn);
    headerLayout->addWidget(new PathEdit());
    headerLayout->addWidget(refreshBtn);

    mHeader->setLayout(headerLayout);
}

void FileExplorer::initContentView()
{
    QGroupBox *qGroupBox = new QGroupBox(mContent);
    QVBoxLayout *fileLayout = new QVBoxLayout(qGroupBox);
    mFileList = new ListView();

//    mFileList->setViewMode(QListView::IconMode);
//    mFileList->setIconSize(QSize(80, 80));
//    mFileList->setGridSize(QSize(100, 100));
//    mFileList->setResizeMode(QListView::Adjust);

    connect(mFileList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(doubleClickListView(QListWidgetItem *)));

    initQuickList();
    fileLayout->addWidget(mFileList);

    qGroupBox->setLayout(fileLayout);
    mContent->addWidget(qGroupBox);
}

void FileExplorer::doubleClickListView(QListWidgetItem *item)
{
    ListViewItem *temp = (ListViewItem *)item;
    FileAttr attr = temp->getAttr();
    if(attr.fileType == NORMAL_FILE){
        QDesktopServices::openUrl(QUrl("file:///" + attr.filePath, QUrl::TolerantMode));
    }else{
        mFileList->removeAll();
        toggleShowMode(false);
        initFileList(attr.filePath,mUtil, mFileList);
    }
}

void FileExplorer::clickTreeView(QTreeWidgetItem *item, int column)
{
    TreeViewItem *temp = (TreeViewItem *)item;
    FileAttr attr = temp->getAttr();
    if(attr.filePath.isNull() || attr.filePath.isEmpty()){
        if(attr.fileName == "quick"){
            mFileList->removeAll();
            initQuickList();
        }else if(attr.fileName == "computer"){
            mFileList->removeAll();
            initExplorerList();
        }else{
            Log::info("Error!");
        }
    }else{
        mFileList->removeAll();
        toggleShowMode(false);
        initFileList(attr.filePath,mUtil, mFileList);
    }
}

void FileExplorer::toggleShowMode(bool iconMode){
    if(!iconMode){
        mFileList->setViewMode(QListView::ListMode);
        mFileList->setIconSize(QSize(20, 20));
        mFileList->setGridSize(QSize(20, 20));
    }else{
        mFileList->setViewMode(QListView::IconMode);
        mFileList->setIconSize(QSize(80, 80));
        mFileList->setGridSize(QSize(100, 100));
        mFileList->setResizeMode(QListView::Adjust);
    }
}

void FileExplorer::initQuickList()
{
    FileAttr temp;

    toggleShowMode(true);

    temp.filePath = QString("%1/Desktop/").arg(home);
    ListViewItem *desktop = new ListViewItem();
    desktop->setText("桌面");
    desktop->setAttr(temp);
    QIcon desktopIcon;
    desktopIcon.addPixmap(QPixmap(":/images/icon/desktop.png"), QIcon::Normal);
    desktop->setIcon(desktopIcon);
    mFileList->addItem(desktop);

    temp.filePath = QString("%1/Downloads/").arg(home);
    ListViewItem *download = new ListViewItem();
    download->setText("下载");
    download->setAttr(temp);
    QIcon downloadIcon;
    downloadIcon.addPixmap(QPixmap(":/images/icon/download.png"), QIcon::Normal);
    download->setIcon(downloadIcon);
    mFileList->addItem(download);

    temp.filePath = QString("%1/Documents/").arg(home);
    ListViewItem *document = new ListViewItem();
    document->setText("文档");
    document->setAttr(temp);
    QIcon documentIcon;
    documentIcon.addPixmap(QPixmap(":/images/icon/document.png"), QIcon::Normal);
    document->setIcon(documentIcon);
    mFileList->addItem(document);

    temp.filePath = QString("%1/Pictures/").arg(home);
    ListViewItem *picture = new ListViewItem();
    picture->setText("图片");
    picture->setAttr(temp);
    QIcon pictureIcon;
    pictureIcon.addPixmap(QPixmap(":/images/icon/picture.png"), QIcon::Normal);
    picture->setIcon(pictureIcon);
    mFileList->addItem(picture);
}

void FileExplorer::initExplorerList()
{
    FileAttr temp;

    mFileList->setViewMode(QListView::IconMode);
    mFileList->setIconSize(QSize(160, 160));
    mFileList->setGridSize(QSize(100, 100));
    mFileList->setResizeMode(QListView::Adjust);

    temp.filePath = QString("%1/Videos/").arg(home);
    ListViewItem *video = new ListViewItem();
    video->setText("视频");
    video->setAttr(temp);
    QIcon videoIcon;
    videoIcon.addPixmap(QPixmap(":/images/icon/video.png"), QIcon::Normal);
    video->setIcon(videoIcon);
    mFileList->addItem(video);

    temp.filePath = QString("%1/Downloads/").arg(home);
    ListViewItem *download = new ListViewItem();
    download->setText("下载");
    download->setAttr(temp);
    QIcon downloadIcon;
    downloadIcon.addPixmap(QPixmap(":/images/icon/download.png"), QIcon::Normal);
    download->setIcon(downloadIcon);
    mFileList->addItem(download);

    temp.filePath = QString("%1/Documents/").arg(home);
    ListViewItem *document = new ListViewItem();
    document->setText("文档");
    document->setAttr(temp);
    QIcon documentIcon;
    documentIcon.addPixmap(QPixmap(":/images/icon/document.png"), QIcon::Normal);
    document->setIcon(documentIcon);
    mFileList->addItem(document);

    temp.filePath = QString("%1/Pictures/").arg(home);
    ListViewItem *picture = new ListViewItem();
    picture->setText("图片");
    picture->setAttr(temp);
    QIcon pictureIcon;
    pictureIcon.addPixmap(QPixmap(":/images/icon/picture.png"), QIcon::Normal);
    picture->setIcon(pictureIcon);
    mFileList->addItem(picture);

    temp.filePath = QString("%1/Music/").arg(home);
    ListViewItem *music = new ListViewItem();
    music->setText("音乐");
    music->setAttr(temp);
    QIcon musicIcon;
    musicIcon.addPixmap(QPixmap(":/images/icon/music.png"), QIcon::Normal);
    music->setIcon(musicIcon);
    mFileList->addItem(music);

    temp.filePath = QString("%1/Desktop/").arg(home);
    ListViewItem *desktop = new ListViewItem();
    desktop->setText("桌面");
    desktop->setAttr(temp);
    QIcon desktopIcon;
    desktopIcon.addPixmap(QPixmap(":/images/icon/desktop.png"), QIcon::Normal);
    desktop->setIcon(desktopIcon);
    mFileList->addItem(desktop);

    QList<FileAttr> fileList = mUtil->getMountedDisks();

    int count = fileList.size();
    for(int i = 0; i != count; i++){
        ListViewItem *item = new ListViewItem();
        QIcon itemIcon;
        FileAttr fileAttr = fileList.at(i);
        if(strcmp(fileAttr.fileName.toLocal8Bit().data(),"/") == 0){
            itemIcon.addPixmap(QPixmap(":/images/icon/system_os_disk.png"), QIcon::Normal);
        }else{
            itemIcon.addPixmap(QPixmap(":/images/icon/normal_disk.png"), QIcon::Normal);
        }
        item->setIcon(itemIcon);
        item->setText(fileAttr.fileName);
        item->setAttr(fileAttr);
        mFileList->addItem(item);
    }
}

void FileExplorer::doubleClickTreeView(QTreeWidgetItem *item, int column)
{
    TreeViewItem *temp = (TreeViewItem *)item;
    FileAttr attr = temp->getAttr();
    if(attr.fileType == NORMAL_FILE){
        QDesktopServices::openUrl(QUrl("file:///" + attr.filePath, QUrl::TolerantMode));
    }else{
        if(temp->depth() != -1){
            temp->clear();
            initFileTree(attr.filePath,mUtil, temp);
        }else{
            // TODO
        }
    }
}

void FileExplorer::initQuickTree()
{
    FileAttr temp;
    TreeViewItem *quickVisit = new TreeViewItem();
    quickVisit->setText(0, "快速访问");
    QIcon visIcon;
    visIcon.addPixmap(QPixmap(":/images/icon/quick_visit.png"), QIcon::Normal);
    quickVisit->setIcon(0, visIcon);
    temp.fileName = "quick";
    quickVisit->setAttr(temp);


    temp.filePath = QString("%1/Desktop/").arg(home);
    TreeViewItem *desktop = new TreeViewItem();
    desktop->setText(0,"桌面");
    desktop->setAttr(temp);
    QIcon desktopIcon;
    desktopIcon.addPixmap(QPixmap(":/images/icon/desktop.png"), QIcon::Normal);
    desktop->setIcon(0, desktopIcon);
    quickVisit->addItem(desktop);

    temp.filePath = QString("%1/Downloads/").arg(home);
    TreeViewItem *download = new TreeViewItem();
    download->setText(0,"下载");
    download->setAttr(temp);
    QIcon downloadIcon;
    downloadIcon.addPixmap(QPixmap(":/images/icon/download.png"), QIcon::Normal);
    download->setIcon(0, downloadIcon);
    quickVisit->addItem(download);

    temp.filePath = QString("%1/Documents/").arg(home);
    TreeViewItem *document = new TreeViewItem();
    document->setText(0,"文档");
    document->setAttr(temp);
    QIcon documentIcon;
    documentIcon.addPixmap(QPixmap(":/images/icon/document.png"), QIcon::Normal);
    document->setIcon(0, documentIcon);
    quickVisit->addItem(document);

    temp.filePath = QString("%1/Pictures/").arg(home);
    TreeViewItem *picture = new TreeViewItem();
    picture->setText(0,"图片");
    picture->setAttr(temp);
    QIcon pictureIcon;
    pictureIcon.addPixmap(QPixmap(":/images/icon/picture.png"), QIcon::Normal);
    picture->setIcon(0, pictureIcon);
    quickVisit->addItem(picture);

    TreeViewItem *split = new TreeViewItem();
    split->setDisabled(true);
    split->setExpanded(false);
    quickVisit->addItem(split);

    mWidget->addTopLevelItem(quickVisit);
}

void FileExplorer::initExplorerTree()
{
    FileAttr temp;
    TreeViewItem *thisComputer = new TreeViewItem();
    thisComputer->setText(0, "此电脑");
    QIcon computerIcon;
    computerIcon.addPixmap(QPixmap(":/images/icon/this_computer.png"), QIcon::Normal);
    thisComputer->setIcon(0, computerIcon);
    temp.fileName = "computer";
    thisComputer->setAttr(temp);

    temp.filePath = QString("%1/Videos/").arg(home);
    TreeViewItem *video = new TreeViewItem();
    video->setText(0,"视频");
    video->setAttr(temp);
    QIcon videoIcon;
    videoIcon.addPixmap(QPixmap(":/images/icon/video.png"), QIcon::Normal);
    video->setIcon(0, videoIcon);
    initFileTree(QString("%1/Videos/").arg(home), mUtil, video);
    thisComputer->addItem(video);

    temp.filePath = QString("%1/Downloads/").arg(home);
    TreeViewItem *download = new TreeViewItem();
    download->setText(0,"下载");
    download->setAttr(temp);
    QIcon downloadIcon;
    downloadIcon.addPixmap(QPixmap(":/images/icon/download.png"), QIcon::Normal);
    download->setIcon(0, downloadIcon);
    initFileTree(QString("%1/Downloads/").arg(home), mUtil, download);
    thisComputer->addItem(download);

    temp.filePath = QString("%1/Documents/").arg(home);
    TreeViewItem *document = new TreeViewItem();
    document->setText(0,"文档");
    document->setAttr(temp);
    QIcon documentIcon;
    documentIcon.addPixmap(QPixmap(":/images/icon/document.png"), QIcon::Normal);
    document->setIcon(0, documentIcon);
    initFileTree(QString("%1/Documents/").arg(home), mUtil, document);
    thisComputer->addItem(document);

    temp.filePath = QString("%1/Pictures/").arg(home);
    TreeViewItem *picture = new TreeViewItem();
    picture->setText(0,"图片");
    picture->setAttr(temp);
    QIcon pictureIcon;
    pictureIcon.addPixmap(QPixmap(":/images/icon/picture.png"), QIcon::Normal);
    picture->setIcon(0, pictureIcon);
    initFileTree(QString("%1/Pictures/").arg(home), mUtil, picture);
    thisComputer->addItem(picture);

    temp.filePath = QString("%1/Music/").arg(home);
    TreeViewItem *music = new TreeViewItem();
    music->setText(0,"音乐");
    music->setAttr(temp);
    QIcon musicIcon;
    musicIcon.addPixmap(QPixmap(":/images/icon/music.png"), QIcon::Normal);
    music->setIcon(0, musicIcon);
    initFileTree(QString("%1/Music/").arg(home), mUtil, music);
    thisComputer->addItem(music);

    temp.filePath = QString("%1/Desktop/").arg(home);
    TreeViewItem *desktop = new TreeViewItem();
    desktop->setText(0,"桌面");
    desktop->setAttr(temp);
    QIcon desktopIcon;
    desktopIcon.addPixmap(QPixmap(":/images/icon/desktop.png"), QIcon::Normal);
    desktop->setIcon(0, desktopIcon);
    initFileTree(QString("%1/Desktop/").arg(home), mUtil, desktop);
    thisComputer->addItem(desktop);

    QList<FileAttr> fileList = mUtil->getMountedDisks();

    int count = fileList.size();
    for(int i = 0; i != count; i++){
        TreeViewItem *item = new TreeViewItem();
        QIcon itemIcon;
        FileAttr fileAttr = fileList.at(i);
        if(strcmp(fileAttr.fileName.toLocal8Bit().data(),"/") == 0){
            itemIcon.addPixmap(QPixmap(":/images/icon/system_os_disk.png"), QIcon::Normal);
        }else{
            itemIcon.addPixmap(QPixmap(":/images/icon/normal_disk.png"), QIcon::Normal);
        }
        item->setIcon(0, itemIcon);
        item->setText(0, fileAttr.fileName);
        item->setAttr(fileAttr);
        initFileTree(fileAttr.fileName, mUtil, item);
        thisComputer->addItem(item);
    }
    mWidget->addTopLevelItem(thisComputer);
}

void initFileTree(QString fileName, FileUtil *mUtil, TreeViewItem *item)
{
    QList<FileAttr> fileList = mUtil->getFileList(fileName);
    int count = fileList.size();
    if(count <= 0) return;
    for(int i = 0; i != count; i++){
        TreeViewItem *temp = new TreeViewItem();
        QIcon tempIcon;
        FileAttr fileAttr = fileList.at(i);
        temp->setText(0, fileAttr.fileName);
        temp->setAttr(fileAttr);
        temp->setDepth(1);

        if(fileAttr.fileType == DIRECTORY){
            tempIcon.addPixmap(QPixmap(":/images/icon/directory.png"), QIcon::Normal);
            temp->setIcon(0, tempIcon);
            item->addItem(temp);
        }
//        else{
//            tempIcon.addPixmap(QPixmap(":/images/icon/file.png"), QIcon::Normal);
//        }

//        initFileTree(fileAttr.filePath, mUtil, temp);

    }
}

void initFileList(QString fileName, FileUtil *mUtil, ListView *list)
{
    QList<FileAttr> fileList = mUtil->getFileList(fileName);
    int count = fileList.size();
    if(count <= 0) return;
    for(int i = 0; i != count; i++){
        ListViewItem *temp = new ListViewItem();
        QIcon tempIcon;
        FileAttr fileAttr = fileList.at(i);
        temp->setText(fileAttr.fileName);
        if(fileAttr.fileType == DIRECTORY){
            tempIcon.addPixmap(QPixmap(":/images/icon/directory.png"), QIcon::Normal);
        }else{
            tempIcon.addPixmap(QPixmap(":/images/icon/file.png"), QIcon::Normal);
        }
        temp->setIcon(tempIcon);
        temp->setAttr(fileAttr);
        list->addItem(temp);
    }
}
