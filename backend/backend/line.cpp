#include "precompiled.h"
#include "line.h"

#include <QxOrm_Impl.h>

// QX_REGISTER_CPP_QX_BUS(Line)

namespace qx {
template <> void register_class(QxClass<Line> & t)
{
    t.id(& Line::id, "line_id");
    t.data(& Line::number, "line_number");
    t.data(& Line::firstbusstop, "line_firstbusstop");
    t.data(& Line::lastbusstop, "line_lastbusstop");
}}
