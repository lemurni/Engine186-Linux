layout (location = 0) in vec4 aVertexPosition;

// here we store backface vertex positions of the volume cube to get view ray exit positions for each fragment
// these will then be used together with entry positions to define a view ray per fragment, stored in a 2D texture
// the idea is to use model space vertex positions of a cube in range [0,1]
// since the interpolated positions will later be used to sample a volume texture with tex coords also in [0,1]
// note that since now we want only the vertices of volume cube backfaces, front face culling must be enabled.
out vec3 exitPos;

uniform mat4 uModelViewProjMat;

void main()
{
    exitPos = aVertexPosition.xyz; // volume cube back vertex positions in model space
    gl_Position = uModelViewProjMat * vec4(aVertexPosition.xyz, 1.0);
}
