#ifndef MBK_TYPES_H_INCLUDED
#define MBK_TYPES_H_INCLUDED
#include <vector>

typedef struct {
	long		xyz[3];
	signed char	norm[4];
	short		uv[2];
} VertexT;

typedef std::vector<VertexT> VertexBufferT;

#endif // MBK_TYPES_H_INCLUDED
