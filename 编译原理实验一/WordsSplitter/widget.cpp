#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    vLayout = new QVBoxLayout();
    hLayout = new QHBoxLayout();

    label1 = new QLabel("请选择文件：");

    lineEdit_filePath = new QLineEdit();
    lineEdit_filePath->setFixedWidth(400);

    toolButton_browse = new QToolButton();
    toolButton_browse->setIcon(QIcon(":/res/browse.png"));
    toolButton_browse->setFixedSize(30,30);

    pushButton_start = new QPushButton();
    pushButton_start->setText("开始解析");
    pushButton_start->setFixedWidth(100);

    pushButton_replace = new QPushButton();
    pushButton_replace->setText("替换成XC++");
    pushButton_replace->setFixedWidth(100);

    hLayout->addStretch();
    hLayout->addWidget(label1);
    hLayout->addWidget(lineEdit_filePath);
    hLayout->addWidget(toolButton_browse);
    hLayout->addSpacing(30);
    hLayout->addWidget(pushButton_start);
    hLayout->addSpacing(20);
    hLayout->addWidget(pushButton_replace);
    hLayout->addStretch();
    vLayout->addItem(hLayout);

    label2 = new QLabel("扫描结果/替换结果：");
    vLayout->addWidget(label2);

    textBrowser_show = new QTextBrowser();
    textBrowser_show->setMinimumSize(600,600);
    vLayout->addWidget(textBrowser_show);

    this->setLayout(vLayout);

    connect(toolButton_browse, SIGNAL(clicked(bool)), this, SLOT(chooseFile()));
    connect(pushButton_start, SIGNAL(clicked(bool)), this, SLOT(analyze()));
    connect(pushButton_replace, SIGNAL(clicked(bool)), this, SLOT(replace()));
}

Widget::~Widget()
{
}

void Widget::chooseFile() {
    lineEdit_filePath->setText(QFileDialog::getOpenFileName(this, tr("选择文件"), "", tr("C++文件(*.cpp)")));
}

void Widget::analyze() {
    if(!lineEdit_filePath->text().contains(".cpp")) {
        QMessageBox::warning(this, "warning", "请选择cpp文件！");
        return;
    }
    QFile file(lineEdit_filePath->text());
    if(file.exists()) {
        Handle handle(lineEdit_filePath->text());
        textBrowser_show->setText(handle.getAnalyzeResult());
    }
    else {
        QMessageBox::warning(this, "warning", "文件不存在！");
    }
}

void Widget::replace() {
    if(!lineEdit_filePath->text().contains(".cpp")) {
        QMessageBox::warning(this, "warning", "请选择cpp文件！");
        return;
    }
    QFile file(lineEdit_filePath->text());
    if(file.exists()) {
        Handle handle(lineEdit_filePath->text());
        textBrowser_show->setText(handle.getReplaceResult());
    }
    else {
        QMessageBox::warning(this, "warning", "文件不存在！");
    }
}
