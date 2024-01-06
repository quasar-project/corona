//
// Created by user on 06.01.2024.
//

#pragma once

#include <cstdint>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using usize = size_t;
using isize = int;
using f32 = float;
using f64 = double;

#define incapsulate_namespace(prev, curr, ns) class prev; namespace ns { using curr = prev; }
#define qt_incapsulate_class(new_name) incapsulate_namespace(Q##new_name, new_name, qt)

qt_incapsulate_class(Object)
qt_incapsulate_class(String)

qt_incapsulate_class(CoreApplication)
qt_incapsulate_class(GuiApplication)
qt_incapsulate_class(Application)
qt_incapsulate_class(Url)
qt_incapsulate_class(Icon)
qt_incapsulate_class(QmlEngine)
qt_incapsulate_class(QmlComponent)
qt_incapsulate_class(QuickWindow)
qt_incapsulate_class(QuickStyle)
