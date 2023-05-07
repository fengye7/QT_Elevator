/****************************************************************************
** Meta object code from reading C++ file 'System.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../System.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'System.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_System_t {
    QByteArrayData data[19];
    char stringdata0[252];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_System_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_System_t qt_meta_stringdata_System = {
    {
QT_MOC_LITERAL(0, 0, 6), // "System"
QT_MOC_LITERAL(1, 7, 7), // "Request"
QT_MOC_LITERAL(2, 15, 0), // ""
QT_MOC_LITERAL(3, 16, 15), // "ElevatorRequest"
QT_MOC_LITERAL(4, 32, 7), // "request"
QT_MOC_LITERAL(5, 40, 13), // "updateDigital"
QT_MOC_LITERAL(6, 54, 9), // "Elevator*"
QT_MOC_LITERAL(7, 64, 3), // "ele"
QT_MOC_LITERAL(8, 68, 12), // "currentfloor"
QT_MOC_LITERAL(9, 81, 11), // "targetfloor"
QT_MOC_LITERAL(10, 93, 13), // "HandleRequest"
QT_MOC_LITERAL(11, 107, 13), // "EnterElevator"
QT_MOC_LITERAL(12, 121, 19), // "on_opendoor_clicked"
QT_MOC_LITERAL(13, 141, 20), // "on_closedoor_clicked"
QT_MOC_LITERAL(14, 162, 22), // "on_floorButton_clicked"
QT_MOC_LITERAL(15, 185, 24), // "on_warningButton_clicked"
QT_MOC_LITERAL(16, 210, 20), // "UpdateDigitalDisplay"
QT_MOC_LITERAL(17, 231, 7), // "Elemove"
QT_MOC_LITERAL(18, 239, 12) // "AfterReached"

    },
    "System\0Request\0\0ElevatorRequest\0request\0"
    "updateDigital\0Elevator*\0ele\0currentfloor\0"
    "targetfloor\0HandleRequest\0EnterElevator\0"
    "on_opendoor_clicked\0on_closedoor_clicked\0"
    "on_floorButton_clicked\0on_warningButton_clicked\0"
    "UpdateDigitalDisplay\0Elemove\0AfterReached"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_System[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       5,    3,   72,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    1,   79,    2, 0x0a /* Public */,
      11,    0,   82,    2, 0x0a /* Public */,
      12,    0,   83,    2, 0x0a /* Public */,
      13,    0,   84,    2, 0x0a /* Public */,
      14,    0,   85,    2, 0x0a /* Public */,
      15,    0,   86,    2, 0x0a /* Public */,
      16,    3,   87,    2, 0x0a /* Public */,
      17,    1,   94,    2, 0x0a /* Public */,
      18,    1,   97,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6, QMetaType::Int, QMetaType::Int,    7,    8,    9,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6, QMetaType::Int, QMetaType::Int,    7,    8,    9,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 6,    7,

       0        // eod
};

void System::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        System *_t = static_cast<System *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Request((*reinterpret_cast< const ElevatorRequest(*)>(_a[1]))); break;
        case 1: _t->updateDigital((*reinterpret_cast< Elevator*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: _t->HandleRequest((*reinterpret_cast< const ElevatorRequest(*)>(_a[1]))); break;
        case 3: _t->EnterElevator(); break;
        case 4: _t->on_opendoor_clicked(); break;
        case 5: _t->on_closedoor_clicked(); break;
        case 6: _t->on_floorButton_clicked(); break;
        case 7: _t->on_warningButton_clicked(); break;
        case 8: _t->UpdateDigitalDisplay((*reinterpret_cast< Elevator*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 9: _t->Elemove((*reinterpret_cast< Elevator*(*)>(_a[1]))); break;
        case 10: _t->AfterReached((*reinterpret_cast< Elevator*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Elevator* >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Elevator* >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Elevator* >(); break;
            }
            break;
        case 10:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Elevator* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (System::*_t)(const ElevatorRequest & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&System::Request)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (System::*_t)(Elevator * , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&System::updateDigital)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject System::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_System.data,
      qt_meta_data_System,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *System::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *System::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_System.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int System::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void System::Request(const ElevatorRequest & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void System::updateDigital(Elevator * _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
