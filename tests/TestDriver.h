#ifndef MAIN_H
#define MAIN_H

#include <QString>
#include <QStringList>
#include <QByteArray>

bool qFilePutContents(QString fileName, QString data);
QByteArray qFileGetContents(QString fileName);

void outputResult(bool result, QString type, QString name);
bool compareStringLists(QStringList a, QStringList b);
QString indents(int number);

#endif // MAIN_H
