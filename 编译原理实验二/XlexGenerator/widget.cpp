#include "widget.h"
#include <QTextCodec>
#include <QDebug>
#include <QStack>
#include <QQueue>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    VLayout = new QVBoxLayout();

    HLayout = new QHBoxLayout();
    lbl1 = new QLabel("请输入正则表达式：");
    HLayout->addWidget(lbl1);
    btn_openFromFile = new QPushButton("从文件中打开");
    btn_openFromFile->setFixedSize(150,40);
    HLayout->addWidget(btn_openFromFile);
    HLayout->addSpacing(20);
    btn_saveToFile = new QPushButton("保存为文件");
    btn_saveToFile->setFixedSize(150,40);
    HLayout->addWidget(btn_saveToFile);
    VLayout->addItem(HLayout);

    le_input = new QLineEdit();
    le_input->setFont(QFont( "SimSun", 15, 5, false));
    VLayout->addWidget(le_input);
    VLayout->addSpacing(30);

    HLayout = new QHBoxLayout();
    HLayout->setSpacing(20);
    btn_generateNFA = new QPushButton("生成NFA");
    HLayout->addWidget(btn_generateNFA);
    btn_generateDFA = new QPushButton("生成DFA");
    HLayout->addWidget(btn_generateDFA);
    btn_generateMinDFA = new QPushButton("生成最小化DFA");
    HLayout->addWidget(btn_generateMinDFA);
    btn_generateProgram = new QPushButton("生成词法分析程序");
    HLayout->addWidget(btn_generateProgram);
    VLayout->addItem(HLayout);

    tbv_show = new QTableView();
    tbv_show->setMinimumSize(800,500);
    VLayout->addWidget(tbv_show);

    this->setLayout(VLayout);
    connect(btn_openFromFile, SIGNAL(clicked(bool)), this, SLOT(onClick_btn_openFromFile()));
    connect(btn_saveToFile, SIGNAL(clicked(bool)), this, SLOT(onClick_btn_saveToFile()));
    connect(btn_generateNFA, SIGNAL(clicked(bool)), this, SLOT(onClick_btn_generateNFA()));
    connect(btn_generateDFA, SIGNAL(clicked(bool)), this, SLOT(onClick_btn_generateDFA()));
    connect(btn_generateMinDFA, SIGNAL(clicked(bool)), this, SLOT(onClick_btn_generateMinDFA()));
    connect(btn_generateProgram, SIGNAL(clicked(bool)), this, SLOT(onClick_btn_generateProgram()));
}

Widget::~Widget()
{
}

//响应点击事件：从文件中读取正则表达式
void Widget::onClick_btn_openFromFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("从文件中打开"), ".", tr("txt文件(*.txt)"));
    if(fileName.size() == 0) return;
    QFile file(fileName);
    if (!file.exists())
    {
        QMessageBox::information(NULL, "提示", "文件不存在！");
    }
    else {
        file.open(QIODevice::ReadOnly|QIODevice::Text);
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        QString content = codec->toUnicode(file.readAll());
        le_input->setText(content);
        file.close();
    }
}

//响应点击事件：把正则表达式保存到文件中
void Widget::onClick_btn_saveToFile() {
    QString fileName=QFileDialog::getSaveFileName(this, tr("保存为文件"), ".", tr("txt文件(*.txt)"));
    if(fileName.size() == 0) return;
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QString content = le_input->text();
    file.write(content.toUtf8());
    file.close();
}

//响应点击事件：生成NFA
void Widget::onClick_btn_generateNFA() {
    if(isEmptyInput()) {
        return;
    }
    generateNFA();
    showNFA();
}

//响应点击事件：生成DFA
void Widget::onClick_btn_generateDFA() {
    if(isEmptyInput()) {
        return;
    }
    generateDFA();
    showDFA();
}

//响应点击事件：生成最小DFA
void Widget::onClick_btn_generateMinDFA() {
    if(isEmptyInput()) {
        return;
    }
    generateMinDFA();
    showMinDFA();
}

//响应点击事件：生成词法分析程序
void Widget::onClick_btn_generateProgram() {
    if(isEmptyInput()) {
        return;
    }
    generateMinDFA();
    childWin = new showProgramWidget();
    childWin->setWindowModality(Qt::ApplicationModal);
    childWin->showProgram(generateProgram());
    childWin->show();
}

