#include "PathEdit.h"

PathEdit::PathEdit()
{
    mSelectBtn = new QPushButton(this);
    mLayout = new QHBoxLayout(this);

    mSelectBtn->resize(20, height());
    mSelectBtn->setCursor(Qt::PointingHandCursor);
    mSelectBtn->setFixedSize(22,22);
    mSelectBtn->setToolTip("刷新");
    mSelectBtn->setStyleSheet("QPushButton{border:1px solid #000;border-image:url(:/images/icon/bottom_dark.png); background:transparent;} \
                              QPushButton:hover{border-image:url(:/images/icon/bottom_active.png)} \
                              QPushButton:pressed{border-image:url(:/images/icon/bottom_active.png)}");
    QMargins margins = textMargins();
    setTextMargins(margins.left(), margins.top(), mSelectBtn->width() + 5, margins.bottom());
    mLayout->addStretch();
    mLayout->addWidget(mSelectBtn, Qt::AlignRight);
    mLayout->setSpacing(0);
    mLayout->setContentsMargins(0, 0, 5, 0);
    setLayout(mLayout);

}
