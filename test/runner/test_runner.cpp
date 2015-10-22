#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <vector>
#include "fixed_vect"

// catch example with std::fixed_vect<> used to initialize v.
TEST_CASE( "vectors can be sized and resized", "[vector]" ) {

	std::fixed_vect<int>::static_sized<10> fvi {1,2,3,4,5};

    std::vector<int> v( fvi.begin(), fvi.end() );

    REQUIRE( v.size() == 5 );
    REQUIRE( v.capacity() >= 5 );

    SECTION( "resizing bigger changes size and capacity" ) {
        v.resize( 10 );

        REQUIRE( v.size() == 10 );
        REQUIRE( v.capacity() >= 10 );
    }
    SECTION( "resizing smaller changes size but not capacity" ) {
        v.resize( 0 );

        REQUIRE( v.size() == 0 );
        REQUIRE( v.capacity() >= 5 );
    }
    SECTION( "reserving bigger changes capacity but not size" ) {
        v.reserve( 10 );

        REQUIRE( v.size() == 5 );
        REQUIRE( v.capacity() >= 10 );
    }
    SECTION( "reserving smaller does not change size or capacity" ) {
        v.reserve( 0 );

        REQUIRE( v.size() == 5 );
        REQUIRE( v.capacity() >= 5 );
    }
}