//判断输入的正则表达式是否为空
bool Widget::isEmptyInput() {
    if(le_input->text().size() == 0) {
        QMessageBox::information(NULL, "提示", "请输入正则表达式！");
        return true;
    }
    return false;
}

//生成NFA
void Widget::generateNFA() {
    NFA.clear();
    QString infixExp = insertAndNode();
    QString postExp = infixToPost(infixExp);
    Thompson(postExp);
}

//让'&'作为连接运算符
QString Widget::insertAndNode() {
    QString regexp = le_input->text();
    if(regexp.length() == 0) return "";
    QString infixExp = "";
    for(int i=0; i<regexp.length()-1; i++) {
        infixExp += regexp[i];
        if(regexp[i] != '(' && regexp[i] != '|') {
            if(regexp[i+1] != ')' && regexp[i+1] != '|' && regexp[i+1] != '*') {
                infixExp += '&';
            }
        }
    }
    infixExp += regexp[regexp.length()-1];
    return infixExp;
}

//栈内运算符优先级(in stack priority)
int Widget::isp(QChar ch) {
    if(ch == '(') return 1;
    else if(ch == '*') return 7;
    else if(ch == '&') return 5;
    else if(ch == '|') return 3;
    else if(ch == ')') return 8;
    else return 0;
}

//栈外运算符优先级(in coming priority)
int Widget::icp(QChar ch) {
    if(ch == '(') return 8;
    else if(ch == '*') return 6;
    else if(ch == '&') return 4;
    else if(ch == '|') return 2;
    else if(ch == ')') return 1;
    else return 0;
}

//判断某字符是否为运算符
bool Widget::isOperator(QChar ch) {
    return (ch == '(' || ch == '*' || ch == '&' || ch == '|' || ch == ')');
}

//中缀表达式转换为后缀表达式
QString Widget::infixToPost(QString infixExp) {
    QString postExp = "";
    QStack<QChar> stk;
    stk.push('#');
    int i = 0;
    while(!stk.empty() && i < infixExp.length()) {
        QChar ch = infixExp[i];
        if(!isOperator(ch)) {
            postExp += ch;
            i++;
        }
        else {
            QChar top = stk.top();
            if(icp(ch) > isp(top)) {
                stk.push(ch);
                i++;
            }
            else if(icp(ch) < isp(top)) {
                postExp += stk.pop();
            }
            else {
                stk.pop();
                i++;
            }
        }
    }
    while(stk.size() > 1) {
        postExp += stk.pop();
    }
    return postExp;
}

//Thompson构造法构造NFA
void Widget::Thompson(QString postExp) {
    NFA.insertVertex(0);
    QStack<int> stk;
    int nodes = 0;
    for(int i=0; i<postExp.length(); i++) {
        if(postExp[i] == '|') {
            if(stk.size() < 4) {
                exit(1);
            }
            int y2 = stk.pop();
            int x2 = stk.pop();
            int y1 = stk.pop();
            int x1 = stk.pop();
            int u = nodes + 1, v = nodes + 2;
            nodes += 2;
            NFA.insertVertex(u);
            NFA.insertVertex(v);
            NFA.insertEdge(u, x1, new QString("ε"));
            NFA.insertEdge(u, x2, new QString("ε"));
            NFA.insertEdge(y1, v, new QString("ε"));
            NFA.insertEdge(y2, v, new QString("ε"));
            stk.push(u);
            stk.push(v);
        }
        else if(postExp[i] == '&') {
            if(stk.size() < 4) {
                exit(1);
            }
            int y2 = stk.pop();
            int x2 = stk.pop();
            int y1 = stk.pop();
            int x1 = stk.pop();
            NFA.insertEdge(y1, x2, new QString("ε"));
            stk.push(x1);
            stk.push(y2);
        }
        else if(postExp[i] == '*') {
            if(stk.size() < 2) {
                exit(1);
            }
            int y1 = stk.pop();
            int x1 = stk.pop();
            int u = nodes + 1, v = nodes + 2;
            nodes += 2;
            NFA.insertVertex(u);
            NFA.insertVertex(v);
            NFA.insertEdge(u, x1, new QString("ε"));
            NFA.insertEdge(u, v, new QString("ε"));
            NFA.insertEdge(y1, x1, new QString("ε"));
            NFA.insertEdge(y1, v, new QString("ε"));
            stk.push(u);
            stk.push(v);
        }
        else {
            int start = nodes + 1, end = nodes + 2;
            nodes += 2;
            NFA.insertVertex(start);
            NFA.insertVertex(end);
            NFA.insertEdge(start, end, new QString(""+postExp[i]));
            stk.push(start);
            stk.push(end);
        }
    }
    if(stk.size() < 2) {
        exit(1);
    }
    stk.pop();
    int x1 = stk.pop();
    NFA.insertEdge(0, x1, new QString("ε"));
}

