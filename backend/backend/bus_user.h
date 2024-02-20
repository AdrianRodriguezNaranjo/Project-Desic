#ifndef BUS_USER_H
#define BUS_USER_H

class Bus_User
{
public:
    long           id;
    QString        username;
    QString        password;
    QString        name;
    QString        discriminator;

    Bus_User() : id(1) { ; }
    virtual ~Bus_User() { ; }
};

#endif // BUS_USER_H
