#ifndef PROTOCOLPARSER_H
#define PROTOCOLPARSER_H

#include <QDomDocument>
#include <QFile>
#include <QList>
#include <QHash>
#include <iostream>
#include "protocolfile.h"
#include "xmllinelocator.h"
#include "protocolsupport.h"

// Forward declarations
class ProtocolDocumentation;
class ProtocolStructure;
class ProtocolStructureModule;
class ProtocolPacket;
class EnumCreator;

class ProtocolParser
{
public:
    ProtocolParser();
    ~ProtocolParser();

    //! Configure a documentation path separate to the main protocol output directory
    void setDocsPath(QString path);

    //! Set LaTeX support
    void setLaTeXSupport(bool on) {latexEnabled = on;}

    //! Set LaTeX header level
    void setLaTeXLevel(int level) {latexHeader = level;}

    //! Option to disable markdown output
    void disableMarkdown(bool disable) {nomarkdown = disable;}

    //! Option to disable helper file output
    void disableHelperFiles(bool disable) {nohelperfiles = disable;}

    //! Option to disable doxygen output
    void disableDoxygen(bool disable) {nodoxygen = disable;}

    //! Option to disable 'about this ICD' section
    void disableAboutSection(bool disable) { noAboutSection = disable; }

    //! Option to enable table of contents section
    void enableTableOfContents(bool enable) {tableOfContents = enable;}

    //! Option to enable title page section
    void setTitlePage(QString pagetext) {titlePage = pagetext;}

    //! Return status of 'About this ICD' section
    bool hasAboutSection() const { return !noAboutSection; }

    //! Option to force documentation for hidden items
    void showHiddenItems(bool show) {showAllItems = show;}

    //! Option to disable unrecognized warnings
    void disableUnrecognizedWarnings(bool disable) {support.disableunrecognized = disable;}

    //! Set the inlinee css
    void setInlineCSS(QString css) {inlinecss = css;}

    //! Disable CSS entirely
    void disableCSS(bool disable) { nocss = disable; }

    //! Parse the DOM from the xml file(s). This kicks off the auto code generation for the protocol
    bool parse(QString filename, QString path, QStringList otherfiles);

    //! Output a warning referencing the main input path and file
    void emitWarning(QString warning) const;

    //! Output a warning referencing an object by name
    void emitWarning(QString hierarchicalName, QString warning) const;

    //! Return a list of QDomNodes that are direct children and have a specific tag
    static QList<QDomNode> childElementsByTagName(const QDomNode& node, QString tag, QString tag2 = QString(), QString tag3 = QString());

    //! Return the value of an attribute from a Dom Element
    static QString getAttribute(QString name, const QDomNamedNodeMap& map, QString defaultIfNone = QString());

    //! Output a long string of text which should be wrapped at 80 characters.
    static void outputLongComment(ProtocolFile& file, const QString& prefix, const QString& comment);

    //! Parse all enumerations which are direct children of a DomNode
    void parseEnumerations(QString parent, const QDomNode& node);

    //! Parse all enumerations which are direct children of a DomNode
    const EnumCreator* parseEnumeration(QString parent, const QDomElement& element);

    //! Output all includes which are direct children of a DomNode
    void outputIncludes(QString parent, ProtocolFile& file, const QDomNode& node) const;

    //! Format a long string of text which should be wrapped at 80 characters.
    static QString outputLongComment(const QString& prefix, const QString& text);

    //! Get a correctly reflowed comment from a DOM
    static QString getComment(const QDomElement& e);

    //! Take a comment line and reflow it for our needs.
    static QString reflowComment(QString comment, QString prefix = QString(), int charlimit = 0);

    //! Find the include name for a specific type
    QString lookUpIncludeName(const QString& typeName) const;

	//! Find the include name for a specific type used to expose properties in QML
	QString lookUpQtPropertyIncludeName(const QString& typeName) const;

    //! Find the enumeration creator for this enum
    const EnumCreator* lookUpEnumeration(const QString& enumName) const;

    //! Replace any text that matches an enumeration name with the value of that enumeration
    QString replaceEnumerationNameWithValue(const QString& text) const;

    //! Determine if text is part of an enumeration.
    QString getEnumerationNameForEnumValue(const QString& text) const;

    //! Find the enumeration with this name and return its comment, or an empty string
    QString getEnumerationValueComment(const QString& name) const;

	//! Find the enumeration with this name and return its actual value or -1
	int getEnumerationNumberForEnumValue(const QString& text) const;

    //! Find the global structure point for a specific type
    const ProtocolStructureModule* lookUpStructure(const QString& typeName) const;

    //! Get the documentation details for a specific global structure type
    void getStructureSubDocumentationDetails(QString typeName, QList<int>& outline, QString& startByte, QStringList& bytes, QStringList& names, QStringList& encodings, QStringList& repeats, QStringList& comments) const;

    //! The version of the protocol generator software
    static const QString genVersion;

    //! Get the string used for inline css.
    static QString getDefaultInlinCSS(void);

