/****************************************************************************
** Meta object code from reading C++ file 'DataSplit.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../DataSplit.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DataSplit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DataSplit_t {
    QByteArrayData data[11];
    char stringdata0[100];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DataSplit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DataSplit_t qt_meta_stringdata_DataSplit = {
    {
QT_MOC_LITERAL(0, 0, 9), // "DataSplit"
QT_MOC_LITERAL(1, 10, 14), // "serialDataSend"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 5), // "char*"
QT_MOC_LITERAL(4, 32, 4), // "data"
QT_MOC_LITERAL(5, 37, 7), // "sendLen"
QT_MOC_LITERAL(6, 45, 14), // "SendFileSignal"
QT_MOC_LITERAL(7, 60, 8), // "sendPath"
QT_MOC_LITERAL(8, 69, 13), // "handleResults"
QT_MOC_LITERAL(9, 83, 7), // "rbuffer"
QT_MOC_LITERAL(10, 91, 8) // "comIndex"

    },
    "DataSplit\0serialDataSend\0\0char*\0data\0"
    "sendLen\0SendFileSignal\0sendPath\0"
    "handleResults\0rbuffer\0comIndex"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DataSplit[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x06 /* Public */,
       6,    1,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    2,   37,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    4,    5,
    QMetaType::Void, QMetaType::QString,    7,

 // slots: parameters
    QMetaType::Void, QMetaType::QByteArray, QMetaType::Int,    9,   10,

       0        // eod
};

void DataSplit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DataSplit *_t = static_cast<DataSplit *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->serialDataSend((*reinterpret_cast< char*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->SendFileSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->handleResults((*reinterpret_cast< QByteArray(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DataSplit::*_t)(char * , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataSplit::serialDataSend)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (DataSplit::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataSplit::SendFileSignal)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject DataSplit::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DataSplit.data,
      qt_meta_data_DataSplit,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *DataSplit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DataSplit::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_DataSplit.stringdata0))
        return static_cast<void*>(const_cast< DataSplit*>(this));
    return QWidget::qt_metacast(_clname);
}

int DataSplit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void DataSplit::serialDataSend(char * _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DataSplit::SendFileSignal(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
