#include "DocumentSerialize.h"
#include "LineStyle.h"
#include "Logger.h"
#include <QTextStream>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "Xml.h"

const int DEFAULT_LINE_WIDTH = 1;

LineStyle::LineStyle () :
  m_width (0),
  m_paletteColor (COLOR_PALETTE_TRANSPARENT),
  m_curveConnectAs (CONNECT_AS_FUNCTION_SMOOTH)
{
}

LineStyle::LineStyle (unsigned int width,
                      ColorPalette paletteColor,
                      CurveConnectAs curveConnectAs) :
  m_width (width),
  m_paletteColor (paletteColor),
  m_curveConnectAs (curveConnectAs)
{
}

LineStyle::LineStyle (const LineStyle &other) :
  m_width (other.width ()),
  m_paletteColor (other.paletteColor()),
  m_curveConnectAs (other.curveConnectAs())
{
}

LineStyle &LineStyle::operator= (const LineStyle &other)
{
  m_width = other.width ();
  m_paletteColor = other.paletteColor();
  m_curveConnectAs = other.curveConnectAs();

  return *this;
}

CurveConnectAs LineStyle::curveConnectAs () const
{
  return m_curveConnectAs;
}

LineStyle LineStyle::defaultAxesCurve ()
{
  return LineStyle (DEFAULT_LINE_WIDTH,
                    COLOR_PALETTE_TRANSPARENT,
                    CONNECT_SKIP_FOR_AXIS_CURVE); // Same default color as used for PointStyle axes curve default
}

LineStyle LineStyle::defaultGraphCurve (int /* index */)
{
  return LineStyle (DEFAULT_LINE_WIDTH,
                    COLOR_PALETTE_BLUE,
                    CONNECT_AS_FUNCTION_SMOOTH); // Same default color as used for PointStyle graph curves default
}

void LineStyle::loadXml(QXmlStreamReader &reader)
{
  LOG4CPP_INFO_S ((*mainCat)) << "LineStyle::loadXml";

  QXmlStreamAttributes attributes = reader.attributes();

  if (attributes.hasAttribute(DOCUMENT_SERIALIZE_LINE_STYLE_WIDTH) &&
      attributes.hasAttribute(DOCUMENT_SERIALIZE_LINE_STYLE_COLOR) &&
      attributes.hasAttribute(DOCUMENT_SERIALIZE_LINE_STYLE_CONNECT_AS)) {

    setWidth (attributes.value(DOCUMENT_SERIALIZE_LINE_STYLE_WIDTH).toInt());
    setPaletteColor ((ColorPalette) attributes.value(DOCUMENT_SERIALIZE_LINE_STYLE_COLOR).toInt());
    setCurveConnectAs ((CurveConnectAs) attributes.value(DOCUMENT_SERIALIZE_LINE_STYLE_CONNECT_AS).toInt());

    // Read until end of this subtree
    while ((reader.tokenType() != QXmlStreamReader::EndElement) ||
    (reader.name() != DOCUMENT_SERIALIZE_LINE_STYLE)){
      loadNextFromReader(reader);
    }
  } else {
    reader.raiseError ("Cannot read line style data");
  }
}

ColorPalette LineStyle::paletteColor() const
{
  return m_paletteColor;
}

void LineStyle::printStream(QString indentation,
                            QTextStream &str) const
{
  str << indentation << "LineStyle\n";
  
  indentation += INDENTATION_DELTA;

  str << indentation << "width=" << m_width << "\n";
  str << indentation << "color=" << colorPaletteToString (m_paletteColor) << "\n";
  str << indentation << "curveConnectAs=" << curveConnectAsToString (m_curveConnectAs) << "\n";
}

void LineStyle::saveXml(QXmlStreamWriter &writer) const
{
  LOG4CPP_INFO_S ((*mainCat))  << "LineStyle::saveXml";

  writer.writeStartElement(DOCUMENT_SERIALIZE_LINE_STYLE);
  writer.writeAttribute (DOCUMENT_SERIALIZE_LINE_STYLE_WIDTH, QString::number(m_width));
  writer.writeAttribute (DOCUMENT_SERIALIZE_LINE_STYLE_COLOR, QString::number (m_paletteColor));
  writer.writeAttribute (DOCUMENT_SERIALIZE_LINE_STYLE_COLOR_STRING, colorPaletteToString (m_paletteColor));
  writer.writeAttribute (DOCUMENT_SERIALIZE_LINE_STYLE_CONNECT_AS, QString::number (m_curveConnectAs));
  writer.writeAttribute (DOCUMENT_SERIALIZE_LINE_STYLE_CONNECT_AS_STRING, curveConnectAsToString (m_curveConnectAs));
  writer.writeEndElement();
}

void LineStyle::setCurveConnectAs(CurveConnectAs curveConnectAs)
{
  m_curveConnectAs = curveConnectAs;
}

void LineStyle::setPaletteColor (ColorPalette paletteColor)
{
  m_paletteColor = paletteColor;
}

void LineStyle::setWidth (int width)
{
  m_width = width;
}

unsigned int LineStyle::width () const
{
  return m_width;
}
