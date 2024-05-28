/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.

This file is part of the Doom 3 GPL Source Code (?Doom 3 Source Code?).

Doom 3 Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/
/*
** QGL.H
*/

#ifndef __QGL_H__
#define __QGL_H__

#if defined(_WIN32)

// #include <glad/glad.h>
#include <GL/glew.h>

#elif defined(MACOS_X)

// magic flag to keep tiger gl.h from loading glext.h
#define GL_GLEXT_LEGACY
#include <OpenGL/gl.h>

#elif defined(__linux__)

// using our local glext.h
// http://oss.sgi.com/projects/ogl-sample/ABI/
#define GL_GLEXT_LEGACY
#define GLX_GLXEXT_LEGACY
#include <GL/gl.h>
#include <GL/glx.h>

#else

#include <gl.h>

#endif

#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef WINAPI
#define WINAPI
#endif

// only use local glext.h if we are not using the system one already
// http://oss.sgi.com/projects/ogl-sample/ABI/
#ifndef GL_GLEXT_VERSION

// #include "glext.h"

#endif

typedef void (*GLExtension_t)(void);

#ifdef __cplusplus
extern "C"
{
#endif

	GLExtension_t GLimp_ExtensionPointer(const char *name);

#ifdef __cplusplus
}
#endif

// multitexture
extern void(APIENTRY *qglMultiTexCoord2fARB)(GLenum texture, GLfloat s, GLfloat t);
extern void(APIENTRY *qglMultiTexCoord2fvARB)(GLenum texture, GLfloat *st);
extern void(APIENTRY *qglActiveTextureARB)(GLenum texture);
extern void(APIENTRY *qglClientActiveTextureARB)(GLenum texture);

// ARB_vertex_buffer_object
extern PFNGLBINDBUFFERARBPROC qglBindBufferARB;
extern PFNGLDELETEBUFFERSARBPROC qglDeleteBuffersARB;
extern PFNGLGENBUFFERSARBPROC qglGenBuffersARB;
extern PFNGLISBUFFERARBPROC qglIsBufferARB;
extern PFNGLBUFFERDATAARBPROC qglBufferDataARB;
extern PFNGLBUFFERSUBDATAARBPROC qglBufferSubDataARB;
extern PFNGLGETBUFFERSUBDATAARBPROC qglGetBufferSubDataARB;
extern PFNGLMAPBUFFERARBPROC qglMapBufferARB;
extern PFNGLUNMAPBUFFERARBPROC qglUnmapBufferARB;
extern PFNGLGETBUFFERPARAMETERIVARBPROC qglGetBufferParameterivARB;
extern PFNGLGETBUFFERPOINTERVARBPROC qglGetBufferPointervARB;

// NV_register_combiners
extern void(APIENTRY *qglCombinerParameterfvNV)(GLenum pname, const GLfloat *params);
extern void(APIENTRY *qglCombinerParameterivNV)(GLenum pname, const GLint *params);
extern void(APIENTRY *qglCombinerParameterfNV)(GLenum pname, const GLfloat param);
extern void(APIENTRY *qglCombinerParameteriNV)(GLenum pname, const GLint param);
extern void(APIENTRY *qglCombinerInputNV)(GLenum stage, GLenum portion, GLenum variable, GLenum input,
										  GLenum mapping, GLenum componentUsage);
extern void(APIENTRY *qglCombinerOutputNV)(GLenum stage, GLenum portion, GLenum abOutput, GLenum cdOutput,
										   GLenum sumOutput, GLenum scale, GLenum bias, GLboolean abDotProduct,
										   GLboolean cdDotProduct, GLboolean muxSum);
extern void(APIENTRY *qglFinalCombinerInputNV)(GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);

// 3D textures
extern void(APIENTRY *qglTexImage3D)(GLenum, GLint, GLint, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *);

// shared texture palette
extern void(APIENTRY *qglColorTableEXT)(int, int, int, int, int, const void *);

// ATI_fragment_shader
extern PFNGLGENFRAGMENTSHADERSATIPROC qglGenFragmentShadersATI;
extern PFNGLBINDFRAGMENTSHADERATIPROC qglBindFragmentShaderATI;
extern PFNGLDELETEFRAGMENTSHADERATIPROC qglDeleteFragmentShaderATI;
extern PFNGLBEGINFRAGMENTSHADERATIPROC qglBeginFragmentShaderATI;
extern PFNGLENDFRAGMENTSHADERATIPROC qglEndFragmentShaderATI;
extern PFNGLPASSTEXCOORDATIPROC qglPassTexCoordATI;
extern PFNGLSAMPLEMAPATIPROC qglSampleMapATI;
extern PFNGLCOLORFRAGMENTOP1ATIPROC qglColorFragmentOp1ATI;
extern PFNGLCOLORFRAGMENTOP2ATIPROC qglColorFragmentOp2ATI;
extern PFNGLCOLORFRAGMENTOP3ATIPROC qglColorFragmentOp3ATI;
extern PFNGLALPHAFRAGMENTOP1ATIPROC qglAlphaFragmentOp1ATI;
extern PFNGLALPHAFRAGMENTOP2ATIPROC qglAlphaFragmentOp2ATI;
extern PFNGLALPHAFRAGMENTOP3ATIPROC qglAlphaFragmentOp3ATI;
extern PFNGLSETFRAGMENTSHADERCONSTANTATIPROC qglSetFragmentShaderConstantATI;

