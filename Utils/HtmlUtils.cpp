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
    stringstream ssbuffer;

    for (Table &tableData : args.listTables) {
        if (!tableData.nameTable.empty())
            ssbuffer << rawCreateBodyTable(_Table({TableRow{tableData.nameTable}}), tableData.getColorTitle);

        Table bodyTableData(tableData);
        bodyTableData.push_front(tableData.fieldsNames);
        ssbuffer << rawCreateBodyTable(bodyTableData, tableData.getColorByPos);
        ssbuffer << HtmlUtils::getBRTag();
    }

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

std::string HtmlUtils::getEmptyHtmlPage()
{
    return
            "<!DOCTYPE html> \n"
            "<html>  \n"
            "<head>  \n"
            "</head> \n"
            "<body>  \n"
            "</body> \n"
            "</html> \n"
            ;
}

std::string HtmlUtils::getBRTag()
{
    return "<br>";
}

std::string HtmlUtils::getColotFromBall(int ball)
{
    cout << "get color ball origin: " << ball << std::endl;
    ball = ball < 40 ? 40 : ball > 110 ? 110 : ball;
    ball -= (85 - ball)*0.5;
cout << "get color ball: " << ball << std::endl;
    ball = ball < 40 ? 40 : ball > 110 ? 110 : ball;

    static const uint8_t min = 0x88;
    static const uint8_t max = 0xEE;
    static const float per = ((float)max - (float)min) / 100.F;

    const uint8_t colorG = min + uint8_t(per * ball);
    const uint8_t colorR = max - uint8_t(per * ball);

    stringstream ss;
    ss << "#" << std::hex << (int)colorR << (int)colorG << "88";
    return ss.str();
}

std::string HtmlUtils::rawCreateBodyTable(const _Table &tableData, const CallbackColorByPos &colorGetter)
{
    auto createOneLine = [&](const StringVector &args,
                             const CallbackColorByPos &getColor,
                             int row)
    {
        stringstream ssSmallBuffer;

        ssSmallBuffer     << "<tr> \n";
        for (size_t col = 0; col < args.size(); ++col)
            ssSmallBuffer << "  <th bgcolor=" << getColor(row, col, args[col]) << ">" << args[col] << "</th> \n";
        ssSmallBuffer     << "</tr>\n";

        return ssSmallBuffer.str();
    };

    stringstream ssbuffer;
    ssbuffer << "<table style=\"width:100%\">\n\n";

    int i = 0;
    for (const auto &pair_obj : tableData) {
        ssbuffer << createOneLine(pair_obj, colorGetter, i++);
    }

    ssbuffer << "</table>\n";

    return ssbuffer.str();
}
