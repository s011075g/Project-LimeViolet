#include "common.glh"

varying vec2 texCoord0;

#if defined(VS_BUILD)
Layout(0) attribute vec3 position;
Layout(1) attribute vec2 texCoord;
Layout(4) attribute mat4 transformMat;

void main()
{
    gl_Position = vec4(position, 1.0) * transformMat;
    texCoord0 = texCoord;
}

#elif defined(FS_BUILD)
uniform sampler2D diffuse;

DeclareFragOutput(0, vec4);
void main()
{
	SetFragOutput(0, texture2D(diffuse, texCoord0));
}
#endif