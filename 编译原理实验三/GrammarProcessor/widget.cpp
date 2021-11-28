#include "widget.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QQueue>
#include <QStandardItemModel>
#include <QHeaderView>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    wholeLayout = new QGridLayout();
    btn_openFromFile = new QPushButton("从文件中打开");
    btn_saveToFile = new QPushButton("保存文法");
    btn_simplifyGrammar = new QPushButton("简化文法");
    btn_factoring = new QPushButton("提取左公因子");
    btn_removeRecursive = new QPushButton("消除左递归");
    btn_first = new QPushButton("求first集合");
    btn_follow = new QPushButton("求follow集合");
    btn_leftmostDerive = new QPushButton("最左推导");
    txe_inputGrammar = new QTextEdit();
    le_inputString = new QLineEdit();
    le_inputString->setFixedWidth(250);
    txb_simplifyGrammar = new QTextBrowser();
    txb_factoring = new QTextBrowser();
    txb_removeRecursive = new QTextBrowser();
    tbv_first = new QTableView();
    tbv_follow = new QTableView();
    tbv_leftmostDerive = new QTableView();

    wholeLayout->setSpacing(10);
    QVBoxLayout* Vlayout;
    QHBoxLayout* Hlayout;

    Vlayout = new QVBoxLayout();
    Vlayout->addWidget(new QLabel("请输入文法规则："));
    Hlayout = new QHBoxLayout();
    Hlayout->setSpacing(5);
    Hlayout->addWidget(btn_openFromFile);
    Hlayout->addWidget(btn_saveToFile);
    Vlayout->addItem(Hlayout);
    txe_inputGrammar->setFixedSize(250,400);
    Vlayout->addWidget(txe_inputGrammar);
    wholeLayout->addItem(Vlayout, 0, 0);

    Vlayout = new QVBoxLayout();
    Vlayout->setSpacing(5);
    Vlayout->addWidget(btn_simplifyGrammar);
    Vlayout->addWidget(btn_factoring);
    Vlayout->addWidget(btn_removeRecursive);
    Vlayout->addWidget(btn_first);
    Vlayout->addWidget(btn_follow);
    QLabel* lb = new QLabel("请输入分析的字符串：");
    lb->setFixedHeight(20);
    Vlayout->addWidget(lb);
    Vlayout->addWidget(le_inputString);
    Vlayout->addWidget(btn_leftmostDerive);
    wholeLayout->addItem(Vlayout, 1, 0);

    Vlayout = new QVBoxLayout();
    Vlayout->addWidget(new QLabel("化简后的文法："));
    Vlayout->addWidget(txb_simplifyGrammar);
    wholeLayout->addItem(Vlayout, 0, 1);

    Vlayout = new QVBoxLayout();
    Vlayout->addWidget(new QLabel("提取左公因子后的文法："));
    Vlayout->addWidget(txb_factoring);
    wholeLayout->addItem(Vlayout, 0, 2);

    Vlayout = new QVBoxLayout();
    Vlayout->addWidget(new QLabel("消除左递归后的文法："));
    Vlayout->addWidget(txb_removeRecursive);
    wholeLayout->addItem(Vlayout, 0, 3);

    Vlayout = new QVBoxLayout();
    Vlayout->addWidget(new QLabel("first集合："));
    Vlayout->addWidget(tbv_first);
    wholeLayout->addItem(Vlayout, 1, 1);

    Vlayout = new QVBoxLayout();
    Vlayout->addWidget(new QLabel("follow集合："));
    Vlayout->addWidget(tbv_follow);
    wholeLayout->addItem(Vlayout, 1, 2);

    Vlayout = new QVBoxLayout();
    Vlayout->addWidget(new QLabel("最左推导过程："));
    Vlayout->addWidget(tbv_leftmostDerive);
    wholeLayout->addItem(Vlayout, 1, 3);

    this->setLayout(wholeLayout);
    connect(btn_openFromFile, SIGNAL(clicked(bool)), this, SLOT(onClick_btn_openFromFile()));
    connect(btn_saveToFile, SIGNAL(clicked(bool)), this, SLOT(onClick_btn_saveToFile()));
    connect(btn_simplifyGrammar, SIGNAL(clicked(bool)), this, SLOT(onClick_btn_simplifyGrammar()));
    connect(btn_factoring, SIGNAL(clicked(bool)), this, SLOT(onClick_btn_factoring()));
    connect(btn_removeRecursive, SIGNAL(clicked(bool)), this, SLOT(onClick_btn_removeRecursive()));
    connect(btn_first, SIGNAL(clicked(bool)), this, SLOT(onClick_btn_first()));
    connect(btn_follow, SIGNAL(clicked(bool)), this, SLOT(onClick_btn_follow()));
    connect(btn_leftmostDerive, SIGNAL(clicked(bool)), this, SLOT(onClick_btn_leftmostDerive()));

}

