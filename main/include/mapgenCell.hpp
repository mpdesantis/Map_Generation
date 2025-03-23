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

        // Accumulators for all neighbor types (differentiated by terrain)
        int water_neighbors = 0;
        int land_neighbors = 0;
        int forest_neighbors = 0;
        int desert_neighbors = 0;

        /* Canvas the Neighborhood */

        // Canvas this cell's neighborhood to categorize neighbor types
        // ie. tally neighbor cells by terrain type
        for (const auto& [neighborId, neighborData]: neighborhood) {
            // State of the neighbor cell for this iteration
            auto nState = neighborData.state;

            // WATER neighbors
            if(nState->terrain == MapgenStateName::WATER) {
                water_neighbors++;
            }
            // LAND neighbors
            if(nState->terrain == MapgenStateName::LAND) {
                land_neighbors++;
            }
            // FOREST neighbors
            if(nState->terrain == MapgenStateName::FOREST) {
                forest_neighbors++;
            }
            // DESERT neighbors
            if(nState->terrain == MapgenStateName::DESERT) {
                desert_neighbors++;
            }
        }

        /* Mutate State Based on Rules and Return */

        // Case: WATER cell
        if(state.terrain == MapgenStateName::WATER) {
            // Uncount this cell from its own neighborhood tally
            water_neighbors--; 

            // Case: WATER --> LAND
            if(land_neighbors == 3) {
                state.terrain = MapgenStateName::LAND;
            }
        } 

        // Case: LAND cell
        else if (state.terrain == MapgenStateName::LAND) {
            // Uncount this cell from its own neighborhood tally
            land_neighbors--; 

            // Case: LAND --> WATER
            if(land_neighbors < 2 || land_neighbors > 3) {
                state.terrain = MapgenStateName::WATER;
            }
        }

        // Case: FOREST cell
        else if (state.terrain == MapgenStateName::FOREST) {
            // Uncount this cell from its own neighborhood tally
            forest_neighbors--; 

            // Case: FOREST --> WATER
            //if(condition) {
            //    state.terrain = MapgenStateName::NEW_STATE;
            //}
        }

        // Case: DESERT cell
        else if (state.terrain == MapgenStateName::DESERT) {
            // Uncount this cell from its own neighborhood tally
            desert_neighbors--; 

            // Case: DESERT --> WATER
            //if(condition) {
            //    state.terrain = MapgenStateName::NEW_STATE;
            //}
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
