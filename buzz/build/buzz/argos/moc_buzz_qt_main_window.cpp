/****************************************************************************
** Meta object code from reading C++ file 'buzz_qt_main_window.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/buzz/argos/buzz_qt_main_window.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'buzz_qt_main_window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CBuzzQTTabWidget_t {
    QByteArrayData data[3];
    char stringdata0[29];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CBuzzQTTabWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CBuzzQTTabWidget_t qt_meta_stringdata_CBuzzQTTabWidget = {
    {
QT_MOC_LITERAL(0, 0, 16), // "CBuzzQTTabWidget"
QT_MOC_LITERAL(1, 17, 10), // "TabRemoved"
QT_MOC_LITERAL(2, 28, 0) // ""

    },
    "CBuzzQTTabWidget\0TabRemoved\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CBuzzQTTabWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void CBuzzQTTabWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CBuzzQTTabWidget *_t = static_cast<CBuzzQTTabWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->TabRemoved(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CBuzzQTTabWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CBuzzQTTabWidget::TabRemoved)) {
                *result = 0;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CBuzzQTTabWidget::staticMetaObject = {
    { &QTabWidget::staticMetaObject, qt_meta_stringdata_CBuzzQTTabWidget.data,
      qt_meta_data_CBuzzQTTabWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CBuzzQTTabWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CBuzzQTTabWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CBuzzQTTabWidget.stringdata0))
        return static_cast<void*>(const_cast< CBuzzQTTabWidget*>(this));
    return QTabWidget::qt_metacast(_clname);
}

int CBuzzQTTabWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTabWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void CBuzzQTTabWidget::TabRemoved()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_CBuzzQTMainWindow_t {
    QByteArrayData data[32];
    char stringdata0[379];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CBuzzQTMainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CBuzzQTMainWindow_t qt_meta_stringdata_CBuzzQTMainWindow = {
    {
QT_MOC_LITERAL(0, 0, 17), // "CBuzzQTMainWindow"
QT_MOC_LITERAL(1, 18, 7), // "NewOpen"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 14), // "OpenRecentFile"
QT_MOC_LITERAL(4, 42, 4), // "Save"
QT_MOC_LITERAL(5, 47, 6), // "SaveAs"
QT_MOC_LITERAL(6, 54, 7), // "SaveAll"
QT_MOC_LITERAL(7, 62, 4), // "Undo"
QT_MOC_LITERAL(8, 67, 4), // "Redo"
QT_MOC_LITERAL(9, 72, 4), // "Copy"
QT_MOC_LITERAL(10, 77, 3), // "Cut"
QT_MOC_LITERAL(11, 81, 5), // "Paste"
QT_MOC_LITERAL(12, 87, 4), // "Find"
QT_MOC_LITERAL(13, 92, 7), // "SetMain"
QT_MOC_LITERAL(14, 100, 7), // "Compile"
QT_MOC_LITERAL(15, 108, 7), // "Execute"
QT_MOC_LITERAL(16, 116, 11), // "CloseEditor"
QT_MOC_LITERAL(17, 128, 5), // "n_idx"
QT_MOC_LITERAL(18, 134, 14), // "SetTabModified"
QT_MOC_LITERAL(19, 149, 10), // "b_modified"
QT_MOC_LITERAL(20, 160, 13), // "UpdateActions"
QT_MOC_LITERAL(21, 174, 24), // "UpdateRecentFilesActions"
QT_MOC_LITERAL(22, 199, 26), // "UpdateSetMainScriptActions"
QT_MOC_LITERAL(23, 226, 15), // "CheckBuzzStatus"
QT_MOC_LITERAL(24, 242, 6), // "n_step"
QT_MOC_LITERAL(25, 249, 27), // "HandleRunTimeErrorSelection"
QT_MOC_LITERAL(26, 277, 21), // "HandleEntitySelection"
QT_MOC_LITERAL(27, 299, 6), // "size_t"
QT_MOC_LITERAL(28, 306, 8), // "un_index"
QT_MOC_LITERAL(29, 315, 23), // "HandleEntityDeselection"
QT_MOC_LITERAL(30, 339, 19), // "VariableTreeChanged"
QT_MOC_LITERAL(31, 359, 19) // "FunctionTreeChanged"

    },
    "CBuzzQTMainWindow\0NewOpen\0\0OpenRecentFile\0"
    "Save\0SaveAs\0SaveAll\0Undo\0Redo\0Copy\0"
    "Cut\0Paste\0Find\0SetMain\0Compile\0Execute\0"
    "CloseEditor\0n_idx\0SetTabModified\0"
    "b_modified\0UpdateActions\0"
    "UpdateRecentFilesActions\0"
    "UpdateSetMainScriptActions\0CheckBuzzStatus\0"
    "n_step\0HandleRunTimeErrorSelection\0"
    "HandleEntitySelection\0size_t\0un_index\0"
    "HandleEntityDeselection\0VariableTreeChanged\0"
    "FunctionTreeChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CBuzzQTMainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      25,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  139,    2, 0x0a /* Public */,
       3,    0,  140,    2, 0x0a /* Public */,
       4,    0,  141,    2, 0x0a /* Public */,
       5,    0,  142,    2, 0x0a /* Public */,
       6,    0,  143,    2, 0x0a /* Public */,
       7,    0,  144,    2, 0x0a /* Public */,
       8,    0,  145,    2, 0x0a /* Public */,
       9,    0,  146,    2, 0x0a /* Public */,
      10,    0,  147,    2, 0x0a /* Public */,
      11,    0,  148,    2, 0x0a /* Public */,
      12,    0,  149,    2, 0x0a /* Public */,
      13,    0,  150,    2, 0x0a /* Public */,
      14,    0,  151,    2, 0x0a /* Public */,
      15,    0,  152,    2, 0x0a /* Public */,
      16,    1,  153,    2, 0x0a /* Public */,
      18,    1,  156,    2, 0x0a /* Public */,
      20,    0,  159,    2, 0x0a /* Public */,
      21,    0,  160,    2, 0x0a /* Public */,
      22,    0,  161,    2, 0x0a /* Public */,
      23,    1,  162,    2, 0x0a /* Public */,
      25,    0,  165,    2, 0x0a /* Public */,
      26,    1,  166,    2, 0x0a /* Public */,
      29,    1,  169,    2, 0x0a /* Public */,
      30,    0,  172,    2, 0x0a /* Public */,
      31,    0,  173,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Bool,   19,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   24,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 27,   28,
    QMetaType::Void, 0x80000000 | 27,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CBuzzQTMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CBuzzQTMainWindow *_t = static_cast<CBuzzQTMainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->NewOpen(); break;
        case 1: _t->OpenRecentFile(); break;
        case 2: _t->Save(); break;
        case 3: _t->SaveAs(); break;
        case 4: _t->SaveAll(); break;
        case 5: _t->Undo(); break;
        case 6: _t->Redo(); break;
        case 7: _t->Copy(); break;
        case 8: _t->Cut(); break;
        case 9: _t->Paste(); break;
        case 10: _t->Find(); break;
        case 11: _t->SetMain(); break;
        case 12: { bool _r = _t->Compile();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 13: _t->Execute(); break;
        case 14: _t->CloseEditor((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->SetTabModified((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: _t->UpdateActions(); break;
        case 17: _t->UpdateRecentFilesActions(); break;
        case 18: _t->UpdateSetMainScriptActions(); break;
        case 19: _t->CheckBuzzStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->HandleRunTimeErrorSelection(); break;
        case 21: _t->HandleEntitySelection((*reinterpret_cast< size_t(*)>(_a[1]))); break;
        case 22: _t->HandleEntityDeselection((*reinterpret_cast< size_t(*)>(_a[1]))); break;
        case 23: _t->VariableTreeChanged(); break;
        case 24: _t->FunctionTreeChanged(); break;
        default: ;
        }
    }
}

const QMetaObject CBuzzQTMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_CBuzzQTMainWindow.data,
      qt_meta_data_CBuzzQTMainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CBuzzQTMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CBuzzQTMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CBuzzQTMainWindow.stringdata0))
        return static_cast<void*>(const_cast< CBuzzQTMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int CBuzzQTMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 25)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 25;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
