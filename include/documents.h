#ifndef DOCUMENTS_H
#define DOCUMENTS_H

#include "vdocument.h"
#include <string>

class TxtDocument : public Document {
private:
    const std::string m_name;
    bool m_activeState;
    menus_t m_menus;
public:
    TxtDocument(const std::string &);
    ~TxtDocument() override;
    void init() override;
    void done() override;
    bool isActive() override;
    bool supportsUserType(const std::string &) const override;
    QWidget *newView(const std::string &) const override;
    const std::string & name() const override;
    const menus_t & menus() const override;
};

class SchDocument : public Document {
private:
    const std::string m_name;
    bool m_activeState;
    menus_t m_menus;
public:
    SchDocument(const std::string &);
    ~SchDocument() override;
    void init() override;
    void done() override;
    bool isActive() override;
    bool supportsUserType(const std::string &) const override;
    QWidget *newView(const std::string &) const override;
    const std::string & name() const override;
    const menus_t & menus() const override;
};

// Not sure what's going on with this one
// It's not an MDI type of thing, but then again,
// This is just a header of document types.
// Certainly a project is a document type, yes?
class PrjDocument : public Document {
private:
    const std::string m_name;
    bool m_activeState;
    menus_t m_menus;
public:
    PrjDocument(const std::string &);
    ~PrjDocument() override;
    void init() override;
    void done() override;
    bool isActive() override;
    bool supportsUserType(const std::string &) const override;
    QWidget *newView(const std::string &) const override;
    const std::string & name() const override;
    const menus_t & menus() const override;
};


#endif
