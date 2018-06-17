#include "SaverFile.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

void SaverFile::saveLocalFile(const char *data, size_t bufferSize)
{
    QString filenamepath = QFileDialog::getSaveFileName(nullptr);
    if (!filenamepath.isEmpty()) {
        QFile filewrite(filenamepath);
        if (filewrite.open(QIODevice::WriteOnly)) {
            filewrite.write(data, bufferSize);
            filewrite.close();
        } else {
            QMessageBox::critical(nullptr, "Помилка", "Неможливо выдкрити файл для запису");
        }
    }
}
