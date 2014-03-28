#ifndef QUADSTRIP_HPP_INCLUDED
#define QUADSTRIP_HPP_INCLUDED

/** \brief Build vertex array for a flat horizontal strip of quad, to be drawn
 * as triangle strip. T is typically GLfloat.
 *
 * \param out_vector std::vector<T>&	The output vector
 * \param w float						Quad width
 * \param h float						Quad height
 * \param numquads int					Number must be >=1
 * \return void
 *
 */
template<typename T>
void QuadStrip2D(std::vector<T>& out_vector,T w,T h,int numquads)
{
	if (__builtin_expect(numquads<1,0))
	{
		return;
	}
	T x1=0,x2=0;
	for (int i=0; i<numquads; i++)
	{
		x1=i*w;
		x2=x1+w;
		out_vector.push_back(x1);
		out_vector.push_back(h);
		out_vector.push_back(x1);
		out_vector.push_back(0);
		out_vector.push_back(x2);
		out_vector.push_back(h);
		out_vector.push_back(x2);
		out_vector.push_back(0);
	}
}

#endif // QUADSTRIP_HPP_INCLUDED
