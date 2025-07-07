#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFileInfo>
#include <QSize>
#include <QMenu>
#include <QDesktopServices> // 添加桌面服务头文件
#include <QUrl> // 添加URL头文件

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置窗口最小尺寸
    setMinimumSize(600, 400);

    updateWindowTitle();

    connect(ui->textEdit, &QTextEdit::textChanged, [this] {
        isModified = true;
        updateWindowTitle();
    });

    // 初始化
    isModified = false;
    currentFilePath = "";

    // 设置右键菜单
    setupContextMenu();

    // 连接关于菜单项
    connect(ui->action_8, &QAction::triggered, this, &MainWindow::openAuthorPage);
    connect(ui->action_9, &QAction::triggered, this, &MainWindow::openGitHubRepo);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 打开作者页面
void MainWindow::openAuthorPage()
{
    QDesktopServices::openUrl(QUrl("https://gnitimg.ac.cn"));
}

// 打开GitHub仓库
void MainWindow::openGitHubRepo()
{
    QDesktopServices::openUrl(QUrl("https://github.com/gnitimg/xiaobaiNoteBook"));
}

// 右键菜单
void MainWindow::setupContextMenu()
{
    ui->textEdit->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->textEdit, &QTextEdit::customContextMenuRequested, [this](const QPoint &pos) {

        QMenu *menu = ui->textEdit->createStandardContextMenu();

        menu->exec(ui->textEdit->mapToGlobal(pos));
        delete menu;
    });
}

void MainWindow::cutText()// 剪切
{
    ui->textEdit->cut();
}

void MainWindow::copyText()// 复制
{
    ui->textEdit->copy();
}

void MainWindow::pasteText()// 粘贴
{
    ui->textEdit->paste();
}

void MainWindow::selectAllText()// 全选
{
    ui->textEdit->selectAll();
}

QString MainWindow::getFileFilter()
{
    return "文本文件 (*.txt);;"
           "Markdown 文件 (*.md);;"
           "HTML 文件 (*.html *.htm);;"
           "CSS 文件 (*.css);;"
           "JavaScript 文件 (*.js);;"
           "C 文件 (*.c);;"
           "C++ 文件 (*.cpp *.cxx *.cc);;"
           "Python 文件 (*.py);;"
           "Go 文件 (*.go);;"
           "C# 文件 (*.cs);;"
           "Java 文件 (*.java);;"
           "Swift 文件 (*.swift);;"
           "Cangjie 文件 (*.cj);;"
           "所有文件 (*)";
}

void MainWindow::updateWindowTitle()
{
    QString title = "小白记事本";
    if (!currentFilePath.isEmpty()) {
        title += " - " + QFileInfo(currentFilePath).fileName();
    }
    if (isModified) {
        title += " *";
    }
    setWindowTitle(title);
}

void MainWindow::loadFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "打开文件", "无法打开文件: " + file.errorString());
        return;
    }

    QTextStream in(&file);
    ui->textEdit->setText(in.readAll());
    file.close();

    currentFilePath = filePath;
    isModified = false;
    updateWindowTitle();
}

void MainWindow::saveFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "保存文件", "无法保存文件: " + file.errorString());
        return;
    }

    QTextStream out(&file);
    out << ui->textEdit->toPlainText();
    file.close();

    currentFilePath = filePath;
    isModified = false;
    updateWindowTitle();
}

// 新建文件
void MainWindow::on_action_triggered()
{
    if (isModified) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "保存更改", "当前文档已修改，是否保存更改？",
                                      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (reply == QMessageBox::Save) {
            on_action_3_triggered();
            if (isModified) return;
        } else if (reply == QMessageBox::Cancel) {
            return;
        }
    }

    ui->textEdit->clear();
    currentFilePath = "";
    isModified = false;
    updateWindowTitle();
}

// 打开
void MainWindow::on_action_2_triggered()
{
    if (isModified) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "保存更改", "当前文档已修改，是否保存更改？",
                                      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (reply == QMessageBox::Save) {
            on_action_3_triggered(); // 保存
            if (isModified) return; // 如果用户取消保存，则不继续打开
        } else if (reply == QMessageBox::Cancel) {
            return;
        }
    }

    QString filePath = QFileDialog::getOpenFileName(
        this, "打开文件", "", getFileFilter()
        );

    if (!filePath.isEmpty()) {
        loadFile(filePath);
    }
}

// 保存
void MainWindow::on_action_3_triggered()
{
    if (currentFilePath.isEmpty()) {
        on_action_4_triggered(); // 另存为
        return;
    }

    saveFile(currentFilePath);
}

// 另存为
void MainWindow::on_action_4_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(
        this, "另存为", "", getFileFilter()
        );

    if (!filePath.isEmpty()) {
        // 确保文件有扩展名
        QFileInfo fileInfo(filePath);
        if (fileInfo.suffix().isEmpty()) {
            // 如果没有扩展名，默认添加.txt
            filePath += ".txt";
        }
        saveFile(filePath);
    }
}

// 退出
void MainWindow::on_action_5_triggered()
{
    close();
}

// 回退
void MainWindow::on_action_6_triggered()
{
    ui->textEdit->undo();
}

// 取消回退
void MainWindow::on_action_7_triggered()
{
    ui->textEdit->redo();
}

// 关闭
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (isModified) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "保存更改", "当前文档已修改，是否保存更改？",
                                      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (reply == QMessageBox::Save) {
            on_action_3_triggered();
            if (isModified) {
                event->ignore();
            } else {
                event->accept();
            }
        } else if (reply == QMessageBox::Discard) {
            event->accept();
        } else {
            event->ignore();
        }
    } else {
        event->accept();
    }
}
