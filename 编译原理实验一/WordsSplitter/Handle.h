#ifndef ANALYZEPROGRAM_H
#define ANALYZEPROGRAM_H

#include <QFile>


//C++关键字
const QString keyword[] = {
    "asm", "auto", "bool", "break", "case",
    "catch", "char", "cin", "class", "const", "const_cast",
    "continue", "cout", "default", "delete", "do", "double",
    "dynamic_cast", "else", "enum", "explicit", "export",
    "extern", "false", "float", "for", "friend", "goto",
    "if", "include", "inline", "int", "long", "main",
    "mutable", "namespace", "new", "operator", "private",
    "protected", "public", "register", "reinterpret_cast", "return",
    "short", "signed", "sizeof", "static", "static_cast",
    "struct", "switch", "template", "this", "throw",
    "true", "try", "typedef", "typeid", "typename",
    "union", "unsigned", "using", "virtual", "void",
    "volatile", "wchar_t", "while"
};

//枚举类型，包含了几种类型的单词（记号）：
//标识符，关键字，数（包括整数、浮点数），字符串、注释、特殊符号（分界符）和运算符号
enum TYPE {ID, KEYW, NUM, STR, NOTE, SPEC, OPER};

class Handle {
public:
    Handle(QString path);
    bool isDigit(QChar c);
    bool isLetter(QChar c);
    bool isKeyword(QString s);
    void analyze();
    void add(TYPE type, QString s);
    void replace();
    QString getAnalyzeResult();
    QString getReplaceResult();

private:
    QString path;
    QString analyzeResult;
    QString replaceResult;
};

#endif // ANALYZEPROGRAM_H
