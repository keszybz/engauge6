#include "CmdAddPointAxis.h"
#include "Document.h"
#include "DocumentSerialize.h"
#include "EngaugeAssert.h"
#include "Logger.h"
#include "MainWindow.h"
#include "QtToString.h"
#include <QXmlStreamReader>

const QString CMD_DESCRIPTION ("Add axis point");

CmdAddPointAxis::CmdAddPointAxis (MainWindow &mainWindow,
                                  Document &document,
                                  const QPointF &posScreen,
                                  const QPointF &posGraph,
                                  double ordinal) :
  CmdAbstract (mainWindow,
               document,
               CMD_DESCRIPTION),
  m_posScreen (posScreen),
  m_posGraph (posGraph),
  m_ordinal (ordinal)
{
  LOG4CPP_INFO_S ((*mainCat)) << "CmdAddPointAxis::CmdAddPointAxis"
                              << " posScreen=" << QPointFToString (posScreen).toLatin1 ().data ()
                              << " posGraph=" << QPointFToString (posGraph).toLatin1 ().data ()
                              << " ordinal=" << ordinal;
}

CmdAddPointAxis::CmdAddPointAxis (MainWindow &mainWindow,
                                  Document &document,
                                  const QString &cmdDescription,
                                  QXmlStreamReader &reader) :
  CmdAbstract (mainWindow,
               document,
               cmdDescription)
{
  LOG4CPP_INFO_S ((*mainCat)) << "CmdAddPointAxis::CmdAddPointAxis";

  QXmlStreamAttributes attributes = reader.attributes();

  if (!attributes.hasAttribute(DOCUMENT_SERIALIZE_SCREEN_X) ||
      !attributes.hasAttribute(DOCUMENT_SERIALIZE_SCREEN_Y) ||
      !attributes.hasAttribute(DOCUMENT_SERIALIZE_GRAPH_X) ||
      !attributes.hasAttribute(DOCUMENT_SERIALIZE_GRAPH_Y) ||
      !attributes.hasAttribute(DOCUMENT_SERIALIZE_IDENTIFIER) ||
      !attributes.hasAttribute(DOCUMENT_SERIALIZE_ORDINAL)) {
      ENGAUGE_ASSERT (false);
  }

  m_posScreen.setX(attributes.value(DOCUMENT_SERIALIZE_SCREEN_X).toDouble());
  m_posScreen.setY(attributes.value(DOCUMENT_SERIALIZE_SCREEN_Y).toDouble());
  m_posGraph.setX(attributes.value(DOCUMENT_SERIALIZE_GRAPH_X).toDouble());
  m_posGraph.setY(attributes.value(DOCUMENT_SERIALIZE_GRAPH_Y).toDouble());
  m_identifierAdded = attributes.value(DOCUMENT_SERIALIZE_IDENTIFIER).toString();
  m_ordinal = attributes.value(DOCUMENT_SERIALIZE_ORDINAL).toDouble();
}

CmdAddPointAxis::~CmdAddPointAxis ()
{
}

void CmdAddPointAxis::cmdRedo ()
{
  LOG4CPP_INFO_S ((*mainCat)) << "CmdAddPointAxis::cmdRedo";

  document().addPointAxisWithGeneratedIdentifier (m_posScreen,
                                                  m_posGraph,
                                                  m_identifierAdded,
                                                  m_ordinal);
  document().updatePointOrdinals (mainWindow().transformation());
  mainWindow().updateAfterCommand();
}

void CmdAddPointAxis::cmdUndo ()
{
  LOG4CPP_INFO_S ((*mainCat)) << "CmdAddPointAxis::cmdUndo";

  document().removePointAxis (m_identifierAdded);
  document().updatePointOrdinals (mainWindow().transformation());
  mainWindow().updateAfterCommand();
}

void CmdAddPointAxis::saveXml (QXmlStreamWriter &writer) const
{
  writer.writeStartElement(DOCUMENT_SERIALIZE_CMD);
  writer.writeAttribute(DOCUMENT_SERIALIZE_CMD_TYPE, DOCUMENT_SERIALIZE_CMD_ADD_POINT_AXIS);
  writer.writeAttribute(DOCUMENT_SERIALIZE_CMD_DESCRIPTION, QUndoCommand::text ());
  writer.writeAttribute(DOCUMENT_SERIALIZE_SCREEN_X, QString::number (m_posScreen.x()));
  writer.writeAttribute(DOCUMENT_SERIALIZE_SCREEN_Y, QString::number (m_posScreen.y()));
  writer.writeAttribute(DOCUMENT_SERIALIZE_GRAPH_X, QString::number (m_posGraph.x()));
  writer.writeAttribute(DOCUMENT_SERIALIZE_GRAPH_Y, QString::number (m_posGraph.y()));
  writer.writeAttribute(DOCUMENT_SERIALIZE_IDENTIFIER, m_identifierAdded);
  writer.writeAttribute(DOCUMENT_SERIALIZE_ORDINAL, QString::number (m_ordinal));
  writer.writeEndElement();
}
