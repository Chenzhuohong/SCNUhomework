#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QTextBrowser>
#include <QTableView>
#include <QLabel>
#include <QLineEdit>

struct Step {
    QString symbolStack;
    QString inputString;
    QString action;
    Step(QString s = "", QString i = "", QString a = "") {
        symbolStack = s;
        inputString = i;
        action = a;
    }
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    QList<QChar> VN;
    QChar start;
    QChar last;
    int increment;
    QList<QList<QString>> productions;
    QList<QSet<QChar>> First;
    QList<QSet<QChar>> Follow;
    QList<QMap<QChar,QString>> analysisTable;
    QVector<Step> processes;

    QGridLayout* wholeLayout;
    QPushButton* btn_openFromFile;
    QPushButton* btn_saveToFile;
    QPushButton* btn_simplifyGrammar;
    QPushButton* btn_factoring;
    QPushButton* btn_removeRecursive;
    QPushButton* btn_first;
    QPushButton* btn_follow;
    QPushButton* btn_leftmostDerive;
    QTextEdit* txe_inputGrammar;
    QLineEdit* le_inputString;
    QTextBrowser* txb_simplifyGrammar;
    QTextBrowser* txb_factoring;
    QTextBrowser* txb_removeRecursive;
    QTableView* tbv_first;
    QTableView* tbv_follow;
    QTableView* tbv_leftmostDerive;

    void simplifyGrammar();
    void factoring();
    void removeRecursive();
    void first();
    void follow();
    void leftmostDerive();

    void removeUnreachable();
    void removeUnterminable();
    void setGrammarInTextBrowser(QTextBrowser* txb);
    QString setToString(QSet<QChar> S);
    QSet<QChar> firstOfChar(QChar c);
    QSet<QChar> firstOfString(QString s);
    void showFirstInTable();
    void showFollowInTable();
    void showLeftmostDeriveTable();
    QString reverse(QString s);

private slots:
    void onClick_btn_openFromFile();
    void onClick_btn_saveToFile();
    void onClick_btn_simplifyGrammar();
    void onClick_btn_factoring();
    void onClick_btn_removeRecursive();
    void onClick_btn_first();
    void onClick_btn_follow();
    void onClick_btn_leftmostDerive();

};
#endif // WIDGET_H
