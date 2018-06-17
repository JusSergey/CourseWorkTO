#include "SaverFile.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

void SaverFile::saveLocalFile(QString filename, const char *data, size_t bufferSize)
{
    if (!filename.isEmpty()) {
        QFile filewrite(filename);
        if (filewrite.open(QIODevice::WriteOnly)) {
            filewrite.write(data, bufferSize);
            filewrite.close();
        } else {
            QMessageBox::critical(nullptr, "Помилка", "Неможливо вiдкрити файл для запису");
        }
    }
}

QString SaverFile::saveLocalFile(const char *data, size_t bufferSize)
{
    QString filenamepath = QFileDialog::getSaveFileName(nullptr);
    saveLocalFile(filenamepath, data, bufferSize);
    return filenamepath;
}
