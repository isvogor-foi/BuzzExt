/****************************************************************************
** Meta object code from reading C++ file 'buzz_qt_editor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/buzz/argos/buzz_qt_editor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'buzz_qt_editor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CBuzzQTEditor_t {
    QByteArrayData data[21];
    char stringdata0[266];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CBuzzQTEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CBuzzQTEditor_t qt_meta_stringdata_CBuzzQTEditor = {
    {
QT_MOC_LITERAL(0, 0, 13), // "CBuzzQTEditor"
QT_MOC_LITERAL(1, 14, 18), // "RecentFilesChanged"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 18), // "EditorCursorUpdate"
QT_MOC_LITERAL(4, 53, 4), // "line"
QT_MOC_LITERAL(5, 58, 6), // "column"
QT_MOC_LITERAL(6, 65, 21), // "EditorFileNameChanged"
QT_MOC_LITERAL(7, 87, 8), // "QString&"
QT_MOC_LITERAL(8, 96, 8), // "filename"
QT_MOC_LITERAL(9, 105, 3), // "New"
QT_MOC_LITERAL(10, 109, 4), // "Open"
QT_MOC_LITERAL(11, 114, 4), // "Save"
QT_MOC_LITERAL(12, 119, 6), // "SaveAs"
QT_MOC_LITERAL(13, 126, 25), // "UpdateLineNumberAreaWidth"
QT_MOC_LITERAL(14, 152, 20), // "HighlightCurrentLine"
QT_MOC_LITERAL(15, 173, 20), // "UpdateLineNumberArea"
QT_MOC_LITERAL(16, 194, 6), // "c_rect"
QT_MOC_LITERAL(17, 201, 4), // "n_dy"
QT_MOC_LITERAL(18, 206, 12), // "CodeModified"
QT_MOC_LITERAL(19, 219, 17), // "UpdateRecentFiles"
QT_MOC_LITERAL(20, 237, 28) // "UpdateLineAndColumnIndicator"

    },
    "CBuzzQTEditor\0RecentFilesChanged\0\0"
    "EditorCursorUpdate\0line\0column\0"
    "EditorFileNameChanged\0QString&\0filename\0"
    "New\0Open\0Save\0SaveAs\0UpdateLineNumberAreaWidth\0"
    "HighlightCurrentLine\0UpdateLineNumberArea\0"
    "c_rect\0n_dy\0CodeModified\0UpdateRecentFiles\0"
    "UpdateLineAndColumnIndicator"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CBuzzQTEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,
       3,    2,   80,    2, 0x06 /* Public */,
       6,    1,   85,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   88,    2, 0x0a /* Public */,
      10,    0,   89,    2, 0x0a /* Public */,
      11,    0,   90,    2, 0x0a /* Public */,
      12,    0,   91,    2, 0x0a /* Public */,
      13,    1,   92,    2, 0x08 /* Private */,
      14,    0,   95,    2, 0x08 /* Private */,
      15,    2,   96,    2, 0x08 /* Private */,
      18,    0,  101,    2, 0x08 /* Private */,
      19,    0,  102,    2, 0x08 /* Private */,
      20,    0,  103,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    4,    5,
    QMetaType::Void, 0x80000000 | 7,    8,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QRect, QMetaType::Int,   16,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CBuzzQTEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CBuzzQTEditor *_t = static_cast<CBuzzQTEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->RecentFilesChanged(); break;
        case 1: _t->EditorCursorUpdate((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->EditorFileNameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->New(); break;
        case 4: { bool _r = _t->Open();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: { bool _r = _t->Save();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 6: { bool _r = _t->SaveAs();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: _t->UpdateLineNumberAreaWidth((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->HighlightCurrentLine(); break;
        case 9: _t->UpdateLineNumberArea((*reinterpret_cast< const QRect(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: _t->CodeModified(); break;
        case 11: _t->UpdateRecentFiles(); break;
        case 12: _t->UpdateLineAndColumnIndicator(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CBuzzQTEditor::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CBuzzQTEditor::RecentFilesChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (CBuzzQTEditor::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CBuzzQTEditor::EditorCursorUpdate)) {
                *result = 1;
            }
        }
        {
            typedef void (CBuzzQTEditor::*_t)(QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CBuzzQTEditor::EditorFileNameChanged)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject CBuzzQTEditor::staticMetaObject = {
    { &QPlainTextEdit::staticMetaObject, qt_meta_stringdata_CBuzzQTEditor.data,
      qt_meta_data_CBuzzQTEditor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CBuzzQTEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CBuzzQTEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CBuzzQTEditor.stringdata0))
        return static_cast<void*>(const_cast< CBuzzQTEditor*>(this));
    return QPlainTextEdit::qt_metacast(_clname);
}

int CBuzzQTEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPlainTextEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void CBuzzQTEditor::RecentFilesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void CBuzzQTEditor::EditorCursorUpdate(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CBuzzQTEditor::EditorFileNameChanged(QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