Widget::~Widget()
{
}

//响应点击事件：从文件中读取文法
void Widget::onClick_btn_openFromFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("从文件中打开"), ".", tr("txt文件(*.txt)"));
    if(fileName.size() == 0) return;
    QFile file(fileName);
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QString content = file.readAll();
    txe_inputGrammar->setText(content);
    file.close();
}

//响应点击事件：把文法规则保存到文件中
void Widget::onClick_btn_saveToFile() {
    QString fileName=QFileDialog::getSaveFileName(this, tr("保存为文件"), ".", tr("txt文件(*.txt)"));
    if(fileName.size() == 0) return;
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QString content = txe_inputGrammar->toPlainText();
    file.write(content.toUtf8());
    file.close();
}

void Widget::onClick_btn_simplifyGrammar() {
    txb_factoring->clear();
    txb_removeRecursive->clear();
    QStandardItemModel *model = new QStandardItemModel();
    tbv_first->setModel(model);
    tbv_follow->setModel(model);
    tbv_leftmostDerive->setModel(model);
    simplifyGrammar();
}

void Widget::onClick_btn_factoring() {
    txb_removeRecursive->clear();
    QStandardItemModel *model = new QStandardItemModel();
    tbv_first->setModel(model);
    tbv_follow->setModel(model);
    tbv_leftmostDerive->setModel(model);
    simplifyGrammar();
    factoring();
}

void Widget::onClick_btn_removeRecursive() {
    QStandardItemModel *model = new QStandardItemModel();
    tbv_first->setModel(model);
    tbv_follow->setModel(model);
    tbv_leftmostDerive->setModel(model);
    simplifyGrammar();
    factoring();
    removeRecursive();
}

void Widget::onClick_btn_first() {
    QStandardItemModel *model = new QStandardItemModel();
    tbv_follow->setModel(model);
    tbv_leftmostDerive->setModel(model);
    simplifyGrammar();
    factoring();
    removeRecursive();
    first();
}

void Widget::onClick_btn_follow() {
    QStandardItemModel *model = new QStandardItemModel();
    tbv_first->setModel(model);
    tbv_leftmostDerive->setModel(model);
    simplifyGrammar();
    factoring();
    removeRecursive();
    follow();
}

void Widget::onClick_btn_leftmostDerive() {
    simplifyGrammar();
    factoring();
    removeRecursive();
    first();
    follow();
    leftmostDerive();
}


void Widget::setGrammarInTextBrowser(QTextBrowser* txb) {
    QString content = "";
    for(int i=0; i<VN.size(); i++) {
        for(int j=0; j<productions[i].size(); j++) {
            content += VN[i] + QString("->") + productions[i][j] + '\n';
        }
    }
    txb->setText(content);
}

