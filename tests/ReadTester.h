#ifndef READTESTER_H
#define READTESTER_H

#include <QString>
#include <QStringList>

#include <Qson.h>

class ReadTester
{
public:
    static bool Run(QString fileName);

private:
    static void handleObject(QsonObj* obj, QStringList* data, int indentLevel);
};

#endif // READTESTER_H
