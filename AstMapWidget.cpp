#include <QWheelEvent>
#include <QtMath>

#include "AstMapWidget.h"
#include "AstMapVisualization.h"

#include "AstS57Format.h"
#include "AstS57ObjectClasses.h"

CAstMapWidget::CAstMapWidget(void)
{    
    m_pMapParser = nullptr;
    m_pParent = nullptr;

    m_dXorigin = 0;
    m_dYorigin = 0;
    m_bMapInit = false;
    m_dXAxisScale=MAX_AXIS_SCALE / 2.0;
    m_dYAxisScale=MAX_AXIS_SCALE / 2.0;
    m_dYAxisScaleCorrection = 1.0;
}

CAstMapWidget::CAstMapWidget( CAstS57Parser *pMapParser, QWidget *parent)
{
    m_pMapParser = pMapParser;
    m_pParent = parent;

    m_dXorigin = 0;
    m_dYorigin = 0;
    m_bMapInit = false;
    m_dXAxisScale = MAX_AXIS_SCALE / 2.0;
    m_dYAxisScale = MAX_AXIS_SCALE / 2.0;
    m_dYAxisScaleCorrection = 1.0;

    if (m_pParent != nullptr)
    {
        connect(this, &CAstMapWidget::ViewMousePosSignal, (CAstMapVisualization *)m_pParent, &CAstMapVisualization::ViewMousePosSlot);
    }

    setMouseTracking(true);
}

void CAstMapWidget::MapAreaInit(void)
{
    if (m_pMapParser == nullptr) return;

    if (m_bMapInit == false)
    {
        //  оэффициент, который учитывает уменьшение длины земной параллели при увеличении широты места
        m_dYAxisScaleCorrection = qCos(((m_pMapParser->GetMinYPosition() / 360.0) * 2.0 * M_PI) / m_pMapParser->GetCOMF());

        double kMap = (double)(m_pMapParser->GetMaxYPosition() - m_pMapParser->GetMinYPosition()) / m_dYAxisScaleCorrection / (double)(m_pMapParser->GetMaxXPosition() - m_pMapParser->GetMinXPosition());
        double kWidget = (double) height() / (double) width();

        if (kMap < kWidget)
        { // ¬писываем карту в виджет по оси X
            m_dXAxisScale = (m_pMapParser->GetMaxXPosition() - m_pMapParser->GetMinXPosition()) / width();
            m_dYAxisScale = m_dXAxisScale * m_dYAxisScaleCorrection;

            m_dXorigin = m_pMapParser->GetMinXPosition();
            m_dYorigin = m_pMapParser->GetMinYPosition() - (height() * m_dYAxisScale - (m_pMapParser->GetMaxYPosition() - m_pMapParser->GetMinYPosition())) / 2;
        }
        else
        { // ¬писываем карту в виджет по оси Y
            m_dYAxisScale = (m_pMapParser->GetMaxYPosition() - m_pMapParser->GetMinYPosition()) / height();
            m_dXAxisScale = m_dYAxisScale / m_dYAxisScaleCorrection;

            m_dYorigin = m_pMapParser->GetMinYPosition();
            m_dXorigin = m_pMapParser->GetMinXPosition() - (width() * m_dXAxisScale - (m_pMapParser->GetMaxXPosition() - m_pMapParser->GetMinXPosition())) / 2;
        }
    }

    m_bMapInit = true;
}

void CAstMapWidget::ScaleChange(double dScaleCoeff)
{
    double dPrevXAxisScale = m_dXAxisScale;
    double dPrevYAxisScale = m_dYAxisScale;

    m_dXAxisScale *= dScaleCoeff;
    m_dYAxisScale *= dScaleCoeff;

    if (dScaleCoeff < 1.0)
    {
        if (m_dXAxisScale < MIN_AXIS_SCALE)                           { m_dXAxisScale = MIN_AXIS_SCALE; }
        if (m_dYAxisScale < MIN_AXIS_SCALE * m_dYAxisScaleCorrection) { m_dYAxisScale = MIN_AXIS_SCALE * m_dYAxisScaleCorrection; }
    }
    else
    {
        if (m_dXAxisScale > MAX_AXIS_SCALE)                           { m_dXAxisScale = MAX_AXIS_SCALE; }
        if (m_dYAxisScale > MAX_AXIS_SCALE * m_dYAxisScaleCorrection) { m_dYAxisScale = MAX_AXIS_SCALE * m_dYAxisScaleCorrection; }
    }

    m_dXorigin += m_CursorPosition.x() * (dPrevXAxisScale - m_dXAxisScale);
    m_dYorigin += m_CursorPosition.y() * (dPrevYAxisScale - m_dYAxisScale);

    CoordinateLimitsControl();
}