//以邻接表的形式打印NFA
void Widget::showNFA() {
    QStandardItemModel* model = new QStandardItemModel();
    int columns = NFA.maxEdgesOfVertex();
    int rows = NFA.getNumberOfVertex();

    /* 设置列数 */
    model->setColumnCount(columns);
    for(int i=0; i<columns; i++) {
        model->setHeaderData(i, Qt::Horizontal, "邻接顶点"+QString::number(i+1,10)+", 边上元素");
    }

    /* 设置行数 */
    model->setRowCount(rows);
    model->setHeaderData(0, Qt::Vertical, "顶点"+QString::number(0,10)+"(START)");
    for(int i=1; i<rows; i++) {
        model->setHeaderData(i, Qt::Vertical, "顶点"+QString::number(i,10));
    }
    model->setHeaderData(rows-1, Qt::Vertical, "顶点"+QString::number(rows-1,10)+"(END)");

    /* 在表格内加入内容 */
    Vertex* p1 = NFA.getFirstVertext();
    while(p1 != NULL) {
        Edge* p2 = p1->first;
        int col = 0;
        while(p2 != NULL) {
            QStandardItem* item = new QStandardItem(QString::number(p2->dest,10) + ", " + *(p2->value));
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            model->setItem(p1->num, col, item);
            col++;
            p2 = p2->next;
        }
        p1 = p1->link;
    }

    tbv_show->setModel(model);
    /* 设置列宽 */
    for(int i=0; i<columns; i++) {
        tbv_show->setColumnWidth(i, 200);
    }
}

//生成DFA
void Widget::generateDFA() {
    generateNFA();
    Dstates.clear();
    Dtran.clear();
    initCharacter();
    subsetConstruction();
}

//初始化所输入的正则表达式中包含的不同字符
void Widget::initCharacter() {
    character.clear();
    QString regexp = le_input->text();
    for(int i=0; i<regexp.length(); i++) {
        if(!isOperator(regexp[i])) {
            if(!character.contains(regexp[i])) {
                character.append(regexp[i]);
            }
        }
    }
}

//求集合S的"ε"闭包
QSet<int> Widget::epsilonClosureOfSet(QSet<int> S) {
    QSet<int> resultSet = S;
    QQueue<int> Q;
    Vertex* vertex;
    Edge* edge;
    for (QSet<int>::iterator it=S.begin(); it!=S.end(); it++) {
        Q.enqueue(*it);
    }
    while(!Q.empty()) {
        int x = Q.dequeue();
        vertex = NFA.getVertexByNum(x);
        edge = vertex->first;
        while(edge != NULL) {
            if(*(edge->value) == "ε") {
                if(!resultSet.contains(edge->dest)) {
                    resultSet.insert(edge->dest);
                    Q.enqueue(edge->dest);
                }
            }
            edge = edge->next;
        }
    }
    return resultSet;
}

//求集合S的val闭包
QSet<int> Widget::move(QSet<int> S, QString val) {
    QSet<int> resultSet;
    Vertex* vertex;
    Edge* edge;
    for (QSet<int>::iterator it=S.begin(); it!=S.end(); it++) {
        vertex = NFA.getVertexByNum(*it);
        edge = vertex->first;
        while(edge != NULL) {
            if(*(edge->value) == val) {
                if(!resultSet.contains(edge->dest)) {
                    resultSet.insert(edge->dest);
                }
            }
            edge = edge->next;
        }
    }
    return resultSet;
}

