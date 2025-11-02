#include "cbuiltdialogs.h"
#include<QtPrintSupport/qprinter.h>
#include<QtPrintSupport/qpagesetupdialog.h>
#include<QtPrintSupport/QPrintDialog>
#include<QtPrintSupport/QPrintPreviewDialog>
#include <QGridLayout>
#include <QDebug>
#include <QPalette>

// CBuiltDialogs 建構子：負責建構整個視窗與所有內建對話盒按鈕
CBuiltDialogs::CBuiltDialogs(QWidget *parent)
    : QDialog(parent)
{
    // 建立網格排版管理員
    QGridLayout *gridLayout = new QGridLayout;
    // 主要顯示文字區
    displayTextEdit = new QTextEdit (QStringLiteral("Qt的標準通用對話盒"));
    // 各種功能按鈕（共九個）
    colorPushBtn    = new QPushButton (QStringLiteral("顏色對話盒"));
    textColorPushBtn= new QPushButton (QStringLiteral("文字顏色對話盒"));
    errorPushBtn    = new QPushButton (QStringLiteral("錯誤訊息盒"));
    filePushBtn     = new QPushButton (QStringLiteral("檔案對話盒"));
    fontPushBtn     = new QPushButton (QStringLiteral("字體對話盒"));
    inputPushBtn    = new QPushButton (QStringLiteral ("輸入對話盒"));
    pagePushBtn     = new QPushButton (QStringLiteral("頁面設定對話盒"));
    progressPushBtn = new QPushButton (QStringLiteral("進度對話盒"));
    printPushBtn    = new QPushButton (QStringLiteral("列印對話盒"));
    // 共用的錯誤訊息盒物件（避免阻塞主執行緒）
    errorBox        = new QErrorMessage(this);
    errorBox->setWindowTitle(QStringLiteral("錯誤訊息盒"));

    // 配置各按鈕到3x3的格子 (最後一格第三列第三欄為文字顏色對話盒)
    gridLayout->addWidget (colorPushBtn,0,0,1,1); 
    gridLayout->addWidget (errorPushBtn,0,1,1,1); 
    gridLayout->addWidget (filePushBtn,0,2,1,1); 
    gridLayout->addWidget (fontPushBtn,1,0,1,1); 
    gridLayout->addWidget (inputPushBtn,1,1,1,1); 
    gridLayout->addWidget (pagePushBtn,1,2,1,1); 
    gridLayout->addWidget (progressPushBtn,2,0,1,1);
    gridLayout->addWidget (printPushBtn,2,1,1,1); 
    gridLayout->addWidget (textColorPushBtn,2,2,1,1); 
    // 顯示區跨整排
    gridLayout->addWidget (displayTextEdit, 3,0,3,3);

    setLayout (gridLayout);                         // 設定主佈局
    setWindowTitle (QStringLiteral("內建對話盒展示")); // 視窗標題
    resize(400,300);                                // 預設大小

    // 將每個按鈕 clicked 訊號連結到單一處理槽（後續用 sender 解析來源）
    connect(colorPushBtn,SIGNAL(clicked()),this,SLOT(doPushBth()));
    connect(textColorPushBtn,SIGNAL(clicked()),this,SLOT(doPushBth()));
    connect(errorPushBtn,SIGNAL(clicked()),this,SLOT(doPushBth()));
    connect(filePushBtn,SIGNAL(clicked()),this,SLOT(doPushBth()));
    connect(fontPushBtn,SIGNAL(clicked()),this,SLOT(doPushBth()));
    connect(inputPushBtn,SIGNAL(clicked()),this,SLOT(doPushBth()));
    connect(progressPushBtn,SIGNAL(clicked()),this,SLOT(doPushBth()));
    connect(pagePushBtn,SIGNAL(clicked()),this,SLOT(doPushBth()));
    connect(printPushBtn,SIGNAL(clicked()),this,SLOT(doPushBth()));
}

