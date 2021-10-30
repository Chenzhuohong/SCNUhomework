#ifndef SHOWPROGRAMWIDGET_H
#define SHOWPROGRAMWIDGET_H

#include <QWidget>
#include<QVBoxLayout>
#include <QTextBrowser>
#include <QPushButton>

class showProgramWidget : public QWidget
{
    Q_OBJECT
public:
    showProgramWidget(QWidget *parent = nullptr);
    void showProgram(QString s);

private:
    QVBoxLayout* VLayout;
    QTextBrowser* txb_show;
    QPushButton* btn_saveProgram;

private slots:
    void onClick_btn_saveProgram();
};

#endif // SHOWPROGRAMWIDGET_H
