#ifndef WIDGET_H
#define WIDGET_H

#include "Handle.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <QPushButton>
#include <QTextBrowser>
#include <QFileDialog>
#include <QMessageBox>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    QVBoxLayout* vLayout;
    QHBoxLayout* hLayout;
    QLabel* label1;
    QLineEdit* lineEdit_filePath;
    QToolButton* toolButton_browse;
    QPushButton* pushButton_start;
    QPushButton* pushButton_replace;
    QLabel* label2;
    QTextBrowser* textBrowser_show;

private slots:
    void chooseFile();
    void analyze();
    void replace();
};
#endif // WIDGET_H
