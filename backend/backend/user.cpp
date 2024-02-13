#include "precompiled.h"
#include "user.h"

#include <QxOrm_Impl.h>

// QX_REGISTER_CPP_QX_BUS(Line)

namespace qx {
template <> void register_class(QxClass<User> & t)
{
    t.id(& User::id, "schedule_id");
    t.data(& User::username, "user_username");
    t.data(& User::password, "user_password");
    t.data(& User::name, "user_name");
    t.data(& User::discriminator, "user_discriminator");
}}
