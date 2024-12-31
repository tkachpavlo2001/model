/****************************************************************************
** Meta object code from reading C++ file 'chartwidgetfactory.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../acs_model_for_experiments/chartwidgetfactory.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chartwidgetfactory.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_iChartWidget_t {
    uint offsetsAndSizes[16];
    char stringdata0[13];
    char stringdata1[23];
    char stringdata2[1];
    char stringdata3[30];
    char stringdata4[21];
    char stringdata5[18];
    char stringdata6[21];
    char stringdata7[22];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_iChartWidget_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_iChartWidget_t qt_meta_stringdata_iChartWidget = {
    {
        QT_MOC_LITERAL(0, 12),  // "iChartWidget"
        QT_MOC_LITERAL(13, 22),  // "signal_to_update_chart"
        QT_MOC_LITERAL(36, 0),  // ""
        QT_MOC_LITERAL(37, 29),  // "signal_to_notify_run_finished"
        QT_MOC_LITERAL(67, 20),  // "slot_to_update_chart"
        QT_MOC_LITERAL(88, 17),  // "slot_to_run_model"
        QT_MOC_LITERAL(106, 20),  // "slot_to_update_model"
        QT_MOC_LITERAL(127, 21)   // "slot_to_relise_change"
    },
    "iChartWidget",
    "signal_to_update_chart",
    "",
    "signal_to_notify_run_finished",
    "slot_to_update_chart",
    "slot_to_run_model",
    "slot_to_update_model",
    "slot_to_relise_change"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_iChartWidget[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   50,    2, 0x06,    1 /* Public */,
       3,    0,   51,    2, 0x06,    2 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    0,   52,    2, 0x08,    3 /* Private */,
       5,    0,   53,    2, 0x0a,    4 /* Public */,
       6,    0,   54,    2, 0x0a,    5 /* Public */,
       7,    0,   55,    2, 0x0a,    6 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject iChartWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_iChartWidget.offsetsAndSizes,
    qt_meta_data_iChartWidget,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_iChartWidget_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<iChartWidget, std::true_type>,
        // method 'signal_to_update_chart'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'signal_to_notify_run_finished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'slot_to_update_chart'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'slot_to_run_model'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'slot_to_update_model'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'slot_to_relise_change'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void iChartWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<iChartWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->signal_to_update_chart(); break;
        case 1: _t->signal_to_notify_run_finished(); break;
        case 2: _t->slot_to_update_chart(); break;
        case 3: _t->slot_to_run_model(); break;
        case 4: _t->slot_to_update_model(); break;
        case 5: _t->slot_to_relise_change(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (iChartWidget::*)();
            if (_t _q_method = &iChartWidget::signal_to_update_chart; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (iChartWidget::*)();
            if (_t _q_method = &iChartWidget::signal_to_notify_run_finished; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
    (void)_a;
}

const QMetaObject *iChartWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *iChartWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_iChartWidget.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "iDataset_config_receiver"))
        return static_cast< iDataset_config_receiver*>(this);
    if (!strcmp(_clname, "iDataset_config_sender"))
        return static_cast< iDataset_config_sender*>(this);
    return QWidget::qt_metacast(_clname);
}

int iChartWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void iChartWidget::signal_to_update_chart()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void iChartWidget::signal_to_notify_run_finished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
namespace {
struct qt_meta_stringdata_iChartWidgetConfig_t {
    uint offsetsAndSizes[16];
    char stringdata0[19];
    char stringdata1[17];
    char stringdata2[1];
    char stringdata3[21];
    char stringdata4[20];
    char stringdata5[15];
    char stringdata6[11];
    char stringdata7[18];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_iChartWidgetConfig_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_iChartWidgetConfig_t qt_meta_stringdata_iChartWidgetConfig = {
    {
        QT_MOC_LITERAL(0, 18),  // "iChartWidgetConfig"
        QT_MOC_LITERAL(19, 16),  // "signal_run_model"
        QT_MOC_LITERAL(36, 0),  // ""
        QT_MOC_LITERAL(37, 20),  // "signal_model_updated"
        QT_MOC_LITERAL(58, 19),  // "signal_update_chart"
        QT_MOC_LITERAL(78, 14),  // "slot_run_model"
        QT_MOC_LITERAL(93, 10),  // "slot_apply"
        QT_MOC_LITERAL(104, 17)   // "slot_run_finished"
    },
    "iChartWidgetConfig",
    "signal_run_model",
    "",
    "signal_model_updated",
    "signal_update_chart",
    "slot_run_model",
    "slot_apply",
    "slot_run_finished"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_iChartWidgetConfig[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   50,    2, 0x06,    1 /* Public */,
       3,    0,   51,    2, 0x06,    2 /* Public */,
       4,    0,   52,    2, 0x06,    3 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,   53,    2, 0x08,    4 /* Private */,
       6,    0,   54,    2, 0x0a,    5 /* Public */,
       7,    0,   55,    2, 0x0a,    6 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject iChartWidgetConfig::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_iChartWidgetConfig.offsetsAndSizes,
    qt_meta_data_iChartWidgetConfig,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_iChartWidgetConfig_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<iChartWidgetConfig, std::true_type>,
        // method 'signal_run_model'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'signal_model_updated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'signal_update_chart'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'slot_run_model'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'slot_apply'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'slot_run_finished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void iChartWidgetConfig::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<iChartWidgetConfig *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->signal_run_model(); break;
        case 1: _t->signal_model_updated(); break;
        case 2: _t->signal_update_chart(); break;
        case 3: _t->slot_run_model(); break;
        case 4: _t->slot_apply(); break;
        case 5: _t->slot_run_finished(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (iChartWidgetConfig::*)();
            if (_t _q_method = &iChartWidgetConfig::signal_run_model; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (iChartWidgetConfig::*)();
            if (_t _q_method = &iChartWidgetConfig::signal_model_updated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (iChartWidgetConfig::*)();
            if (_t _q_method = &iChartWidgetConfig::signal_update_chart; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
    (void)_a;
}

const QMetaObject *iChartWidgetConfig::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *iChartWidgetConfig::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_iChartWidgetConfig.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "iDataset_config_sender"))
        return static_cast< iDataset_config_sender*>(this);
    return QWidget::qt_metacast(_clname);
}

int iChartWidgetConfig::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void iChartWidgetConfig::signal_run_model()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void iChartWidgetConfig::signal_model_updated()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void iChartWidgetConfig::signal_update_chart()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