//子集构造法构造DFA
void Widget::subsetConstruction() {
    QQueue<QSet<int>> Q;
    QSet<int> first;
    first.insert(NFA.getFirstVertext()->num);
    Q.enqueue(epsilonClosureOfSet(first));
    Dstates.append(epsilonClosureOfSet(first));
    while(!Q.empty()) {
        QList<QSet<int>> DtranItem;
        QSet<int> S = Q.dequeue();
        DtranItem.append(S);
        for(int i=0; i<character.size(); i++) {
            QSet<int> U = epsilonClosureOfSet(move(S, QString(character[i])));
            if(U.size()>0 && !Dstates.contains(U)) {
                Dstates.append(U);
                Q.enqueue(U);
            }
            DtranItem.append(U);
        }
        Dtran.append(DtranItem);
    }
}

//把集合变成字符串形式
QString Widget::setToString(QSet<int> S) {
    if(S.size() == 0) return "";
    QList<int> tempList = S.values();
    std::sort(tempList.begin(), tempList.end());
    QString result = "{";
    for (int i=0; i<tempList.size(); i++) {
        result = result + QString::number(tempList[i], 10) + ",";
    }
    result[result.length()-1] = '}';
    return result;
}

//打印DFA(打印DFA的转换表Dtran)
void Widget::showDFA() {
    QStandardItemModel* model = new QStandardItemModel();
    int columns = character.size();
    int rows = Dtran.size();

    /* 设置列数 */
    model->setColumnCount(columns);
    for(int i=0; i<columns; i++) {
        model->setHeaderData(i, Qt::Horizontal, character[i]);
    }

    /* 设置行数 */
    model->setRowCount(rows);
    for(int i=0; i<rows; i++) {
        model->setHeaderData(i, Qt::Vertical, setToString(Dtran[i][0]));
    }

    /* 在表格内加入内容 */
    for(int i=0; i<rows; i++) {
        for(int j=0; j<columns; j++) {
            QStandardItem* item = new QStandardItem(setToString(Dtran[i][j+1]));
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            model->setItem(i, j, item);
        }
    }

    tbv_show->setModel(model);
    /* 设置列宽 */
    for(int i=0; i<columns; i++) {
        tbv_show->setColumnWidth(i, 200);
    }
}

//生成MinDFA
void Widget::generateMinDFA() {
    generateDFA();
    QHash<QSet<int>, QSet<int>> equalStates;
    MinDtran = Dtran;
    int sizeOfMinDtran = 0;
    while(sizeOfMinDtran != MinDtran.size()) {
        sizeOfMinDtran = MinDtran.size();
        for(int i=0; i<MinDtran.size()-1; i++) {
            for(int j=i+1; j<MinDtran.size(); j++) {
                if((isLastState(MinDtran[i][0]) && isLastState(MinDtran[j][0])) ||
                   (!isLastState(MinDtran[i][0]) && !isLastState(MinDtran[j][0]))) {
                    bool flag = true;
                    for(int k=1; k<=character.size(); k++) {
                        if(equalStates.contains(MinDtran[i][k])) {
                            MinDtran[i][k] = equalStates[MinDtran[i][k]];
                        }
                        if(equalStates.contains(MinDtran[j][k])) {
                            MinDtran[j][k] = equalStates[MinDtran[j][k]];
                        }
                        if(MinDtran[i][k] != MinDtran[j][k]) {
                            flag = false;
                            break;
                        }
                    }
                    if(flag) {
                        equalStates[MinDtran[j][0]] = MinDtran[i][0];
                        MinDtran.removeAt(j);
                        j--;
                    }
                }
            }
        }
    }
    for(int i=0; i<MinDtran.size(); i++) {
        for(int j=1; j<=character.size(); j++) {
            if(equalStates.contains(MinDtran[i][j])) {
                MinDtran[i][j] = equalStates[MinDtran[i][j]];
            }
        }
    }
}

//返回集合S在MinDtran的索引
int Widget::indexOfMinDtran(QSet<int> S) {
    for(int i=0; i<MinDtran.size(); i++) {
        if(S == MinDtran[i][0]) {
            return i;
        }
    }
    return -1;
}

//判断是否为终态
bool Widget::isLastState(QSet<int> S) {
    return S.contains(NFA.getNumberOfVertex()-1);
}

