/****************************************************************************
** Meta object code from reading C++ file 'tcpkernel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../0116diskclient/kernel/tcpkernel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcpkernel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TCPKernel_t {
    QByteArrayData data[18];
    char stringdata0[301];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TCPKernel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TCPKernel_t qt_meta_stringdata_TCPKernel = {
    {
QT_MOC_LITERAL(0, 0, 9), // "TCPKernel"
QT_MOC_LITERAL(1, 10, 15), // "signal_register"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 17), // "STRU_REGISTER_RS*"
QT_MOC_LITERAL(4, 45, 12), // "signal_login"
QT_MOC_LITERAL(5, 58, 14), // "STRU_LOGIN_RS*"
QT_MOC_LITERAL(6, 73, 18), // "signal_getfilelist"
QT_MOC_LITERAL(7, 92, 20), // "STRU_GETFILELIST_RS*"
QT_MOC_LITERAL(8, 113, 21), // "signal_uploadfileinfo"
QT_MOC_LITERAL(9, 135, 23), // "STRU_UPLOADFILEINFO_RS*"
QT_MOC_LITERAL(10, 159, 16), // "signal_sharelink"
QT_MOC_LITERAL(11, 176, 19), // "STRU_SHARELLINK_RS*"
QT_MOC_LITERAL(12, 196, 12), // "signal_break"
QT_MOC_LITERAL(13, 209, 14), // "STRU_BREAK_RS*"
QT_MOC_LITERAL(14, 224, 17), // "signal_searchfile"
QT_MOC_LITERAL(15, 242, 19), // "STRU_SEARCHFILE_RS*"
QT_MOC_LITERAL(16, 262, 16), // "signal_downlowad"
QT_MOC_LITERAL(17, 279, 21) // "STRU_DOWNLOADFILE_RS*"

    },
    "TCPKernel\0signal_register\0\0STRU_REGISTER_RS*\0"
    "signal_login\0STRU_LOGIN_RS*\0"
    "signal_getfilelist\0STRU_GETFILELIST_RS*\0"
    "signal_uploadfileinfo\0STRU_UPLOADFILEINFO_RS*\0"
    "signal_sharelink\0STRU_SHARELLINK_RS*\0"
    "signal_break\0STRU_BREAK_RS*\0"
    "signal_searchfile\0STRU_SEARCHFILE_RS*\0"
    "signal_downlowad\0STRU_DOWNLOADFILE_RS*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TCPKernel[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       4,    1,   57,    2, 0x06 /* Public */,
       6,    1,   60,    2, 0x06 /* Public */,
       8,    1,   63,    2, 0x06 /* Public */,
      10,    1,   66,    2, 0x06 /* Public */,
      12,    1,   69,    2, 0x06 /* Public */,
      14,    1,   72,    2, 0x06 /* Public */,
      16,    1,   75,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void, 0x80000000 | 7,    2,
    QMetaType::Void, 0x80000000 | 9,    2,
    QMetaType::Void, 0x80000000 | 11,    2,
    QMetaType::Void, 0x80000000 | 13,    2,
    QMetaType::Void, 0x80000000 | 15,    2,
    QMetaType::Void, 0x80000000 | 17,    2,

       0        // eod
};

void TCPKernel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TCPKernel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signal_register((*reinterpret_cast< STRU_REGISTER_RS*(*)>(_a[1]))); break;
        case 1: _t->signal_login((*reinterpret_cast< STRU_LOGIN_RS*(*)>(_a[1]))); break;
        case 2: _t->signal_getfilelist((*reinterpret_cast< STRU_GETFILELIST_RS*(*)>(_a[1]))); break;
        case 3: _t->signal_uploadfileinfo((*reinterpret_cast< STRU_UPLOADFILEINFO_RS*(*)>(_a[1]))); break;
        case 4: _t->signal_sharelink((*reinterpret_cast< STRU_SHARELLINK_RS*(*)>(_a[1]))); break;
        case 5: _t->signal_break((*reinterpret_cast< STRU_BREAK_RS*(*)>(_a[1]))); break;
        case 6: _t->signal_searchfile((*reinterpret_cast< STRU_SEARCHFILE_RS*(*)>(_a[1]))); break;
        case 7: _t->signal_downlowad((*reinterpret_cast< STRU_DOWNLOADFILE_RS*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TCPKernel::*)(STRU_REGISTER_RS * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TCPKernel::signal_register)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TCPKernel::*)(STRU_LOGIN_RS * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TCPKernel::signal_login)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TCPKernel::*)(STRU_GETFILELIST_RS * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TCPKernel::signal_getfilelist)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TCPKernel::*)(STRU_UPLOADFILEINFO_RS * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TCPKernel::signal_uploadfileinfo)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (TCPKernel::*)(STRU_SHARELLINK_RS * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TCPKernel::signal_sharelink)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (TCPKernel::*)(STRU_BREAK_RS * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TCPKernel::signal_break)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (TCPKernel::*)(STRU_SEARCHFILE_RS * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TCPKernel::signal_searchfile)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (TCPKernel::*)(STRU_DOWNLOADFILE_RS * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TCPKernel::signal_downlowad)) {
                *result = 7;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TCPKernel::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_TCPKernel.data,
    qt_meta_data_TCPKernel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TCPKernel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TCPKernel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TCPKernel.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "IKernel"))
        return static_cast< IKernel*>(this);
    return QObject::qt_metacast(_clname);
}

int TCPKernel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void TCPKernel::signal_register(STRU_REGISTER_RS * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TCPKernel::signal_login(STRU_LOGIN_RS * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TCPKernel::signal_getfilelist(STRU_GETFILELIST_RS * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void TCPKernel::signal_uploadfileinfo(STRU_UPLOADFILEINFO_RS * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void TCPKernel::signal_sharelink(STRU_SHARELLINK_RS * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void TCPKernel::signal_break(STRU_BREAK_RS * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void TCPKernel::signal_searchfile(STRU_SEARCHFILE_RS * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void TCPKernel::signal_downlowad(STRU_DOWNLOADFILE_RS * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
