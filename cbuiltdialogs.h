#ifndef CBUILTDIALOGS_H
#define CBUILTDIALOGS_H

#include <QDialog>
#include <QPushButton>
#include <QTextEdit>

class CBuiltDialogs : public QDialog
{
    Q_OBJECT

public:
    CBuiltDialogs(QWidget *parent = nullptr);
    ~CBuiltDialogs();

private:
    QTextEdit* displayTextEdit;
    QPushButton* colorPushBtn;
    QPushButton* errorPushBtn;
    QPushButton* filePushBtn;
    QPushButton* fontPushBtn;
    QPushButton* inputPushBtn;
    QPushButton* pagePushBtn;
    QPushButton* progressPushBtn;
    QPushButton* printPushBtn;
private slots:
    //void doPushBth();

};
#endif // CBUILTDIALOGS_H
