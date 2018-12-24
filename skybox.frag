//#version 330 core
varying mediump vec3 TexCoords;

uniform mediump samplerCube skybox;

void main()
{
    gl_FragColor = textureCube(skybox, TexCoords);

}
