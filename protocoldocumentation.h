#ifndef PROTOCOLDOCUMENTATION_H
#define PROTOCOLDOCUMENTATION_H

#include <QDomElement>
#include <QString>
#include <QStringList>
#include "protocolsupport.h"

class ProtocolParser;

class ProtocolDocumentation
{
public:

    //! Construct the document object
    ProtocolDocumentation(ProtocolParser* parse, QString Parent, ProtocolSupport supported);

    //! Virtual destructor
    virtual ~ProtocolDocumentation(void) {;}

    //! Set the element from the DOM
    virtual void setElement(QDomElement element) {e = element;}

    //! Get the element
    QDomElement& getElement(void) {return e;}

    //! Parse the document from the DOM
    virtual void parse(void);

    //! Return top level markdown documentation for this packet
    virtual QString getTopLevelMarkdown(bool global = false, const QStringList& ids = QStringList()) const;

    //! Documentation is by definition never hidden from the documentation
    virtual bool isHidden(void) const {return false;}

    //! The hierarchical name of this object
    virtual QString getHierarchicalName(void) const {return parent + ":" + name;}

    //! Check names against the list of C keywords
    virtual void checkAgainstKeywords(void){}

    //! Output a warning
    void emitWarning(QString warning, const QString& subname = QString()) const;

    //! Test the list of attributes and warn if any of them are unrecognized or repeated
    void testAndWarnAttributes(const QDomNamedNodeMap& map, const QStringList& attriblist = QStringList(), const QString& subname = QString()) const;

    //! Helper function to create a list of ProtocolDocumentation objects
    static void getChildDocuments(ProtocolParser* parse, QString Parent, ProtocolSupport support, const QDomElement& e, QList<ProtocolDocumentation*>& list);

	//! Get header file containing the structure definition exposed to QML
	QString lookUpQtPropertyIncludeName(const QString &structName) const;
public:

    //! String used to tab code in (perhaps one day we'll make this user changeable)
    static const QString TAB_IN;

    QString name;           //!< The name of this encodable
    QString title;          //!< The title of this encodable (used for documentation)
    QString comment;        //!< The comment that goes with this encodable

	QString label;			//!< The label of this encodable
	bool uiEnabled = false;		//!< When set this encodable generates UI code (provided that UI support is enabled from command line)
    bool propsEnabled = false;  //!< Generate classes that expose C++ properties in QML without generating QML code
	bool isArrayItem = false;	//!< Enabled when this structure is an array item. Used when generating QML components.

protected:
    ProtocolSupport support;//!< Information about what is supported
    ProtocolParser* parser = nullptr; //!< The parser object
    QString parent;         //!< The parent name of this encodable
    QDomElement e;          //!< The DOM element which is the source of this object's data

    static QStringList keywords;//!< keywords for the C language

    static QStringList variablenames;//!< variables used by protogen

private:
    int outlineLevel;       //!< The paragraph outline level
    QString docfile;        //!< File for external documentation
};

#endif // PROTOCOLDOCUMENTATION_H
