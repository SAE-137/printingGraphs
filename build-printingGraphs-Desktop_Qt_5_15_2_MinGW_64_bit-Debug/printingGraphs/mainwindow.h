#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphfactory.h"
#include "readerfactory.h"
#include <QMainWindow>
#include <QFileSystemModel>
#include <QListView>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QtCharts/QChartView>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(std::shared_ptr<GraphFactory> graphFactory,
               std::shared_ptr<ReaderFactory> readerFactory,
               QWidget* parent = nullptr);
    ~MainWindow();

    void PopulateGraphSelector();

private slots:
    void handleFolderOpen();
    void handleFilePick(const QModelIndex& index);
    void renderGraph();
    void toggleMonochrome(bool checked);

private:
    void setupInterface();
    void connectSignals();

    Ui::MainWindow* ui;

    QFileSystemModel* fileModel;
    QListView* fileList;
    QtCharts::QChartView* chartDisplay;
    QPushButton* openDirBtn;
    QPushButton* drawGraphBtn;
    QCheckBox* monoMode;
    QComboBox* graphSelector;
    QLabel* graphLabel;
    DataContainer currentData;

    std::shared_ptr<ReaderFactory> readerFactory;
    std::shared_ptr<GraphFactory> graphFactory;
};

#endif // MAINWINDOW_H