// EXT_stencil_two_side
extern PFNGLACTIVESTENCILFACEEXTPROC qglActiveStencilFaceEXT;

// ATI_separate_stencil
extern PFNGLSTENCILOPSEPARATEATIPROC qglStencilOpSeparateATI;
extern PFNGLSTENCILFUNCSEPARATEATIPROC qglStencilFuncSeparateATI;

// ARB_texture_compression
extern PFNGLCOMPRESSEDTEXIMAGE2DARBPROC qglCompressedTexImage2DARB;
extern PFNGLGETCOMPRESSEDTEXIMAGEARBPROC qglGetCompressedTexImageARB;

// ARB_vertex_program / ARB_fragment_program
extern PFNGLVERTEXATTRIBPOINTERARBPROC qglVertexAttribPointerARB;
extern PFNGLENABLEVERTEXATTRIBARRAYARBPROC qglEnableVertexAttribArrayARB;
extern PFNGLDISABLEVERTEXATTRIBARRAYARBPROC qglDisableVertexAttribArrayARB;
extern PFNGLPROGRAMSTRINGARBPROC qglProgramStringARB;
extern PFNGLBINDPROGRAMARBPROC qglBindProgramARB;
extern PFNGLGENPROGRAMSARBPROC qglGenProgramsARB;
extern PFNGLPROGRAMENVPARAMETER4FVARBPROC qglProgramEnvParameter4fvARB;
extern PFNGLPROGRAMLOCALPARAMETER4FVARBPROC qglProgramLocalParameter4fvARB;

// GL_EXT_depth_bounds_test
extern PFNGLDEPTHBOUNDSEXTPROC qglDepthBoundsEXT;

//===========================================================================

// non-windows systems will just redefine qgl* to gl*
#if defined(__APPLE__) || defined(ID_GL_HARDLINK)

#include "qgl_linked.h"

#else

// windows systems use a function pointer for each call so we can do our log file intercepts

#if defined(_WIN32)

extern int(WINAPI *qwglChoosePixelFormat)(HDC, CONST PIXELFORMATDESCRIPTOR *);
extern int(WINAPI *qwglDescribePixelFormat)(HDC, int, UINT, LPPIXELFORMATDESCRIPTOR);
extern int(WINAPI *qwglGetPixelFormat)(HDC);
extern BOOL(WINAPI *qwglSetPixelFormat)(HDC, int, CONST PIXELFORMATDESCRIPTOR *);
extern BOOL(WINAPI *qwglSwapBuffers)(HDC);

extern BOOL(WINAPI *qwglCopyContext)(HGLRC, HGLRC, UINT);
extern HGLRC(WINAPI *qwglCreateContext)(HDC);
extern HGLRC(WINAPI *qwglCreateLayerContext)(HDC, int);
extern BOOL(WINAPI *qwglDeleteContext)(HGLRC);
extern HGLRC(WINAPI *qwglGetCurrentContext)(VOID);
extern HDC(WINAPI *qwglGetCurrentDC)(VOID);
extern PROC(WINAPI *qwglGetProcAddress)(LPCSTR);
extern BOOL(WINAPI *qwglMakeCurrent)(HDC, HGLRC);
extern BOOL(WINAPI *qwglShareLists)(HGLRC, HGLRC);
extern BOOL(WINAPI *qwglUseFontBitmaps)(HDC, DWORD, DWORD, DWORD);

extern BOOL(WINAPI *qwglUseFontOutlines)(HDC, DWORD, DWORD, DWORD, FLOAT,
										 FLOAT, int, LPGLYPHMETRICSFLOAT);

extern BOOL(WINAPI *qwglDescribeLayerPlane)(HDC, int, int, UINT,
											LPLAYERPLANEDESCRIPTOR);
extern int(WINAPI *qwglSetLayerPaletteEntries)(HDC, int, int, int,
											   CONST COLORREF *);
extern int(WINAPI *qwglGetLayerPaletteEntries)(HDC, int, int, int,
											   COLORREF *);
extern BOOL(WINAPI *qwglRealizeLayerPalette)(HDC, int, BOOL);
extern BOOL(WINAPI *qwglSwapLayerBuffers)(HDC, UINT);

#endif // _WIN32

#if defined(__linux__)

// GLX Functions
extern XVisualInfo *(*qglXChooseVisual)(Display *dpy, int screen, int *attribList);
extern GLXContext (*qglXCreateContext)(Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct);
extern void (*qglXDestroyContext)(Display *dpy, GLXContext ctx);
extern Bool (*qglXMakeCurrent)(Display *dpy, GLXDrawable drawable, GLXContext ctx);
extern void (*qglXSwapBuffers)(Display *dpy, GLXDrawable drawable);
extern GLExtension_t (*qglXGetProcAddressARB)(const GLubyte *procname);

// make sure the code is correctly using qgl everywhere
// don't enable that when building glimp itself obviously..
#if !defined(GLIMP)
#include "../sys/linux/qgl_enforce.h"
#endif

#endif // __linux__

#endif // hardlinlk vs dlopen

#endif