void CAstMapWidget::CoordinateLimitsControl(void)
{
    if ((m_pMapParser->GetMaxXPosition() - m_pMapParser->GetMinXPosition()) >= width() * m_dXAxisScale)
    { // ≈сли размер выводимой картинки по оси X больше видимой области окна, то запрет вывода неохваченной картой области по оси X
        if (m_dXorigin < m_pMapParser->GetMinXPosition()) {
            m_dXorigin = m_pMapParser->GetMinXPosition();
        }

        if (m_dXorigin + width() * m_dXAxisScale > m_pMapParser->GetMaxXPosition()) {
            m_dXorigin = m_pMapParser->GetMaxXPosition() - width() * m_dXAxisScale;
        }
    }
    else
    { // ≈сли размер выводимой картинки по оси X меньше видимой области окна, то запрет выхода карты за пределы видимой области окна по оси X
        if (m_dXorigin > m_pMapParser->GetMinXPosition()) {
            m_dXorigin = m_pMapParser->GetMinXPosition();
        }

        if (m_dXorigin + width() * m_dXAxisScale < m_pMapParser->GetMaxXPosition()) {
            m_dXorigin = m_pMapParser->GetMaxXPosition() - width() * m_dXAxisScale;
        }
    }

    if ((m_pMapParser->GetMaxYPosition() - m_pMapParser->GetMinYPosition()) >= height() * m_dYAxisScale)
    { // ≈сли размер выводимой картинки по оси Y больше видимой области окна, то запрет вывода неохваченной картой области по оси Y
        if (m_dYorigin < m_pMapParser->GetMinYPosition()) {
            m_dYorigin = m_pMapParser->GetMinYPosition();
        }

        if (m_dYorigin + height() * m_dYAxisScale > m_pMapParser->GetMaxYPosition()) {
            m_dYorigin = m_pMapParser->GetMaxYPosition() - height() * m_dYAxisScale;
        }
    }
    else
    { // ≈сли размер выводимой картинки по оси Y меньше видимой области окна, то запрет выхода карты за пределы видимой области окна по оси Y
        if (m_dYorigin > m_pMapParser->GetMinYPosition()) {
            m_dYorigin = m_pMapParser->GetMinYPosition();
        }

        if (m_dYorigin + height() * m_dYAxisScale < m_pMapParser->GetMaxYPosition()) {
            m_dYorigin = m_pMapParser->GetMaxYPosition() - height() * m_dYAxisScale;
        }
    }
}

void CAstMapWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    { // ≈сли нажата лева€ кнопка мышки, то зафиксировать позицию курсора дл€ реализации перетаскивани€ карты по области окна
        m_LButtonOriginCursorPosition = event->pos();
        m_LButtonOriginCursorPosition.setY(height() - m_LButtonOriginCursorPosition.y());   // ѕереход из системы координат мышки на виджете в систему координат QPainter

        setCursor(Qt::ClosedHandCursor);
    }
}

void CAstMapWidget::mouseReleaseEvent(QMouseEvent *event)
{
    unsetCursor();
}

void CAstMapWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    { // Ѕыл даблклик левой кнопки мышки. ”величение карты
        m_LButtonOriginCursorPosition = event->pos();
        m_LButtonOriginCursorPosition.setY(height() - m_LButtonOriginCursorPosition.y());   // ѕереход из системы координат мышки на виджете в систему координат QPainter

        ScaleChange(1.0 / (SCALE_CHANGE_MULTIPLIER * DOUBLECLICK_FACTOR));

        if (m_pParent != nullptr)
            m_pParent->update();
    }
}

void CAstMapWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_pMapParser == nullptr) return;

    m_CursorPosition = event->pos();
    m_CursorPosition.setY(height() - m_CursorPosition.y());   // ѕереход из системы координат мышки на виджете в систему координат QPainter

    ViewMousePosSignal(m_CursorPosition);

    if (event->buttons() == Qt::LeftButton)
    { // ≈сли нажата лева€ кнопка мышки, то реализовать перетаскивание карты по области окна
        m_dXorigin += (m_LButtonOriginCursorPosition.x() - m_CursorPosition.x()) * m_dXAxisScale;
        m_dYorigin += (m_LButtonOriginCursorPosition.y() - m_CursorPosition.y()) * m_dYAxisScale;

        CoordinateLimitsControl();

        m_LButtonOriginCursorPosition = m_CursorPosition;

        if (m_pParent != nullptr)
            m_pParent->update();
    }
    else
    {
        unsetCursor();
    }
}

void CAstMapWidget::wheelEvent(QWheelEvent *event)
{
   QPoint numDegrees = event->angleDelta();

   if (!numDegrees.isNull())
   {
       double dScaleCoeff;
       if (numDegrees.y() > 0) dScaleCoeff = 1.0 / SCALE_CHANGE_MULTIPLIER;
       else                    dScaleCoeff = SCALE_CHANGE_MULTIPLIER;

       ScaleChange(dScaleCoeff);
   }

   event->accept();

   if (m_pParent != nullptr)
       m_pParent->update();

}

void CAstMapWidget::paintEvent(QPaintEvent *event)
{
    if (m_pMapParser == nullptr) return;

    MapAreaInit();

    m_pPainter = new QPainter(this);
    m_pPainter->setRenderHint(QPainter::Antialiasing);
    QPen Pen(Qt::black, Qt::SolidLine);
    Pen.setWidth(1);
    m_pPainter->setPen(Pen);

    QMap<unsigned long, CAstS57Object> *pObjects = m_pMapParser->GetObjectsList();

    // –исуем рамку карты
    m_pPainter->drawRect(0, 0, width(), height() - 1);

    // –исуем границу карты
    long Xmin = (m_pMapParser->GetMinXPosition() - m_dXorigin) / m_dXAxisScale;
    long Ymin = (m_pMapParser->GetMinYPosition() - m_dYorigin) / m_dYAxisScale;
    long Xmax = (m_pMapParser->GetMaxXPosition() - m_dXorigin) / m_dXAxisScale;
    long Ymax = (m_pMapParser->GetMaxYPosition() - m_dYorigin) / m_dYAxisScale;

    m_pPainter->drawRect(Xmin, height() - Ymin, Xmax - Xmin, -(Ymax - Ymin) - 1);

    foreach(CAstS57Object object, *pObjects)
    {
        switch (object.GetObjectClass())
        {
        case COALNE:              // Coastline
        case FNCLNE:              // Fence/wall
        case SLCONS:              // Shoreline Construction
        case BUISGL:              // Building, single
        case LNDRGN:              // Land region
        case M_QUAL:              // Quality of data
        {
            if (object.GetObjectPrimitiveType() == POINT_PTYPE) break;

            QList<unsigned long> *pVectorIDsList = object.GetRelatedObjectIDList();
            foreach (unsigned long vecID, *pVectorIDsList)
            {
                CAstS57Vector *pVector = m_pMapParser->GetVector(vecID);
                QList<CNode> NodesList = pVector->GetNodesList();

//                bool prevNode = false;
                long lCurXPosition = 0,  lCurYPosition = 0;
//                long lPrevXPosition = 0, lPrevYPosition = 0;


                long lPrevXPosition = m_pMapParser->GetVector(pVector->GetBegVectorID())->GetNodesList()[0].GetXPosition();
                long lPrevYPosition = m_pMapParser->GetVector(pVector->GetBegVectorID())->GetNodesList()[0].GetYPosition();


                foreach(CNode node, NodesList)
                {
                    lCurXPosition = node.GetXPosition();
                    lCurYPosition = node.GetYPosition();
                    if (/*prevNode == true &&*/ node.GetVPosition() == 0)
                    {
                        Line((lPrevXPosition - m_dXorigin) / m_dXAxisScale, (lPrevYPosition - m_dYorigin) / m_dYAxisScale, (lCurXPosition - m_dXorigin) / m_dXAxisScale, (lCurYPosition - m_dYorigin) / m_dYAxisScale);
                    }
                    lPrevXPosition = lCurXPosition;
                    lPrevYPosition = lCurYPosition;

//                    prevNode = true;

                }

                lCurXPosition = m_pMapParser->GetVector(pVector->GetEndVectorID())->GetNodesList()[0].GetXPosition();
                lCurYPosition = m_pMapParser->GetVector(pVector->GetEndVectorID())->GetNodesList()[0].GetYPosition();

                Line((lPrevXPosition - m_dXorigin) / m_dXAxisScale, (lPrevYPosition - m_dYorigin) / m_dYAxisScale, (lCurXPosition - m_dXorigin) / m_dXAxisScale, (lCurYPosition - m_dYorigin) / m_dYAxisScale);

            }
            break;
        }

        case BOYCAR:              // Buoy, cardinal
        case BOYINB:              // Buoy, installation
        case BOYISD:              // Buoy, isolated danger
        case BOYLAT:              // Buoy, lateral
        case BOYSAW:              // Buoy, safe water
        case BOYSPP:              // Buoy, special purpose/general
        {
            QList<unsigned long> *pVectorIDsList = object.GetRelatedObjectIDList();
            foreach (unsigned long vecID, *pVectorIDsList)
            {
                CAstS57Vector *pVector = m_pMapParser->GetVector(vecID);
                QList<CNode> NodesList = pVector->GetNodesList();

                if (NodesList.size() == 0) continue;

                CNode node = NodesList.at(0);
                m_pPainter->drawEllipse((node.GetXPosition() - m_dXorigin) / m_dXAxisScale, height() - 1 - (node.GetYPosition() - m_dYorigin) / m_dYAxisScale, 10, 10);
            }
            break;
        }

        case BCNCAR:              // Beacon, cardinal
        case BCNISD:              // Beacon, isolated danger
        case BCNLAT:              // Beacon, lateral
        case BCNSAW:              // Beacon, safe water
        case BCNSPP:              // Beacon, special purpose/general
        {
            QList<unsigned long> *pVectorIDsList = object.GetRelatedObjectIDList();
            foreach (unsigned long vecID, *pVectorIDsList)
            {
                CAstS57Vector *pVector = m_pMapParser->GetVector(vecID);
                QList<CNode> NodesList = pVector->GetNodesList();

                if (NodesList.size() == 0) continue;

                CNode node = NodesList.at(0);
                m_pPainter->drawRect((node.GetXPosition() - m_dXorigin) / m_dXAxisScale, height() - 1 - (node.GetYPosition() - m_dYorigin) / m_dYAxisScale, 10, 10);
            }
            break;
        }
        }
    }


/*    QPainterPath path;
    path.moveTo(200, 800);
    path.lineTo(200, 300);
    path.cubicTo(800, 0, 500, 500, 600, 100);

//    QPainter painter(this);
    m_pPainter->drawPath(path);

    QRadialGradient gradient(250, 250, 1000, 500, 500);
       gradient.setColorAt(0, QColor::fromRgbF(1, 1, 0, 1));
       gradient.setColorAt(1, QColor::fromRgbF(0, 0, 1, 0.5));

       QBrush brush(gradient);

       m_pPainter->fillPath(path, brush);
*/

//    m_pPainter->setPen(QPen(Qt::red, Qt::SolidLine));

    m_pPainter->end();
}

