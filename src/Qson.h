/*
 * qson (Qson.h)
 *  Part of QSON (http://github.com/waddlesplash/qson/).
 *
 * Copyright (c) 2012-2013 WaddleSplash
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall
 * be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
*/

#ifndef QSON_H
#define QSON_H

#include <QObject>
#include <QString>
#include <QIODevice>
#include "QsonObj.h"

class QsonDoc : public QsonObj
{
    Q_OBJECT
public:
    explicit QsonDoc(QObject* parent = 0);

    bool readFile(const QString filepath);
    bool readJSON(QIODevice *device);
    bool readJSON(const QString data);

    bool writeFile(const QString filepath);
    bool writeJSON(QIODevice *device);
    QString writeJSON();

#ifdef UNIT_TEST
signals:
    void logEvent(QString event);
#endif

private:
    bool parser_lexer();
    QString parser_readStr();
    QVariant parser_readNum();
    bool parser_isIntBetter(int a0,int a1);

    bool writer();
    void writer_writeItem(QsonObj* i);
    void writer_writeStr(QString str);
    void writer_writeVar(QVariant var);

    QChar parser_curchar;
    int parser_loc;
    int parser_length;
    QString parser_data;

    QString writer_data;
};

#endif // QSON_H
