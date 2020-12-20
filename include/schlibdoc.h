#ifndef SCHLIBDOC_H
#define SCHLIBDOC_H

#include "vdocument.h"
#include "menuadder.h"
#include <string>

class SchLibDocument : public IDocument, public IMenuAdder {
private:
    const std::string m_name;
    bool m_activeState;
    menus_t m_menus;
public:
    SchLibDocument(const std::string &);
    ~SchLibDocument() override;
    void init() override;
    void done() override;
    bool isActive() override;
    bool supportsUserType(const std::string &) const override;
    QWidget *newView(const std::string &) const override;
    const std::string & name() const override;
    const menus_t & menus() const override;
};


#endif
