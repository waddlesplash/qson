/*
 * qson (QsonObj.cpp)
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

#include "QsonObj.h"

QsonObj::QsonObj(QObject *parent, QVariant key) :
    QObject(parent)
{
    myKey = key;
    myVariantValue = "";
}
QsonObj::QsonObj(QsonObj *parent, QVariant key, bool isFJO) :
    QObject(parent)
{
    myKey = key;
    myVariantValue = "";
    if(!isFJO) { return; }
    myParent = parent;
}

void QsonObj::setKey(QVariant newKey)
{
    myParent->zDONTCALL_setChildKey(this,myKey,newKey);
    myKey = newKey;
}

void QsonObj::zDONTCALL_setChildKey(QsonObj* o, QVariant oldKey, QVariant newKey)
{
    childItems.remove(oldKey.toString());
    childItems.insert(newKey.toString(),o);
}

QsonObj* QsonObj::addChild(QVariant key, QVariant value)
{
    QsonObj* newObj = new QsonObj(this,key,true);
    newObj->setVariantValue(value);

    childItems.insert(key.toString(),newObj);
    childItems_ordOfIns.insert(childItems_ordOfIns.count(),newObj);

    return newObj;
}

void QsonObj::removeChild(QVariant key)
{
    QsonObj *objToRem = childItems.value(key.toString());
    childItems.remove(key.toString());
    childItems_ordOfIns.removeOne(objToRem);
}
void QsonObj::removeChild(QsonObj *object)
{
    childItems.remove(childItems.key(object));
    childItems_ordOfIns.removeOne(object);
}
