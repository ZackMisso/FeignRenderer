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

// [] TODO: create global html page
// [] TODO: create global html page automation
// [] TODO: create individual unit test page
// [] TODO: create individual unit test page automation
// [] TODO: create logging info for individual unit test
// [] TODO: create logging info automation for individual unit test
// [] TODO: modify individual unit test page for displaying logging info
// [] TODO: create tags for each unit test
// [] TODO: create a sort by tags feature
void UnitTestSiteAssembler::create_global_html_page()
{
    LOG("Auto-generating unit tests results web-viewer");
    int images_per_row = 3;

    std::vector<std::string> paths = std::vector<std::string>();
    std::vector<std::string> dirs = std::vector<std::string>();
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
    home_page << "Each image represents an individual unit test which links to a separate page" << std::endl;
    home_page << "containing information regarding that specific test and whether the current build" << std::endl;
    home_page << "is passing. This viewer is only verified to work on Chrome for now." << std::endl;
    home_page << std::endl;

    // this is for reference, TODO: delete later
    // <div>
    //     <a href="home.html"><img src="../images/test_000.png" style="width:30%; float:left; padding:5px" /></a>
    //     <a href="home.html"><img src="../images/test_000.png" style="width:30%; float:left; padding:5px" /></a>
    //     <a href="home.html"><img src="../images/test_000.png" style="width:30%; float:left; padding:5px" /></a>
    // </div>
    int count = 0;
    for (int i = 0; i < paths.size(); ++i)
    {
        if (count == 0)
        {
            home_page << "<div>" << std::endl;
        }
        home_page << "<a href=\"";
        home_page << "home.html"; // TODO: replace with individual test page
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

void UnitTestSiteAssembler::create_test_html_page()
{
    // TODO: need to design by hand first, then automate
    // TODO: need to add description / metadata info to test files
    // TODO: maybe add notes supporrt in test files?
}

FEIGN_END()
