#include "QGridLayout"

#include "AstMapVisualization.h"
#include "ui_AstMapVisualization.h"

CAstMapVisualization::CAstMapVisualization(CAstS57Parser *pMapParser, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CAstMapVisualization)
{
    ui->setupUi(this);

    // Инициализация виджета карты
    QGridLayout * grid = new QGridLayout();

    m_pMapWidget = new CAstMapWidget(pMapParser, this);
    grid->addWidget(m_pMapWidget);

    setLayout(grid);
}

CAstMapVisualization::~CAstMapVisualization()
{
    delete ui;
}


void CAstMapVisualization::ViewMousePosSlot(QPoint pos)
{
    ui->label_X->setText(QString::number(pos.x()));
    ui->label_Y->setText(QString::number(pos.y()));
}