// 主事件槽，根據是哪一顆按鈕被點擊進行不同對話盒示範
void CBuiltDialogs::doPushBth()
{
    // 取得觸發事件來源
    QPushButton *btn = qobject_cast<QPushButton*>(sender());

    // 1. 顏色對話盒：更改顯示文字區的背景色
    if(btn == colorPushBtn)
    {
        QPalette palette=displayTextEdit->palette();
        const QColor& color=
            QColorDialog::getColor(palette.color(QPalette::Base),
                                   this,tr("設定背景文字"));
        if(color.isValid())
        {
            palette.setColor(QPalette::Base, color);
            displayTextEdit->setPalette(palette);
        }
    }
    // 2. 文字顏色對話盒：更改顯示文字區的文字顏色
    if(btn == textColorPushBtn)
    {
        QPalette palette=displayTextEdit->palette();
        const QColor& color=
            QColorDialog::getColor(palette.color(QPalette::Text),
                                   this,tr("改文字顏色"));
        if(color.isValid())
        {
            palette.setColor(QPalette::Text, color);
            displayTextEdit->setPalette(palette);
        }
    }
    // 3. 錯誤訊息盒（非阻塞，可多次 showMessage 疊加測試）
    if(btn == errorPushBtn)
    {
        errorBox->showMessage(QStringLiteral("錯誤訊息盒實例xx:"));
        errorBox->showMessage(QStringLiteral("錯誤訊息盒實例yy:"));
        errorBox->showMessage(QStringLiteral("錯誤訊息盒實例zz:"));
        errorBox->show();
    }
    // 4. 檔案對話盒：選擇本機檔案並顯示檔名到文字區
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
    // 5. 字體對話盒：讓使用者選擇文字區的字型樣式
    if (btn == fontPushBtn)
    {
        bool ok;
        const QFont& font = QFontDialog::getFont(&ok,
                                                 displayTextEdit->font(),
                                                 this,
                                                 QStringLiteral("字體對話盒"));
        if (ok) displayTextEdit->setFont (font);
    }
    // 6. 進度對話盒：展示一個假讀取循環（主執行緒計數，每步處理 Qt 事件）
    if (btn == progressPushBtn)
    {
        QProgressDialog progress(QStringLiteral("正在讀取檔案"),
                                 QStringLiteral("取消"),0,10000,this);
        progress.setWindowTitle(QStringLiteral("進度對話方塊"));
        progress.show();
        for (int i=0; i<10000; i++)
        {
            progress.setValue(i);
            qApp->processEvents(); // 允許 UI 響應
            if(progress.wasCanceled()) // 使用者取消則跳出迴圈
                break;
            qDebug() << i;
        }
        progress.setValue(10000);
    }
    // 7. 輸入對話盒：呼叫可輸入文字的小視窗，並將結果顯示到文字區
    if (btn == inputPushBtn)
    {
        bool ok;
        QString text = QInputDialog::getText(this,
                                            QStringLiteral("輸入對話盒"),
                                            QStringLiteral("輸入文字"),
                                            QLineEdit:: Normal,
                                            QDir::home().dirName(),
                                            &ok
                                            );
    if (ok && !text.isEmpty()) displayTextEdit->setText(text);
    }
    // 8. 頁面設定對話盒：呼叫列印驅動相關紙張與邊界設定窗
    if (btn == pagePushBtn)
    {
        QPrinter printer (QPrinter:: HighResolution);
        QPageSetupDialog *dlg = new QPageSetupDialog(&printer, this);
        dlg->setWindowTitle (QStringLiteral ("頁面設定對話方塊"));
        if (dlg->exec()== QDialog:: Accepted)
        {
            // 使用者按下確定，可進一步取得設定內容
        }
    }
    // 9. 列印對話盒（顯示系統內建列印選擇與預覽窗）
    if (btn ==printPushBtn)
    {
        QPrinter printer (QPrinter:: HighResolution);
        QPrintDialog dialog(&printer, this);
        if (dialog.exec() != QDialog:: Accepted)
            return;
    }
}

// 析構函數（目前不需做額外釋放）
CBuiltDialogs::~CBuiltDialogs()
{

}
