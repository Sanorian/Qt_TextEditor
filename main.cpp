#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

class MainWindow : public QMainWindow
{
private:
   QPlainTextEdit *textarea;
   QString currentFile;

public slots:
   void newFile() {
       textarea->clear();
       currentFile.clear();
   }

   void openFile() {
       currentFile = QFileDialog::getOpenFileName(this, tr("Open File"));
       if (!currentFile.isEmpty()) {
           QFile file(currentFile);
           if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
               QMessageBox::warning(this, tr("Warning"), tr("Cannot read file %1:\n%2.")
                                  .arg(file.fileName()).arg(file.errorString()));
               return;
           }
           QTextStream in(&file);
           textarea->setPlainText(in.readAll());
       }
   }

   void saveFile() {
       if (currentFile.isEmpty()) {
           saveFileAs();
       } else {
           QFile file(currentFile);
           if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
               QMessageBox::warning(this, tr("Warning"), tr("Cannot write file %1:\n%2.")
                                  .arg(file.fileName()).arg(file.errorString()));
               return;
           }
           QTextStream out(&file);
           out << textarea->toPlainText();
       }
   }

   void saveFileAs() {
       QString fileName = QFileDialog::getSaveFileName(this, tr("Save File As"));
       if (fileName.isEmpty())
           return;
       currentFile = fileName;
       saveFile();
   }

public:
   MainWindow(QWidget *parent = nullptr): QMainWindow(parent), textarea(new QPlainTextEdit(this)), currentFile()
   {
       setCentralWidget(textarea);

       QToolBar *toolbar = new QToolBar(this);
       toolbar->setMovable(false);
       addToolBar(toolbar);

       QAction *newFileAction = new QAction(tr("New"), this);
       newFileAction->setShortcut(QKeySequence::New);
       connect(newFileAction, &QAction::triggered, this, &MainWindow::newFile);
       toolbar->addAction(newFileAction);

       QAction *openFileAction = new QAction(tr("Open"), this);
       openFileAction->setShortcut(QKeySequence::Open);
       connect(openFileAction, &QAction::triggered, this, &MainWindow::openFile);
       toolbar->addAction(openFileAction);

       QAction *saveFileAction = new QAction(tr("Save"), this);
       saveFileAction->setShortcut(QKeySequence::Save);
       connect(saveFileAction, &QAction::triggered, this, &MainWindow::saveFile);
       toolbar->addAction(saveFileAction);

       QAction *saveAsFileAction = new QAction(tr("Save As"), this);
       saveAsFileAction->setShortcut(QKeySequence::Save);
       connect(saveAsFileAction, &QAction::triggered, this, &MainWindow::saveFileAs);
       toolbar->addAction(saveAsFileAction);
   }
};

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   MainWindow w;
   w.show();
   return a.exec();
}
