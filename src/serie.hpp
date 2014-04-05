#ifndef SERIE_HPP_INCLUDED
#define SERIE_HPP_INCLUDED

template<int N>
constexpr inline float SerieCos(float x,float c[])
{
	return ( c[N-1]*cos( x*N ) )+SerieCos<N-1>(x,c);
}

template<>
constexpr inline float SerieCos<0>(float,float[])
{
	return 0;
}

#endif // SERIE_HPP_INCLUDED
