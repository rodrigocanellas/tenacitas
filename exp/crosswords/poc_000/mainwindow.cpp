//#include "mainwindow.h"
//#include "ui_mainwindow.h"

//using namespace tenacitas;

//#include <tenacitas.lib/src//log.h>



//MainWindow::MainWindow(QWidget *parent)
//    : QMainWindow(parent)
//    , ui(new Ui::MainWindow)
//{
//    ui->setupUi(this);

//    connect(
//        ui->rbbVertical  , &QRadioButton::clicked,
//        this, &MainWindow::on_rbbVertical_clicked
//        );

//    connect(
//        ui->rdbHorizontal, &QRadioButton::clicked,
//        this, &MainWindow::on_rdbHorizontal_clicked
//        );

//    connect(
//        ui->tblGrid, &QTableWidget::cellActivated,
//        this, &MainWindow::on_tblGrid_cellActivated
//        );

//    connect(
//        ui->tblGrid, &QTableWidget::cellEntered,
//        this, &MainWindow::on_tblGrid_cellActivated
//        );

//    connect(
//        ui->tblGrid, &QTableWidget::cellClicked,
//        this, &MainWindow::on_tblGrid_cellActivated
//        );
//}

//MainWindow::~MainWindow()
//{
//    delete ui;
//}

//void MainWindow::on_tblGrid_cellActivated(int row, int column)
//{

//    m_start_row = m_current_row = row;
//    m_start_col = m_current_col = column;

//    ui->lblRowValue->setText(QString::number(m_start_row+1));
//    ui->lblColumnValue->setText(QString::number(m_start_col+1));
//}

//void MainWindow::on_rbbVertical_clicked()
//{
//    m_current_orientation=crosswords::mod::orientation::vert;
//}

//void MainWindow::on_rdbHorizontal_clicked()
//{
//    m_current_orientation=crosswords::mod::orientation::hori;
//}

//void MainWindow::keyPressEvent(QKeyEvent *event)
//{
//    if (m_current_orientation == crosswords::mod::orientation::undef){
//        return;
//    }

//    if (m_current_col > ui->tblGrid->columnCount()) {
//        return;
//    }

//    if (m_current_row > ui->tblGrid->rowCount()) {
//        return;
//    }

//    auto _key{event->key()};

//    if ( (_key >= Qt::Key_A) && (_key <= Qt::Key_Z)) {
//        QChar _char{_key};

//        QTableWidgetItem *_cell = ui->tblGrid->item(m_current_row, m_current_col);
//        if(!_cell)
//        {
//            _cell = new QTableWidgetItem;
//            ui->tblGrid->setItem(m_current_row, m_current_col,_cell);
//        }


//        _cell->setText(_char);

//        if (m_current_orientation == crosswords::mod::orientation::vert) {
//            ++m_current_row;
//        }
//        else {
//            ++m_current_col;
//        }
//    }




//}
