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
        Table(Args&&... args) :
            _Table(std::forward<Args>(args)...),
            getColorByPos(getDefaultColorByPos()),
            getColorTitle(getDefaultColorTitle()) {}

        TableRow fieldsNames;
        string nameTable;

        CallbackColorByPos getColorByPos;
        CallbackColorByPos getColorTitle;

        static CallbackColorByPos getDefaultColorByPos() {
            return [](int row, int col, string text)
            {
                (void)col;
                (void)text;
                return row == 0 ? "#d0d0d0" :
                       row  & 1 ? "#f0f0f0" : "#ffFFff";
            };
        }

        static CallbackColorByPos getDefaultColorTitle() {
            return [](int row, int col, string text)
            {
                (void)row;
                (void)col;
                (void)text;
                return "#c0baba";
            };
        }

    };
    using ListTables = list<Table>;

public:
    struct InfoGenerateFile {
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
    static string getColotFromBall(int ball);

private:
    static string rawCreateBodyTable(const _Table &tableData, const CallbackColorByPos &colorGetter);
};

#endif // _HTMLUTILS_H
