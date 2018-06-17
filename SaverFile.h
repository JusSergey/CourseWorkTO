#ifndef SAVERFILE_H
#define SAVERFILE_H

#include <QString>

class SaverFile {
public:
    SaverFile() = delete;
    static void saveLocalFile(QString filename, const char *data, size_t bufferSize);
    static QString saveLocalFile(const char *data, size_t bufferSize);
};

#endif // SAVERFILE_H