/*
 painter.drawLine(0,0,width(),height());                //нарисовать линию через рабочую область формы
 painter.setBrush(QBrush(Qt::green,Qt::SolidPattern));  //создать и установить кисть - зелЄна€ слошна€ заливка
 QPoint center(width()/2,height()/2);
 int rad = qMin(width()/4,height()/4);
 painter.drawEllipse(center,rad,rad);                   //нарисовать окружность по центру
 painter.setFont(QFont("sans-serif",-1,10));            //установить шрифт заданного начертани€ и размера 10 пт
 QRect rect(center.x()-rad,center.y()-rad,rad*5,rad*5);
 QString sss=QString::fromLocal8Bit(" оличество импульсов:\n") + QString::number(ucImpulseNumber);
 painter.drawText(rect, Qt::AlignCenter, sss);          //вывели строку текста, выравненную по центру
*/

void CAstMapWidget::Line(int from_x, int from_y, int to_x, int to_y)
{
    m_pPainter->drawLine(from_x, height() - from_y - 1, to_x, height() - to_y - 1);
}

void CAstMapWidget::Text(int x, int y, QString text)
{
    m_pPainter->drawText(x, height() - y - 1, text);
}

void CAstMapWidget::Polygon(QPolygon polygon)
{
//    m_pPainter->draw
}
