#ifndef __SUBJECT_H__
#define __SUBJECT_H__
#include "observer.h"

class Subject {
    Observer* ob;
public:
    void notifyObserver();
    void attach(Observer* _ob);
};

void Subject::notifyObserver() {
    ob->notify(*this);
}

void Subject::attach(Observer* _ob) {
    ob = _ob;
}

#endif
