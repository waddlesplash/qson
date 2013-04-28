/*
 * qson (Qson.cpp)
 *  Part of Qson (http://github.com/waddlesplash/qson/).
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

#include <QFile>
#include "Qson.h"

QsonDoc::QsonDoc(QObject *parent) :
    QsonObj(parent)
{
    parser_curchar = ' ';
    parser_loc = 0;
    parser_length = 0;
    parser_data = "";
    writer_data = "";
    setType(List);
}

bool QsonDoc::readFile(const QString filepath)
{
    QFile f(filepath);
    if(!f.exists()) { return false; }
    if(!f.open(QFile::ReadOnly)) { return false; }
    bool result = readJSON(f.readAll());
    f.close();
    return result;
}

bool QsonDoc::readJSON(QIODevice* device)
{
    QIODevice::OpenMode o = device->openMode();
    if((o == QIODevice::ReadOnly) || (o == QIODevice::ReadWrite))
    {
        QString data = device->readAll();
        return readJSON(data);
    }
    return false;
}

bool QsonDoc::readJSON(const QString data)
{
    parser_data = data;
    parser_length = parser_data.size();
    if(parser_length == 0) { return false; }

    return parser_lexer();
}

bool QsonDoc::parser_lexer()
{
    QsonObj* curParent = this;
    QsonObj* newObj;
    QVariant newObj_Key;
    QVariant newObj_Val;

    QString in = "";
    QString reading = "key";

    for(parser_loc = 0; parser_loc < parser_length;parser_loc++) {
        parser_curchar = parser_data[parser_loc];

        switch(parser_curchar.toLatin1())
        {
        case '{':
        {
            in += "{";

            if(in == "{")
            { break; }
            else if(reading == "array")
            { newObj_Key = curParent->childCount(); }

            curParent = curParent->addChild(newObj_Key);
            curParent->setType(List);
            newObj_Key = "";

            reading = "key";

            break;
        }
        case '}':
            if(in.endsWith('{') && !(in.size() == 1))
            {
                in.chop(1);
                curParent = curParent->parent();

                if(in.endsWith('['))
                { reading = "array"; }
                else
                { reading = "key"; }
            }
            else if(in.size() == 1)
            { return true; }
            else
            { return false; }

            break;

        case '[':
        {
            in += "[";
            if(reading == "array")
            { newObj_Key = QString::number(curParent->childCount()); }

            curParent = curParent->addChild(newObj_Key);
            curParent->setType(Array);
            newObj_Key = "";

            reading = "array";
        }
            break;
        case ']':
            if(in.endsWith('['))
            {
                in.chop(1);
                curParent = curParent->parent();

                if(in.endsWith('['))
                { reading = "array"; }
                else
                { reading = "key"; }
            }
            else
            { return false; }

            break;

        case 't':
            if((reading == "value") && (parser_data.mid(parser_loc,4) == "true"))
            {
                newObj_Val = true;
                newObj = curParent->addChild(newObj_Key,newObj_Val);
                newObj->setType(Variant);
                parser_loc += 3;

                reading = "key";
                newObj_Key = "";
                newObj_Val = "";
            }

            break;

        case 'f':
            if((reading == "value") && (parser_data.mid(parser_loc,5) == "false"))
            {
                newObj_Val = false;
                newObj = curParent->addChild(newObj_Key,newObj_Val);
                newObj->setType(Variant);
                parser_loc += 4;

                reading = "key";
                newObj_Key = "";
                newObj_Val = "";
            }

            break;

        case 'n':
            if((reading == "value") && (parser_data.mid(parser_loc,4) == "null"))
            {
                QVariant nullVariant(QVariant::String);
                newObj = curParent->addChild(newObj_Key,nullVariant);
                newObj->setType(Variant);
                parser_loc += 3;

                reading = "key";
                newObj_Key = "";
            }

            break;

        case '"':
            if(reading == "value")
            {
                newObj_Val = parser_readStr();
                newObj = curParent->addChild(newObj_Key,newObj_Val);
                newObj->setType(Variant);

                reading = "key";
                newObj_Key = "";
                newObj_Val = "";
            }
            else if(reading == "key")
            {
                newObj_Key = parser_readStr();
            }
            else if(reading == "array")
            {
                QString newArrayVal = parser_readStr();
                newObj = curParent->addChild(curParent->childCount(),newArrayVal);
                newObj->setType(Variant);
            }

            break;

        case ':':
            reading = "value";
            break;

        default:
            if(parser_curchar.isDigit() && (reading == "value"))
            {
                newObj_Val = parser_readNum();
                newObj = curParent->addChild(newObj_Key,newObj_Val);
                newObj->setType(Variant);

                reading = "key";
                newObj_Key = "";
                newObj_Val = "";
            }
            else if(parser_curchar.isDigit() && (reading == "array"))
            {
                QVariant newArrayVal = parser_readNum();
                newObj = curParent->addChild(curParent->childCount(),newArrayVal);
                newObj->setType(Variant);
            }
            break;
        }
    }
    return false;
}
QString QsonDoc::parser_readStr()
{
    if(parser_curchar != '"') { return ""; }

    QString ret;

    parser_loc += 1;
    while(1)
    {
        if(parser_data[parser_loc] == '\\')
        {
            parser_loc += 1;
            char cur = parser_data[parser_loc].toLatin1();
            switch(cur)
            { // as per http://www.json.org/
            case 'b':
                ret += "\b";
                break;
            case 'f':
                ret += "\f";
                break;
            case 'n':
                ret += "\n";
                break;
            case 'r':
                ret += "\r";
                break;
            case 't':
                ret += "\t";
                break;
            case 'u':
                ret += QChar(parser_data.mid(parser_loc+1,4).toInt(0,16));
                parser_loc += 4;
                break;
            default:
                ret += parser_data[parser_loc];
                break;
            }
        }
        else if(parser_data[parser_loc] == '"')
        {
            return ret;
        }
        else
        { ret += parser_data[parser_loc]; }
        parser_loc += 1;
    }

    return ret;
}
QVariant QsonDoc::parser_readNum()
{
    QVariant ret;
    QString raw;
    int addr2 = 0, addr = parser_data.indexOf(',',parser_loc);

    addr2 = parser_data.indexOf(' ',parser_loc);
    if(parser_isIntBetter(addr,addr2)) { addr = addr2; }

    addr2 = parser_data.indexOf(' ',parser_loc);
    if(parser_isIntBetter(addr,addr2)) { addr = addr2; }

    addr2 = parser_data.indexOf('\n',parser_loc);
    if(parser_isIntBetter(addr,addr2)) { addr = addr2; }

    addr2 = parser_data.indexOf('\r',parser_loc);
    if(parser_isIntBetter(addr,addr2)) { addr = addr2; }

    addr2 = parser_data.indexOf(']',parser_loc);
    if(parser_isIntBetter(addr,addr2)) { addr = addr2; }

    addr2 = parser_data.indexOf('}',parser_loc);
    if(parser_isIntBetter(addr,addr2)) { addr = addr2; }
    if(addr == -1) { return ""; }

    raw = parser_data.mid(parser_loc,addr-parser_loc);
    ret = raw.toDouble(); // TODO: locale handling

    parser_loc = addr;

    return ret;
}
bool QsonDoc::parser_isIntBetter(int a0, int a1)
{
    return (a1 != -1) && ((a1 < a0) || (a0 == -1));
}

bool QsonDoc::writeFile(const QString filepath)
{
    QFile f(filepath);
    if(!f.open(QFile::WriteOnly)) { return false; }
    bool result = writer();
    if(result) { f.write(writer_data.toUtf8()); }
    f.close();
    return result;
}

bool QsonDoc::writeJSON(QIODevice* device)
{
    QIODevice::OpenMode o = device->openMode();
    if((o == QIODevice::WriteOnly) || (o == QIODevice::ReadWrite))
    {
        bool result = writer();
        if(result) { device->write(writer_data.toUtf8()); }
        return result;
    }
    return false;
}

QString QsonDoc::writeJSON()
{
    writer();
    return writer_data;
}

bool QsonDoc::writer()
{
    writer_data = "{";

    QsonObj* o = 0;
    foreach(o,this->children())
    { writer_writeItem(o); }
    writer_data += "}";
    return true; // TODO: fail on bad cases, e.g. string writer
}

void QsonDoc::writer_writeItem(QsonObj* i)
{
    QsonObj* o = 0;

    if(i->parent()->type() != Array)
    { writer_writeVar(i->key()); }

    switch(i->type())
    {
    case List:
        if(i->parent()->type() != Array)
        { writer_data += ":{"; }
        else { writer_data += "{"; }

        foreach(o,i->children())
        { writer_writeItem(o); }

        if(i->parent()->children().last() != i)
        { writer_data += "},"; }
        else { writer_data += "}"; }
        break;

    case Array:
        if(i->parent()->type() != Array)
        { writer_data += ":["; }
        else { writer_data += "["; }

        foreach(o,i->children())
        { writer_writeItem(o); }

        if(i->parent()->children().last() != i)
        { writer_data += "],"; }
        else { writer_data += "]"; }
        break;

    case Variant:
        if(i->parent()->type() != Array)
        { writer_data += ":"; }

        writer_writeVar(i->variant()); // TODO: locale
        if(i->parent()->children().last() != i)
        { writer_data += ","; }
        break;

    default:
        break;
    }
}

void QsonDoc::writer_writeStr(QString str)
{
    QString w = "\"";
    for(int i = 0;i<str.size();i++)
    {
        switch(str[i].toLatin1())
        {
        case '"':
            w += "\\\"";
            break;

        case '\\':
            w += "\\\\";
            break;

        case '\b':
            w += "\\b";
            break;

        case '\f':
            w += "\\f";
            break;

        case '\t':
            w += "\\t";
            break;

        default:
            w += str[i];
            break;
        }
    }
    w += "\"";
    writer_data += w;
}

void QsonDoc::writer_writeVar(QVariant var)
{
    if(!var.isNull())
    {
        switch(var.type())
        {
        case QVariant::String:
            writer_writeStr(var.toString());
            break;
        default:
            writer_data += var.toString();
            break;
        }
    }
    else { writer_data += "null"; }
}
