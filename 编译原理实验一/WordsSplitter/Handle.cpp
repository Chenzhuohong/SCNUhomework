#include "Handle.h"
#include <QTextCodec>
#include <QDebug>

Handle::Handle(QString path) {
    this->path = path;
}

bool Handle::isDigit(QChar c) {
    return (c >= '0' && c <= '9');
}

bool Handle::isLetter(QChar c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

bool Handle::isKeyword(QString s) {
    int n = sizeof(keyword) / sizeof(keyword[0]);
    for(int i = 0; i < n; i++) {
        if(s == keyword[i]) return true;
    }
    return false;
}

void Handle::analyze() {
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString content = codec->toUnicode(file.readAll());
    int i = 0;
    QString buf;
    while(i < content.size()) {
        buf = "";
        while(i < content.size() && (content[i] == ' ' || content[i] == '\t' || content[i] == '\n')) {
            i++;
        }
        if(i == content.size()) break;

        //如果以字母或者下划线开头
        if(isLetter(content[i]) || content[i] == '_') {
            do {
                buf = buf + content[i];
                i++;
            }while(isLetter(content[i]) || isDigit(content[i]) || content[i] == '_');
            if(isKeyword(buf)) {
                add(KEYW, buf);
            }
            else {
                add(ID, buf);
            }
        }

        //如果以数字开头
        else if(isDigit(content[i])) {
            do {
                buf = buf + content[i];
                i++;
            }while(isDigit(content[i]) || content[i] == '.');
            add(NUM, buf);
        }

        //如果以其他字符开头
        else {
            TYPE type = OPER;
            switch(content[i].toLatin1()) {
            case '[':
            case ']':
            case '.':
            case '~':
            case ',':
            case '?': {
                buf = content[i];
                i++;
                break;
            }
            case ':': {
                buf = content[i];
                i++;
                if(content[i] == ':') {
                    buf += content[i];
                    i++;
                }
                break;
            }
            case '-': {
                buf = content[i];
                i++;
                if(content[i] == '-' || content[i] == '>' || content[i] == '=') {
                    buf += content[i];
                    i++;
                }
                break;
            }
            case '+': {
                buf = content[i];
                i++;
                if(content[i] == '+' || content[i] == '=') {
                    buf += content[i];
                    i++;
                }
                break;
            }
            case '<': {
                buf = content[i];
                i++;
                if(content[i] == '<') {
                    buf += content[i];
                    i++;
                    if(content[i] == '=') {
                        buf += content[i];
                        i++;
                    }
                }
                else if(content[i] == '=') {
                    buf += content[i];
                    i++;
                }
                else {
                    QString temp = "";
                    int j = i;
                    while(isLetter(content[j]) || isDigit(content[j]) || content[j] == '_' || content[j] == '.') {
                        temp += content[j];
                        j++;
                    }
                    if(content[j] == '>') {
                        temp += content[j];
                        buf += temp;
                        type = SPEC;
                        i = j + 1;
                    }
                }
                break;
            }
            case '>': {
                buf = content[i];
                i++;
                if(content[i] == '>') {
                    buf += content[i];
                    i++;
                    if(content[i] == '=') {
                        buf += content[i];
                        i++;
                    }
                }
                else if(content[i] == '=') {
                    buf += content[i];
                    i++;
                }
                break;
            }
            case '=':
            case '!':
            case '*':
            case '%':
            case '^': {
                buf = content[i];
                i++;
                if(content[i] == '=') {
                    buf += content[i];
                    i++;
                }
                break;
            }
            case '&': {
                buf = content[i];
                i++;
                if(content[i] == '&' || content[i] == '=') {
                    buf += content[i];
                    i++;
                }
                break;
            }
            case '|': {
                buf = content[i];
                i++;
                if(content[i] == '|' || content[i] == '=') {
                    buf += content[i];
                    i++;
                }
                break;
            }
            case '\'': {
                buf = content[i];
                i++;
                while(content[i] != '\'') {
                    buf += content[i];
                    if(content[i] == '\\') {
                        buf += content[i+1];
                        i++;
                    }
                    i++;
                }
                buf += content[i];
                i++;
                type = STR;
                break;
            }
            case '"': {
                buf = content[i];
                i++;
                while(content[i] != '"') {
                    buf += content[i];
                    if(content[i] == '\\') {
                        buf += content[i+1];
                        i++;
                    }
                    i++;
                }
                buf += content[i];
                i++;
                type = STR;
                break;
            }
            case '/': {
                buf = content[i];
                i++;
                if(content[i] == '=') {
                    buf += content[i];
                    i++;
                }
                //处理单行注释
                else if(content[i] == '/') {
                    buf += content[i];
                    i++;
                    while(i < content.size() && content[i] != '\n') {
                        i++;
                    }
                    type = NOTE;
                }
                //处理多行注释
                else if(content[i] == '*') {
                    buf += content[i];
                    i++;
                    add(NOTE, buf);
                    while(i + 1 < content.size()) {
                        if(content[i] == '*' && content[i+1] == '/') {
                            buf = "*/";
                            i += 2;
                            break;
                        }
                        i++;
                    }
                    type = NOTE;
                }
                break;
            }
            default: {
                buf = content[i];
                i++;
                type = SPEC;
                break;
            }
            }
            add(type, buf);
        }
    }
    file.close();
}

void Handle::add(TYPE type, QString s) {
    analyzeResult += s;
    int cnt = 4 - (s.length() - 1) / 8;
    cnt = (cnt > 0) ? cnt : 1;
    for(int i = 0; i < cnt; i++)
        analyzeResult += '\t';
    switch (type) {
    case ID: analyzeResult = analyzeResult + "标识符"; break;
    case KEYW: analyzeResult = analyzeResult + "关键字"; break;
    case NUM: analyzeResult = analyzeResult + "数"; break;
    case STR: analyzeResult = analyzeResult + "字符串"; break;
    case NOTE: analyzeResult = analyzeResult + "注释符"; break;
    case SPEC: analyzeResult = analyzeResult + "特殊符号"; break;
    case OPER: analyzeResult = analyzeResult + "运算符号"; break;
    default: break;
    }
    analyzeResult = analyzeResult + '\n';
}

void Handle::replace() {
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString content = codec->toUnicode(file.readAll());
    int i = 0;
    QString buf;
    while(i < content.size()) {
        buf = "";
        while(i < content.size() && (content[i] == ' ' || content[i] == '\t' || content[i] == '\n')) {
            replaceResult += content[i];
            i++;
        }
        if(i == content.size()) break;

        //如果以字母或者下划线开头
        if(isLetter(content[i]) || content[i] == '_') {
            do {
                buf = buf + content[i];
                i++;
            }while(isLetter(content[i]) || isDigit(content[i]) || content[i] == '_');
            if(buf == "include") {
                replaceResult += "import";
            }
            else if(buf == "int") {
                replaceResult += "Integer";
            }
            else if(buf == "char") {
                replaceResult += "Character";
            }
            else if(buf == "bool") {
                replaceResult += "Boolean";
            }
            else if(buf == "float") {
                replaceResult += "Float";
            }
            else if(buf == "double") {
                replaceResult += "Double";
            }
            else if(buf == "long") {
                replaceResult += "Long";
            }
            else if(buf == "break") {
                replaceResult += "shutdown";
            }
            else if(buf == "while") {
                replaceResult += "when";
            }
            else {
                replaceResult += buf;
            }
        }

        //如果以数字开头
        else if(isDigit(content[i])) {
            do {
                buf = buf + content[i];
                i++;
            }while(isDigit(content[i]) || content[i] == '.');
            replaceResult += buf;
        }

        //如果以其他字符开头
        else {
            if (i + 1 < content.size() && content[i] == '/' && content[i + 1] == '*') {
                buf = "{";
                i += 2;
                while(i + 1 < content.size()) {
                    if(content[i] == '*' && content[i+1] == '/') {
                        buf += "}";
                        i += 2;
                        break;
                    }
                    buf += content[i];
                    i++;
                }
            }
            else {
                buf = content[i];
                i++;
            }
            replaceResult += buf;
        }
    }
    file.close();
}

QString Handle::getAnalyzeResult() {
    this->analyze();
    return analyzeResult;
}

QString Handle::getReplaceResult() {
    this->replace();
    return replaceResult;
}
