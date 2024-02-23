#include "precompiled.h"
#include "user_lines.h"

#include <QxOrm_Impl.h>

// QX_REGISTER_CPP_QX_BUS(Line)

namespace qx {
template <> void register_class(QxClass<UserLine> & t)
{
    t.id(& UserLine::userId, "userId");
    t.data(& UserLine::lineId, "lineId");
}}
