
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QTableView>
#include<QPushButton>
#include<QLabel>
#include<QComboBox>
#include<QCheckBox>
#include<QListView>

#include <QtCharts/QChartView>
#include "DataContainer.h"
#include "LineGraph.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected);
    void on_setFiles();
    void on_printGraph();






private:
    Ui::MainWindow *ui;

    QFileSystemModel *rightPartModel;
    QFileSystemModel *leftPartModel;
    QTreeView *treeView;
    QTableView *tableView;
    // кнопки
    QPushButton* m_setFiles = nullptr;
    QPushButton* m_printGraph = nullptr;

    //надписи
    QLabel* m_chartDiscription = nullptr;

    QComboBox* m_chartsType = nullptr;

    QCheckBox* m_blackWhite = nullptr;

    QtCharts::QChartView* chartView;
    DataContainer dataContainer;

    LineGraph m_lineGraph;
    QFileSystemModel* m_fileExplorer = nullptr;
    QListView* m_listView = nullptr;

};

#endif // MAINWINDOW_H

















