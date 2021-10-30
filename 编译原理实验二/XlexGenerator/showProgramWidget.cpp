#include "showProgramWidget.h"
#include <QFileDialog>
#include <QMessageBox>

showProgramWidget::showProgramWidget(QWidget *parent) : QWidget(parent)
{
    btn_saveProgram = new QPushButton("另存为cpp文件");
    btn_saveProgram->setFixedSize(200,40);

    txb_show = new QTextBrowser();
    txb_show->setMinimumSize(1000,800);

    VLayout = new QVBoxLayout();
    VLayout->addWidget(btn_saveProgram);
    VLayout->addWidget(txb_show);
    this->setLayout(VLayout);

    connect(btn_saveProgram, SIGNAL(clicked(bool)), this, SLOT(onClick_btn_saveProgram()));
}

//显示词法分析程序
void showProgramWidget::showProgram(QString s) {
    txb_show->setText(s);
}

//保存词法分析程序
void showProgramWidget::onClick_btn_saveProgram() {
    QString fileName=QFileDialog::getSaveFileName(this, tr("保存为文件"), ".", tr("cpp文件(*.cpp)"));
    if(fileName.size() == 0) return;
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QString content = txb_show->toPlainText();
    file.write(content.toUtf8());
    file.close();
}
