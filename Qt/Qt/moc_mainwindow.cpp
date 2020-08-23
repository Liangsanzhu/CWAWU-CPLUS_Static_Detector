/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[23];
    char stringdata0[325];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 23), // "on_pushButton_2_clicked"
QT_MOC_LITERAL(4, 58, 18), // "stringToHtmlFilter"
QT_MOC_LITERAL(5, 77, 8), // "QString&"
QT_MOC_LITERAL(6, 86, 3), // "str"
QT_MOC_LITERAL(7, 90, 24), // "slot_tabBarDoubleClicked"
QT_MOC_LITERAL(8, 115, 12), // "slot_tabDrag"
QT_MOC_LITERAL(9, 128, 5), // "index"
QT_MOC_LITERAL(10, 134, 5), // "point"
QT_MOC_LITERAL(11, 140, 13), // "slot_closeTab"
QT_MOC_LITERAL(12, 154, 25), // "on_action_alter_triggered"
QT_MOC_LITERAL(13, 180, 25), // "on_action_about_triggered"
QT_MOC_LITERAL(14, 206, 19), // "txtbrowser_click_t1"
QT_MOC_LITERAL(15, 226, 12), // "QMouseEvent*"
QT_MOC_LITERAL(16, 239, 5), // "event"
QT_MOC_LITERAL(17, 245, 19), // "txtbrowser_click_t2"
QT_MOC_LITERAL(18, 265, 24), // "on_action_code_triggered"
QT_MOC_LITERAL(19, 290, 9), // "msecSleep"
QT_MOC_LITERAL(20, 300, 4), // "msec"
QT_MOC_LITERAL(21, 305, 10), // "build_llvm"
QT_MOC_LITERAL(22, 316, 8) // "run_test"

    },
    "MainWindow\0on_pushButton_clicked\0\0"
    "on_pushButton_2_clicked\0stringToHtmlFilter\0"
    "QString&\0str\0slot_tabBarDoubleClicked\0"
    "slot_tabDrag\0index\0point\0slot_closeTab\0"
    "on_action_alter_triggered\0"
    "on_action_about_triggered\0txtbrowser_click_t1\0"
    "QMouseEvent*\0event\0txtbrowser_click_t2\0"
    "on_action_code_triggered\0msecSleep\0"
    "msec\0build_llvm\0run_test"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x08 /* Private */,
       3,    0,   85,    2, 0x08 /* Private */,
       4,    1,   86,    2, 0x08 /* Private */,
       7,    0,   89,    2, 0x08 /* Private */,
       8,    2,   90,    2, 0x08 /* Private */,
      11,    1,   95,    2, 0x08 /* Private */,
      12,    0,   98,    2, 0x08 /* Private */,
      13,    0,   99,    2, 0x08 /* Private */,
      14,    1,  100,    2, 0x08 /* Private */,
      17,    1,  103,    2, 0x08 /* Private */,
      18,    0,  106,    2, 0x08 /* Private */,
      19,    1,  107,    2, 0x08 /* Private */,
      21,    0,  110,    2, 0x08 /* Private */,
      22,    0,  111,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QPoint,    9,   10,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 15,   16,
    QMetaType::Void, 0x80000000 | 15,   16,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   20,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_pushButton_clicked(); break;
        case 1: _t->on_pushButton_2_clicked(); break;
        case 2: _t->stringToHtmlFilter((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->slot_tabBarDoubleClicked(); break;
        case 4: _t->slot_tabDrag((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QPoint(*)>(_a[2]))); break;
        case 5: _t->slot_closeTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->on_action_alter_triggered(); break;
        case 7: _t->on_action_about_triggered(); break;
        case 8: _t->txtbrowser_click_t1((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 9: _t->txtbrowser_click_t2((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 10: _t->on_action_code_triggered(); break;
        case 11: _t->msecSleep((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->build_llvm(); break;
        case 13: _t->run_test(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


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