    //! Return the path of the xml source file
    QString getInputPath(void) {return inputpath;}

    //! Return true if the element has a particular attribute set to {'true','yes','1'}
    static bool isFieldSet(const QDomElement &e, QString label);

    //! Return true if the value set to {'true','yes','1'}
    static bool isFieldSet(QString value);

    //! Return true if the value of an attribute is 'true', 'yes', or '1'
    static bool isFieldSet(QString attribname, QDomNamedNodeMap map);

    //! Return true if the element has a particular attribute set to {'false','no','0'}
    static bool isFieldClear(const QDomElement &e, QString label);

    //! Return true if the value is set to {'false','no','0'}
    static bool isFieldClear(QString value);

    //! Determine if the value of an attribute is either {'false','no','0'}
    static bool isFieldClear(QString attribname, QDomNamedNodeMap map);

    //! Set the license text
    void setLicenseText(const QString text) { support.licenseText = text; }

    QString getLicenseText() const { return support.licenseText; }

    //! Set QML support
    void setUiSupport(bool on) {uiEnabled = on;}

    //! Return status of QML support
    bool hasUiSupport() const { return uiEnabled; }

    //! Get Qt controller object name used in QML files
    QString getQtControllerObjectName(void) const;

protected:

    //! Parses a single XML file handling any require tags to flatten a file
    bool parseFile(QString xmlFilename);

    //! Create markdown documentation
    void outputMarkdown(bool isBigEndian, QString inlinecss);

    //! Get the table of contents, based on the file contents
    QString getTableOfContents(const QString& filecontents);

    //! Get the "About this ICD" section to file
    QString getAboutSection(bool isBigEndian);

    //! Output the doxygen HTML documentation
    void outputDoxygen(void);

    //! Protocol support information
    ProtocolSupport support;

    ProtocolHeaderFile header;   //!< The header file (*.h)
    QString name;   //!< Base name of the protocol
    QString title;  //!< Title name of the protocol used in documentation
    QString comment;//!< Comment description of the protocol
    QString version;//!< The version string
    QString api;    //!< The protocol API enumeration

    QString docsDir;    //!< Directory target for storing documentation markdown

    int latexHeader;    //!< Top heading level for LaTeX output
    bool latexEnabled;  //!< Generate LaTeX markdown automagically
    bool nomarkdown;    //!< Disable markdown output
    bool nohelperfiles; //!< Disable helper file output
    bool nodoxygen;     //!< Disable doxygen output
    bool noAboutSection;//!< Disable extra 'about' section in the generated documentation
    bool showAllItems;  //!< Generate documentation even for elements with 'hidden="true"'
    QString inlinecss;  //!< CSS used for markdown output
    bool nocss;         //!< Disable all CSS output
    bool tableOfContents;//!< Enable table of contents
    QString titlePage;  //!< Title page information
    bool uiEnabled;    //!< Generate QML related files automagically

    QStringList filesparsed;
    QList<XMLLineLocator*>lines;
    QList<ProtocolDocumentation*> alldocumentsinorder;
    QList<ProtocolDocumentation*> documents;
    QList<ProtocolStructureModule*> structures;
    QList<ProtocolPacket*> packets;
    QList<EnumCreator*> enums;
    QList<EnumCreator*> globalEnums;
    QString inputpath;
    QString inputfile;
	QHash<QString, QString> struct2propsHeader;

    ProtocolSourceFile controllerSource;//!< The source file for controller class that allow to access global structures in QML
    ProtocolHeaderFile controllerHeader;//!< The header file for controller class that allow to access global structures in QML
    ProtocolSourceFile propviewsource;//!< The QML source file for the properties view

private:

    //! Get Qt controller class name
    QString getQtControllerClassName(void) const;

    //! Get the declaration that goes in the header which declares the controller class to access its properties in QML
    QString getQtControllerClassDeclaration(void) const;

    //! Get the definition that goes in the source file which defines the controller class to access its properties in QML
    QString getQtControllerClassDefinition(void) const;

    //! Get the text at the beginning of the QML file that allows to view properties
    static QString getQmlFileBegin(void);

    //! Get the text at the end of the QML file that allows to view properties
    static QString getQmlFileEnd(void);

    //! Create the header file for the top level module of the protocol
    void createProtocolHeader(const QDomElement& docElem);

    //! Create the source file for the controller class that allows to access global structures in QML
    void createControllerSource(void);

    //! Create the header file for the controller class that allows to access global structures in QML
    void createControllerHeader(void);

    //! Finish the protocol header file
    void finishProtocolHeader(void);

	//! Preprocess packets for UI generation
	void preprocessPackets();

	//! Search and set uiEnabled flag for struct with name in argument
    ProtocolStructureModule* setPropsEnabledForStruct(const QString &curStructName, bool isArrayItem);

	//! Search structs contained in current struct and apply setUiEnabledForStruct
	void searchStruct(ProtocolStructureModule* curStruct);
};

#endif // PROTOCOLPARSER_H
