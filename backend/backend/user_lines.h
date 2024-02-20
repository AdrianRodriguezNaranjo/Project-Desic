#ifndef USER_LINES_H
#define USER_LINES_H

class UserLine
{
public:
    long userId;
    long lineId;

    UserLine() : userId(0), lineId(0) {}
    UserLine(long userId, long lineId) : userId(userId), lineId(lineId) {}
    virtual ~UserLine() {}
};

#endif // USER_LINES_H
