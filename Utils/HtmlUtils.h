#ifndef _HTMLUTILS_H
#define _HTMLUTILS_H

#include "StringUtilities.h"
#include "types.h"
#include <functional>

class HtmlUtils {
public:
    using TableRow = StringVector;

private:
    using _Table = list<TableRow>;

public:
    struct Table : public _Table {
        template <typename ...Args>
        Table(Args&&... args) : _Table(std::forward<Args>(args)...){}
        TableRow headTitle;
        string nameTest = "This is a title";
    };
    using ListTables = list<Table>;

public:
    struct InfoGenerateFile {
        CallbackColorByPos getColorByPos =
                [](int row, int col)
        {
            (void)col;
            return row == 0 ? "#d0d0d0" :
                   row  & 1 ? "#f0f0f0" : "#ffFFff";
        };

        CallbackColorByPos getColorTitle =
                [](int row, int col)
        {
            (void)row;
            (void)col;
            return "#c0baba";
        };

        string pageTitle;
        string outfile = "default.html";
        TableRow headTitle;
        ListTables listTables;

        //css params
        bool collapse_border = true;
        bool border_width = 1;
    };

    using IGF = InfoGenerateFile &;

public:
    static bool createHtmlPage(IGF args);
    static string createAndGetHtmlPage(IGF args);
    static string createBody(IGF args);
    static string createTableList(IGF args);
    static string createCSSStyle(IGF args);
    static string compressHtmlPage(string inputPage);
    static string getEmptyHtmlPage();
    static string getBRTag();

private:
    static string rawCreateBodyTable(const _Table &tableData, const CallbackColorByPos &colorGetter);
};

#endif // _HTMLUTILS_H
