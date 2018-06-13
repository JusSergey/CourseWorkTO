#ifndef _HTMLUTILS_H
#define _HTMLUTILS_H

#include "StringUtilities.h"
#include "types.h"
#include <functional>

class HtmlUtils {
public:
    struct InfoGenerateFile {
        CallbackColorByPos getColorByPos =
                [](int row, int col)
        {
            return row == 0 ? "#ddDDdd" :
                   row  & 1 ? "#f0f0f0" : "#ffFFff";
        };

        string pageTitle;
        string outfile = "default.html";
        StringVector headTitle;
        ListStrVects data;

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
};

#endif // _HTMLUTILS_H