//打印MinDFA(打印MinDFA的转换表MinDtran)
void Widget::showMinDFA() {
    QStandardItemModel* model = new QStandardItemModel();
    int columns = character.size();
    int rows = MinDtran.size();

    /* 设置列数 */
    model->setColumnCount(columns);
    for(int i=0; i<columns; i++) {
        model->setHeaderData(i, Qt::Horizontal, character[i]);
    }

    /* 设置行数 */
    model->setRowCount(rows);
    for(int i=0; i<rows; i++) {
        QString content = QString('A'+i);
        if(MinDtran[i][0].contains(0)) {
            content += "(START)";
        }
        if(isLastState(MinDtran[i][0])) {
            content += "(END)";
        }
        model->setHeaderData(i, Qt::Vertical, content);
    }

    /* 在表格内加入内容 */
    for(int i=0; i<rows; i++) {
        for(int j=0; j<columns; j++) {
            QString content;
            if(MinDtran[i][j+1].size() == 0) {
                content = "";
            }
            else {
                content = QString('A'+indexOfMinDtran(MinDtran[i][j+1]));
            }
            QStandardItem* item = new QStandardItem(content);
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            model->setItem(i, j, item);
        }
    }

    tbv_show->setModel(model);
    /* 设置列宽 */
    for(int i=0; i<columns; i++) {
        tbv_show->setColumnWidth(i, 200);
    }
}

//生成词法分析程序
QString Widget::generateProgram() {
    QString result = "";
    int chars = character.size();
    int states = MinDtran.size();
    int* lastStates = new int[states];
    for(int i=0; i<states; i++) {
        if(isLastState(MinDtran[i][0])) {
            lastStates[i] = 1;
        }
        else {
            lastStates[i] = 0;
        }
    }

    result += "#include <iostream>\n";
    result += "using namespace std;\n";

    result += "char edges[] = {";
    for(int i=0; i<chars-1; i++) {
        result += "'" + QString(character[i]) + "',";
    }
    if(chars > 0) {
        result += "'" + QString(character[chars-1]) + "'";
    }
    result += "};\n";

    result += "int DFA[][" + QString::number(chars) + "] = {";
    for(int i=0; i<states-1; i++) {
        result += "{";
        for(int j=1; j<chars; j++) {
            result += QString::number(indexOfMinDtran(MinDtran[i][j])) + ",";
        }
        if(chars > 0) {
            result += QString::number(indexOfMinDtran(MinDtran[i][chars]));
        }
        result += "},";
    }
    if(states > 0) {
        result += "{";
        for(int j=1; j<chars; j++) {
            result += QString::number(indexOfMinDtran(MinDtran[states-1][j])) + ",";
        }
        if(chars > 0) {
            result += QString::number(indexOfMinDtran(MinDtran[states-1][chars]));
        }
        result += "}";
    }
    result += "};\n";

    result += "bool isLastState[] = {";
    for(int i=0; i<states-1; i++) {
        result += QString::number(lastStates[i]) + ",";
    }
    if(states > 0) {
        result += QString::number(lastStates[states-1]);
    }
    result += "};\n\n";

    result += "int indexInEdges(char ch) {\n";
    result += "\tint index = -1;\n";
    result += "\tfor(int i=0; i<" + QString::number(chars) + "; i++) {\n";
    result += "\t\tif(edges[i] == ch) {\n";
    result += "\t\t\tindex = i;\n";
    result += "\t\t}\n";
    result += "\t}\n";
    result += "\treturn index;\n";
    result += "}\n\n";

    result += "int main() {\n";
    result += "\tint state = 0;\n";
    result += "\tstring s;\n";
    result += "\tgetline(cin,s);\n";
    result += "\tfor(int i=0; i<s.length(); i++) {\n";
    result += "\t\tint index = indexInEdges(s[i]);\n";
    result += "\t\tif(index == -1) {\n";
    result += "\t\t\tbreak;\n";
    result += "\t\t}\n";
    result += "\t\tstate = DFA[state][index];\n";
    result += "\t\tif(state == -1) {\n";
    result += "\t\t\tbreak;\n";
    result += "\t\t}\n";
    result += "\t}\n";
    result += "\tif(isLastState[state]) {\n";
    result += "\t\tcout<<\"YES\"<<endl;\n";
    result += "\t}\n";
    result += "\telse {\n";
    result += "\t\tcout<<\"NO\"<<endl;\n";
    result += "\t}\n";
    result += "\treturn 0;\n";
    result += "}\n";

    return result;
}



