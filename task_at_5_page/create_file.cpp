#include <fstream>
#include <iostream>

enum FeatureType {eUnknown, eCircle, eTriangle, eSquare};
void write_m( FeatureType type, size_t len )
{
    std::ofstream s ("features.dat", std::ifstream::binary|std::ios::trunc );

    s.write( (char*)(&type), sizeof( FeatureType ) );

    for ( int i = 0; i < len; ++i )
	s.write( (char*)(&i), sizeof( FeatureType ) );
}

int main(int argc, char* argv[])
{
    if ( argc < 2 ) return 1;

    std::ofstream s ("features.dat", std::ifstream::binary|std::ios::trunc );
    
    switch (atoi( argv[1] )) 
    {
    	    case eCircle: 	write_m( eCircle, 3); break;
	    case eTriangle:    	write_m( eTriangle, 6); break;
	    case eSquare: 	write_m( eSquare, 8); break;
    }

    return 0;
}