void Widget::removeUnreachable() {
    if(VN.isEmpty()) return;
    QSet<QChar> reachable;
    QQueue<QChar> q;
    reachable.insert(start);
    q.enqueue(start);
    while(!q.isEmpty()) {
        QChar left = q.dequeue();
        int i = VN.indexOf(left);
        if(i != -1) {
            QList<QString> right = productions[i];
            for(int j=0; j<right.size(); j++) {
                QString str = right[j];
                for(int k=0; k<str.size(); k++) {
                    if(str[k].isUpper()) {
                        if(!reachable.contains(str[k])) {
                            reachable.insert(str[k]);
                            q.enqueue(str[k]);
                        }
                    }
                }
            }
        }
    }
    QSet<QChar> unreachable;
    for(int i=0; i<VN.size(); i++) {
        QChar left = VN[i];
        if(!reachable.contains(left)) {
            unreachable.insert(left);
            VN.removeAt(i);
            productions.removeAt(i);
            i--;
        }
    }
    for(int i=0; i<productions.size(); i++) {
        for(int j=0; j<productions[i].size(); j++) {
            for(int k=0; k<productions[i][j].size(); k++) {
                if(unreachable.contains(productions[i][j][k])) {
                    productions[i].removeAt(j);
                    j--;
                    break;
                }
            }
        }
        if(productions[i].size() == 0) {
            VN.removeAt(i);
            productions.removeAt(i);
            i--;
        }
    }
}

void Widget::removeUnterminable() {
    if(VN.isEmpty()) return;
    QSet<QChar> terminable, unterminable;
    for(int i=0; i<VN.size(); i++) {
        for(int j=0; j<productions[i].size(); j++) {
            if(productions[i][j].isLower()) {
                terminable.insert(VN[i]);
                break;
            }
        }
    }
    bool flag = true;
    while(flag) {
        flag = false;
        for(int i=0; i<VN.size(); i++) {
            if(terminable.contains(VN[i])) continue;
            for(int j=0; j<productions[i].size(); j++) {
                bool isTerminable = true;
                for(int k=0; k<productions[i][j].size(); k++) {
                    if(productions[i][j][k].isUpper() && !terminable.contains(productions[i][j][k])) {
                        isTerminable = false;
                        break;
                    }
                }
                if(isTerminable) {
                    terminable.insert(VN[i]);
                    flag = true;
                    break;
                }
            }
        }
    }
    for(int i=0; i<VN.size(); i++) {
        if(!terminable.contains(VN[i])) {
            unterminable.insert(VN[i]);
            VN.removeAt(i);
            productions.removeAt(i);
            i--;
        }
    }
    for(int i=0; i<productions.size(); i++) {
        for(int j=0; j<productions[i].size(); j++) {
            for(int k=0; k<productions[i][j].size(); k++) {
                if(unterminable.contains(productions[i][j][k])) {
                    productions[i].removeAt(j);
                    j--;
                    break;
                }
            }
        }
        if(productions[i].size() == 0) {
            VN.removeAt(i);
            productions.removeAt(i);
            i--;
        }
    }
}

//把集合变成字符串形式
QString Widget::setToString(QSet<QChar> S) {
    if(S.size() == 0) return "";
    QList<QChar> tempList = S.values();
    std::sort(tempList.begin(), tempList.end());
    QString result = "{";
    for (int i=0; i<tempList.size(); i++) {
        result = result + tempList[i] + ",";
    }
    result[result.length()-1] = '}';
    return result;
}

QSet<QChar> Widget::firstOfChar(QChar c) {
    QSet<QChar> result;
    if(c=='@' || c.isLower()) {
        result.insert(c);
    }
    else {
        int i = VN.indexOf(c);
        if(i != -1) {
            for(int j=0; j<productions[i].size(); j++) {
                if(productions[i][j][0] != VN[i]) {
                    result += firstOfString(productions[i][j]);
                }
            }
        }
    }
    return result;
}

