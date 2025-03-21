#ifndef MAPGEN_CELL_HPP
#define MAPGEN_CELL_HPP

// Standard
#include <cmath>

// Other
#include <nlohmann/json.hpp>

// Cadmium V2
#include <cadmium/modeling/celldevs/grid/cell.hpp>
#include <cadmium/modeling/celldevs/grid/config.hpp>

// Models
#include "mapgenState.hpp"

using namespace cadmium::celldevs;


/**
 * Atomic Model Cell Definition
 */
class mapgen : public GridCell<MapgenState, double> {

    public:

    /**
     * Constants
     */
    static constexpr double DEFAULT_DELAY_TIME = 1.00;
    // TODO: static counter for round tracking in multi-level aggregation

    /**
     * Constructor
     */
    mapgen(const std::vector<int>& id, 
            const std::shared_ptr<const GridCellConfig<MapgenState, double>>& config
          ): GridCell<MapgenState, double>(id, config) { }

    /**
     * Local Computation Function (tau)
     */
    [[nodiscard]] MapgenState localComputation(MapgenState state,
        const std::unordered_map<std::vector<int>, NeighborData<MapgenState, double>>& neighborhood) const override {

        /* Local Variables */

        // TODO: Accumulators for all neighbour types (ie. terrain)
        // Number of LAND neighbours
        int live_neighbors = 0;

        /* Canvas the Neighbourhood */

        // Assess this cell's neighbourhood
        // ie. tally neighbour cells by terrain type
        for (const auto& [neighborId, neighborData]: neighborhood) {
            auto nState = neighborData.state;

            // Count LAND neighbours
            if(nState->terrain == MapgenStateName::LAND) {
                live_neighbors++;
            }
            // TODO: Count other neighbours

        }

        /* Mutate State Based on Rules and Return */

        // Case: This cell is LAND
        //if(state.life == true) {
        if(state.terrain == MapgenStateName::LAND) {
            // Uncount this cell as a neighbour
            live_neighbors--; 
            // Case: Change from LAND to WATER
            if(live_neighbors < 2 || live_neighbors > 3) {
                //state.life = false;
                state.terrain = MapgenStateName::WATER;
            }
        } 
        // Case: This cell is WATER
        else {
            // Case: Change from WATER to LAND
            if(live_neighbors == 3) {
                //state.life = true;
                state.terrain = MapgenStateName::LAND;
            }
        }

        return state;
    }

    /**
     * Delay function (D)
     */
    [[nodiscard]] double outputDelay(const MapgenState& state) const override {
        return DEFAULT_DELAY_TIME;
    }
};

#endif // MAPGEN_CELL_HPP
