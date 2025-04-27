/**
 * Author:    Zackary Misso
 * Version:   0.1.0
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#include <feign/test/tester.h>
#include <imedit/image.h>
#include <imedit/im_util.h>
#include <feign/core/api.h>
#include <feign/parsers/json_parser.h>
#include <dirent.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

FEIGN_BEGIN()

// [X] TODO: create global html page
// [X] TODO: create individual html pages per test
// [X] TODO: create logging info for individual unit test
// [] TODO: create automatic graphing for unit clockings and error
// [] TODO LATER: create tags for each unit test
// [] TODO LATER: create a sort by tags feature
void UnitTestSiteAssembler::create_global_html_page()
{
    LOG("Auto-generating unit tests results web-viewer");
    int images_per_row = 3;

    // std::vector<std::string> dirs = std::vector<std::string>();
    std::vector<std::string> paths = std::vector<std::string>();
    std::vector<std::string> names = std::vector<std::string>();

    // parse all of the scenes in the test scenes folder
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir("../scenes/unit_tests/scenes/")) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            std::string str = std::string(ent->d_name);
            if (str.length() > 5 && str.substr(str.length() - 5) == ".json")
                paths.push_back(str.substr(0, str.length() - 5));
        }
    }

    // sort them all to be in alphabetical order (TODO: make this tag based later)
    sort(paths.begin(), paths.end());

    // create the main page
    std::ofstream home_page;
    home_page.open("../scenes/unit_tests/web_viewer/home.html");

    home_page << "<!DOCTYPE html>" << std::endl;
    home_page << "<meta charset=\"utf-8\">" << std::endl;
    home_page << std::endl;
    home_page << "**Unit Tests for the Feign Rendering Engine**" << std::endl;
    home_page << "*By: Zack Misso*" << std::endl;
    home_page << std::endl;
    home_page << "Scenes" << std::endl;
    home_page << "======" << std::endl;
    home_page << std::endl;
    home_page << "<p>Each image represents an individual unit test which links to a separate page" << std::endl;
    home_page << "containing information regarding that specific test and whether the current build" << std::endl;
    home_page << "is passing. This viewer is only verified to work on Chrome for now.<p>" << std::endl;
    home_page << std::endl;

    int count = 0;
    for (int i = 0; i < paths.size(); ++i)
    {
        if (count == 0)
        {
            home_page << "<div>" << std::endl;
        }
        home_page << "<a href=\"";
        home_page << paths[i] + ".html"; // TODO: replace with individual test page
        home_page << "\"><img src=\"../images/" << paths[i] << ".png\" style=\"width:30%; float:left; padding:5px\" /></a>";
        home_page << std::endl;
        if (++count == images_per_row)
        {
            count = 0;
            home_page << "</div>" << std::endl;
        }
    }
    if (count != 0)
    {
        // catch in case the number of unit tests are not divisible by <images_per_row>.
        home_page << "</div>" << std::endl;
    }

    home_page << std::endl;
    home_page << "Contact Me" << std::endl;
    home_page << "==========" << std::endl;
    home_page << std::endl;
    home_page << "If there are any questions please feel free to email me at zack441@mac.com" << std::endl;
    home_page << std::endl;
    home_page << "<!--- Markdeep & image comparison library - probably no need to change anything below --><!--- Markdeep & image comparison library - probably no need to change anything below -->" << std::endl;
    home_page << "<style class=\"fallback\">" << std::endl;
    home_page << "    body {" << std::endl;
    home_page << "        visibility: hidden;" << std::endl;
    home_page << "        white-space: pre;" << std::endl;
    home_page << "        font-family: monospace" << std::endl;
    home_page << "    }" << std::endl;
    home_page << "</style>" << std::endl;
    home_page << "<script src=\"resources/markdeep.min.js\"></script>" << std::endl;
    home_page << "<script>window.alreadyProcessedMarkdeep || (document.body.style.visibility = \"visible\")</script>" << std::endl;
    home_page << "<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.11.0/jquery.min.js\"></script>" << std::endl;
    home_page << "<script src=\"resources/jquery.event.move.js\"></script>" << std::endl;
    home_page << "<script src=\"resources/jquery.twentytwenty.js\"></script>" << std::endl;
    home_page << "<link href=\"resources/offcanvas.css\" rel=\"stylesheet\">" << std::endl;
    home_page << "<link href=\"resources/twentytwenty.css\" rel=\"stylesheet\" type=\"text/css\" />" << std::endl;
    home_page << "<script>" << std::endl;
    home_page << "    $(window).load(function () { $(\".twentytwenty-container\").twentytwenty({ default_offset_pct: 0.5 }); });" << std::endl;
    home_page << "</script>" << std::endl;

    home_page.close();
}

void UnitTestSiteAssembler::create_test_html_page(UnitTestData &testLog)
{
    LOG("creating test html page for " + testLog.test_name);

    std::string test_name = testLog.test_name.substr(0, testLog.test_name.length() - 5);

    std::ofstream test_page;
    test_page.open("../scenes/unit_tests/web_viewer/" + test_name + ".html");
    
    test_page << "<!DOCTYPE html>" << std::endl;
    test_page << "<meta charset=\"utf-8\">" << std::endl;
    test_page << std::endl;
    test_page << "**unit test: " + test_name + "**" << std::endl;
    test_page << std::endl;
    test_page << "unit test originally created by: <a href=\"https://zackarymisso.com\">Zack Misso</a>" << std::endl;
    test_page << std::endl;
    test_page << "*Description: this is a test description*" << std::endl;
    test_page << std::endl;
    test_page << "*Keywords: blah,blah,blah*" << std::endl;
    test_page << std::endl;
    test_page << "*report automatically generated*" << std::endl;
    test_page << std::endl;
    test_page << "<b style=\"color:green\">PLEASE VIEW IN CHROME, OTHERWISE SCRIPTS MIGHT BREAK</b>" << std::endl;
    test_page << std::endl;
    test_page << "Generated Images" << std::endl;
    test_page << "================" << std::endl;
    test_page  << std::endl;
    test_page << "<div class=\"row\">" << std::endl;
    test_page << "  <div class=\"column\">" << std::endl;
    test_page << "    <p align=\"center\" style=\"color:blue;\"><b>current build</b></p>" << std::endl;
    test_page << "    <!-- <br /> -->" << std::endl;
    test_page << "    <img src=\"../images/" + test_name + ".png\" style=\"width:100%;\" />" << std::endl;
    test_page << "  </div>" << std::endl;
    test_page << "  <div class=\"column\">" << std::endl;
    test_page << "    <p align=\"center\" style=\"color:blue;\"><b>reference</b></p>" << std::endl;
    test_page << "    <!-- <br /> -->" << std::endl;
    test_page << "    <img src=\"../images/" + test_name + "_ref.png\" style=\"width:100%;\" />" << std::endl;
    test_page << "  </div>" << std::endl;
    test_page << "</div>" << std::endl;
    test_page << std::endl;
    test_page << "Comparisons" << std::endl;
    test_page << "===========" << std::endl;
    test_page << std::endl;
    test_page << "Direct comparison" << std::endl;
    test_page << "-----------------" << std::endl;
    test_page << std::endl;
    test_page << "<div class=\"twentytwenty-container\">" << std::endl;
    test_page << "    <img src=\"../images/" + test_name + ".png\" alt=\"current\">" << std::endl;
    test_page << "    <img src=\"../images/" + test_name + "_ref.png\" alt=\"referenece\">" << std::endl;
    test_page << "    <!-- <img src=\"../images/" + test_name + ".png\" alt=\"current2\">" << std::endl;
    test_page << "    <img src=\"../images/" + test_name + "_ref.png\" alt=\"referenece2\"> -->" << std::endl;
    test_page << "</div>" << std::endl;
    test_page << std::endl;
    test_page << "Error" << std::endl;
    test_page << "-----" << std::endl;
    test_page << std::endl;
    test_page << "*error images report relative error, metrics reported below*" << std::endl;
    test_page << std::endl;
    test_page << "<div class=\"row\">" << std::endl;
    test_page << "  <div class=\"column\">" << std::endl;
    test_page << "    <p align=\"center\" style=\"color:blue;\"><b>positive error</b></p>" << std::endl;
    test_page << "    <!-- <br /> -->" << std::endl;
    test_page << "    <a href=\"home.html\"><img src=\"../images/test_001_pos_error.png\" style=\"width:100%;\" /></a>" << std::endl;
    test_page << "  </div>" << std::endl;
    test_page << "  <div class=\"column\">" << std::endl;
    test_page << "    <p align=\"center\" style=\"color:blue;\"><b>negative error</b></p>" << std::endl;
    test_page << "    <!-- <br /> -->" << std::endl;
    test_page << "    <a href=\"home.html\"><img src=\"../images/" + test_name + "_neg_error.png\" style=\"width:100%;\" /></a>" << std::endl;
    test_page << "  </div>" << std::endl;
    test_page << "</div>" << std::endl;
    test_page << std::endl;
    test_page << "<ul>" << std::endl;
    test_page << "<li style=\"color:blue\">Error from reference:          " << testLog.image_error << "</li>" << std::endl;
    test_page << "<li style=\"color:blue\">Squared error from reference:  " << testLog.image_sqr_error << "</li>" << std::endl;
    test_page << "<li style=\"color:blue\">Relative error from reference: " << testLog.image_rel_error << "</li>" << std::endl;
    test_page << "</ul>" << std::endl;
    test_page << std::endl;
    test_page << std::endl;
    test_page << "<p style=\"color:blue\" align=\"center\">Test is TODO</p>" << std::endl;
    test_page << std::endl;
    test_page << "Stat History" << std::endl;
    test_page << "============" << std::endl;
    test_page << std::endl;
    test_page << "Starting February 1st 2025, the Feign Renderer will begin keeping track" << std::endl;
    test_page << "of the average render times, and various errors for each of its unit tests" << std::endl;
    test_page << "on a weekly basis. These metrics will be reported to keep track of the Feign" << std::endl;
    test_page << "Renderer's performance as new features are continuously added. All unit tests" << std::endl;
    test_page << "are ran on a single thread using the same computer to maintain consistency." << std::endl;
    test_page << std::endl;
    test_page << "<div class=\"twentytwenty-container\">" << std::endl;
    test_page << "    <img src=\"../images/" + test_name + ".png\" alt=\"render time\">" << std::endl;
    test_page << "    <img src=\"../images/" + test_name + "_ref.png\" alt=\"error\">" << std::endl;
    test_page << "    <img src=\"../images/" + test_name + ".png\" alt=\"squared error\">" << std::endl;
    test_page << "    <img src=\"../images/" + test_name + "_ref.png\" alt=\"relative error\">" << std::endl;
    test_page << "</div>" << std::endl;
    test_page << std::endl;
    test_page << "Change-log" << std::endl;
    test_page << "==========" << std::endl;
    test_page << std::endl;
    test_page << "Here we document any significant changes to this unit test which may" << std::endl;
    test_page << "explain changes in performance, look, etc." << std::endl;
    test_page << std::endl;
    test_page << "<ul>" << std::endl;
    test_page << "    <li>2/02/2025: implemented automated unit-test individual tests" << std::endl;
    test_page << "    <li>08/25/2024: implemented automated unit-test homepage" << std::endl;
    test_page << "</ul>" << std::endl;
    test_page << std::endl;
    test_page << "<!-- Scenes" << std::endl;
    test_page << "Each image represents an individual unit test which links to a separate page" << std::endl;
    test_page << "containing information regarding that specific test and whether the current build" << std::endl;
    test_page << "is passing. This viewer is only verified to work on Chrome for now. -->" << std::endl;
    test_page << std::endl;
    test_page << "Contact Me" << std::endl;
    test_page << "==========" << std::endl;
    test_page << std::endl;
    test_page << "If there are any questions please feel free to email me at zack441@mac.com" << std::endl;
    test_page << std::endl;
    test_page << "<!--- Markdeep & image comparison library - probably no need to change anything below -->" << std::endl;
    test_page << "<style class=\"fallback\">body{visibility:hidden;white-space:pre;font-family:monospace}</style><script src=\"resources/markdeep.min.js\"></script><script>window.alreadyProcessedMarkdeep||(document.body.style.visibility=\"visible\")</script>" << std::endl;
    test_page << "<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.11.0/jquery.min.js\"></script>" << std::endl;
    test_page << "<script src=\"resources/jquery.event.move.js\"></script>" << std::endl;
    test_page << "<script src=\"resources/jquery.twentytwenty.js\"></script>" << std::endl;
    test_page << "<!-- <script src=\"jeri.js\"></script> -->" << std::endl;
    test_page << "<link href=\"resources/offcanvas.css\" rel=\"stylesheet\">" << std::endl;
    test_page << "<link href=\"resources/twentytwenty.css\" rel=\"stylesheet\" type=\"text/css\" />" << std::endl;
    test_page << "<script>" << std::endl;
    test_page << "$(window).load(function(){$(\".twentytwenty-container\").twentytwenty({default_offset_pct: 0.5});});" << std::endl;
    test_page << "</script>" << std::endl;

    test_page.close();
}

// TODO: compute the average of the previous 10 clockings and report the
//       difference between the current run and the average on the site.
void UnitTestSiteAssembler::append_to_test_records(UnitTestData &test_log) {
    LOG("recording clocking data for " + test_log.test_name);

    std::string test_name = test_log.test_name.substr(0, test_log.test_name.length() - 5);

    std::ofstream clockings_file;
    clockings_file.open("../scenes/unit_tests/web_viewer/clocking_logs/" + test_name + ".dat", std::ios_base::app);

    std::cout << "CLOCKINGS STR: " << test_log.clockings.to_string() << std::endl;

    clockings_file << test_log.clockings.to_string() << std::endl;
    
    clockings_file.close();
}

FEIGN_END()
