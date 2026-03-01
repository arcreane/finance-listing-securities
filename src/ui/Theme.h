#ifndef THEME_H
#define THEME_H

#include <QString>

class QApplication;

class Theme
{
public:
    static void apply(QApplication &app);
    static QString stylesheet();
};

#endif // THEME_H
