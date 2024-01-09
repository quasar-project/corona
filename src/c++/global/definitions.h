//
// Created by user on 06.01.2024.
//

#pragma once

#define invokable Q_INVOKABLE
#define qt_slot Q_SLOT
#define ensure(expr, msg) if(!(expr)) { llerror(msg); return; }
#define ensure_or(expr, msg, retval) if(!(expr)) { llerror(msg); return retval; }
#define qt_registered_enum Q_ENUM
// bug: MOC doesn't work with redefined Q_OBJECT macro
// #define qt_registered_object Q_OBJECT
#define qt_property Q_PROPERTY
#define qt_register_class_in_metasystem Q_DECLARE_METATYPE