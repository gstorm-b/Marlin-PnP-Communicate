/****************************************************************************
** Meta object code from reading C++ file 'marlin_host.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Marlin-PnP/serial/marlin_host.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'marlin_host.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMarlin_HostENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSMarlin_HostENDCLASS = QtMocHelpers::stringData(
    "Marlin_Host",
    "MH_Signal_Connected",
    "",
    "MH_Signal_Disconnected",
    "MH_Signal_ConnectFailed",
    "msg",
    "MH_Signal_ErrorOccurred"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSMarlin_HostENDCLASS_t {
    uint offsetsAndSizes[14];
    char stringdata0[12];
    char stringdata1[20];
    char stringdata2[1];
    char stringdata3[23];
    char stringdata4[24];
    char stringdata5[4];
    char stringdata6[24];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSMarlin_HostENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSMarlin_HostENDCLASS_t qt_meta_stringdata_CLASSMarlin_HostENDCLASS = {
    {
        QT_MOC_LITERAL(0, 11),  // "Marlin_Host"
        QT_MOC_LITERAL(12, 19),  // "MH_Signal_Connected"
        QT_MOC_LITERAL(32, 0),  // ""
        QT_MOC_LITERAL(33, 22),  // "MH_Signal_Disconnected"
        QT_MOC_LITERAL(56, 23),  // "MH_Signal_ConnectFailed"
        QT_MOC_LITERAL(80, 3),  // "msg"
        QT_MOC_LITERAL(84, 23)   // "MH_Signal_ErrorOccurred"
    },
    "Marlin_Host",
    "MH_Signal_Connected",
    "",
    "MH_Signal_Disconnected",
    "MH_Signal_ConnectFailed",
    "msg",
    "MH_Signal_ErrorOccurred"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMarlin_HostENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   38,    2, 0x06,    1 /* Public */,
       3,    0,   39,    2, 0x06,    2 /* Public */,
       4,    1,   40,    2, 0x06,    3 /* Public */,
       6,    1,   43,    2, 0x06,    5 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,

       0        // eod
};

Q_CONSTINIT const QMetaObject Marlin_Host::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_CLASSMarlin_HostENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMarlin_HostENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMarlin_HostENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Marlin_Host, std::true_type>,
        // method 'MH_Signal_Connected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'MH_Signal_Disconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'MH_Signal_ConnectFailed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'MH_Signal_ErrorOccurred'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>
    >,
    nullptr
} };

void Marlin_Host::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Marlin_Host *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->MH_Signal_Connected(); break;
        case 1: _t->MH_Signal_Disconnected(); break;
        case 2: _t->MH_Signal_ConnectFailed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->MH_Signal_ErrorOccurred((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Marlin_Host::*)();
            if (_t _q_method = &Marlin_Host::MH_Signal_Connected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Marlin_Host::*)();
            if (_t _q_method = &Marlin_Host::MH_Signal_Disconnected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Marlin_Host::*)(QString );
            if (_t _q_method = &Marlin_Host::MH_Signal_ConnectFailed; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Marlin_Host::*)(QString );
            if (_t _q_method = &Marlin_Host::MH_Signal_ErrorOccurred; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject *Marlin_Host::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Marlin_Host::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMarlin_HostENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int Marlin_Host::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Marlin_Host::MH_Signal_Connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Marlin_Host::MH_Signal_Disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Marlin_Host::MH_Signal_ConnectFailed(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Marlin_Host::MH_Signal_ErrorOccurred(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
