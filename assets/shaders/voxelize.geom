layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out; // options are points, line_strip, triangle_strip. we want output triangles.

uniform mat4 uViewProjMatOrthoX;
uniform mat4 uViewProjMatOrthoY;
uniform mat4 uViewProjMatOrthoZ;
uniform int uVoxelStorageMode; // 0: regular 3D texture voxel grid, 1: voxel octree

in VertexData
{
    vec2 texCoords;
    vec3 positionVS; // vertex position in view space
    vec3 normalVS;   // vertex normal in view space
} v_in[];

out VertexData
{
    vec2 texCoords;
    vec3 positionVS;
    vec3 normalVS;
} v_out;

flat out int axisOfTriangleProjection; // to restore 3D voxel data in fragment shader, flat to avoid interpolation

// we want to use the GPU 2D rasterizer to help in voxelizing our triangles,
// the rasterizer would produce a raster of fragments in the view direction
// but we want a raster in the directions of an axis aligned 3D voxel grid
// so before the rasterization, we use geometry shader to project triangles
// along axis that maximizes the area of the triangle 2D projection, to minimize information loss
// after rasterization, the fragment shader computes 3D voxel data from all triangle fragments in the voxel

// another reason why we need the geometry shader is for conservative rasterization
// TODO

void main() {

    // determine in which coordinate axis the triangle normal is facing the most
    // to find the projection axis that yields the maximum projected area
    vec3 N = normalize(normalVS);
    if (abs(N.x) > abs(N.y) && abs(N.x) > abs(N.z)
    {
        axisOfTriangleProjection = 0;
    }
    else if (abs(N.y) > abs(N.x) && abs(N.y) > abs(N.z)
    {
        axisOfTriangleProjection = 1;
    }
    else
    {
        axisOfTriangleProjection = 2;
    }

    // project triangle vertices



}
