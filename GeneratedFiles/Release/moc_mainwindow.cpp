/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[16];
    char stringdata0[304];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 19), // "openActionTriggered"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 20), // "closeActionTriggered"
QT_MOC_LITERAL(4, 53, 19), // "lastActionTriggered"
QT_MOC_LITERAL(5, 73, 19), // "nextActionTriggered"
QT_MOC_LITERAL(6, 93, 21), // "toLeftActionTriggered"
QT_MOC_LITERAL(7, 115, 22), // "toRightActionTriggered"
QT_MOC_LITERAL(8, 138, 24), // "toEnlargeActionTriggered"
QT_MOC_LITERAL(9, 163, 23), // "toLessenActionTriggered"
QT_MOC_LITERAL(10, 187, 21), // "deleteActionTriggered"
QT_MOC_LITERAL(11, 209, 14), // "aboutTriggered"
QT_MOC_LITERAL(12, 224, 16), // "aboutQtTriggered"
QT_MOC_LITERAL(13, 241, 20), // "lastActionTriggered1"
QT_MOC_LITERAL(14, 262, 20), // "nextActionTriggered1"
QT_MOC_LITERAL(15, 283, 20) // "openActionTriggered1"

    },
    "MainWindow\0openActionTriggered\0\0"
    "closeActionTriggered\0lastActionTriggered\0"
    "nextActionTriggered\0toLeftActionTriggered\0"
    "toRightActionTriggered\0toEnlargeActionTriggered\0"
    "toLessenActionTriggered\0deleteActionTriggered\0"
    "aboutTriggered\0aboutQtTriggered\0"
    "lastActionTriggered1\0nextActionTriggered1\0"
    "openActionTriggered1"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x0a /* Public */,
       3,    0,   85,    2, 0x0a /* Public */,
       4,    0,   86,    2, 0x0a /* Public */,
       5,    0,   87,    2, 0x0a /* Public */,
       6,    0,   88,    2, 0x0a /* Public */,
       7,    0,   89,    2, 0x0a /* Public */,
       8,    0,   90,    2, 0x0a /* Public */,
       9,    0,   91,    2, 0x0a /* Public */,
      10,    0,   92,    2, 0x0a /* Public */,
      11,    0,   93,    2, 0x0a /* Public */,
      12,    0,   94,    2, 0x0a /* Public */,
      13,    0,   95,    2, 0x0a /* Public */,
      14,    0,   96,    2, 0x0a /* Public */,
      15,    0,   97,    2, 0x0a /* Public */,

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
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->openActionTriggered(); break;
        case 1: _t->closeActionTriggered(); break;
        case 2: _t->lastActionTriggered(); break;
        case 3: _t->nextActionTriggered(); break;
        case 4: _t->toLeftActionTriggered(); break;
        case 5: _t->toRightActionTriggered(); break;
        case 6: _t->toEnlargeActionTriggered(); break;
        case 7: _t->toLessenActionTriggered(); break;
        case 8: _t->deleteActionTriggered(); break;
        case 9: _t->aboutTriggered(); break;
        case 10: _t->aboutQtTriggered(); break;
        case 11: _t->lastActionTriggered1(); break;
        case 12: _t->nextActionTriggered1(); break;
        case 13: _t->openActionTriggered1(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
