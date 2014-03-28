#ifndef CHECKGLERRORS_H_INCLUDED
#define CHECKGLERRORS_H_INCLUDED
// Writes message, then last OpenGL error to standard output.
// Returns true if there was an error.
#ifndef NDEBUG
bool CheckGLErrors(const char* message);
#else
#define CheckGLErrors(x)
#endif
#endif // CHECKGLERRORS_H_INCLUDED
