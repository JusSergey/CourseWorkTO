#include "SubsystemFilesystem.h"
#include "Utils/HtmlUtils.h"
#include <sys/ioctl.h>
#include <linux/hdreg.h>
#include <fstream>
using namespace std;

SubsystemFilesystem *SubsystemFilesystem::instance = nullptr;

std::string SubsystemFilesystem::readDeviceInfo()
{

}

ActiveDev::TestResult SubsystemFilesystem::test()
{

}

string SubsystemFilesystem::getPrintableInfo() const
{
    const string path = "info_hard_drive";

    system((string("fdisk -l >> ") + path).c_str());

    auto result = _getDataFromFile(path);

    system((string("rm ") + path).c_str());

    return result;
}

string SubsystemFilesystem::getHTMLCode() const
{
    HtmlUtils::InfoGenerateFile inputDataForHtml;

    string rawData = getPrintableInfo();
    auto res = getStructureInfo(rawData);

    static const string FIND_LINE = "\n\n";
    auto posNextChunk = rawData.find(FIND_LINE.c_str());
    HtmlUtils::Table overviewTable = getStructureInfo(rawData);
    overviewTable.nameTable = "Загальна інформація";
    inputDataForHtml.listTables.emplace_back(std::move(overviewTable));

    if (posNextChunk != string::npos) {
        string nextChunk = rawData.substr(posNextChunk + FIND_LINE.length());
        auto strList = StringUtil::cropToStrings(nextChunk, '\n');
        HtmlUtils::Table table;
        table.nameTable = "Інформація про томи";
        auto headTitle = StringUtil::cropToStrings(strList.front(), ' ');
        table.fieldsNames.insert(table.fieldsNames.end(), headTitle.begin(), headTitle.end());
        const size_t countColumns = headTitle.size();
        strList.pop_front();
        for (auto strRowList : strList) {
            auto strListRawLines = StringUtil::cropToStrings(strRowList, ' ');
            size_t i(0);
            string lastItemInRow;
            HtmlUtils::TableRow rowData;
            for (auto strField : strListRawLines) {
                if (++i < countColumns) {
                    rowData.push_back(strField);
                } else {
                    lastItemInRow += strField;
                    lastItemInRow += " ";
                }
            }
            if (!lastItemInRow.empty()) {
                rowData.push_back(std::move(lastItemInRow));
            }
            table.push_back(std::move(rowData));
        }
        inputDataForHtml.listTables.emplace_back(std::move(table));

        cout << "\nremove spaces: \n" << StringUtil::removeRepeatChars(nextChunk, ' ');
    }

    return HtmlUtils::createAndGetHtmlPage(inputDataForHtml);
}

static constexpr size_t SIZE_FILE = 1024*1024 * 512;
static constexpr size_t BLCK_FILE = 8;

double SubsystemFilesystem::testWrite()
{
    char *buffer = new char[SIZE_FILE / BLCK_FILE];

    auto delay_time = _getItervalBetweenCalculate([&]{
        ofstream wrf("test_file");
        if (!wrf.is_open())
            throw ("can't create file for test speed write (double SubsystemFilesystem::testWrite())");

        for (size_t i = 0; i < BLCK_FILE; ++i)
            wrf.write(buffer, SIZE_FILE/BLCK_FILE);

        wrf.flush();
        wrf.close();

    });

    (cout << "delay_write: " << delay_time << '\n').flush();

    delete []buffer;
    return delay_time;
}

double SubsystemFilesystem::testRead()
{
    char *buffer = new char[SIZE_FILE / BLCK_FILE];
    bool isEnableFileCache = false;

    auto delay_time = _getItervalBetweenCalculate([&]{

        isEnableFileCache = _getDataFromCommand("/sbin/sysctl vm.drop_caches=3").empty();

        ifstream rdf("test_file");

        if (!rdf.is_open()){
            throw ("can't create file for test speed read (double SubsystemFilesystem::testRead())");
        }

        for (size_t i = 0; i < BLCK_FILE; ++i)
            rdf.read(buffer, SIZE_FILE/BLCK_FILE);

        rdf.close();
    });

    if (isEnableFileCache)
        (cout << "CACHE_ENABLE was TRUE..  test read was failed\n ").flush();
    else
        (cout << "CACHE_ENABLE was FALSE.. test read was success\n").flush();

    delete []buffer;
    return delay_time;
}
