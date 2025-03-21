#ifndef MAPGEN_CELL_HPP
#define MAPGEN_CELL_HPP

#include <cmath>
#include <nlohmann/json.hpp>
#include <cadmium/modeling/celldevs/grid/cell.hpp>
#include <cadmium/modeling/celldevs/grid/config.hpp>
#include "mapgenState.hpp"

using namespace cadmium::celldevs;

class mapgen : public GridCell<mapgenState, double> {
	public:
	mapgen(const std::vector<int>& id, 
			const std::shared_ptr<const GridCellConfig<mapgenState, double>>& config
		  ): GridCell<mapgenState, double>(id, config) { }

	[[nodiscard]] mapgenState localComputation(mapgenState state,
		const std::unordered_map<std::vector<int>, NeighborData<mapgenState, double>>& neighborhood) const override {
		int live_neighbors = 0;

		for (const auto& [neighborId, neighborData]: neighborhood) {
			auto nState = neighborData.state;

			if(nState->life == true) {
				live_neighbors++;
			}

		}

		if(state.life == true) {
			live_neighbors--; //Self is a neighbor, we do not care about that yet.
			if(live_neighbors < 2 || live_neighbors > 3) {
				state.life = false;
			}
		} else {
			if(live_neighbors == 3) {
				state.life = true;
			}
		}

		return state;
	}

	[[nodiscard]] double outputDelay(const mapgenState& state) const override {
		return 1.;
	}
};

#endif // MAPGEN_CELL_HPP
