#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphfactory.h"
#include "igraphs.h"
#include"idatareader.h"
#include"readerfactory.h"
#include <QMainWindow>
#include <QFileSystemModel>
#include <QListView>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void FillComboBox();

private slots:
    void on_openFolder();
    void on_fileSelected(const QModelIndex &index);
    void on_printGraph();
    void on_blackWhiteToggled(bool checked);

private:
    Ui::MainWindow *ui;

    QFileSystemModel *m_fileExplorer;
    QListView *m_listView;
    QtCharts::QChartView *graphView;
    QPushButton *m_openFolder;
    QPushButton *m_printGraph;
    QCheckBox *m_blackWhite;
    QComboBox *m_graphsType;
    QLabel *m_GraphDescription;
    DataContainer m_data;

    // Для хранения данных графика
    QVector<QPointF> m_dataPoints;


    std::shared_ptr<ReaderFactory> m_readerFactory;
    std::shared_ptr<GraphFactory> m_graphFactory;
};

#endif // MAINWINDOW_H
