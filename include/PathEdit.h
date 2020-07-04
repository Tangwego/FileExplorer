#ifndef PATHEDIT_H
#define PATHEDIT_H

#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>

class PathEdit: public QLineEdit
{
    Q_OBJECT
public:
    PathEdit();
    void addPathItem(QString name){

    };

    void addSplit(){

    };

private:
    QPushButton *mSelectBtn;
    QHBoxLayout *mLayout;

};

#endif // PATHEDIT_H
