#include <QFileDialog>

#include "AstMainWindow.h"
#include "ui_AstMainWindow.h"

#include "AstMapVisualization.h"

CMainWindow::CMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CMainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButtonOpenFile, &QPushButton::clicked, this, &CMainWindow::OpenFileSlot);

}

CMainWindow::~CMainWindow()
{
    delete ui;
}


void CMainWindow::OpenFileSlot(void)
{
//    QString FileName = QString::fromLocal8Bit("D:/MyDocuments/Проекты/Марин с мониторингом/Карты от Волгобалта/1V620101.000");
    QString FileName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("Открыть картографический файл"), "D:/MyDocuments", "*.0??");

    if (FileName.size() == 0)
        return;

    QFile File(FileName);
    if (!File.open(QIODevice::ReadOnly))
    {
        ui->labelFileName->setText(QString::fromLocal8Bit("Ошибка открытия файла!"));
        return;
    }

    ui->labelFileName->setText(FileName);

    QByteArray FileData = File.readAll();

    File.close();

    m_S57Parser.clear();
    m_S57Parser.SetDataRecordsList(FileData);

    m_sConsoleBuffer.clear();
    m_sConsoleBuffer.append(m_S57Parser.GetComment() + "\n"
                            "COMF = " + QString::number(m_S57Parser.GetCOMF()) + "\n"
                            "SOMF = " + QString::number(m_S57Parser.GetSOMF()) + "\n\n");
    m_sConsoleBuffer.append("Map Area {Xmin, Ymin}-{Xmax, Ymax} = {" +
                            QString::number(m_S57Parser.GetMinYPosition()) + "; " +
                            QString::number(m_S57Parser.GetMinXPosition()) + "} - {" +
                            QString::number(m_S57Parser.GetMaxYPosition()) + "; " +
                            QString::number(m_S57Parser.GetMaxXPosition()) + "}\n\n");

    int iRecordNumber = 0;
    QMap<unsigned long, CDRrecord> DRrecords = m_S57Parser.GetRecordsList();

    foreach (CDRrecord record, DRrecords)
    {
        m_sConsoleBuffer.append("Out datablock number " + QString::number(iRecordNumber) + ":\n");
        m_sConsoleBuffer.append("Record ID = " + QString::number(record.GetID()) + "\n");
        m_sConsoleBuffer.append("Len = " + QString::number(record.GetLen()) + "\n");
        m_sConsoleBuffer.append("Data offset = " + QString::number(record.GetDataOffset()) + "\n");
        m_sConsoleBuffer.append("Tag Field Size = " + QString::number(record.GetTagFieldSize()) + "\n");
        m_sConsoleBuffer.append("Len Field Size = " + QString::number(record.GetLenFieldSize()) + "\n");
        m_sConsoleBuffer.append("Offset Field Size = " + QString::number(record.GetOffsetFieldSize()) + "\n");

        QList<CDRfield> FieldsList = record.GetFieldsList();

        m_sConsoleBuffer.append("Fields Number = " + QString::number(FieldsList.size()) + "\n");

        foreach (CDRfield field, FieldsList)
        {
            m_sConsoleBuffer.append("      " + field.GetTag() + ": Offset = " + QString::number(field.GetOffset()) + "\tLen = " + QString::number(field.GetLen()) + "\t");
            m_sConsoleBuffer.append(field.GetRAWdata().toHex(' ') + "\n");
        }

        m_sConsoleBuffer.append("\n");

        iRecordNumber++;
    }

    int iObjectNumber = 0;
    QMap<unsigned long, CAstS57Object> *pObjects = m_S57Parser.GetObjectsList();

    foreach (CAstS57Object object, *pObjects)
    {
        m_sConsoleBuffer.append("-----------------------------------------------------------------------------------------\n");
        m_sConsoleBuffer.append("Out object record number " + QString::number(iObjectNumber) + ":\n");
        m_sConsoleBuffer.append("Object ID = " + QString::number(object.GetID()) + "\n");
        m_sConsoleBuffer.append("Object Class = " + QString::number(object.GetObjectClass()) + "\n");

        QList<unsigned long> *pVectorIDsList = object.GetRelatedObjectIDList();
        m_sConsoleBuffer.append("Vectors Number = " + QString::number(pVectorIDsList->size()) + "\n");

        foreach (unsigned long vecID, *pVectorIDsList)
        {
            CAstS57Vector *pVector = m_S57Parser.GetVector(vecID);
            QList<CNode> NodesList = pVector->GetNodesList();

            m_sConsoleBuffer.append("Vector ID = " + QString::number(vecID) + "\n");
            m_sConsoleBuffer.append("Nodes Number = " + QString::number(NodesList.size()) + "\n");

            foreach (CNode node, NodesList)
            {
                m_sConsoleBuffer.append("Latitude = " + QString::number(node.GetYPosition()) +
                                        "\tLongitude  = " + QString::number(node.GetXPosition()) +
                                        "\tAltitude = " + QString::number(node.GetVPosition()) +
                                        "\tType = " + QString::number(node.GetType()) + "\n");
            }

            m_sConsoleBuffer.append("\n");
        }

        iObjectNumber++;
    }

    ui->textEditOutConsole->setText(m_sConsoleBuffer);

    CAstMapVisualization *MapWindow = new CAstMapVisualization(&m_S57Parser, nullptr);
    MapWindow->show();

}
