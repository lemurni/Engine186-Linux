layout (location = 0) in vec4 aVertexPosition;

// entry positions for rays through the volume
// these will be interpolated by the gpu and then accessible by same name in fragment shader
// since we already have a texture of exit positions (interpolated volume cube backface vertices) from another shader
// we have all the possible rays (entry-exit pairs) through the volume
// entry and exit positions are interpolated model space vertex positions of a cube in range [0,1],
// since they will be used to sample the volume texture (tex coords normalized to [0,1])
// note that since we already have the backface exit positions back face culling should be enabled
out vec3 entryPos;

uniform mat4 uModelViewProjMat;

void main()
{
    entryPos = aVertexPosition.xyz; // volume cube front vertex positions in model space
    gl_Position = uModelViewProjMat * vec4(aVertexPosition.xyz, 1.0);
}
