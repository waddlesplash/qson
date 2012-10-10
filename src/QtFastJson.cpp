/*
 * qtfastjson (QtFastJson.cpp)
 *  Part of QtFastJSON (http://github.com/waddlesplash/qtfastjson/).
 *
 * Copyright (c) 2012 WaddleSplash
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

#include "QtFastJson.h"

QtFastJsonDoc::QtFastJsonDoc(QObject *parent) :
    QtFastJsonObject(parent)
{
}

bool QtFastJsonDoc::readJSON(QIODevice* device)
{
    QIODevice::OpenMode o = device->openMode();
    if((o == QIODevice::ReadOnly) || (o == QIODevice::ReadWrite))
    {
        QString data = device->readAll();
        return readJSON(data);
    }
    return false;
}

bool QtFastJsonDoc::readJSON(const QString data)
{
    jsonDat = data;
    json_length = jsonDat.size();
    if(json_length == 0) { return false; }

    return parser_lexer();
}

bool QtFastJsonDoc::parser_lexer()
{
    QtFastJsonObject* curParent = this;
    QtFastJsonObject* newObj;
    QVariant newObj_Key;
    QVariant newObj_Val;

    QString in = "";
    QString reading = "key";

    for(json_loc = 0; json_loc < json_length;json_loc++) {
        json_curChar = jsonDat[json_loc];

        switch(json_curChar.toLatin1())
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
                curParent = curParent->jsonParent();

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
                curParent = curParent->jsonParent();

                if(in.endsWith('['))
                { reading = "array"; }
                else
                { reading = "key"; }
            }
            else
            { return false; }

            break;

        case 't':
            if((reading == "value") && (jsonDat.mid(json_loc,4) == "true"))
            {
                newObj_Val = true;
                newObj = curParent->addChild(newObj_Key,newObj_Val);
                newObj->setType(Variant);
                json_loc += 3;

                reading = "key";
                newObj_Key = "";
                newObj_Val = "";
            }

            break;

        case 'f':
            if((reading == "value") && (jsonDat.mid(json_loc,5) == "false"))
            {
                newObj_Val = false;
                newObj = curParent->addChild(newObj_Key,newObj_Val);
                newObj->setType(Variant);
                json_loc += 4;

                reading = "key";
                newObj_Key = "";
                newObj_Val = "";
            }

            break;

        case 'n':
            if((reading == "value") && (jsonDat.mid(json_loc,4) == "null"))
            {
                QVariant nullVariant(QVariant::String);
                newObj = curParent->addChild(newObj_Key,nullVariant);
                newObj->setType(Variant);
                json_loc += 3;

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
            if(json_curChar.isDigit() && (reading == "value"))
            {
                newObj_Val = parser_readNum();
                newObj = curParent->addChild(newObj_Key,newObj_Val);
                newObj->setType(Variant);

                reading = "key";
                newObj_Key = "";
                newObj_Val = "";
            }
            else if(json_curChar.isDigit() && (reading == "array"))
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
QString QtFastJsonDoc::parser_readStr()
{
    if(json_curChar != '"') { return ""; }

    QString ret;

    json_loc += 1;
    while(1)
    {
        if(jsonDat[json_loc] == '\\')
        {
            json_loc += 1;
            char cur = jsonDat[json_loc].toLatin1();
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
                ret += QChar(jsonDat.mid(json_loc+1,4).toInt(0,16));
                json_loc += 4;
                break;
            default:
                ret += jsonDat[json_loc];
                break;
            }
        }
        else if(jsonDat[json_loc] == '"')
        {
            return ret;
        }
        else
        { ret += jsonDat[json_loc]; }
        json_loc += 1;
    }

    return ret;
}

QVariant QtFastJsonDoc::parser_readNum()
{
    QVariant ret;
    QString raw;
    int addr = jsonDat.indexOf(',',json_loc);
    if(addr == -1) { addr = jsonDat.indexOf(' ',json_loc); }
    if(addr == -1) { addr = jsonDat.indexOf('\n',json_loc); }
    if(addr == -1) { addr = jsonDat.indexOf('\r',json_loc); }
    if(addr == -1) { addr = jsonDat.indexOf('}',json_loc); }
    if(addr == -1) { addr = jsonDat.indexOf(']',json_loc); }
    if(addr == -1) { return ""; }

    raw = jsonDat.mid(json_loc,addr-json_loc);
    ret = raw.toDouble();

    json_loc = addr;

    return ret;
}
