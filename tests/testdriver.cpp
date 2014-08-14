#include <QCoreApplication>
#include <QFile>
#include <iostream>

#include <Qson.h>
#include "ReadTester.h"

QString indents(int number)
{
    QString ret = "";
    for (int i = 0; i < number; i++) {
        ret += "\t";
    }
    return ret;
}

bool qFilePutContents(QString fileName, QString data)
{
    QFile f(fileName);
    if (!f.open(QFile::WriteOnly))
        return false;
    f.write(data.toUtf8());
    f.close();
    return true;
}

QByteArray qFileGetContents(QString fileName)
{
    QFile f(fileName);
    if (!f.open(QFile::ReadOnly))
        return "";

    QByteArray ret = f.readAll();
    f.close();
    return ret;
}

bool compareStringLists(QStringList a, QStringList b)
{
    if (a.size() != b.size())
        return false;
    for (int i = 0; i < a.size(); i++) {
        if (a.at(i) != b.at(i))
            return false;
    }
    return true;
}

void outputResult(bool result, QString type, QString name)
{
    if (result)
        std::cout << "\033[1;32mPASS: \033[0m ";
    else
        std::cout << "\033[1;31mFAIL: \033[0m ";
    std::cout << type.toUtf8().constData();
    std::cout << " ";
    std::cout << name.toUtf8().constData();
    std::cout << "\n";
}

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    int failed = 0, passed = 0;
    QStringList items = QString::fromUtf8(qFileGetContents("testcases.lst")).split("\n");

    foreach (QString item, items) {
        if (ReadTester::Run(item))
            passed++;
        else
            failed++;
    }

    if (failed == 0) {
        printf("\n" "SUCCESS! All %d tests passed!\n", passed);
        return 0;
    } else {
        printf("\n" "OHNOES! %d tests failed, %d tests succeeded.\n", failed, passed);
        return 1;
    }
}
