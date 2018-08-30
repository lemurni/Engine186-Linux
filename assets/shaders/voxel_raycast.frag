
// interpolated fragment entry position of a ray through the volume
in vec3 entryPos;

out vec4 oFragColor;

uniform sampler2D uExitPositions; // precalculated exit positions for an orthogonal ray from each fragment
uniform sampler3D uVoxelDiffuseReflectivity;
uniform float uRaycastStepSize;
uniform vec2 uScreenDimensions;

void main()
{
    oFragColor = vec4(0, 1, 0, 1); return;


    vec3 exitPos = texture(uExitPositions, gl_FragCoord.st / uScreenDimensions).xyz;

    if (entryPos == exitPos) {
        discard;
    }

    // since view rays have same fragment coordinates
    // with entryPos and uExitPositions texture we have start and end points of rays for all fragments
    // now we only need to step along the rays and sample the 3D texture
    vec3 raySegment = exitPos - entryPos;
    vec3 rayDirection = normalize(raySegment);
    vec3 rayDelta = rayDirection * uRaycastStepSize;
    int steps = int(length(raySegment) / uRaycastStepSize);

    vec3 currentVoxelPos = entryPos;
    vec3 firstHitPos = vec3(0); // first hit nonzero voxel position

    // sampled voxel data
    vec3 voxelDiffuseReflectivity;

    // step through the voxels along the fragment view ray
    for (int i = 0; i < steps; ++i) {

        voxelDiffuseReflectivity = texture(uVoxelDiffuseReflectivity, currentVoxelPos).rgb;

        // break at first hit non empty voxel
        if (length(voxelDiffuseReflectivity) > 0) {
            firstHitPos = currentVoxelPos;
            break;
        }

        currentVoxelPos += rayDelta;
    }

    oFragColor = vec4(voxelDiffuseReflectivity, 1);

}
