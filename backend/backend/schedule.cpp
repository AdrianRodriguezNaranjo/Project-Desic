#include "precompiled.h"
#include "schedule.h"

#include <QxOrm_Impl.h>

// QX_REGISTER_CPP_QX_BUS(Line)

namespace qx {
template <> void register_class(QxClass<Schedule> & t)
{
    t.id(& Schedule::id, "schedule_id");
    t.data(& Schedule::line_id, "shcedule_line_id");
    t.data(& Schedule::time, "schedule_time");
}}
