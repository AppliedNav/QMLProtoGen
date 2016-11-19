#include "protocoldocumentation.h"
#include "protocolparser.h"
#include <QDir>
#include <QFile>

//! The list of C language keywords, visible to all encodables
QStringList ProtocolDocumentation::keywords = QStringList() << "auto" << "double" << "int" << "struct" << "break" << "else" << "long" << "switch" << "case" << "enum" << "register" << "typedef" << "char" << "extern" << "return" << "union" << "const" << "float" << "short" << "unsigned" << "continue" << "for" << "signed" << "void" << "default" << "goto" << "sizeof" << "volatile" << "do" << "if" << "static" << "while";

//! Construct the document object, with details about the overall protocol
ProtocolDocumentation::ProtocolDocumentation(ProtocolParser* parse, QString Parent) :
    parser(parse),
    parent(Parent),
    outlineLevel(0)
{
}


//! Parse the document from the DOM
void ProtocolDocumentation::parse(void)
{
    // We have two features we care about in the documentation, "name" which
    // gives the paragraph, and "comment" which gives the documentation to add
    QDomNamedNodeMap map = e.attributes();

    name = ProtocolParser::getAttribute("name", map);
    comment = ProtocolParser::reflowComment(ProtocolParser::getAttribute("comment", map));
    docfile = ProtocolParser::getAttribute("file", map);
    QString outline = ProtocolParser::getAttribute("paragraph", map);

    // The outline level is a number
    if(!outline.isEmpty())
        outlineLevel = outline.toInt();
}


//! Return top level markdown documentation for this packet
QString ProtocolDocumentation::getTopLevelMarkdown(bool global, const QStringList& ids) const
{
    QString markdown;
    int level = outlineLevel;

    // Make sure the outline level is acceptable
    if((level <= 0) || (level > 10))
    {
        if(global)
            level = 2;
        else
            level = 3;
    }

    if(!name.isEmpty())
    {
        for(int i = 0; i < level; i++)
            markdown += "#";

        markdown += " " + name +"\n\n";
    }

    if(!comment.isEmpty())
        markdown += comment + "\n\n";

    if(!docfile.isEmpty())
    {
        QFile file(parser->getInputPath() + docfile);

        if(file.open(QFile::Text | QFile::ReadOnly))
        {
            markdown += QString(file.readAll()) + "\n\n";
            file.close();
        }

    }

    return markdown;
}


/*!
 * Output a warning to stdout. The warning will include the hierarchical name used to describe this objects location in the xml
 * \param warning is the warning text
 */
void ProtocolDocumentation::emitWarning(QString warning) const
{
    QString name = getHierarchicalName();
    int line = parser->getLineNumber(name);
    parser->emitWarning(QString::number(line) + ":0: warning: " + name + ": " + warning);
}


/*!
 * Helper function to create a list of ProtocolDocumentation objects based upon the DOM
 * \param parse points to the global protocol parser that owns everything
 * \param Parent is the name of the parent object that owns the created objects
 * \param e is the DOM element which may have documentation children
 * \param list receives the list of allocated objects.
 */
void ProtocolDocumentation::getChildDocuments(ProtocolParser* parse, QString Parent, const QDomElement& e, QList<ProtocolDocumentation*>& list)
{
    // The list of documentation that goes inside this packet
    QList<QDomNode> documents = ProtocolParser::childElementsByTagName(e, "Documentation");

    // Parse all the document objects
    for(int i = 0; i < documents.count(); i++)
    {
        // Create the document and parse its xml
        ProtocolDocumentation* doc = new ProtocolDocumentation(parse, Parent);

        doc->setElement(documents.at(i).toElement());
        doc->parse();

        // Keep it around
        list.push_back(doc);
    }

}// ProtocolDocumentation::getChildDocuments