//
// Created by user on 06.01.2024.
//

#pragma once

#define invokable Q_INVOKABLE
#define qt_slot Q_SLOT
#define ensure(expr, msg) if(!(expr)) { llerror(msg); return; }
#define ensure_or(expr, msg, retval) if(!(expr)) { llerror(msg); return retval; }