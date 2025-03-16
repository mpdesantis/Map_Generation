#ifndef CM_UL_TEST_HPP
#define CM_UL_TEST_HPP

// Cadmium V2
#include "cadmium/modeling/devs/coupled.hpp"

// Project atomic models
#include "ul.hpp"

using namespace cadmium;

struct CmUlTest : public Coupled {

    /**
     * Constructor for top coupled model.
     *
     * @param id ID of the model.
     */
    CmUlTest(const std::string& id) : Coupled(id) {
        auto ul = addComponent<Ul>("UL");
    }

};

#endif
