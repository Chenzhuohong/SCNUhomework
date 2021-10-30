#ifndef WIDGET_H
#define WIDGET_H

#include "Graph.h"
#include "showProgramWidget.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTableView>
#include <QList>
#include <QSet>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    bool isEmptyInput();
    void generateNFA();
    QString insertAndNode();
    int isp(QChar ch);
    int icp(QChar ch);
    bool isOperator(QChar ch);
    QString infixToPost(QString infixExp);
    void Thompson(QString postExp);
    void showNFA();

    void generateDFA();
    void initCharacter();
    QSet<int> epsilonClosureOfSet(QSet<int> S);
    QSet<int> move(QSet<int> S, QString val);
    void subsetConstruction();
    QString setToString(QSet<int> S);
    void showDFA();

    void generateMinDFA();
    int indexOfMinDtran(QSet<int> S);
    bool isLastState(QSet<int> S);
    void showMinDFA();

    QString generateProgram();

private:
    Graph NFA;
    QList<QChar> character;
    QList<QSet<int>> Dstates;
    QList<QList<QSet<int>>> Dtran;
    QList<QList<QSet<int>>> MinDtran;
    QVBoxLayout* VLayout;
    QHBoxLayout *HLayout;
    QLabel* lbl1;
    QPushButton* btn_openFromFile;
    QPushButton* btn_saveToFile;
    QLineEdit* le_input;
    QPushButton* btn_generateNFA;
    QPushButton* btn_generateDFA;
    QPushButton* btn_generateMinDFA;
    QPushButton* btn_generateProgram;
    QTableView* tbv_show;
    showProgramWidget* childWin;

private slots:
    void onClick_btn_openFromFile();
    void onClick_btn_saveToFile();
    void onClick_btn_generateNFA();
    void onClick_btn_generateDFA();
    void onClick_btn_generateMinDFA();
    void onClick_btn_generateProgram();

};
#endif // WIDGET_H