QSet<QChar> Widget::firstOfString(QString s) {
    QSet<QChar> result;
    QSet<QChar> temp;
    QSet<QChar> epsilonSet;
    epsilonSet.insert('@');
    int k=0;
    do {
        temp = firstOfChar(s[k]);
        result += (temp - epsilonSet);
        k++;
    }while(k<s.size() && temp.contains('@'));
    if(temp.contains('@')) {
        result += epsilonSet;
    }
    return result;
}

void Widget::showFirstInTable() {
    QStandardItemModel* model = new QStandardItemModel();
    model->setColumnCount(2);
    model->setRowCount(VN.size());
    model->setHeaderData(0, Qt::Horizontal, "非终结符");
    model->setHeaderData(1, Qt::Horizontal, "first集合");
    for(int i=0; i<VN.size(); i++) {
        QStandardItem* item;
        item = new QStandardItem(VN[i]);
        model->setItem(i, 0, item);
        item = new QStandardItem(setToString(First[i]));
        model->setItem(i, 1, item);
    }
    tbv_first->setModel(model);
    tbv_first->verticalHeader()->setVisible(false);
}

void Widget::showFollowInTable() {
    QStandardItemModel* model = new QStandardItemModel();
    model->setColumnCount(2);
    model->setRowCount(VN.size());
    model->setHeaderData(0, Qt::Horizontal, "非终结符");
    model->setHeaderData(1, Qt::Horizontal, "follow集合");
    for(int i=0; i<VN.size(); i++) {
        model->setItem(i, 0, new QStandardItem(VN[i]));
        model->setItem(i, 1, new QStandardItem(setToString(Follow[i])));
    }
    tbv_follow->setModel(model);
    tbv_follow->verticalHeader()->setVisible(false);
}

void Widget::showLeftmostDeriveTable() {
//    for(int i=0; i<VN.size(); i++) {
//        qDebug()<<VN[i];
//        for(QMap<QChar,QString>::iterator it=analysisTable[i].begin(); it!=analysisTable[i].end(); it++) {
//            qDebug()<<it.key()<<it.value();
//        }
//        qDebug()<<"----------------------------------------------";
//    }

    QStandardItemModel* model = new QStandardItemModel();
    model->setColumnCount(3);
    model->setRowCount(processes.size());
    model->setHeaderData(0, Qt::Horizontal, "符号栈");
    model->setHeaderData(1, Qt::Horizontal, "输入串");
    model->setHeaderData(2, Qt::Horizontal, "动作");
    for(int i=0; i<processes.size(); i++) {
        model->setItem(i, 0, new QStandardItem(processes[i].symbolStack));
        model->setItem(i, 1, new QStandardItem(processes[i].inputString));
        model->setItem(i, 2, new QStandardItem(processes[i].action));
    }
    tbv_leftmostDerive->setModel(model);
}

QString Widget::reverse(QString s) {
    QString result = "";
    for(int i=s.size()-1; i>=0; i--) {
        result += s[i];
    }
    return result;
}

void Widget::simplifyGrammar() {
    VN.clear();
    productions.clear();
    QString str = txe_inputGrammar->toPlainText();
    int len = str.length();
    int i = 0;
    while(i < len) {
        QString p = "";
        while(i<len && str[i] != '\n') {
            p += str[i];
            i++;
        }
        if(p.length() > 0) {
            if(p.length() < 4) {
                QMessageBox::information(NULL, "提示", "文法输入有误。");
                return;
            }
            else {
                if(!VN.contains(p[0])) {
                    VN.append(p[0]);
                    productions.append(QList<QString>());
                }
                int index = VN.indexOf(p[0]);
                productions[index].append(p.mid(3));
            }
        }
        i++;
    }
    if(VN.size() > 0) {
        start = VN[0];
    }
    removeUnreachable();
    removeUnterminable();
    setGrammarInTextBrowser(txb_simplifyGrammar);
}

