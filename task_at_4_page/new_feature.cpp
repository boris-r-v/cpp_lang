#include "new_feature.h"

FigureIface::type_t UnsupportedFigure::type() 
{ 
    return FigureIface::type_t::eUnknown;
}
size_t UnsupportedFigure::points_number( )
{ 
    return 0; 
}
void UnsupportedFigure::draw( points_t const& ) 
{ 
    throw UnSupportedFigure (); 
}

FigureIface::type_t Circle::type() 
{ 
    return FigureIface::type_t::eCircle;
}
size_t  Circle::points_number( )
{ 
    return 3; 
}
void  Circle::draw( points_t const& ) 
{ 
    std::cout << "drawCircle" << std::endl;
}

FigureIface::type_t Triangle::type() 
{ 
    return FigureIface::type_t::eTriangle;
}
size_t Triangle::points_number( )
{ 
    return 6; 
}
void Triangle::draw( points_t const& ) 
{ 
    std::cout << "drawTriangle" << std::endl;
}

FigureIface::type_t Square::type() 
{ 
    return FigureIface::type_t::eSquare;
}
size_t Square::points_number( )
{ 
    return 8; 
}
void Square::draw( points_t const& ) 
{ 
    std::cout << "drawPolygon" << std::endl;
}

points_t Reader::read_figure_points_from_stream( std::ifstream& _stream )
{
    points_t ret;
    if ( _stream.is_open() and _stream.good() ) 
    {	
	size_t pn = read_figure_type_from_stream( _stream )->points_number();
	_stream.seekg(sizeof(FeatureType), _stream.beg );
	for ( int i = 0; i < pn; ++i )
	{
	    point_t p;
	    _stream.read( reinterpret_cast<char*>(&p),  sizeof( point_t ) );
	    ret.push_back( p );
	}
    }
    return ret;
}
FigureIface* Reader::read_figure_type_from_stream( std::ifstream& _stream )
{
    if ( _stream.is_open() and _stream.good() ) 
    {	
	_stream.seekg(0, _stream.beg );
	FeatureType type;
	_stream.read( reinterpret_cast<char*>(&type),  sizeof( FeatureType ) );
	switch ( type )
	{
    	    case eCircle: 	return Circle::instance();		//n = 3; break;
	    case eTriangle:    	return Triangle::instance();	//n = 6; break;
	    case eSquare: 	return Square::instance();		//n = 8; break;
	    default: 		return UnsupportedFigure::instance();
	}
    }
    return  UnsupportedFigure::instance();
}

void Painter::draw( FigureIface* _figure, points_t const& _points )
{
    try
    {	
	_figure->draw( _points );    
    }
    catch ( UnSupportedFigure const& e )
    {
		std::cout << e.what() << std::endl;
    }
    catch ( std::exception const& e )
    {
	std::cout << "ERROR: " << e.what() << std::endl;
    }
}	    

Feature::Feature():
   reader_( Reader::instance() ),
   painter_( Painter::instance() ),
   figure_ ( UnsupportedFigure::instance() )
{
}

bool Feature::isValid()
{
    return figure_->type() != UnsupportedFigure::instance()->type();
}
bool Feature::read( std::ifstream& s )
{
    figure_ = reader_->read_figure_type_from_stream( s );
    points_ = std::move( reader_->read_figure_points_from_stream( s ) );  
}
void Feature::draw()
{
    painter_->draw( figure_, points_ );
}

int main(int argc, char* argv[])
{

    Feature feature;
    /*Так как по дестриптору получить имя файла нет переносимой возможности - то интерфейс взаимодействия всеже несколько придеться изменить*/
    //FILE* file = fopen("features.dat", "r");
    //feature.read(file);
    std::ifstream s ("features.dat", std::ifstream::binary );
    feature.read(s);
    if (!feature.isValid())
        return 1;

    feature.draw();	//надо все же порисовать чего нибудь
    return 0;
}
