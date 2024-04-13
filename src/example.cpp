#include "ProgressMonitor/ProgressMonitor.h" // Include the header file of the monitor
#include <cstdlib>


// Use:
/* if you have a check for something in a required block for example:

    if (TOKEN.empty()) {
        ProgressMonitor::abort(EXIT, "Missing token!");
    }

    the `abort` function by defualt the abortType will be return 1
    you can change that by specifying EXIT so the program will exit at status 1 (error)


*/

//

// Here is how you can make a ProgressMonitor function of string type like below
ProgressMonitor::Function function1 = []() -> std::string {
    /* Your code here... and please use 'ostringstream' for redirecting the output strings to be besides the bar
    - and setSleep for sleeping inside ProgressMonitor functions
    Here is an example:
    */
    std::ostringstream oss;
    // Below is just simulation of extarcting files
    // You can implement your own logic here.
    oss << "Extracting file 1." << std::endl << std::flush;
    ProgressMonitor::setSleep(100); // the sleep will be in milliseconds, 1000 is 1 sec.
    oss << "Extracting file 2." << std::endl << std::flush;
    ProgressMonitor::setSleep(100);
    oss << "Extracting file 3." << std::endl << std::flush;
    ProgressMonitor::setSleep(100);

    oss << "All done." << std::endl << std::flush;
    ProgressMonitor::setSleep(500);

    return oss.str();
};

// Please use `ProgressMonitor::exec` to execute external commands
ProgressMonitor::Function function2 = []() -> std::string {
    // this is a bash code example or you can use it like `ProgressMonitor::exec("bash mycode.sh");`
    std::string command = R"(
    list_them=$(ls .)
    for i in $list_them; do
        echo "Verifying: $i"
    done
    echo "Done verifying."
    )";

    std::ostringstream oss;
    oss << ProgressMonitor::exec(command.c_str()) << std::endl << std::flush;
    ProgressMonitor::setSleep(1000);
    return oss.str();
};


auto main() -> int {

    // funcName use a text the match the task of the function
    // for example function1 is meant for extracting data...
    ProgressMonitor::setFunction(function1, "Extarcting data");
    ProgressMonitor::setFunction(function2, "Updating");
    // add more function if you want.

    // Customize the bar!
    // You can change the finish text! Check out below.
    //ProgressMonitor::setFinishText("Done executing all functions.");
    // You chnage the bar width or remove setBarWidth to keep the default width (30)! Check out below.
    ProgressMonitor::setBarWidth(20);

    // Change the color of the fill part of the progress bar to green.
    ProgressMonitor::setColorTo(ProgressMonitor::FILL, ProgressMonitor::GREEN);

    // Change the color of the leading symbol of the progress bar to cyan.
    ProgressMonitor::setColorTo(ProgressMonitor::LEAD, ProgressMonitor::CYAN);

    // Change the color of the starting symbol of the progress bar to yellow.
    ProgressMonitor::setColorTo(ProgressMonitor::START, ProgressMonitor::YELLOW);

    // Change the color of the ending symbol of the progress bar to yellow.
    ProgressMonitor::setColorTo(ProgressMonitor::END, ProgressMonitor::YELLOW);

    // Change the color of the function name displayed alongside the progress bar to yellow.
    ProgressMonitor::setColorTo(ProgressMonitor::FUNC_NAME, ProgressMonitor::YELLOW);

    // Enable capturing and displaying the output of monitored functions.
    ProgressMonitor::captureFunctionsOutput(true);


    ProgressMonitor::setBarStartSymbol("("); // remove this line to keep the default `[`
    ProgressMonitor::setBarEndSymbol(")"); // remove this line to keep the default `]`
    ProgressMonitor::setBarFillSymbol("="); // remove this line to keep the default `▇`
    ProgressMonitor::setBarLeadSymbol("->"); // remove this line to keep the default `-▶`

    // call startMonitoring to start monitoring the selected functions above.
    ProgressMonitor::startMonitoring();

    return EXIT_SUCCESS;
}