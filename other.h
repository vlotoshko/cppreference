#ifndef OTHER_H
#define OTHER_H

void testOther();

class Base
{
public:
    Base() = default;
    Base(const Base&) = default;

    virtual void show() const;
    virtual ~Base();

    int b;
};

class Derived : public Base
{
public:
    Derived() : d(0), ch('a') {}
    int d;
    void show() const;
    void newFunc();
    char ch;
};

#endif // OTHER_H
