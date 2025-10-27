#include "cbuiltdialogs.h"
#include <QGridLayout>
#include <QDebug>
#include <QPalette>
CBuiltDialogs::CBuiltDialogs(QWidget *parent)
    : QDialog(parent)
{
    QGridLayout *gridLayout = new QGridLayout;
    displayTextEdit = new QTextEdit (QStringLiteral("Qt的標準通用對話盒"));
    colorPushBtn    = new QPushButton (QStringLiteral("顏色對話盒"));
    errorPushBtn    = new QPushButton (QStringLiteral("錯誤訊息盒"));
    filePushBtn     = new QPushButton (QStringLiteral("檔案對話盒"));
    fontPushBtn     = new QPushButton (QStringLiteral("字體對話盒"));
    inputPushBtn    = new QPushButton (QStringLiteral ("輸入對話盒"));
    pagePushBtn     = new QPushButton (QStringLiteral("頁面設定對話盒"));
    progressPushBtn = new QPushButton (QStringLiteral("進度對話盒"));
    printPushBtn    = new QPushButton (QStringLiteral("列印對話盒"));

    gridLayout->addWidget (colorPushBtn,0,0,1,1);
    gridLayout->addWidget (errorPushBtn,0,1,1,1);
    gridLayout->addWidget (filePushBtn,0,2,1,1);
    gridLayout->addWidget (fontPushBtn,1,0,1,1);
    gridLayout->addWidget (inputPushBtn,1,1,1,1);
    gridLayout->addWidget (pagePushBtn,1,2,1,1);
    gridLayout->addWidget (progressPushBtn,2,0,1,1);
    gridLayout->addWidget (printPushBtn,2,1,1,1);
    gridLayout->addWidget (displayTextEdit, 3,0,3,3);

        setLayout (gridLayout);
        setWindowTitle (QStringLiteral("內建對話盒展示"));
        resize(400,300);

        connect(colorPushBtn,SIGNAL(clicked()),this,SLOT(doPushBth()));
        connect(errorPushBtn,SIGNAL(clicked()),this,SLOT(doPushBth()));
        connect(filePushBtn,SIGNAL(clicked()),this,SLOT(doPushBth()));
        connect(fontPushBtn,SIGNAL(clicked()),this,SLOT(doPushBth()));
        connect(inputPushBtn,SIGNAL(clicked()),this,SLOT(doPushBth()));
        connect(progressPushBtn,SIGNAL(clicked()),this,SLOT(doPushBth()));
        connect(pagePushBtn,SIGNAL(clicked()),this,SLOT(doPushBth()));
        connect(printPushBtn,SIGNAL(clicked()),this,SLOT(doPushBth()));
}
void CBuiltDialogs::doPushBth()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if(btn == colorPushBtn)
    {
        QPalette palette=displayTextEdit->palette();
        const QColor& color=
            QColorDialog::getColor(palette.color(QPalette::Text),
                                this,tr("設定背景文字"));
        if(color.isValid())
        {
            palette.setColor(QPalette::Text, color);
            displayTextEdit->setPalette(palette);
        }
    }
    if(btn == errorPushBtn)
    {
        QErrorMessage box(this);
        box.setWindowTitle  (QStringLiteral("錯誤訊息盒"));
        box.showMessage     (QStringLiteral("錯誤訊息盒實例xx:"));
        box.showMessage     (QStringLiteral("錯誤訊息盒實例yy:"));
        box.showMessage     (QStringLiteral("錯誤訊息盒實例zz:"));
        box.exec();
    }
    if(btn ==filePushBtn)
    {
        QString fileName = QFileDialog::getOpenFileName(this,
                         QStringLiteral("開啟檔案"),
                        tr("."),
                        QStringLiteral("任何檔案(*0*)"
                                    ";;文字檔(*.txt)"
                                    ";;XML檔(*.xml)"));
        displayTextEdit->setText(fileName);
    }
    if (btn == progressPushBtn)
    {
        QProgressDialog progress(QStringLiteral("正在讀取檔案"),
                                 QStringLiteral("取消"),0,10000,this);
        progress.setWindowTitle(QStringLiteral("進度對話方塊"));
        progress.show();
        for (int i=0; i<10000; i++)
        {
            progress.setValue(i);
            qApp->processEvents();
            if(progress.wasCanceled())
                break;
            qDebug() << i;
        }
        progress.setValue(10000);
    }
}

CBuiltDialogs::~CBuiltDialogs()
{

}