void Widget::factoring() {
    if(VN.isEmpty()) return;
    increment = 0;
    last = VN.last();
    for(int i=0; i<VN.size(); i++) {
        for(int j=0; j<productions[i].size()-1; j++) {
            bool flag = true;
            while(flag) {
                flag = false;
                QList<QString> followStrings;
                QChar begin = productions[i][j][0];
                for(int k=j+1; k<productions[i].size(); k++) {
                    if(productions[i][k][0] == begin) {
                        flag = true;
                        if(productions[i][k].size() > 1) {
                            followStrings.append(productions[i][k].mid(1));
                        }
                        else {
                            followStrings.append(QString("@"));
                        }
                        productions[i].removeAt(k);
                        k--;
                    }
                }
                if(flag) {
                    increment++;
                    QChar newVN = last.toLatin1() + increment;
                    if(productions[i][j].size() > 1) {
                        followStrings.insert(0, productions[i][j].mid(1));
                    }
                    else {
                        followStrings.insert(0, QString("@"));
                    }
                    productions[i][j] = QString(begin) + QString(newVN);
                    VN.insert(i+1, newVN);
                    productions.insert(i+1, followStrings);
                }
            }
        }
    }
    removeUnreachable();
    removeUnterminable();
    setGrammarInTextBrowser(txb_factoring);
}

void Widget::removeRecursive() {
    if(VN.isEmpty()) return;
    for(int i=0; i<VN.size(); i++) {
        for(int j=0; j<productions[i].size(); j++) {
            QChar begin = productions[i][j][0];
            if(begin.isUpper()) {
                for(int k=0; k<i; k++) {
                    if(VN[k] == begin) {
                        if(productions[i][j].size() == 1) {
                            for(int u=0; u<productions[k].size(); u++) {
                                productions[i].insert(j, productions[k][u]);
                                j++;
                            }
                        }
                        else {
                            for(int u=0; u<productions[k].size(); u++) {
                                QString replacement = "";
                                if(productions[k][u] != "@") {
                                    replacement = productions[k][u];
                                }
                                productions[i].insert(j, replacement + productions[i][j].mid(1));
                                j++;
                            }
                        }
                        productions[i].removeAt(j);
                        j--;
                        break;
                    }
                }
            }
        }

        for(int j=0; j<productions[i].size(); j++) {
            if(productions[i][j][0] == VN[i]) {
                QString str = "";
                if(productions[i][j].size() > 1) {
                    str = productions[i][j].mid(1);
                }
                increment++;
                QChar newVN = last.toLatin1() + increment;
                for(int k=0; k<productions[i].size(); k++) {
                    if(k == j) continue;
                    if(productions[i][k] == "@") {
                        productions[i][k] = newVN;
                    }
                    else {
                        productions[i][k] += newVN;
                    }
                }
                productions[i].removeAt(j);
                VN.insert(i+1, newVN);
                QList<QString> followStrings;
                followStrings.append(str + newVN);
                followStrings.append("@");
                productions.insert(i+1, followStrings);
                break;
            }
        }
    }
    removeUnreachable();
    removeUnterminable();
    setGrammarInTextBrowser(txb_removeRecursive);
}

void Widget::first() {
    First.clear();
    for(int i=0; i<VN.size(); i++) {
        First.append(firstOfChar(VN[i]));
    }
    showFirstInTable();
}

