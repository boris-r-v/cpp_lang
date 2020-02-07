/** \file new_feature.cpp
 @brief Двухсвязный список допускающих хранение пользовательских структур данныхРефакторинг кода из main_test.cpp

    Так как требования к оформлению выдвинуты не были - то все в одном файле
    Идея такая выделить алгоритмы рисования фигур в отдельные классы(паттерн Стретегию), снизить связность класннов выделив интерфейсы:
	1. читателя бинарника;
	2. рисователя фигур
	3. стоим иерархию фигур от интерфейса фигура к различным типам ( неизвестный, круг, треугольник, квадрат)
	4. судя по приведенному main - рисование довольно частый процесс и коордиаты каждой фигуры лежат в отдельном файле, поэтому вся реализация использует Одиночек для экономии аллокаций
	5. делаем адаптер, чтобы вставить отрефакторенный код вместо старого. 
	    К сожалению этого не получилось в полной мере так как по отрытому дескримптору невозможно определить путь к файлу, а сишные фукнции не используем.
	    Иначе зачем вообще старый код трогать - починить ошибки исправить и пусть дальше работает.

*/
#include <fstream>		//for std::ifstream
#include <memory>		//for std::unique_ptr
#include <exception>		//for std::exception
#include <vector>		//for std::vector
#include <iostream>		//for std::cout

typedef double point_t;
typedef std::vector<point_t> points_t;
class NotSupportedFigure : public std::exception
{ 
    public:
	NotSupportedFigure() throw() {}
	virtual ~NotSupportedFigure() throw() {}
        virtual const char* what() const throw(){ return "Unsupported figure"; }
};
template < typename T >
class Singleton
{
    protected:
	Singleton() = default;
    	~Singleton() = default;
    	Singleton( Singleton const& ) = delete;
	Singleton& operator=( Singleton const& ) = delete;

    public:
	static T* instance()
    	{
	    static T _instance_;
	    return &_instance_;
	}
};

/**
    @brief Интерфейс фигуры
*/
class FigureIface
{
    public:
	enum class type_t {eUnknown, eCircle, eTriangle, eSquare};
	virtual size_t points_number( ) = 0;
	virtual type_t type( ) = 0;
	virtual void draw( points_t const& ) = 0;
};

/**
    @brief Неподдерживаемая фигруа
*/
class UnsupportedFigure: public FigureIface, public Singleton<UnsupportedFigure>
{
    public:
	type_t type() { return type_t::eUnknown;}
	size_t points_number( ){ return 0; }
	void draw( points_t const& ) { throw NotSupportedFigure (); }
};


/**
    @brief Круг
*/
class Circle: public FigureIface, public Singleton<Circle>
{
    public:
	type_t type() { return type_t::eCircle;}
	size_t points_number( ){ return 3; }
	void draw( points_t const& ) { std::cout << "drawCircle" << std::endl;}
};

/**
    @brief Полигон
*/
class Triangle: public FigureIface, public Singleton<Triangle>
{
    public:
	type_t type() { return type_t::eTriangle;}
	size_t points_number( ){ return 6; }
	void draw( points_t const& ) { std::cout << "drawTriangle" << std::endl;}
};

/**
    @brief Полигон
*/
class Square: public FigureIface, public Singleton<Square>
{
    public:
	type_t type() { return type_t::eSquare;}
	size_t points_number( ){ return 8; }
	void draw( points_t const& ) { std::cout << "drawPolygon" << std::endl;}
};

/**
    @brief Интерфейс чтения координат
*/
class ReaderIface
{
    public:
	virtual points_t read_figure_points_from_stream( std::ifstream& ) = 0;
	virtual FigureIface* read_figure_type_from_stream( std::ifstream& ) = 0;
};
/**
    @brief Реализация интерфейса чтения координат
*/
class Reader: public ReaderIface, public Singleton<Reader>
{
	enum FeatureType {eUnknown, eCircle, eTriangle, eSquare};

    public:
    	points_t read_figure_points_from_stream( std::ifstream& _stream )
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
    
		;		
	    }
	    return ret;
	}
	FigureIface* read_figure_type_from_stream( std::ifstream& _stream )
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
};

/**
    @brief Интерфейс Feature
*/
class PainterIface
{
    public:
	virtual void draw( FigureIface* _state, points_t const& ) = 0;	    
};

/**
    @brief Реализация интерфейса Feature
*/
class Painter: public PainterIface, public Singleton<Painter>
{
    public:
	void draw( FigureIface* _figure, points_t const& _points )
	{
	    try
	    {	
		_figure->draw( _points );    
	    }
	    catch ( NotSupportedFigure const& e )
	    {
		std::cout << e.what() << std::endl;
	    }
	    catch ( std::exception const& e )
	    {
		std::cout << "ERROR: " << e.what() << std::endl;
	    }
	}	    

};

/**
    @brief Адаптер на старый Feature
*/
class Feature
{
	PainterIface* painter_;
	ReaderIface* reader_;
	FigureIface* figure_;
	points_t points_;
    public:
	Feature():
	    reader_( Reader::instance() ),
	    painter_( Painter::instance() ),
	    figure_ ( UnsupportedFigure::instance() )
	{
	    
	}
        bool isValid()
	{
	    return figure_->type() != UnsupportedFigure::instance()->type();
	}
        bool read( std::ifstream& s )
	{
	    figure_ = reader_->read_figure_type_from_stream( s );
    	    points_ = std::move( reader_->read_figure_points_from_stream( s ) );  

	}
        void draw()
	{
	    painter_->draw( figure_, points_ );
	}
}; 

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
