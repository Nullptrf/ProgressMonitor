
# ProgressMonitor

## Introduction
The `ProgressMonitor` class provides a flexible way to monitor the progress of multiple functions in a C++ program. It displays progress bars for each function along with customizable symbols and colors.

## Features
- Monitor the progress of multiple functions concurrently.
- Customize progress bar symbols and colors.
- Display progress bars in the terminal.
- Capture function output during monitoring.
- Set sleep time between progress updates.
- Display a summary of function execution.

## Usage
1. **Include the header file:**
   ```cpp
   #include "ProgressMonitor.h"

2. **Define a ProgressMonitor function to monitor:**
    ```cpp
    ProgressMonitor::Function myFunction1 = []() -> std::string {
        // Your code goes here.
        //please use 'ostringstream' for redirecting the output strings to be besides the bar.
        // use setSleep instead of the standard sleeps functions
        std::ostringstream oss;
        oss << "Process of myFunction" << std::endl << 
        std::flush;
        ProgressMonitor::setSleep(1000); // 1000 milliseconds
        oss << "Process of myFunction finished" << std::endl << 
        std::flush;
        return oss.str();
    };
    // add more functions if you want.

**Note: the ProgressMonitor only supports std::string function for now!**

3. **Register functions with ProgressMonitor:**
    ```cpp
    // funcName use a text the match the task of the function
    // for example myFunction1 is meant for extracting data...
    ProgressMonitor::setFunction(myFunction1, "extracting data");
    ProgressMonitor::setFunction(myFunction2, "<function task>");
    // You need to register your functions before calling the startMonitoring()
    ```

## Customization
4. **Customize progress bar appearance:**
    ```cpp
    // You can change the finish text! Check out below.
    ProgressMonitor::setFinishText("Done executing all functions.");
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

    //You can abort the monitoring process by calling the abort function:
    ProgressMonitor::abort(ProgressMonitor::AbortT::EXIT, "Reason for aborting.");
    // or
    ProgressMonitor::abort(ProgressMonitor::AbortT::RETURN, "Reason for aborting.");
    ```


5. **After registering and customizing**
    ```cpp
    // Call the monitoring function to start monitoring.
    ProgressMonitor::startMonitoring();
    ```

#### Check out [example.cpp](https://github.com/i-Taylo/ProgressMonitor/blob/main/src/example.cpp) for more explained code.
***

#### 1> Me accounts
<div style="margin: -20px;"></div>
<div style="padding-top: 10px;">
Telegram: <img src="https://github.com/gauravghongde/social-icons/raw/master/SVG/Color/Telegram.svg" alt="Telegram" width="10" height="10">
<a href="t.me/v9y_7v2">V9y_7V2</a>
<br>
Instagram: <img src="https://github.com/gauravghongde/social-icons/raw/master/SVG/Color/Instagram.svg" alt="Instagram" width="10" height="10">
<a href="https://instagram.com/v9y_7">V9y_7</a>


</div>
