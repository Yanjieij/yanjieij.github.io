/****************************************************************************
** Meta object code from reading C++ file 'MCES.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../MCES.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MCES.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MCES_t {
    QByteArrayData data[16];
    char stringdata0[385];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MCES_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MCES_t qt_meta_stringdata_MCES = {
    {
QT_MOC_LITERAL(0, 0, 4), // "MCES"
QT_MOC_LITERAL(1, 5, 38), // "on_policySelection_currentInd..."
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 5), // "index"
QT_MOC_LITERAL(4, 51, 37), // "on_modelSelection_currentInde..."
QT_MOC_LITERAL(5, 89, 30), // "on_startSimulateButton_clicked"
QT_MOC_LITERAL(6, 120, 30), // "on_pauseSimulateButton_clicked"
QT_MOC_LITERAL(7, 151, 24), // "update_simulate_progress"
QT_MOC_LITERAL(8, 176, 23), // "updat_left_time_counter"
QT_MOC_LITERAL(9, 200, 26), // "simulate_passenger_request"
QT_MOC_LITERAL(10, 227, 23), // "activate_elevators_move"
QT_MOC_LITERAL(11, 251, 22), // "refresh_simulate_graph"
QT_MOC_LITERAL(12, 274, 26), // "on_elevatorNum_textChanged"
QT_MOC_LITERAL(13, 301, 24), // "on_maxHeight_textChanged"
QT_MOC_LITERAL(14, 326, 26), // "on_cabinVolume_textChanged"
QT_MOC_LITERAL(15, 353, 31) // "on_simulateDuration_textChanged"

    },
    "MCES\0on_policySelection_currentIndexChanged\0"
    "\0index\0on_modelSelection_currentIndexChanged\0"
    "on_startSimulateButton_clicked\0"
    "on_pauseSimulateButton_clicked\0"
    "update_simulate_progress\0"
    "updat_left_time_counter\0"
    "simulate_passenger_request\0"
    "activate_elevators_move\0refresh_simulate_graph\0"
    "on_elevatorNum_textChanged\0"
    "on_maxHeight_textChanged\0"
    "on_cabinVolume_textChanged\0"
    "on_simulateDuration_textChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MCES[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x0a /* Public */,
       4,    1,   82,    2, 0x0a /* Public */,
       5,    0,   85,    2, 0x0a /* Public */,
       6,    0,   86,    2, 0x0a /* Public */,
       7,    0,   87,    2, 0x0a /* Public */,
       8,    0,   88,    2, 0x0a /* Public */,
       9,    0,   89,    2, 0x0a /* Public */,
      10,    0,   90,    2, 0x0a /* Public */,
      11,    0,   91,    2, 0x0a /* Public */,
      12,    0,   92,    2, 0x0a /* Public */,
      13,    0,   93,    2, 0x0a /* Public */,
      14,    0,   94,    2, 0x0a /* Public */,
      15,    0,   95,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Int,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MCES::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MCES *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_policySelection_currentIndexChanged((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 1: _t->on_modelSelection_currentIndexChanged((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 2: _t->on_startSimulateButton_clicked(); break;
        case 3: _t->on_pauseSimulateButton_clicked(); break;
        case 4: _t->update_simulate_progress(); break;
        case 5: _t->updat_left_time_counter(); break;
        case 6: { int _r = _t->simulate_passenger_request();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 7: _t->activate_elevators_move(); break;
        case 8: _t->refresh_simulate_graph(); break;
        case 9: _t->on_elevatorNum_textChanged(); break;
        case 10: _t->on_maxHeight_textChanged(); break;
        case 11: _t->on_cabinVolume_textChanged(); break;
        case 12: _t->on_simulateDuration_textChanged(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MCES::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MCES.data,
    qt_meta_data_MCES,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MCES::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MCES::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MCES.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MCES::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
