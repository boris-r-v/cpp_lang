/** \file new_feature.h

 @brief Refactoring Feature.

*/
#include <fstream>		//for std::ifstream
#include <memory>		//for std::unique_ptr
#include <exception>		//for std::exception
#include <vector>		//for std::vector
#include <iostream>		//for std::cout


typedef double point_t;
typedef std::vector<point_t> points_t;
/**
    @brief Unsupporter Figure exceptions
*/
class UnSupportedFigure : public std::exception
{ 
    public:
	UnSupportedFigure() throw() {}
	virtual ~UnSupportedFigure() throw() {}
        virtual const char* what() const throw(){ return "Unsupported figure"; }
};
/**
    @brief Signleton base class, used in FigureIface implementation
*/
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
    @brief Interface class of figure
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
    @brief Implementation of unsupported figure
*/
class UnsupportedFigure: public FigureIface, public Singleton<UnsupportedFigure>
{
    public:
	type_t type();
	size_t points_number( );
	void draw( points_t const& );
};


/**
    @brief Implementation of circle
*/
class Circle: public FigureIface, public Singleton<Circle>
{
    public:
	type_t type();
	size_t points_number( );
	void draw( points_t const& );
};

/**
    @brief Implementation of triangle
*/
class Triangle: public FigureIface, public Singleton<Triangle>
{
    public:
	type_t type();
	size_t points_number( );
	void draw( points_t const& );
};

/**
    @brief Implementation of Square
*/
class Square: public FigureIface, public Singleton<Square>
{
    public:
	type_t type();
	size_t points_number( );
	void draw( points_t const& );
};

/**
    @brief Interface class of file reader
*/
class ReaderIface
{
    public:
	virtual points_t read_figure_points_from_stream( std::ifstream& ) = 0;
	virtual FigureIface* read_figure_type_from_stream( std::ifstream& ) = 0;
};
/**
    @brief Implementation of file reader
*/
class Reader: public ReaderIface, public Singleton<Reader>
{
	enum FeatureType {eUnknown, eCircle, eTriangle, eSquare};

    public:
    	points_t read_figure_points_from_stream( std::ifstream& _stream );
	FigureIface* read_figure_type_from_stream( std::ifstream& _stream );
};

/**
    @brief Inteface of painter
*/
class PainterIface
{
    public:
	virtual void draw( FigureIface* _state, points_t const& ) = 0;	    
};

/**
    @brief Implementation of painter
*/
class Painter: public PainterIface, public Singleton<Painter>
{
    public:
	void draw( FigureIface* _figure, points_t const& _points );
};

/**
    @brief Adapter (almost) to the old Feature

    It was not possible to completely repeat the old interface of Feature,
    because there is no portable ability to get the file path from the file descriptor (FILE* fd).
*/
class Feature
{
	PainterIface* painter_;
	ReaderIface* reader_;
	FigureIface* figure_;
	points_t points_;
    public:
	Feature();
        bool isValid();
        bool read( std::ifstream& s );	
        void draw();
}; 

