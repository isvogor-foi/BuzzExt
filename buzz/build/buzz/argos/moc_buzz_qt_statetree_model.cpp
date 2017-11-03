/****************************************************************************
** Meta object code from reading C++ file 'buzz_qt_statetree_model.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/buzz/argos/buzz_qt_statetree_model.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'buzz_qt_statetree_model.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CBuzzQTStateTreeModel_t {
    QByteArrayData data[3];
    char stringdata0[31];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CBuzzQTStateTreeModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CBuzzQTStateTreeModel_t qt_meta_stringdata_CBuzzQTStateTreeModel = {
    {
QT_MOC_LITERAL(0, 0, 21), // "CBuzzQTStateTreeModel"
QT_MOC_LITERAL(1, 22, 7), // "Refresh"
QT_MOC_LITERAL(2, 30, 0) // ""

    },
    "CBuzzQTStateTreeModel\0Refresh\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CBuzzQTStateTreeModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x0a /* Public */,
       1,    1,   25,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void CBuzzQTStateTreeModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CBuzzQTStateTreeModel *_t = static_cast<CBuzzQTStateTreeModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Refresh(); break;
        case 1: _t->Refresh((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject CBuzzQTStateTreeModel::staticMetaObject = {
    { &QAbstractItemModel::staticMetaObject, qt_meta_stringdata_CBuzzQTStateTreeModel.data,
      qt_meta_data_CBuzzQTStateTreeModel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CBuzzQTStateTreeModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CBuzzQTStateTreeModel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CBuzzQTStateTreeModel.stringdata0))
        return static_cast<void*>(const_cast< CBuzzQTStateTreeModel*>(this));
    return QAbstractItemModel::qt_metacast(_clname);
}

int CBuzzQTStateTreeModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractItemModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
struct qt_meta_stringdata_CBuzzQTStateTreeVariableModel_t {
    QByteArrayData data[1];
    char stringdata0[30];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CBuzzQTStateTreeVariableModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CBuzzQTStateTreeVariableModel_t qt_meta_stringdata_CBuzzQTStateTreeVariableModel = {
    {
QT_MOC_LITERAL(0, 0, 29) // "CBuzzQTStateTreeVariableModel"

    },
    "CBuzzQTStateTreeVariableModel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CBuzzQTStateTreeVariableModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void CBuzzQTStateTreeVariableModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject CBuzzQTStateTreeVariableModel::staticMetaObject = {
    { &CBuzzQTStateTreeModel::staticMetaObject, qt_meta_stringdata_CBuzzQTStateTreeVariableModel.data,
      qt_meta_data_CBuzzQTStateTreeVariableModel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CBuzzQTStateTreeVariableModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CBuzzQTStateTreeVariableModel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CBuzzQTStateTreeVariableModel.stringdata0))
        return static_cast<void*>(const_cast< CBuzzQTStateTreeVariableModel*>(this));
    return CBuzzQTStateTreeModel::qt_metacast(_clname);
}

int CBuzzQTStateTreeVariableModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CBuzzQTStateTreeModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_CBuzzQTStateTreeFunctionModel_t {
    QByteArrayData data[1];
    char stringdata0[30];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CBuzzQTStateTreeFunctionModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CBuzzQTStateTreeFunctionModel_t qt_meta_stringdata_CBuzzQTStateTreeFunctionModel = {
    {
QT_MOC_LITERAL(0, 0, 29) // "CBuzzQTStateTreeFunctionModel"

    },
    "CBuzzQTStateTreeFunctionModel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CBuzzQTStateTreeFunctionModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void CBuzzQTStateTreeFunctionModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject CBuzzQTStateTreeFunctionModel::staticMetaObject = {
    { &CBuzzQTStateTreeModel::staticMetaObject, qt_meta_stringdata_CBuzzQTStateTreeFunctionModel.data,
      qt_meta_data_CBuzzQTStateTreeFunctionModel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CBuzzQTStateTreeFunctionModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CBuzzQTStateTreeFunctionModel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CBuzzQTStateTreeFunctionModel.stringdata0))
        return static_cast<void*>(const_cast< CBuzzQTStateTreeFunctionModel*>(this));
    return CBuzzQTStateTreeModel::qt_metacast(_clname);
}

int CBuzzQTStateTreeFunctionModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CBuzzQTStateTreeModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
