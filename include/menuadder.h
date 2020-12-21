#ifndef MENUADDER_H
#define MENUADDER_H


#include <QMenu>
#include <vector>
using menus_t = std::vector<QMenu *>;

// This interface is provided for documents
// or other thing that need to add a menu
// to the mainWindow.

class IMenuSource {
public:
    virtual ~IMenuSource() {}
    virtual const menus_t & menus() const = 0;
};


#endif // MENUADDER_H
