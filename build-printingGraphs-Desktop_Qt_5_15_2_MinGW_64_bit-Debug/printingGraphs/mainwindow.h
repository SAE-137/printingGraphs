#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QFileSystemModel>
#include <QListView>
#include <QtCharts>
#include <memory>
#include <QModelIndex>

#include "GraphFactory.h"
#include "ReaderFactory.h"
#include "graphfactory.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(std::shared_ptr<GraphFactory> chart, std::shared_ptr<ReaderFactory> reader, QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void handleFolderSelection();
    void loadDataFromSelection(const QModelIndex& ix);
    void updateGraphView();
    void toggleMonochromeMode(bool checked);
    void exportChartToPDF();
protected:
    void resizeEvent(QResizeEvent* event) override;
private:
    QComboBox* initChartTypeSelector() const;
    void enableSaveButtonIfNeeded();
    void activateBWToggleIfDataReady();
    void deactivateSaveButton();
    void deactivateBWToggle();
    void resetGraphView();
    void applyMonochromeStyle();
private:
    QComboBox* m_comboBoxCharts = nullptr;
    QPushButton* m_pushButtonSave = nullptr;
    QPushButton* m_pushButtonFolder = nullptr;
    QCheckBox* m_checkBoxBlackAndWhite = nullptr;
    QFileSystemModel* m_fileExplorer = nullptr;
    QListView* m_listView = nullptr;
    QtCharts::QChartView* m_chartView = nullptr;
    std::shared_ptr<ReaderFactory> m_readerFactory;
    std::shared_ptr<GraphFactory> m_chartFactory;
    DataContainer m_currentData;
};
#endif // MAINWINDOW_H