void Widget::follow() {
    Follow.clear();
    for(int i=0; i<VN.size(); i++) {
        Follow.append(QSet<QChar>());
    }
    if(Follow.size() > 0) {
        Follow[0].insert('$');
    }
    QSet<QChar> temp;
    QSet<QChar> epsilonSet;
    epsilonSet.insert('@');
    bool flag = true;
    while(flag) {
        flag = false;
        for(int i=0; i<VN.size(); i++) {
            for(int j=0; j<productions[i].size(); j++) {
                for(int k=0; k<productions[i][j].size(); k++) {
                    if(productions[i][j][k].isUpper()) {
                        int index = VN.indexOf(productions[i][j][k]);
                        if(k == productions[i][j].size() - 1) {
                            if((Follow[i] - Follow[index]).size() > 0) {
                                Follow[index] += Follow[i];
                                flag = true;
                            }
                        }
                        else {
                            temp = firstOfString(productions[i][j].mid(k+1));
                            if((temp - epsilonSet - Follow[index]).size() > 0) {
                                Follow[index] += (temp - epsilonSet);
                                flag = true;
                            }
                            if(temp.contains('@')) {
                                if((Follow[i] - Follow[index]).size() > 0) {
                                    Follow[index] += Follow[i];
                                    flag = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    showFollowInTable();
}

void Widget::leftmostDerive() {
    if(VN.isEmpty()) return;
    analysisTable.clear();
    for(int i=0; i<VN.size(); i++) {
        analysisTable.append(QMap<QChar,QString>());
        for(int j=0; j<productions[i].size(); j++) {
            QSet<QChar> firstOfCurrentProduction = firstOfString(productions[i][j]);
            for(QSet<QChar>::iterator it=firstOfCurrentProduction.begin(); it!=firstOfCurrentProduction.end(); it++) {
                if(*it != '@') {
                    analysisTable[i].insert(*it, productions[i][j]);
                }
                else {
                    QSet<QChar> followOfCurrentVN = Follow[i];
                    for(QSet<QChar>::iterator it2=followOfCurrentVN.begin(); it2!=followOfCurrentVN.end(); it2++) {
                        analysisTable[i].insert(*it2, productions[i][j]);
                    }
                }
            }
        }
    }

    processes.clear();
    QString symbolStack = start;
    QString inputString = le_inputString->text();
    while(1) {
        if(symbolStack.isEmpty()) {
            if(inputString.isEmpty()) {
                processes.append(Step(symbolStack, inputString, "成功"));
            }
            else {
                processes.append(Step(symbolStack, inputString, "失败"));
            }
            break;
        }
        QChar top = symbolStack.back();
        if(top.isLower()) {
            if(inputString.isEmpty()) {
                processes.append(Step(symbolStack, inputString, "失败"));
                break;
            }
            else {
                if(top == inputString[0]) {
                    processes.append(Step(symbolStack, inputString, "匹配"));
                    if(symbolStack.size() > 1) {
                        symbolStack = symbolStack.mid(0, symbolStack.size()-1);
                    }
                    else {
                        symbolStack = "";
                    }
                    if(inputString.size() > 1) {
                        inputString = inputString.mid(1);
                    }
                    else {
                        inputString = "";
                    }
                }
                else {
                    processes.append(Step(symbolStack, inputString, "失败"));
                    break;
                }
            }
        }
        else {
            int index = VN.indexOf(top);
            if(index == -1) {
                return;
            }
            if(inputString.isEmpty()) {
                if(productions[index].contains("@")) {
                    processes.append(Step(symbolStack, inputString, QString(top)+"->@"));
                    if(symbolStack.size() > 1) {
                        symbolStack = symbolStack.mid(0, symbolStack.size()-1);
                    }
                    else {
                        symbolStack = "";
                    }
                }
                else {
                    processes.append(Step(symbolStack, inputString, "失败"));
                    break;
                }
            }
            else {
                if(analysisTable[index].contains(inputString[0])) {
                    QString value = analysisTable[index].value(inputString[0]);
                    QString action = QString(top)+"->" + value;
                    processes.append(Step(symbolStack, inputString, action));
                    if(symbolStack.size() > 1) {
                        symbolStack = symbolStack.mid(0, symbolStack.size()-1);
                    }
                    else {
                        symbolStack = "";
                    }
                    if(value != "@") {
                        symbolStack += reverse(value);
                    }
                }
                else {
                    processes.append(Step(symbolStack, inputString, "失败"));
                    break;
                }
            }
        }
    }
    showLeftmostDeriveTable();
}

