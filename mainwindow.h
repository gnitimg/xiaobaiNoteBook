#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QAction>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_triggered();     // 新建
    void on_action_2_triggered();   // 打开
    void on_action_3_triggered();   // 保存
    void on_action_4_triggered();   // 另存为
    void on_action_5_triggered();   // 退出
    void on_action_6_triggered();   // 回退
    void on_action_7_triggered();   // 取消回退

    // 右键菜单动作
    void cutText();
    void copyText();
    void pasteText();
    void selectAllText();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    QString currentFilePath;
    bool isModified = false;

    // 右键菜单动作
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *selectAllAction;

    void setupContextMenu();
    void loadFile(const QString &filePath);
    void saveFile(const QString &filePath);
    void updateWindowTitle();
    QString getFileFilter();
};

#endif // MAINWINDOW_H
