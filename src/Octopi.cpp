#include "Octopi.h"

void Octopi::Update(Entities& entities) {
    printStatus = entities["sensor.octoprint_print_status"].state;

    // To stop later operations from failing
    if (printStatus == "unavailable")
        return;

    printFile = entities["sensor.octoprint_print_file"].state;
    printProgress = std::stoi(entities["sensor.octoprint_print_progress"].state);

    std::string completionDate = entities["sensor.octoprint_approximate_completion_time"].state;
    completionTime = completionDate.substr(completionDate.length() - 8, 5);
    printTimeTotal = entities["sensor.octoprint_approximate_total_print_time"].state;
    printTimeDone = entities["sensor.octoprint_print_time"].state;
    printTimeLeft = entities["sensor.octoprint_print_time_left"].state;

    socTemp = std::stof(entities["sensor.octoprint_soc_temperature"].state);
    bedTarget = std::stof(entities["sensor.octoprint_bed_target"].state);
    bedTemp = std::stof(entities["sensor.octoprint_bed_temperature"].state);
    toolTarget = std::stof(entities["sensor.octoprint_tool_0_target"].state);
    toolTemp = std::stof(entities["sensor.octoprint_tool_0_temperature"].state);
}
