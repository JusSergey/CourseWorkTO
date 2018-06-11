#include "HtmlUtils.h"
#include <sstream>
#include <fstream>
using std::pair;
using std::string;
using std::ofstream;
using std::stringstream;

bool HtmlUtils::createHtmlPage(IGF args)
{
    auto outPage = createAndGetHtmlPage(args);

    {
        ofstream outFile(args.outfile);
        outFile << outPage;
        outFile.flush();
        outFile.close();
    }

    return true;
}

std::string HtmlUtils::createAndGetHtmlPage(HtmlUtils::IGF args)
{
    stringstream ssbuffer;
    ssbuffer <<
            "<!DOCTYPE html>                            \n"
            "<html>                                     \n"
            << createCSSStyle(args) <<                 "\n"
            "<head>                                     \n"
            "  <title>" << args.pageTitle << "</title>  \n"
            "</head>                                  \n\n"

            "<body>                                     \n"
            << createBody(args) <<
            "</body>                                  \n\n"

            "</html>                                    \n"
            ;

    return ssbuffer.str();
}

string HtmlUtils::createBody(HtmlUtils::IGF args)
{
    stringstream ssbuffer;
    ssbuffer << createTableList(args);
    return ssbuffer.str();
}

string HtmlUtils::createTableList(HtmlUtils::IGF args)
{
    auto createOneLine = [&](const StringVector &args,
                             const CallbackColorByPos &getColor,
                             int row)
    {
        stringstream ssSmallBuffer;

        ssSmallBuffer     << "<tr>                                                                \n";
        for (int col = 0; col < args.size(); ++col)
            ssSmallBuffer << "  <th bgcolor=" << getColor(row, col) << ">" << args[col] << "</th> \n";
        ssSmallBuffer     << "</tr>                                                               \n";

        return ssSmallBuffer.str();
    };

    stringstream ssbuffer;
    ssbuffer << "<table style=\"width:100%\">\n\n"
             << createOneLine(args.headTitle, args.getColorByPos, 0);

    int i = 1;
    for (const auto &pair_obj : args.data) {
        ssbuffer << createOneLine(pair_obj, args.getColorByPos, i++);
    }

    ssbuffer << "</table>\n";

    return ssbuffer.str();
}

string HtmlUtils::createCSSStyle(HtmlUtils::IGF args)
{
    stringstream ssbuffer;
    ssbuffer <<
            "<style>                                                    \n"
            "    table, th, td {                                        \n"
            "        border: " << args.border_width << "px solid gray;  \n"
            "        border-collapse: collapse;                         \n"
            "    }                                                      \n"
            "</style>                                                   \n";

    return ssbuffer.str();
}

string HtmlUtils::compressHtmlPage(string inputPage)
{
    auto ol = StringUtil::cropToStrings(inputPage, '\n');
    inputPage.clear();

    for (const auto &str : ol) {
        auto beginPos = str.find_first_not_of(' ');
        auto endPos = str.find_last_not_of(' ');
        // is valid positions
        if (beginPos != string::npos && endPos != string::npos)
            inputPage += str.substr(beginPos, endPos+1) + "\n";
    }

    return inputPage;
}
