#include "nlohmann/json.hpp"
#include <cadmium/modeling/celldevs/grid/coupled.hpp>
#include <cadmium/simulation/logger/csv.hpp>
#include <cadmium/simulation/root_coordinator.hpp>
#include <chrono>
#include <fstream>
#include <string>
#include "include/conwayCell.hpp"

using namespace cadmium::celldevs;
using namespace cadmium;

std::shared_ptr<GridCell<conwayState, double>> addGridCell(const coordinates & cellId, const std::shared_ptr<const GridCellConfig<conwayState, double>>& cellConfig) {
	auto cellModel = cellConfig->cellModel;

	if (cellModel == "conway") {
		return std::make_shared<conway>(cellId, cellConfig);
	} else {
		throw std::bad_typeid();
	}
}

int main(int argc, char ** argv) {

    /* Constants and Defaults */
    const std::string DEFAULT_CONFIG_FILE_PATH = "config/mapgen_config.json";
    constexpr int DEFAULT_SIM_TIME = 500;
    const std::string DEFAULT_OUTPUT_FILE_PATH = "output/mapgen_grid_log.csv";

    /* Argument Parsing */

    // 1. Configuration File (JSON)
	std::string configFilePath = (argc > 1) ? argv[1] : DEFAULT_CONFIG_FILE_PATH;
    std::cout << "Config File: " << configFilePath << std::endl;

    // 2. Simulation Duration (double)
	double simTime = (argc > 2) ? std::stod(argv[2]) : DEFAULT_SIM_TIME;
    std::cout << "Simulation Duration: " << simTime << std::endl;

    // 3. Output File (JSON)
	std::string outputFilePath = (argc > 3) ? argv[3] : DEFAULT_OUTPUT_FILE_PATH;
    std::cout << "Output File: " << outputFilePath << std::endl;

    /* Models */
	auto model = std::make_shared<GridCellDEVSCoupled<conwayState, double>>("mapgen", addGridCell, configFilePath);
	model->buildModel();
	
    /* Logs */
	auto rootCoordinator = RootCoordinator(model);
	rootCoordinator.setLogger<CSVLogger>(outputFilePath, ";");
	
    /* Simulation */
	rootCoordinator.start();
	rootCoordinator.simulate(simTime);
	rootCoordinator.stop();
    return 0;
}
