#ifndef USER_H
#define USER_H

class User
{
public:
    long           id;
    QString        username;
    QString        password;
    QString        name;
    QString        discriminator;

    User() : id(1) { ; }
    virtual ~User() { ; }
};

#endif // USER_H
